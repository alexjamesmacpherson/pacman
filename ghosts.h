/**
 * Header file responsible for drawing and handling Ghosts
 */

#ifndef COURSEWORK_GHOSTS_H
#define COURSEWORK_GHOSTS_H

// Allow access of ticks, eaten ghost count and Pacman from main file
extern int ticks;
extern int ghostsEaten;
extern Pacman pacman;

// Enum defines possible ghost colours, defined as new type for usability
typedef enum {RED, PINK, BLUE, YELLOW} color;
// Enum defines possible ghost movement types, defined as new type for usability
typedef enum {CHASE, SCATTER, FRIGHTENED, DEAD, LEAVE, SPAWN} movement;
extern movement wave;   // Allow access of wave-defined AI targeting mode from main file
// Direction enum already declared in Pacman.h; no need to redeclare type

class Ghost
{
private:
    /// List of private variables which Pacman uses
    float x;        // X position relative to map - float allows for smooth movement between tiles
    float x_init;   // Initial X position stored for later resets
    float y;        // Y position relative to map - float allows for smooth movement between tiles
    float y_init;   // Initial X position stored for later resets
    float d_pos;    // Delta position - the amount the ghost should move each tick
    color colour;   // Colour of ghost
    direction dir;  // Direction of movement
    int tex_count;  // Counter to determine which texture to draw
    movement ai;    // Movement AI type
    bool reverse;   // Flag determining whether to reverse the ghost
    int timeout;    // Timeout used to determine when to leave FRIGHTENED mode AI, -1 = default
    bool drawScore; // Flag determining whether to draw the score for eating this ghost

public:
    /**
     * Constructor & Reset methods initialise all variables to starting state
     */
    Ghost(float x, float y, color c)
    {
        this->x = x;
        x_init = x;
        this->y = y;
        y_init = y;
        d_pos = 0.1f;
        colour = c;
        tex_count = 0;
        reverse = false;
        timeout = -1;
        drawScore = false;

        // Determine starting direction and movement type based on colour
        switch(colour)
        {
            case RED:
                dir = LEFT;
                ai = wave;      // RED starts outside the pen and is immediately in the first targeting mode defined by wave
                break;
            case PINK:
                dir = DOWN;
                ai = LEAVE;     // PINK starts in LEAVE mode, leaving the SPAWN pen straight away
                break;
            case BLUE:          // YELLOW and BLUE both start facing UP, trapped within the SPAWN
            case YELLOW:        // Thusly, overflow switch case
                dir = UP;
                ai = SPAWN;
                break;
        }
    }
    void reset()
    {
        x = x_init;
        y = y_init;
        d_pos = 0.1f;
        tex_count = 0;
        reverse = false;
        timeout = -1;
        drawScore = false;

        // Reset starting direction and movement type based on colour
        switch(colour)
        {
            case RED:
                dir = LEFT;
                ai = wave;      // RED starts outside the pen and is immediately in the first targeting mode defined by wave
                break;
            case PINK:
                dir = DOWN;
                ai = LEAVE;     // PINK starts in LEAVE mode, leaving the SPAWN pen straight away
                break;
            case BLUE:          // YELLOW and BLUE both start facing UP, trapped within the SPAWN
            case YELLOW:        // Thusly, overflow switch case
                dir = UP;
                ai = SPAWN;
                break;
        }
    }

    /**
     * Determines and returns absolute X coordinate of map tile on which ghost resides
     *
     * @return - integer, X coordinate of current tile
     */
    int getX()
    {
        return floor(x + 0.5);
    }

    /**
     * Determines and returns absolute Y coordinate of map tile on which ghost resides
     *
     * @return - integer, Y coordinate of current tile
     */
    int getY()
    {
        return floor(y + 0.5);
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
        return (int)(y * 10.0f) % 10 == 0 && (int)(x * 10.0f) % 10 == 0;
    }

    /**
     * Count the number of exits from the current position
     * If the ghost is not at the tile center, exits=0
     * If the ghost is at the tile center, exit count can determine what kind of junction the ghost is at
     *      2 Exits: Straight/Corner (corner requires && isImpassible(getNextTile(dir)) to confirm)
     *      3 Exits: T-Junction
     *      4 Exits: X-Intersection
     *
     * @return integer number of exits from the current position
     */
    int countExits()
    {
        int exits = 0;
        if(atTileCenter())
        {
            if(!isImpassible(getNextTile(UP)))
                exits++;
            if(!isImpassible(getNextTile(RIGHT)))
                exits++;
            if(!isImpassible(getNextTile(DOWN)))
                exits++;
            if(!isImpassible(getNextTile(LEFT)))
                exits++;
        }
        return exits;
    }

