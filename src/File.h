#pragma once

#include "Type.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int32_t id;
    int32_t data_offset;
    int32_t size;
}
File;

void File_Print(const File);

File File_Load(FILE* const);

#define FILE_ZERO (0)
#define FILE_ONE  (1)

#define FILE_VILLAGER_MAX_SPEED  (130)
#define FILE_VILLAGER_ACCEL       (60)
#define FILE_VILLAGER_HEALTH      (50)
#define FILE_VILLAGER_ATTACK      ( 5)
#define FILE_VILLAGER_WIDTH     (2200)

#define FILE_KNIGHT_MAX_SPEED  (180)
#define FILE_KNIGHT_ACCEL      ( 80)
#define FILE_KNIGHT_HEALTH     (200)
#define FILE_KNIGHT_ATTACK      (13)
#define FILE_KNIGHT_WIDTH     (3400)

#define FILE_TEUTONIC_KNIGHT_MAX_SPEED (100)
#define FILE_TEUTONIC_KNIGHT_ACCEL      (80)
#define FILE_TEUTONIC_KNIGHT_HEALTH    ( 60)
#define FILE_TEUTONIC_KNIGHT_ATTACK     (18)
#define FILE_TEUTONIC_KNIGHT_WIDTH    (2500)

// File priority ensures the software renderer will not render the same pixel twice.
// This is essentially a Z-Buffer stored in the alpha channel of all VRAM pixels.
// Larger priorities take precedence, and may not be drawn over by smaller priorities.

#define FILE_PRIO_TERRAIN  (  1)
#define FILE_PRIO_SHADOW   (  2)
#define FILE_PRIO_DECAY    (  3)
#define FILE_PRIO_FALLING  (  5)
#define FILE_PRIO_UNIT     (  5)
#define FILE_PRIO_BUILDING (  5)
#define FILE_PRIO_HIGHEST  (255)

#define FILE_BERRY_BUSH_HEALTH  (100)
#define FILE_BERRY_BUSH_WIDTH  (4000)

#define FILE_FEUDAL_BARRACKS_HEALTH (500)
#define FILE_DIMENSIONS_BARRACKS {3,3}

#define FILE_FEUDAL_HOUSE_HEALTH (500)
#define FILE_DIMENSIONS_HOUSE {3,2}

// For more details about X-Macros:
//   https://en.wikipedia.org/wiki/X_Macro
//
// NOTE: This X-Macro is used in:
//   Interfac.[ch]
//   Graphics.[ch]
//   Terrain.[ch]
//   Registrar.c
//
// To get an idea of how the X-Macro expands in Graphics.c, for instance, run:
//
//   gcc -E Graphics.c | clang-format

#define FILE_NONE (-1)
#define FILE_DIMENSIONS_ZERO {0,0}

