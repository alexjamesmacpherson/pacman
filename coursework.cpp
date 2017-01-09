/**
 * Main file responsible for running the game.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <png.h>
#include <vector>
#include <fstream>
using namespace std;    // No need to write std::-bla all the time

// Lab header files
#include "png_load.h"
#include "load_and_bind_texture.h"

// Custom header files
#include "textures.h"
#include "map.h"
#include "ui.h"
#include "pacman.h"
#include "ghosts.h"

// Game ticks, enabling crude timing based on the rendering speed of the game
int ticks = 0;
/**
 * Timestamp represents symbolic points in the game, allowing specific timing from a certain tick:
 *      -1: default, unset state
 * Set to current ticks when Pacman dies, creating a pause before entering DEATH-mode
 * Set to current ticks on entering DEATH-mode, ensuring READY-mode is entered a given number of ticks later
 */
int timestamp = -1;

// Game score, level, remaining lives, extra life flag, remaining pills to be eaten, number of fruits consumed and fruit spawned flag all initialised
int score = 0;
int level = 0;
int lives = 2;
bool extraLife = false; // True if received
int pillsLeft = 244;
int fruits = 0;
bool fruitSpawned = false;

// Ghost AI targeting is wave-based, varying between CHASE and SCATTER over time
movement wave = SCATTER;
// Counts how many ghosts have been eaten since consuming the last big pill
int ghostsEaten = 0;

// Initialise Pacman object
Pacman pacman;

// Initialise array of ghosts, passing starting positions and colour
Ghost ghosts[4] =
        {
                Ghost(13.5,19,RED),
                Ghost(13.5,16,PINK),
                Ghost(11.5,16,BLUE),
                Ghost(15.5,16,YELLOW)
        };

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
gamemode mode = READY;
gamemode tempMode;      // Save gamemode when pausing the game

/**
 * Reset level:
 *      Set ticks, timestamp, eaten ghost count and fruit spawned flag to initial values
 *      Call reset() method on Pacman and all Ghosts
 *      Enter READY mode
 * This function is called when advancing level, resetting a level on death, or when restarting the game
 */
void resetLevel()
{
    ticks = 0;
    timestamp = -1;
    pacman.reset();
    wave = SCATTER;
    ghostsEaten = 0;
    fruitSpawned = false;
    for(int i = 0; i < 4; i++)
        ghosts[i].reset();
    mode = READY;
}

/**
 * Called when any key is pressed
 * As mode=GAMEOVER, restart the game by resetting all variables to initial values
 * Also reset map, Pacman and ghosts to default
 */
void restartGame()
{
    score = 0;
    level = 0;
    lives = 2;
    extraLife = false;
    pillsLeft = 244;
    fruits = 0;
    resetMap();
    resetLevel();
}

/**
 * Check all possible collisions:
 *      Call Pacman to eat its current tile, incrementing score accordingly
 *       - If all pills are eaten, move to the next level
 *       - Once score exceeds 10,000, award a bonus life
 *       - On eating a big pill, set ghosts to FRIGHTENED
 *       - Release ghosts from the SPAWN pen after a specific number of pills have been eaten
 *       - If a fruit is eaten, pause the game briefly to display the score for eating it
 *      Check whether Pacman has collided with a ghost
 *       - Set mode=DEATH if collision has occurred with alive ghost
 *       - If the ghost is frightened, eat it (set AI=DEAD)
 */
void checkCollisions()
{
    // Eat current tile, increment score
    int scoreIncrement = pacman.eat();
    score += scoreIncrement;

    if(scoreIncrement == 50)        // If score is increased by 50, a big pill has been eaten - set ghosts to FRIGHTENED
    {
        for(int i = 0; i < 4; i++)
        {
            if(ghosts[i].getAI() == wave || ghosts[i].getAI() == FRIGHTENED)
                ghosts[i].setAI(FRIGHTENED, true);
        }
    }
    else if(scoreIncrement >= 100)  // If score is increased by more than 100, a fruit has been eaten, pause game briefly to show score
    {
        timestamp = ticks;
        pacman.stopChomping();
        mode = FRUIT;
    }

    // Award extra life for reaching 10000 points
    if(!extraLife && score > 10000)
    {
        lives++;
        extraLife = true;
    }

    // If all pills have been eaten, stop Pacman's animation and set timestamp to restart level after short pause
    if(pillsLeft == 0)
    {
        timestamp = ticks;
        pacman.stopChomping();
    }
    // Ghosts exit SPAWN pen when a certain number of pills have been eaten
    // To prevent all piling out at once after a death, tick timers only allow the ghosts to leave after a certain point
    else if(ghosts[2].getAI() == SPAWN && pillsLeft <= 244 - 30 && ticks >= 300)    // BLUE leaves after 30 pills are eaten
        ghosts[2].setAI(LEAVE, false);
    else if(ghosts[3].getAI() == SPAWN && pillsLeft <= 244 * 2/3 && ticks >= 420)   // YELLOW leaves after 1/3 of the pills are eaten
        ghosts[3].setAI(LEAVE, false);

    // Check for ghost collisions
    for(int i = 0; i < 4; i++)
    {
        if(ghosts[i].getX() == pacman.getX() && ghosts[i].getY() == pacman.getY())
        {
            if(ghosts[i].getAI() == wave)   // If the ghost is alive and not FRIGHTENED, Pacman will die
            {                               // Begin DEATH procedure by setting timestamp and stopping Pacman's animation
                timestamp = ticks;
                pacman.stopChomping();
                break;
            }
            else if(ghosts[i].getAI() == FRIGHTENED)    // If ghost is FRIGHTENED, it can be eaten itself
            {                                           // Set ghost AI to DEAD, increasing the score and count of ghosts eaten since the last big pill
                ghosts[i].setAI(DEAD, false);            // Briefly pause the game to show score for eating ghost
                score += 200 * pow(2, min(ghostsEaten++, 3));
                timestamp = ticks;
                pacman.stopChomping();
                mode = EAT;
            }
        }
    }
}

