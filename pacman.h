/**
 * Header file responsible for drawing and handling Pacman
 */

#ifndef COURSEWORK_PACMAN_H
#define COURSEWORK_PACMAN_H

// Allow access of ticks, count of remaining pills, number of fruits eaten and fruit spawned flag from main file
extern int ticks;
extern int pillsLeft;
extern int fruits;
extern bool fruitSpawned;

// Enum defines possible movement directions, defined as new type for usability
typedef enum {NONE, UP, RIGHT, DOWN, LEFT} direction;


/**
 * For ease of reference and handling Pacman, he is defined as an object type
 * All variables are private, not needing to be accessed externally
 * If variables are externally required, getters and setters are provided
 */
class Pacman
{
private:
    /// List of private variables which Pacman uses
    float x;                // X position relative to map - float allows for smooth movement between tiles
    float y;                // Y position relative to map - float allows for smooth movement between tiles
    float angle;            // Angle at which to draw the sprite - class var to retain angle when dir=NONE
    direction dir;          // Direction of movement
    direction tempDir;      // Temporary direction storage
    direction saveDir;      // Secondary direction storage for stopping and starting animation
    int tex_count;          // Counter to determine which texture to draw
    float dead_tex_count;   // Counter to determine which sprite of death animation sequence to draw
    bool ready;             // Flag prevents incorrect Pacman texture or position rounding at start of game

public:
    /**
     * Constructor & Reset methods initialise all variables to starting state
     */
    Pacman()
    {
        x = 13.5f;
        y = 7.0f;
        angle = 0.0f;
        dir = NONE;
        tempDir = NONE;
        tex_count = 10;
        dead_tex_count = 0;
        ready = false;
    }
    void reset()
    {
        x = 13.5f;
        y = 7.0f;
        angle = 0.0f;
        dir = NONE;
        tempDir = NONE;
        tex_count = 10;
        dead_tex_count = 0;
        ready = false;
    }

    /**
     * Determines and returns absolute X coordinate of map tile on which Pacman resides
     *
     * @return - integer, X coordinate of current tile
     */
    int getX()
    {
        return round(x);
    }

    /**
     * Determines and returns absolute Y coordinate of map tile on which Pacman resides
     *
     * @return - integer, Y coordinate of current tile
     */
    int getY()
    {
        return round(y);
    }

    /**
     * Determines and returns the next tile in the given direction of movement
     *
     * @param d - direction of movement in which to check to the next tile
     * @return -  next tile in direction d
     */
    tile getNextTile(direction d)
    {
        // Return next tile in given direction
        switch(d)
        {
            case UP:
                return getTile(getX(),getY() + 1);
            case RIGHT:
                return getTile(getX() + 1,getY());
            case DOWN:
                return getTile(getX(),getY() - 1);
            case LEFT:
                return getTile(getX() - 1,getY());
            default:
                return getTile(getX(),getY());  // If d=NONE, return current tile
        }
    }

    /**
     * Determines whether Pacman is currently at the center of a tile
     * Expression basically validates that the first decimal point of each coordinate is a zero
     * If each is a zero, Pacman is at the center of his tile
     *
     * @return - boolean, true if at center
     */
    bool atTileCenter()
    {
        return (int)round(y * 10.0f) % 10 == 0 && (int)round(x * 10.0f) % 10 == 0;
    }

    /**
     * Return Pacman's current direction
     *
     * @return - direction of movement
     */
    direction getDirection()
    {
        return dir;
    }

    /**
     * Set Pacman's next direction to the given input.
     *
     * @param d - direction in which to face when possible
     */
    void setDirection(direction d)
    {
        tempDir = d;
    }

    /**
     * Method handles all movement functionality of Pacman, updating his position and direction
     *
     * Directional updates occur under the following cases:
     *      If at tile center and proposed new direction is not a WALL or GATE, update current direction
     *      If at tile center and the current direction results in a wall collision, stop moving
     *
     * With any direction changes complete, move Pacman in current direction by set amount (10% of tile)
     *      Every movement rounds the unchanged position coordinate, preventing buggy direction change detection
     *      If not moving, round both position coordinates to ensure Pacman is at tile center
     */
    void move()
    {
        // Ascertain whether direction can be changed
        // Direction can only be changed at the center of a tile
        if(atTileCenter())
        {
            if(!isImpassible(getNextTile(tempDir))) // If the proposed direction is not impassible, update direction
                dir = tempDir;
            else if(isImpassible(getNextTile(dir))) // If the current direction is impassible, set dir=NONE
                dir = NONE;
        }
        // The only exception to the above rule is at game start (when ready=false), as Pacman starts between two tiles
        if(!ready && tempDir != NONE && !isImpassible(getNextTile(tempDir)))
        {
            dir = tempDir;
            if(!ready)
                ready = true;   // Ready flag is set to true, enabling position rounding when dir=NONE and correct texture drawing
        }


        // Perform smooth movement between tiles in the current direction of movement
        // While moving, round the unchanging position coordinate, preventing directional query mishaps
        switch(dir)
        {
            case UP:
                y += 0.1f;
                x = round(x);
                break;
            case RIGHT:
                x += 0.1f;
                y = round(y);
                break;
            case DOWN:
                y -= 0.1f;
                x = round(x);
                break;
            case LEFT:
                x -= 0.1f;
                y = round(y);
                break;
            default:                // If not moving, round both coordinates, centering Pacman within the tile
                if(ready)           // Only do if Pacman has already moved (ready=true)
                {                   // This allows starting X position to be non-rounded
                    x = round(x);
                    y = round(y);
                }
                break;
        }
    }