//  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  | NAME                                         | FILE         | PRIO              | WALKABLE  | TYPE                  | MAX_SPEED                     | ACCEL                     | HEALTH                     | ATTACK                     | WIDTH                     | ROTATABLE  | SINGLE_FRAME | MULTI_STATE  | EXPIRE  | BUILDING | DIMENSIONS           //  XXX. REQUIRES A BUILDING CHECK. XXX. NAMES MUST MATCH TRAIT NAMES EXACTLY.
//  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define FILE_X_INTERFAC \
    FILE_X(FILE_INTERFAC_NONE,                       (FILE_NONE),   FILE_PRIO_HIGHEST,  (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO)

#define FILE_X_TERRAIN \
    FILE_X(FILE_TERRAIN_NONE,                        (FILE_NONE),   FILE_PRIO_HIGHEST,  (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_DIRT,                                     (   0),   FILE_PRIO_TERRAIN,  (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_GRASS,                                    (   1),   FILE_PRIO_TERRAIN,  (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_WATER,                                    (   2),   FILE_PRIO_TERRAIN,  (false),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_FARM,                                     (   3),   FILE_PRIO_TERRAIN,  (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO)

#define FILE_X_GRAPHICS \
    FILE_X(FILE_GRAPHICS_NONE,                       (FILE_NONE),   FILE_PRIO_HIGHEST,  (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   1000,                       (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_FEUDAL_BARRACKS_NORTH_EUROPEAN,           (  38),   FILE_PRIO_BUILDING, (false),    TYPE_BARRACKS,          FILE_ZERO,                      FILE_ZERO,                  FILE_FEUDAL_BARRACKS_HEALTH, FILE_ZERO,                   FILE_ZERO,                  (false),     (true),        (false),       (false),  (true),    FILE_DIMENSIONS_BARRACKS) \
    FILE_X(FILE_KNIGHT_FIGHTING,                          ( 190),   FILE_PRIO_UNIT,     (true ),    TYPE_KNIGHT,            FILE_KNIGHT_MAX_SPEED,          FILE_KNIGHT_ACCEL,          FILE_KNIGHT_HEALTH,          FILE_KNIGHT_ATTACK,          FILE_KNIGHT_WIDTH,          (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_KNIGHT_FALLING,                           ( 191),   FILE_PRIO_FALLING,  (true ),    TYPE_KNIGHT,            FILE_KNIGHT_MAX_SPEED,          FILE_KNIGHT_ACCEL,          FILE_KNIGHT_HEALTH,          FILE_KNIGHT_ATTACK,          FILE_KNIGHT_WIDTH,          (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_KNIGHT_IDLE,                              ( 192),   FILE_PRIO_UNIT,     (true ),    TYPE_KNIGHT,            FILE_KNIGHT_MAX_SPEED,          FILE_KNIGHT_ACCEL,          FILE_KNIGHT_HEALTH,          FILE_KNIGHT_ATTACK,          FILE_KNIGHT_WIDTH,          (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_KNIGHT_DECAYING,                          ( 193),   FILE_PRIO_DECAY,    (true ),    TYPE_KNIGHT,            FILE_KNIGHT_MAX_SPEED,          FILE_KNIGHT_ACCEL,          FILE_KNIGHT_HEALTH,          FILE_KNIGHT_ATTACK,          FILE_KNIGHT_WIDTH,          (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_KNIGHT_MOVING,                            ( 194),   FILE_PRIO_UNIT,     (true ),    TYPE_KNIGHT,            FILE_KNIGHT_MAX_SPEED,          FILE_KNIGHT_ACCEL,          FILE_KNIGHT_HEALTH,          FILE_KNIGHT_ATTACK,          FILE_KNIGHT_WIDTH,          (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_STONE_MINE,                               ( 334),   FILE_PRIO_UNIT,     (false),    TYPE_STONE_MINE,        FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TREE_STUMPS,                              ( 335),   FILE_PRIO_UNIT,     (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TREE_CHOPPED_DOWN,                        ( 370),   FILE_PRIO_UNIT,     (false),    TYPE_TREE_CHOPPED_DOWN, FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_FIGHTING,                   ( 430),   FILE_PRIO_UNIT,     (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_FALLING,                    ( 431),   FILE_PRIO_FALLING,  (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_IDLE,                       ( 432),   FILE_PRIO_UNIT,     (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_DECAYING,                   ( 433),   FILE_PRIO_DECAY,    (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_MOVING,                     ( 434),   FILE_PRIO_UNIT,     (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TEUTONIC_KNIGHT_FIGHTING,                 ( 359),   FILE_PRIO_UNIT,     (true ),    TYPE_TEUTONIC_KNIGHT,   FILE_TEUTONIC_KNIGHT_MAX_SPEED, FILE_TEUTONIC_KNIGHT_ACCEL, FILE_TEUTONIC_KNIGHT_HEALTH, FILE_TEUTONIC_KNIGHT_ATTACK, FILE_TEUTONIC_KNIGHT_WIDTH, (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TEUTONIC_KNIGHT_FALLING,                  ( 360),   FILE_PRIO_FALLING,  (true ),    TYPE_TEUTONIC_KNIGHT,   FILE_TEUTONIC_KNIGHT_MAX_SPEED, FILE_TEUTONIC_KNIGHT_ACCEL, FILE_TEUTONIC_KNIGHT_HEALTH, FILE_TEUTONIC_KNIGHT_ATTACK, FILE_TEUTONIC_KNIGHT_WIDTH, (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TEUTONIC_KNIGHT_IDLE,                     ( 361),   FILE_PRIO_UNIT,     (true ),    TYPE_TEUTONIC_KNIGHT,   FILE_TEUTONIC_KNIGHT_MAX_SPEED, FILE_TEUTONIC_KNIGHT_ACCEL, FILE_TEUTONIC_KNIGHT_HEALTH, FILE_TEUTONIC_KNIGHT_ATTACK, FILE_TEUTONIC_KNIGHT_WIDTH, (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TEUTONIC_KNIGHT_DECAYING,                 ( 362),   FILE_PRIO_DECAY,    (true ),    TYPE_TEUTONIC_KNIGHT,   FILE_TEUTONIC_KNIGHT_MAX_SPEED, FILE_TEUTONIC_KNIGHT_ACCEL, FILE_TEUTONIC_KNIGHT_HEALTH, FILE_TEUTONIC_KNIGHT_ATTACK, FILE_TEUTONIC_KNIGHT_WIDTH, (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_TEUTONIC_KNIGHT_MOVING,                   ( 363),   FILE_PRIO_UNIT,     (true ),    TYPE_TEUTONIC_KNIGHT,   FILE_TEUTONIC_KNIGHT_MAX_SPEED, FILE_TEUTONIC_KNIGHT_ACCEL, FILE_TEUTONIC_KNIGHT_HEALTH, FILE_TEUTONIC_KNIGHT_ATTACK, FILE_TEUTONIC_KNIGHT_WIDTH, (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_WOODCUTTER_CHOPPING,        ( 452),   FILE_PRIO_UNIT,     (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_WOODCUTTER_CARRYING,        ( 453),   FILE_PRIO_UNIT,     (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_WOODCUTTER_FALLING,         ( 454),   FILE_PRIO_FALLING,  (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_WOODCUTTER_IDLE,            ( 455),   FILE_PRIO_UNIT,     (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_MALE_VILLAGER_WOODCUTTER_DECAYING,        ( 456),   FILE_PRIO_DECAY,    (true ),    TYPE_VILLAGER,          FILE_VILLAGER_MAX_SPEED,        FILE_VILLAGER_ACCEL,        FILE_VILLAGER_HEALTH,        FILE_VILLAGER_ATTACK,        FILE_VILLAGER_WIDTH,        (true),      (false),       (true),        (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_FEUDAL_HOUSE_NORTH_EUROPEAN,              ( 569),   FILE_PRIO_BUILDING, (false),    TYPE_HOUSE,             FILE_ZERO,                      FILE_ZERO,                  FILE_FEUDAL_HOUSE_HEALTH,    FILE_ZERO,                   FILE_ZERO,                  (false),     (true),        (false),       (false),  (true),    FILE_DIMENSIONS_HOUSE) \
    FILE_X(FILE_FOREST_TREE_SHADOW,                       ( 587),   FILE_PRIO_SHADOW,   (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_BERRY_BUSH,                               ( 698),   FILE_PRIO_UNIT,     (false),    TYPE_BERRY_BUSH,        FILE_ZERO,                      FILE_ZERO,                  FILE_BERRY_BUSH_HEALTH,      FILE_ZERO,                   FILE_BERRY_BUSH_WIDTH,      (false),     (true ),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_WAYPOINT_FLAG,                            ( 795),   FILE_PRIO_UNIT,     (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ONE, /* else crash? */  FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_RIGHT_CLICK_RED_ARROWS,                   ( 796),   FILE_PRIO_UNIT,     (true ),    TYPE_NONE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ONE,                    FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (true),   (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_GOLD_MINE,                                (1297),   FILE_PRIO_UNIT,     (false),    TYPE_GOLD_MINE,         FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO) \
    FILE_X(FILE_FOREST_TREE,                              (1410),   FILE_PRIO_UNIT,     (false),    TYPE_TREE,              FILE_ZERO,                      FILE_ZERO,                  FILE_ZERO,                   FILE_ZERO,                   FILE_ZERO,                  (false),     (false),       (false),       (false),  (false),   FILE_DIMENSIONS_ZERO)