/**
 * Ghost AI targeting mode is set in waves, adding small respite where all enemies back off for a short period
 * Timings of each wave are roughly:
 *      SCATTER:    480 ticks       (begin at ticks =  240 - PLAY-mode start)
 *      CHASE:      900 ticks       (begin at ticks =  720)
 *      SCATTER:    300 ticks       (begin at ticks = 1620)
 *      CHASE:      900 ticks       (begin at ticks = 1920)
 *      SCATTER:    180 ticks       (begin at ticks = 2820)
 *      CHASE:      900 ticks       (begin at ticks = 3000)
 *      SCATTER:    180 ticks       (begin at ticks = 3900)
 *      CHASE:      indefinitely    (begin at ticks = 4100)
 * On changing wave, the ghost's direction is reversed
 */
void aiWave()
{
    bool waveChanged = false;   // Flag to only update AI on a wave change - prevents constantly switching direction as wave is updated every tick
    movement thisWave = wave;
    if(ticks == 720 || ticks == 1920 || ticks == 3000 || ticks == 4100)
    {
        waveChanged = true;
        wave = CHASE;
    }
    else if(ticks == 1620 || ticks == 2820 || ticks == 3900)
    {
        waveChanged = true;
        wave = SCATTER;
    }

    // Set new wave for all ghosts only if they are currently in a wave-based mode
    for(int i = 0; i < 4; i++)
    {
        if(waveChanged && ghosts[i].getAI() == thisWave)
            ghosts[i].setAI(wave, true);
    }
}

/**
 * Compute all game logic prior to redrawing anything
 * Logic to compute varies on gamemode
 */
void gameLoop()
{
    // Perform certain logic depending on game mode
    switch(mode)
    {
        case READY:     // After 240 ticks, enter PLAY mode
            if(ticks > 240)
                mode = PLAY;
            break;
        case PLAY:      // Main play loop
            if(timestamp == -1)         // If timestamp is not set, execute all PLAY-mode logic
            {
                checkCollisions();      // Check Pacman's collisions with pills and ghosts
                pacman.move();          // Move Pacman
                checkCollisions();      // Check collisions again to ensure simultaneous tile switches register correct collisions
                aiWave();               // Update the ghost AI targeting wave
                // Move each ghost - pass RED ghost for BLUE's CHASE mode AI
                for(int i = 0; i < 4; i++)
                    ghosts[i].move(ghosts[0]);
                // If no fruit is currently spawned, enough pills have been eaten,
                // The eaten fruit count doesn't exceed the level and a random quantifier is satisfied, spawn a fruit
                if(!fruitSpawned && fruits <= level && pillsLeft <= 240 - 30 &&  rand() % 1500 == 0)
                    spawnFruit();
            }
            else
            {
                if(ticks == timestamp + 120)    // If timestamp is set, incur a short pause
                {                               // Timestamp is only set in PLAY-mode when Pacman dies or level is complete
                    if(pillsLeft == 0)          // If no pills remain, level is complete
                    {                           // Reset map & pill count and enter READY-mode for next level
                        pillsLeft = 244;
                        level++;
                        resetMap();
                        resetLevel();
                    }
                    else                        // If there are still pills remaining, Pacman has died
                    {
                        timestamp = ticks;      // Set timestamp for correct death animation timing
                        mode = DEATH;           // Enter DEATH-mode
                    }
                }
            }
            break;
        case FRUIT:     // Pause the game briefly on eating a fruit
        case EAT:       // Also pause on eating a ghost - logical behaviour is identical so overflow switch case
            if(ticks == timestamp + 120)
            {
                timestamp = -1;
                pacman.startChomping();
                mode = PLAY;
            }
            break;
        case DEATH:     // 240 ticks after death, enter READY (reset level) or GAMEOVER mode depending on remaining lives
            if(ticks > timestamp + 180)
            {
                if(lives == 0)
                {
                    mode = GAMEOVER;
                    if(score > highscore)
                    {
                        highscore = score;
                        setHighscore();
                    }
                }
                else
                {
                    lives--;        // Decrease remaining lives on death
                    resetFruit();   // Remove any spawned fruits
                    resetLevel();   // Reset characters and variables to retry level
                }
            }
            break;
    }

    // Draw the current frame
    glutPostRedisplay();

    // Increment game ticks once the frame is drawn, but only if not paused
    if(mode != PAUSE)
        ticks++;
}