    /**
     * Method updates direction to navigate around a corner
     */
    void turnCorner()
    {
        if(dir != DOWN && !isImpassible(getNextTile(UP)))
            dir = UP;
        else if(dir != LEFT && !isImpassible(getNextTile(RIGHT)))
            dir = RIGHT;
        else if(dir != UP && !isImpassible(getNextTile(DOWN)))
            dir = DOWN;
        else if(dir != RIGHT && !isImpassible(getNextTile(LEFT)))
            dir = LEFT;
    }

    /**
     * Reverse the ghost's direction of movement, then resetting the corresponding flag
     */
    void reverseDirection()
    {
        switch(dir)
        {
            case UP:
                dir = DOWN;  break;
            case RIGHT:
                dir = LEFT;  break;
            case DOWN:
                dir = UP;    break;
            case LEFT:
                dir = RIGHT; break;
        }
        reverse = false;
    }

    /**
     * Get the current AI movement mode of the ghost
     *
     * @return - movement type of ghost
     */
    movement getAI()
    {
        return ai;
    }

    /**
     * Update the ghost's AI movement mode, reversing its direction on doing so if required
     *
     * @param newAI - AI movement type for ghost to now use
     */
    void setAI(movement newAI, bool switchDir)
    {
        ai = newAI;
        reverse = switchDir;
        if(newAI == FRIGHTENED)
        {
            timeout = 0;
            setSpeed(50);
        }
        else if(newAI == DEAD)
        {
            setSpeed(100);
            drawScore = true;
        }
    }

    /**
     * Set the speed of the ghost
     *
     * @param percentage - Float representing speed. 100% sets d_pos=0.1f (normal playing speed)
     */
    void setSpeed(float percentage)
    {
        d_pos = percentage / 1000;
    }

    /**
     * Special movement behaviour:
     *      Very simply move ghost up and down within the SPAWN pen
     */
    void aiSpawn()
    {
        setSpeed(50);   // Set movement speed to 50%
        if((int)(y * 10.0f) % 10 == 5 && (int)(x * 10.0f) % 10 == 5 && isImpassible(getNextTile(dir)))
        {
            switch(dir)
            {
                case UP:
                    dir = DOWN;
                    break;
                case DOWN:
                    dir = UP;
                    break;
            }
        }
    }

    /**
     * Special movement behaviour:
     *      If within SPAWN pen and not heading down, move towards the center of the enclosure
     *      Once at center of pen, head up until exited PEN
     *      Immediately head LEFT and set speed to 100%
     *      Once the first tile center is reached, enter AI of current wave
     */
    void aiLeave()
    {
        if(y < 19 && dir != DOWN)
        {
            setSpeed(50);   // Set movement speed to 50%
            if(x < 13.4)
                dir = RIGHT;
            else if(x > 13.6)
                dir = LEFT;
            else
                dir = UP;
        }
        else if(y >= 19)
        {
            if(!atTileCenter())
            {
                dir = LEFT;
                setSpeed(100);
            }
            else
                ai = wave;
        }
        else if((int)(y * 10.0f) % 10 == 5 && isImpassible(getNextTile(dir)))
            dir = UP;
    }

    /**
     * Calculate and return the straight line distance between two points (tiles) in the map
     *
     * @param p1 - Vector storing the x,y map coordinates of the first point
     * @param p2 - Vector storing the x,y map coordinates of the second point
     * @return -   Straight line distance between p1 and p2, floating point precision
     */
    float distanceBetween(std::vector<int> p1, std::vector<int> p2)
    {
        float d_x = p1[0] - p2[0];
        float d_y = p1[1] - p2[1];
        return sqrt((d_x * d_x) + (d_y * d_y));
    }

