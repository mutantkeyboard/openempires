#include "Sockets.h"

#include "Util.h"

#include <stdlib.h>
#include <stdbool.h>

Sockets Sockets_Init(const int32_t port, const int32_t users)
{
    IPaddress ip;
    SDLNet_ResolveHost(&ip, NULL, port);
    static Sockets zero;
    Sockets sockets = zero;
    sockets.users = users;
    sockets.self = SDLNet_TCP_Open(&ip);
    sockets.set = SDLNet_AllocSocketSet(COLOR_COUNT);
    return sockets;
}

void Sockets_Free(const Sockets sockets)
{
    SDLNet_TCP_Close(sockets.self);
    SDLNet_FreeSocketSet(sockets.set);
}

static Sockets Add(Sockets sockets, TCPsocket socket)
{
    for(int32_t i = 0; i < COLOR_COUNT; i++)
        if(sockets.socket[i] == NULL)
        {
            SDLNet_TCP_AddSocket(sockets.set, socket);
            sockets.socket[i] = socket;
            return sockets;
        }
    return sockets;
}

Sockets Sockets_Service(Sockets sockets, const int32_t timeout)
{
    if(SDLNet_CheckSockets(sockets.set, timeout))
        for(int32_t i = 0; i < COLOR_COUNT; i++)
        {
            TCPsocket socket = sockets.socket[i];
            if(SDLNet_SocketReady(socket))
            {
                static Overview zero;
                Overview overview = zero;
                const int32_t max = sizeof(overview);
                const int32_t bytes = SDLNet_TCP_Recv(socket, &overview, max);
                if(bytes <= 0)
                {
                    SDLNet_TCP_DelSocket(sockets.set, socket);
                    sockets.cycles[i] = 0;
                    sockets.parity[i] = 0;
                    sockets.queue_size[i] = 0;
                    sockets.packet.overview[i] = zero;
                    sockets.socket[i] = NULL;
                }
                if(bytes == max)
                {
                    sockets.cycles[i] = overview.cycles;
                    sockets.parity[i] = overview.parity;
                    sockets.queue_size[i] = overview.queue_size;
                    if(Overview_UsedAction(overview))
                        sockets.packet.overview[i] = overview;
                }
            }
        }
    return sockets;
}

static Sockets Clear(Sockets sockets)
{
    static Packet zero;
    sockets.packet = zero;
    sockets.turn++;
    return sockets;
}

static int32_t GetCycleSetpoint(const Sockets sockets)
{
    int32_t setpoint = 0;
    int32_t count = 0;
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        const int32_t cycles = sockets.cycles[i];
        if(cycles > 0)
        {
            setpoint += cycles;
            count++;
        }
    }
    return (count > 0) ? (setpoint / count) : 0;
}

static int32_t GetCycleMax(const Sockets sockets)
{
    int32_t max = 0;
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        const int32_t cycles = sockets.cycles[i];
        if(cycles > max)
            max = cycles;
    }
    return max;
}

static int32_t GetCycleMin(const Sockets sockets)
{
    int32_t min = INT32_MAX;
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        const int32_t cycles = sockets.cycles[i];
        if(cycles != 0)
            if(cycles < min)
                min = cycles;
    }
    return min;
}

static Sockets CalculateControlChars(Sockets sockets, const int32_t setpoint)
{
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        const int32_t cycles = sockets.cycles[i];
        if(cycles  > setpoint) sockets.control[i] = PACKET_CONTROL_SLOW_DOWN;
        if(cycles == setpoint) sockets.control[i] = PACKET_CONTROL_STEADY;
        if(cycles  < setpoint) sockets.control[i] = PACKET_CONTROL_SPEED_UP;
    }
    return sockets;
}

static void Print(const Sockets sockets, const int32_t setpoint)
{
    printf("%d :: %d\n", sockets.turn, setpoint);
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        const uint64_t parity = sockets.parity[i];
        const int32_t cycles = sockets.cycles[i];
        const char control = sockets.control[i];
        const char queue_size = sockets.queue_size[i];
        const char parity_symbol = sockets.is_stable ? '!' : '?';
        TCPsocket socket = sockets.socket[i];
        printf("%d :: %d :: %c :: 0x%016lX :: %c :: %d :: %d\n",
                i, socket != NULL, parity_symbol, parity, control, cycles, queue_size);
    }
}

static void Send(const Sockets sockets, const int32_t max, const bool game_running)
{
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        TCPsocket socket = sockets.socket[i];
        if(socket)
        {
            const int32_t offset = 2; // XXX. Make this ping dependent.
            Packet packet = sockets.packet;
            packet.control = sockets.control[i];
            packet.turn = sockets.turn;
            packet.exec_cycle = max + offset;
            packet.client_id = i;
            packet.is_stable = sockets.is_stable;
            packet.game_running = game_running;
            if(!sockets.is_stable)
                packet = Packet_ZeroOverviews(packet);
            SDLNet_TCP_Send(socket, &packet, sizeof(packet));
        }
    }
}

static bool ShouldRelay(const int32_t cycles, const int32_t interval)
{
    return (cycles % interval) == 0;
}

// ----- max
//   a
// ----- setpoint (must be above threshold)
//   b
// ----- min
static Sockets CheckStability(Sockets sockets, const int32_t setpoint, const int32_t min, const int32_t max)
{
    const int32_t a = max - setpoint;
    const int32_t b = setpoint - min;
    const int32_t window = 3;
    const int32_t threshold = 60;
    sockets.is_stable = setpoint > threshold && a < window && b < window;
    return sockets;
}

static void CheckParity(const Sockets sockets)
{
    if(sockets.is_stable)
        for(int32_t j = 0; j < COLOR_COUNT; j++)
        {
            const int32_t cycles_check = sockets.cycles[j];
            const int32_t parity_check = sockets.parity[j];
            for(int32_t i = 0; i < COLOR_COUNT; i++)
            {
                const int32_t cycles = sockets.cycles[i];
                const int32_t parity = sockets.parity[i];
                if((cycles == cycles_check)
                && (parity != parity_check)) // XXX. Make this kill the client.
                    Util_Bomb("CLIENT_ID %d :: OUT OF SYNC - PARITY MISMATCH BETWEEN CLIENTS\n", i);
            }
        }
}

static Sockets CountConnectedPlayers(Sockets sockets)
{
    int32_t count = 0;
    for(int32_t i = 0; i < COLOR_COUNT; i++)
    {
        TCPsocket socket = sockets.socket[i];
        if(socket != NULL)
            count += 1;
    }
    sockets.users_connected = count;
    return sockets;
}

static bool GetGameRunning(const Sockets sockets)
{
    return sockets.users_connected == sockets.users;
}

Sockets Sockets_Relay(Sockets sockets, const int32_t cycles, const int32_t interval)
{
    if(ShouldRelay(cycles, interval))
    {
        const int32_t setpoint = GetCycleSetpoint(sockets);
        const int32_t min = GetCycleMin(sockets);
        const int32_t max = GetCycleMax(sockets);
        sockets = CalculateControlChars(sockets, setpoint);
        sockets = CheckStability(sockets, setpoint, min, max);
        sockets = CountConnectedPlayers(sockets);
        const bool game_running = GetGameRunning(sockets);
        Print(sockets, setpoint);
        CheckParity(sockets);
        Send(sockets, max, game_running);
        return Clear(sockets);
    }
    return sockets;
}

Sockets Sockets_Accept(const Sockets sockets)
{
    const TCPsocket client = SDLNet_TCP_Accept(sockets.self);
    return (client != NULL)
        ? Add(sockets, client)
        : sockets;
}