/**
 * Method tidies up display() switch on gamemode, drawing common PLAY-mode features
 */
void drawPlayScreen()
{
    drawMap();
    drawLevel();
    drawScore();
    drawLives();
    drawFruits();
    drawHelp();
}

/**
 * Method tidies up display() switch on gamemode, drawing characters
 */
void drawCharacters()
{
    pacman.draw();
    for(int i = 0; i < 4; i++)
        ghosts[i].draw();
}

/**
 * Draw all elements of the game, depending on the gamemode
 * Common elements have been extracted to the above methods for simplicity of code
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);   // Clear display buffer colour
    glMatrixMode(GL_MODELVIEW);     // Set matrix mode - no further projection is required in this 2D game
    glLoadIdentity();

    // Draw specific items pertaining to current gamemode
    switch(mode)
    {
        case READY:
            drawPlayScreen();
            drawCharacters();
            drawReady();
            break;
        case PLAY:
            drawPlayScreen();
            drawCharacters();
            break;
        case FRUIT:
            drawPlayScreen();
            for(int i = 0; i < 4; i++)
                ghosts[i].draw();
            pacman.drawFruitScore();
            break;
        case EAT:
            drawPlayScreen();
            for(int i = 0; i < 4; i++)
                ghosts[i].drawEaten();
            break;
        case PAUSE:
            drawPause(tempMode == GAMEOVER);
            drawLevel();
            drawScore();
            drawLives();
            drawFruits();
            drawQuit();
            break;
        case DEATH:
            drawPlayScreen();
            pacman.drawDead();
            break;
        case GAMEOVER:
            drawPlayScreen();
            drawGameover();
            break;
    }

    glutSwapBuffers();
}


/**
 * Keyboard input handlers for all user input
 *      keyboard() - handle normal key input (ie. letters, space bar, ESC key, etc.)
 *      special() -  handle special key input (ie. arrow keys, etc.)
 *
 * @param key - key pressed by user
 */
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 27:    // Escape Key pauses/quits game
            if(mode != PAUSE)
            {
                tempMode = mode;    // Save gamemode to re-enter on unpausing game
                mode = PAUSE;
            }
            else if(mode == PAUSE)
                exit(1);
            break;
        default:    // For any other key, unpause if mode=PAUSE or restart game if mode=GAMEOVER
            if(mode == PAUSE && tempMode != GAMEOVER)
                mode = tempMode;
            else if(mode == GAMEOVER || mode == PAUSE)
                restartGame();
            break;
    }
}
void special(int key, int, int)
{
    // Update Pacman's direction, pause/unpause or restart game depending on game mode
    if(mode == PLAY || mode == EAT || mode == READY)    // Update direction if game is currently playable
    {
        switch (key)
        {
            case GLUT_KEY_UP:       pacman.setDirection(UP);    break;
            case GLUT_KEY_RIGHT:    pacman.setDirection(RIGHT); break;
            case GLUT_KEY_DOWN:     pacman.setDirection(DOWN);  break;
            case GLUT_KEY_LEFT:     pacman.setDirection(LEFT);  break;
        }
    }
    else
    {
        switch(key)
        {
            default:    // For any special key input, unpause if mode=PAUSE or restart game if mode=GAMEOVER
                if(mode == PAUSE && tempMode != GAMEOVER)
                    mode = tempMode;
                else if(mode == GAMEOVER || mode == PAUSE)
                    restartGame();
                break;
        }
    }
}

/**
 * Handler to pause the game (halt the gameLoop function) when minimised
 *
 * @param vis - window visibility, defined as a GLUT variable
 */
void visibility(int vis)
{
    if (vis==GLUT_VISIBLE)
        glutIdleFunc(gameLoop);
    else
        glutIdleFunc(NULL);
}

/**
 * Initialise the world and load and bind all textures
 */
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Each tile is an 8x8 area in world coordinates (WC).
    // Game map is 28x31 tiles (224x248 WC).
    // Window is map size plus a reasonable margin (300x300 WC).
    /// Given the size of the textures I'm using, each point in WC represents a single pixel.
    gluOrtho2D(0, 300, 0, 300);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Set background to black
    loadBindTextures();                     // Load and bind all textures to be used later as sprites
    getHighscore();                         // Retrieve high score from local file, if it exists, otherwise init file with value 0
}

/**
 * Initialise the program, creating handlers for keyboard input and visibility (minimising the game)
 * Calls the init() method to initialise the world and all textures
 * Enters main loop, starting the game
 */
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Pac-Man");
    glutDisplayFunc(display);

    // Keyboard input handlers
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutVisibilityFunc(visibility);

    init();

    glutMainLoop();

    return 0;
}