    /**
     * Determine the direction from the current junction which yields the closest straight line distance to a target tile
     *      Method checks the next tile in every traversible direction from the junction
     *      The optimal direction is only updated should it prove closer than previously checked exits
     *
     * Note: this does not always give the shortest PATH to the target
     *      However, this behaviour is as the original Pacman was designed
     *
     * @param target - Vector storing the x,y map coordinates of the target tile
     * @return -       Direction of shortest straight line distance to target
     */
    direction targetTile(std::vector<int> target)
    {
        std::vector<int> next_pos;  // Initialise next position, updated in each possible direction
        float distance = 999;       // Set max distance to unreachable value
        direction newDir;           // Initialise returned direction

        // Check UP exit
        // UP exits have an additional condition such that, at 4 specific intersections, the ghost cannot opt to travel UP
        if(!(getY() == 19 && (getX() == 12 || getX() == 15)) && !(getY() == 7 && (getX() == 12 || getX() == 15)))
        {
            if(dir != DOWN && !isImpassible(getNextTile(UP)))   // Prevent direction reversing and ensure exit is traversible
            {
                next_pos = {getX(), getY() + 1};
                float d = distanceBetween(next_pos, target);    // Get distance between target and next tile in exit direction
                if(d < distance)                                // If distance is shorter than any previously found, update direction to be returned
                {                                               // Also update lowest found distance for future checks
                    distance = d;
                    newDir = UP;                                // This process is repeated for all traversible exits from the current junction
                }
            }
        }

        // Check RIGHT exit
        if(dir != LEFT && !isImpassible(getNextTile(RIGHT)))
        {
            next_pos = {getX() + 1, getY()};
            float d = distanceBetween(next_pos, target);
            if(d < distance)
            {
                distance = d;
                newDir = RIGHT;
            }
        }

        // Check DOWN exit
        if(dir != UP && !isImpassible(getNextTile(DOWN)))
        {
            next_pos = {getX(), getY() - 1};
            float d = distanceBetween(next_pos, target);
            if(d < distance)
            {
                distance = d;
                newDir = DOWN;
            }
        }

        // Check LEFT exit
        if(dir != RIGHT && !isImpassible(getNextTile(LEFT)))
        {
            next_pos = {getX() - 1, getY()};
            float d = distanceBetween(next_pos, target);
            if(d < distance)
            {
                distance = d;
                newDir = LEFT;
            }
        }
        return newDir;  // Return newly found direction, yielding lowest straight line distance to target
    }

    /**
     * SCATTER movement AI attempts to force all ghosts to diverge from one another
     * Each ghost is set to target a point outside the map (one in each of the four corners) based on their colour
     *
     * If left in SCATTER mode, this will cause each to loop around a small section of the map in a different corner
     * SCATTER mode is not normally enabled long enough for this to occur, instead just forcing ghosts to separate
     */
    void aiScatter()
    {
        std::vector<int> target;
        switch(colour)
        {
            case RED:
                target = {25, 33}; break;
            case PINK:
                target = {2, 33}; break;
            case BLUE:
                target = {27, -2}; break;
            case YELLOW:
                target = {0, -2}; break;
        }
        dir = targetTile(target);   // Set direction at junction to head towards SCATTER point
        setSpeed(100);              // Set movement speed to 100%
    }

    /**
     * Calculate an target as Pacman +- an offset of given size in Pacman's direction of movement
     *
     * @param offsetSize - Size of offset to apply
     * @return -           New target vector, accounting for offset
     */
    std::vector<int> pacmanDirectionalOffset(int offsetSize)
    {
        std::vector<int> offset = {pacman.getX(), pacman.getY()};
        switch(pacman.getDirection())
        {
            case UP:
                offset[1] += offsetSize;
                break;
            case RIGHT:
                offset[0] += offsetSize;
                break;
            case DOWN:
                offset[1] -= offsetSize;
                break;
            case LEFT:
                offset[0] -= offsetSize;
                break;
        }
        return offset;
    }

