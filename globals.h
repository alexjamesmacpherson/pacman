/**
 * Header file responsible for initialising, storing and manipulating global variables and methods
 * Contents include a number of game loop-based functions, moved from the main file to improve readability of code
 */

#ifndef COURSEWORK_GLOBALS_H
#define COURSEWORK_GLOBALS_H

// Game ticks, effectively enacting a frame counter
int ticks = 0;
/**
 * Timestamp represents symbolic points in the game, allowing specific frame-based timing from a certain tick:
 *      -1: default, unset state
 * Set to current ticks in a number of situations, effecting a short pause in the game
 * Set to current ticks on entering DEATH-mode, ensuring READY-mode is entered a given number of ticks later
 */
int timestamp = -1;

// Game mode initialised to READY
gamemode mode = READY;
gamemode tempMode;      // Save gamemode when pausing the game

// Game score, level, remaining lives, extra life flag, remaining pills to be eaten, number of fruits consumed and fruit spawned flag all initialised
int score = 0;
int level = 0;
int lives = 2;
bool extraLife = false; // True if received
int pillsLeft = 244;
int fruits = 0;
bool fruitSpawned = false;

// Counts how many ghosts have been eaten since consuming the last big pill
int ghostsEaten = 0;

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

#endif //COURSEWORK_GLOBALS_H
