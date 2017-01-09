/**
 * Header file responsible for declaring all the new types used throughout this project
 * All enums are declared using typedef, defining them as new types for usability
 */

#ifndef COURSEWORK_TYPES_H
#define COURSEWORK_TYPES_H

/**
 * Tile types defined as enum:
 *      W: Wall
 *      G: Gate
 *      P: Portal
 *      n: Non-Filled Path
 *      o: Pill
 *      e: Eaten Pill
 *      O: Big Pill
 *      E: Eaten Big Pill
 *      F: Fruit
 * Tile is defined as new type for ease of use.
 * Each tile-type is used to determine behaviour of Pacman & Ghosts.
 */
typedef enum {W, G, P, n, o, e, O, E, F} tile;

// Enum defines possible movement directions
typedef enum {NONE, UP, RIGHT, DOWN, LEFT} direction;

// Enum defines possible ghost colours
typedef enum {RED, PINK, BLUE, YELLOW} color;

// Enum defines possible ghost movement types
typedef enum {CHASE, SCATTER, FRIGHTENED, DEAD, LEAVE, SPAWN} movement;

/**
 * Game modes defined as enum:
 *      READY:    Set game to starting state, display READY! text until game begins
 *      PLAY:     Game is in play
 *      FRUIT:    Pause game briefly during play upon eating a fruit
 *      EAT:      Pause game briefly during play upon eating a ghost - the two differ only in what is drawn
 *      PAUSE:    Game is paused, draw help screen
 *      DEATH:    Pacman has been eaten, play death sequence
 *      GAMEOVER: Game is over, display GAME OVER text until quit/restart
 * Gamemode is defined as new type for ease of use.
 * Default starting mode is READY
 */
typedef enum {READY, PLAY, FRUIT, EAT, PAUSE, DEATH, GAMEOVER} gamemode;

#endif //COURSEWORK_TYPES_H