    /**
     * CHASE mode AI is different for every ghost colour:
     *      RED:    Targets and chases Pacman
     *      PINK:   Targets four tiles ahead of Pacman in his direction of movement, attempting an ambush tactic
     *      BLUE:   The most complex AI type (explained below) often acts somewhat erratically
     *              However, if RED is closely chasing Pacman, BLUE will generally also be in pursuit:
     *                      Find point two tiles ahead of Pacman
     *                      Find vector from RED ghost to this point
     *                      Double this vector - the point at the end of this doubled vector is the target
     *      YELLOW: Targets and chases Pacman as RED does until within 8 tiles range, then emulating SCATTER behaviour
     */
    void aiChase(Ghost redGhost)
    {
        std::vector<int> target = {pacman.getX(), pacman.getY()};   // Default target for RED and (sometimes) YELLOW
        std::vector<int> current_pos = {getX(), getY()};            // Current position, stored as vector
        int d_x;    // Delta X initialised for use in BLUE's targeting
        int d_y;    // Delta Y initialised for use in BLUE's targeting

        // Determine ghost chasing behaviour based on their colour
        switch(colour)
        {
            case PINK:                                  // PINK looks ahead of Pacman and tries to ambush him
                target = pacmanDirectionalOffset(4);    // Update target to reflect offset in Pacman's direction of movement
                break;
            case BLUE:
                target = pacmanDirectionalOffset(2);    // Start by finding the point 2 tiles ahead of Pacman in his direction of movement
                d_x = redGhost.getX() - target[0];      // Find the X difference between this point and RED ghost
                d_y = redGhost.getY() - target[1];      // Find the Y difference between this point and RED ghost
                // BLUE's target is then twice the change in X and Y from RED's position
                target = {redGhost.getX() + 2 * d_x, redGhost.getY() + 2 * d_y};
                break;
            case YELLOW:
                if(distanceBetween(current_pos, target) <= 8)   // If YELLOW is closer than 8 tiles to Pacman, he chases as RED does
                    target = {0, -2};                           // If closer than 8 tiles, it emulates SCATTER AI behaviour
                break;
        }
        dir = targetTile(target);   // Set direction to that of least straight line distance to target
        setSpeed(100);              // Set movement speed to 100%
    }

    /**
     * FRIGHTENED mode AI chooses a direction randomly at each junction, moving at half speed
     */
    void aiFrightened()
    {
        direction newDir;
        do
        {
            newDir = static_cast<direction>((rand() % LEFT) + 1);   // Choose random direction from UP, RIGHT, DOWN or LEFT
        } while(isImpassible(getNextTile(newDir)));                 // Ensure random direction is traversible

        dir = newDir;   // Set new direction
        setSpeed(50);   // Set movement speed to 50%
    }

    void aiDead()
    {
        std::vector<int> target = {14, 19};
        dir = targetTile(target);
        setSpeed(100);
    }

    void checkSpecialCases()
    {
        if(timeout >= 600)
        {
            if(ai == FRIGHTENED)
            {
                ai = wave;
                setSpeed(100);
            }
            timeout = -1;
            ghostsEaten = 0;
        }
        else if(timeout != -1)
            timeout++;

        if(ai == DEAD)
        {
            if(x > 13.4 && x < 13.6)
            {
                if(getY() == 19)
                {
                    dir = DOWN;
                    setSpeed(50);
                }
                else if(getY() <= 17 && getY() > 15)
                {
                    ai = LEAVE;
                }
            }
        }
    }

    void move(Ghost redGhost)
    {
        checkSpecialCases();

        // Handle special case movement behaviours
        if(ai == SPAWN)         // Behaviour within SPAWN pen
            aiSpawn();
        else if(ai == LEAVE)    // AI to LEAVE SPAWN pen
            aiLeave();
        // Handle PORTAL collision - only teleport if at center of tile
        else if(atTileCenter() && getTile(getX(),getY()) == P)
        {
            if(dir == RIGHT)
                x = 1;
            else
                x = 26;
        }
        // If the a new AI mode has been set, reverse the current direction
        else if(reverse)
            reverseDirection();
        // If no special case exists, direction can only be changed when 2 or more exits exist
        else if(countExits() == 2 && isImpassible(getNextTile(dir)))    // Ghost is at corner so must turn
            turnCorner();
        else if(countExits() > 2)   // Ghost is at junction - run targeting AI and update direction
        {
            switch(ai)
            {
                case SCATTER:       // Scatter all ghosts to each of the four corners
                    aiScatter();        break;
                case CHASE:         // Target and hunt Pacman, passing RED ghost for BLUE's AI
                    aiChase(redGhost);  break;
                case FRIGHTENED:    // Flee from Pacman randomly
                    aiFrightened();     break;
                case DEAD:
                    aiDead();           break;
            }
        }

        // Half speed when travelling down PORTAL corridors
        if(getY() == 16 && (getX() < 6 || getX() > 21) && ai != DEAD)
            setSpeed(50);

        // Perform smooth movement between tiles in the current direction of movement
        // Round the unchanging position coordinate, preventing directional query mishaps
        switch(dir)
        {
            case UP:
                y += d_pos;
                if(ai != SPAWN && ai != LEAVE && ai != DEAD)
                    x = round(x);
                break;
            case RIGHT:
                x += d_pos;
                if(ai != LEAVE)
                    y = round(y);
                break;
            case DOWN:
                y -= d_pos;
                if(ai != SPAWN && ai != LEAVE && ai != DEAD)
                    x = round(x);
                break;
            case LEFT:
                x -= d_pos;
                if(ai != LEAVE)
                    y = round(y);
                break;
        }
    }

