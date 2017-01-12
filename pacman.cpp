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
#include <chrono>
#include <unistd.h>
using namespace std;    // No need to write std::-bla all the time
using namespace std::chrono;    // No need to write std::chrono::-bla all the time

// Lab header files
#include "png_load.h"
#include "load_and_bind_texture.h"

// Custom header files
#include "types.h"
#include "textures.h"
#include "map.h"
#include "ui.h"
#include "pacman.h"
#include "ghosts.h"
#include "globals.h"

/**
 * Time stamping frames under the guise of a fixed frame rate helps to ensure the game plays similarly across all systems
 * On my laptop, for example, the idle loop was able to iterate much more quickly than my desktop
 * This resulted in a wildly different game experience
 *
 * By using a fixed frame rate, the game loop can only iterate slower than a fixed rate (30fps)
 * This prevents high-performance systems rendering frames more quickly than the player can comprehend them
 */
float frameLength = 1000/30;    // Length of a single frame in ms (30fps)
milliseconds last;      // Init last frame time
milliseconds now;       // Init current frame time

/**
 * Compute all game logic prior to redrawing anything
 * Logic to compute varies on gamemode
 */
void gameLoop()
{
    // Set current frame time to current time
    now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    // If the elapsed time since the last frame is less than the frame length, sleep until the frame should be drawn
    if(float(now.count() - last.count()) < frameLength)
        usleep((frameLength - (now.count() - last.count())) * 1000);
    // As current frame is now being rendered, set last frame to current frame for next game loop iteration
    last = now;

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
                checkCollisions();      // Check Pac-Man's collisions with pills and ghosts
                pacman.move();          // Move Pac-Man
                checkCollisions();      // Check collisions again to ensure simultaneous tile switches register correct collisions
                aiWave();               // Update the ghost AI targeting wave
                // Move each ghost - pass RED ghost for BLUE's CHASE mode AI
                for(int i = 0; i < 4; i++)
                    ghosts[i].move(ghosts[0]);
                // If no fruit is currently spawned, enough pills have been eaten,
                // The eaten fruit count doesn't exceed the level and a random quantifier is satisfied, spawn a fruit
                if(!fruitSpawned && fruits < level && pillsLeft <= 240 - 30 &&  rand() % 1500 == 0)
                    spawnFruit();
            }
            else
            {
                if(ticks == timestamp + 90)     // If timestamp is set, incur a short pause
                {                               // Timestamp is only set in PLAY-mode when Pac-Man dies or level is complete
                    if(pillsLeft == 0)          // If no pills remain, level is complete
                    {                           // Reset map & pill count and enter READY-mode for next level
                        pillsLeft = 244;
                        level++;
                        resetMap();
                        resetLevel();
                    }
                    else                        // If there are still pills remaining, Pac-Man has died
                    {
                        timestamp = ticks;      // Set timestamp for correct death animation timing
                        mode = DEATH;           // Enter DEATH-mode
                    }
                }
            }
            break;
        case FRUIT:     // Pause the game briefly on eating a fruit
        case EAT:       // Also pause on eating a ghost - logical behaviour is identical so overflow switch case
            if(ticks == timestamp + 90)
            {
                timestamp = -1;
                pacman.startChomping();
                mode = PLAY;
            }
            break;
        case DEATH:     // 180 ticks after death, enter READY (reset level) or GAMEOVER mode depending on remaining lives
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
 * Draw all elements of the game, depending on the game mode
 * Common elements have been extracted to methods in globals.h for simplicity of code
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
    // Update Pac-Man's direction, pause/unpause or restart game depending on game mode
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
    // Init start time for frame rate cap
    last = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
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
