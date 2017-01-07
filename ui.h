/**
 * Header file responsible for drawing all UI elements. Also reads/writes high score to file.
 */

#ifndef COURSEWORK_UI_H
#define COURSEWORK_UI_H

// Allow access of score, level and lives from main file
extern int score;
extern int level;
extern int lives;

// Initialise high score as integer to be set on init()
int highscore;

/**
 * Draw READY! tooltip, which is displayed prior to the game playing
 */
void drawReady()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    translateMapCoords(11,13);          // Translate to point within map at which READY! tooltip should be drawn
    drawSprite(ready_tex, 48, 8, 0);    // Draw READY! sprite at current location

    glPopMatrix();
}

/**
 * Draw GAME OVER tooltip, which is displayed once all lives are lost
 */
void drawGameover()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    translateMapCoords(9,13);           // Translate to point within map at which GAME OVER tooltip should be drawn
    drawSprite(gameover_tex, 80, 8, 0); // Draw GAME OVER sprite at current location

    glPopMatrix();
}

/**
 * Draws a number as a set of sprites
 *
 * @param number - integer number to draw
 */
void drawNumberAsSprite(int number)
{
    glPushMatrix();

    string str = to_string(number);   // Convert number to string to allow iteration
    for(int i = str.length() - 1; i >= 0; i--)  // Draw each digit as an individual sprite
    {
        switch(str[i])
        {
            case '0':
                drawSprite(num_0_tex, 8, 8, 0);     // Draw number 0 sprite at current location
                break;
            case '1':
                drawSprite(num_1_tex, 8, 8, 0);     // Draw number 1 sprite at current location
                break;
            case '2':
                drawSprite(num_2_tex, 8, 8, 0);     // Draw number 2 sprite at current location
                break;
            case '3':
                drawSprite(num_3_tex, 8, 8, 0);     // Draw number 3 sprite at current location
                break;
            case '4':
                drawSprite(num_4_tex, 8, 8, 0);     // Draw number 4 sprite at current location
                break;
            case '5':
                drawSprite(num_5_tex, 8, 8, 0);     // Draw number 5 sprite at current location
                break;
            case '6':
                drawSprite(num_6_tex, 8, 8, 0);     // Draw number 6 sprite at current location
                break;
            case '7':
                drawSprite(num_7_tex, 8, 8, 0);     // Draw number 7 sprite at current location
                break;
            case '8':
                drawSprite(num_8_tex, 8, 8, 0);     // Draw number 8 sprite at current location
                break;
            case '9':
                drawSprite(num_9_tex, 8, 8, 0);     // Draw number 9 sprite at current location
                break;
        }
        translateMapCoords(-1,0);   // Translate one tile left for next digit
    }
    // If number is a single digit, justify with another zero
    if(str.length() == 1)
        drawSprite(num_0_tex, 8, 8, 0);

    glPopMatrix();
}

/**
 * Draw game score and any previously attained high score
 */
void drawScore()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    translateMapCoords(6.5,32.5);       // Translate to point above map at which the score tooltip should be drawn
    drawSprite(score_tex, 80, 8, 0);    // Draw SCORE tooltip at current location

    translateMapCoords(4,-1);           // Translate to point above map at which the high score should be drawn
    drawNumberAsSprite(highscore);      // Draw high score sprites at current location

    translateMapCoords(6,0);            // Translate to point above map at which the score should be drawn
    drawNumberAsSprite(score);          // Draw score sprites at current location

    glPopMatrix();
}

/**
 * Draw game level
 */
void drawLevel()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    translateMapCoords(1,32.5);         // Translate to point above map at which the 1UP tooltip should be drawn
    drawSprite(one_up_tex, 24, 8, 0);   // Draw 1UP tooltip at current location

    translateMapCoords(3,-1);           // Translate to point above map at which the level should be drawn
    drawNumberAsSprite(level);          // Draw level sprites at new location

    glPopMatrix();
}

/**
 * Draw lives count, displayed beneath the map
 */
void drawLives()
{
    glPushMatrix();

    translateMapOrigin();                   // Translate to map origin
    translateMapCoords(3,-2);               // Translate to point beneath map, from which lives should be drawn
    for(int i = 0; i < lives; i++)
    {
        drawSprite(life_tex, 14, 14, 0);    // Draw life counter sprite at current location
        translateMapCoords(2,0);            // Translate to right where next life counter sprite should be drawn
    }

    glPopMatrix();
}

/**
 * Draw HELP tooltip when playing game
 */
void drawHelp()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    translateMapCoords(19,32);          // Translate to point above map at which the HELP tooltip should be drawn
    drawSprite(help_tex, 64, 8, 0);     // Draw HELP tooltip at current location

    glPopMatrix();
}

/**
 * Draw PAUSE screen
 */
void drawPause(bool gameover)
{
    glPushMatrix();

    translateMapOrigin();                       // Translate to map origin
    if(!gameover)
        drawSprite(pause_tex, 224, 248, 0);     // Draw PAUSE screen as a sprite
    else
        drawSprite(pause_alt_tex, 224, 248, 0); // Draw alternate PAUSE screen as a sprite (restart text only when mode=GAMEOVER)

    glPopMatrix();
}

/**
 * Draw QUIT tooltip when mode=PAUSE
 */
void drawQuit()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    translateMapCoords(19,32);          // Translate to point above map at which the QUIT tooltip should be drawn
    drawSprite(quit_tex, 64, 8, 0);     // Draw QUIT tooltip at current location

    glPopMatrix();
}

/**
 * Get high score from locally stored file - if no such file exists, create file with value=0
 */
void getHighscore()
{
    fstream file("highscore.txt");
    if(file.good())
        file >> highscore;
    else
    {
        ofstream newFile("highscore.txt");
        newFile << 0;
        newFile.close();
    }
    file.close();

}

/**
 * Write high score to file, allowing persistence after quitting the game - if file does not already exist, create it
 */
void setHighscore()
{
    ofstream file("highscore.txt");
    file.clear();
    file << highscore;
    file.close();
}

#endif //COURSEWORK_UI_H