    /**
     * Draw ghost of correct colour at its current location
     */
    void draw()
    {
        // Reset drawScore flag - when eaten, drawEaten() is called during pause, do not draw score again
        drawScore = false;

        glPushMatrix();

        translateMapOrigin();               // Translate to map origin
        translateMapCoords(x,y);            // Translate to current (x,y)
        glTranslatef(-3.0f, -3.0f, 0.0f);   // Account for over-sized sprite (14x14 on 8x8 tile)

        // Determine which colour ghost to draw and whether it is the alternate texture (wiggle animation)
        unsigned int ghost_tex;
        if(ai == FRIGHTENED)    // If in FRIGHTENED mode, draw the correct skin
        {
            if(timeout >= 480 && tex_count % 30 >= 15)  // Draw white (flashing) skin when FRIGHTENED mode is nearing its end
            {
                ghost_tex = tex_count % 20 < 10 ? ghost_flee_2_tex : ghost_flee_3_tex;
            }
            else
                ghost_tex = tex_count % 20 < 10 ? ghost_flee_0_tex : ghost_flee_1_tex;
        }
        else if(ai != DEAD)
        {
            switch(colour)      // If not in FRIGHTENED or DEAD AI, draw the ghost according to colour
            {
                case RED:
                    ghost_tex = tex_count % 20 < 10 ? ghost_r_0_tex : ghost_r_1_tex; break;
                case PINK:
                    ghost_tex = tex_count % 20 < 10 ? ghost_p_0_tex : ghost_p_1_tex; break;
                case BLUE:
                    ghost_tex = tex_count % 20 < 10 ? ghost_b_0_tex : ghost_b_1_tex; break;
                case YELLOW:
                    ghost_tex = tex_count % 20 < 10 ? ghost_y_0_tex : ghost_y_1_tex; break;
            }
        }

        // Draw ghost sprite with determined texture at current location only if not DEAD
        if(ai != DEAD)
            drawSprite(ghost_tex, 14, 14, 0);

        // Only draw the ghost's eyes if it's not FRIGHTENED
        if(ai != FRIGHTENED)
        {
            // Determine which way the ghost's eyes should be facing
            unsigned int eyes_tex;
            switch(dir)
            {
                case UP:
                    eyes_tex = eye_u_tex; break;
                case RIGHT:
                    eyes_tex = eye_r_tex; break;
                case DOWN:
                    eyes_tex = eye_d_tex; break;
                case LEFT:
                    eyes_tex = eye_l_tex; break;
            }

            // Draw direction-based eyes sprite atop ghost body
            drawSprite(eyes_tex, 14, 14, 0);
        }

        // Increment texture counter every frame
        tex_count++;

        glPopMatrix();
    }

    /**
     * If ghost has just been eaten, draw the score for eating it, otherwise draw as normal
     */
    void drawEaten()
    {
        if(drawScore)
        {
            glPushMatrix();

            translateMapOrigin();               // Translate to map origin
            translateMapCoords(x,y);            // Translate to current (x,y)
            glTranslatef(-4.0f, 0.0f, 0.0f);    // Account for over-sized sprite (16x8 on 8x8 tile)

            // Determine which score sprite to draw based on the number of ghosts eaten since the last big pill was eaten
            unsigned int score_tex;
            switch(ghostsEaten)
            {
                case 1:
                    score_tex = score_200_tex;  break;
                case 2:
                    score_tex = score_400_tex;  break;
                case 3:
                    score_tex = score_800_tex;  break;
                default:
                    score_tex = score_1600_tex; break;
            }

            // Draw correct score sprite at current location
            drawSprite(score_tex, 16, 8, 0);

            glPopMatrix();
        }
        else
            draw();     // If the ghost hasn't just been eaten, draw it as normal
    }
};

#endif //COURSEWORK_GHOSTS_H