    /**
     * "Eat" the current tile:
     *      Pill: empty the array position appropriately, reduce remaining pill count and return score
     *      Portal: teleport to the opposite portal based on Pacman's direction of movement
     *      Fruit: increment consumed fruit count, empty array position and determine & return how much the fruit is worth
     * Only eat current tile if at tile center
     *
     * @return - integer score increment from eating tile
     */
    int eat()
    {
        if(atTileCenter())
        {
            switch(getTile(getX(),getY()))
            {
                case o:
                    setTile(getX(),getY(),e);
                    pillsLeft--;
                    return 10;
                case O:
                    setTile(getX(),getY(),E);
                    pillsLeft--;
                    return 50;
                case P:
                    if(dir == RIGHT)
                        x = 1;
                    else
                        x = 26;
                    return 0;
                case F:
                    setTile(getX(),getY(),e);
                    fruitSpawned = false;
                    switch(fruits++)
                    {
                        case 0:
                            return 100;
                        case 1:
                            return 300;
                        case 2:
                            return 500;
                        case 3:
                            return 700;
                        case 4:
                            return 1000;
                        case 5:
                            return 2000;
                        case 6:
                            return 3000;
                        case 7:
                            return 5000;
                    }
            }
        }
        return 0;
    }

    /**
     * Draw Pacman at his current location
     */
    void draw()
    {
        glPushMatrix();

        translateMapOrigin();               // Translate to map origin
        translateMapCoords(x,y);            // Translate to current (x,y)
        glTranslatef(-2.0f, -2.0f, 0.0f);   // Account for over-sized sprite (13x13 on 8x8 tile)

        // Determine rotation angle of sprite based on direction
        switch(dir)
        {
            case UP:
                angle = 270.0f;
                break;
            case RIGHT:
                angle = 180.0f;
                break;
            case DOWN:
                angle = 90.0f;
                break;
            case LEFT:
                angle = 0.0f;
                break;
        }

        // Determine which texture to draw based on tick-incremented counter
        unsigned int pacman_tex;
        if(tex_count % 20 < 5)
            pacman_tex = pac_0_tex;
        else if(tex_count % 20 < 10 || tex_count % 20 >= 15)
            pacman_tex = pac_1_tex;
        else
            pacman_tex = pac_2_tex;

        // Draw Pacman sprite with determined texture at determined angle
        drawSprite(pacman_tex, 13, 13, angle);

        // Increment texture counter only if moving
        // If stationary, continue until sprite animation cycle is complete
        if(!(dir == NONE && tex_count % 20 < 5) && ready)
            tex_count++;

        glPopMatrix();
    }

    /**
     * Stop Pacman's eating animation when he dies/completes level or pause it upon eating a ghost - also save tempDir
     */
    void stopChomping()
    {
        saveDir = tempDir;
        tempDir = NONE;
        ready = false;
    }

    /**
     * Restart Pacman's eating animation, resetting tempDir if it hasn't since been changed
     */
    void startChomping()
    {
        if(tempDir == NONE)
            tempDir = saveDir;
        ready = true;
    }

    /**
     * Draw Pacman's death animation sequence at his current location
     */
    void drawDead()
    {
        glPushMatrix();

        translateMapOrigin();               // Translate to map origin
        translateMapCoords(x,y);            // Translate to current (x,y)
        glTranslatef(-3.0f, -4.0f, 0.0f);   // Account for over-sized sprite (15x15 on 8x8 tile)

        // Determine which texture to draw based on tick-incremented counter
        unsigned int dead_tex;
        switch((int)floor(dead_tex_count / 5))
        {
            case 0:
                dead_tex = dead_0_tex;  break;
            case 1:
                dead_tex = dead_1_tex;  break;
            case 2:
                dead_tex = dead_2_tex;  break;
            case 3:
                dead_tex = dead_3_tex;  break;
            case 4:
                dead_tex = dead_4_tex;  break;
            case 5:
                dead_tex = dead_5_tex;  break;
            case 6:
                dead_tex = dead_6_tex;  break;
            case 7:
                dead_tex = dead_7_tex;  break;
            case 8:
                dead_tex = dead_8_tex;  break;
            case 9:
                dead_tex = dead_9_tex;  break;
            case 10:
                dead_tex = dead_10_tex; break;
        }

        // Draw current sprite of Pacman's death animation sequence
        if(dead_tex_count < 55)
            drawSprite(dead_tex, 15, 15, 0);

        // Increment dead texture counter
        dead_tex_count++;

        glPopMatrix();
    }

    /**
     * Upon eating a fruit, draw the score for eating said fruit during the short pause INSTEAD of drawing Pacman
     */
    void drawFruitScore()
    {
        glPushMatrix();

        translateMapOrigin();               // Translate to map origin
        translateMapCoords(x,y);            // Translate to current (x,y)
        glTranslatef(-6.0f, 0.0f, 0.0f);   // Account for over-sized sprite (20x8 on 8x8 tile)

        // Determine which fruit score texture to draw based on how many fruits have been eaten
        drawSprite(f_score_tex[fruits - 1], 20, 8, 0);

        glPopMatrix();
    }
};

#endif //COURSEWORK_PACMAN_H
