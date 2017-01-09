/**
 * Header file responsible for storing and drawing the game map.
 *
 * Initially, map was drawn using GL_LINE_LOOPs etc. but textures provided a more authentic look.
 * This original code can be found commented out at the bottom of this file.
 */

#ifndef COURSEWORK_MAP_H
#define COURSEWORK_MAP_H

// Allow access of ticks and fruit count from main file
extern int ticks;
extern int fruits;
extern bool fruitSpawned;

/// TILES: 8x8, SPRITES: 14x14, MAP: 224x248, WINDOW: 300x300 - map starts at (38,26), ends at (262,274)
// 2D tile array stores game map
tile map[28][31] =
        {
                {W,W,W,W,W,W,W,W,W,W,W,W,n,n,n,W,P,W,n,n,n,W,W,W,W,W,W,W,W,W,W},
                {W,o,o,o,o,W,W,O,o,o,o,W,n,n,n,W,n,W,n,n,n,W,o,o,o,o,O,o,o,o,W},
                {W,o,W,W,o,W,W,o,W,W,o,W,n,n,n,W,n,W,n,n,n,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,o,o,o,W,W,o,W,n,n,n,W,n,W,n,n,n,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,W,W,W,o,W,n,n,n,W,n,W,n,n,n,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,W,W,W,o,W,W,W,W,W,n,W,W,W,W,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,W},
                {W,o,W,W,W,W,W,o,W,W,o,W,W,W,W,W,n,W,W,W,W,W,W,W,W,o,W,W,W,o,W},
                {W,o,W,W,W,W,W,o,W,W,o,W,W,W,W,W,n,W,W,W,W,W,W,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,o,o,o,W,W,o,n,n,n,n,n,n,n,n,n,W,W,o,o,o,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,o,W,W,o,W,W,n,W,W,W,W,W,n,W,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,o,W,W,o,W,W,n,W,W,n,n,W,n,W,W,o,W,W,o,W,W,W,o,W},
                {W,o,o,o,o,W,W,o,o,o,o,W,W,n,W,W,n,n,W,n,n,n,o,W,W,o,o,o,o,o,W},
                {W,o,W,W,W,W,W,n,W,W,W,W,W,n,W,W,n,n,G,n,W,W,W,W,W,o,W,W,W,W,W},
                {W,o,W,W,W,W,W,n,W,W,W,W,W,n,W,W,n,n,G,n,W,W,W,W,W,o,W,W,W,W,W},
                {W,o,o,o,o,W,W,o,o,o,o,W,W,n,W,W,n,n,W,n,n,n,o,W,W,o,o,o,o,o,W},
                {W,o,W,W,o,W,W,o,W,W,o,W,W,n,W,W,n,n,W,n,W,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,o,W,W,o,W,W,n,W,W,W,W,W,n,W,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,o,o,o,W,W,o,n,n,n,n,n,n,n,n,n,W,W,o,o,o,o,W,W,W,o,W},
                {W,o,W,W,W,W,W,o,W,W,o,W,W,W,W,W,n,W,W,W,W,W,W,W,W,o,W,W,W,o,W},
                {W,o,W,W,W,W,W,o,W,W,o,W,W,W,W,W,n,W,W,W,W,W,W,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,W},
                {W,o,W,W,o,W,W,W,W,W,o,W,W,W,W,W,n,W,W,W,W,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,W,W,W,o,W,n,n,n,W,n,W,n,n,n,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,o,o,o,W,W,o,W,n,n,n,W,n,W,n,n,n,W,o,W,W,o,W,W,W,o,W},
                {W,o,W,W,o,W,W,o,W,W,o,W,n,n,n,W,n,W,n,n,n,W,o,W,W,o,W,W,W,o,W},
                {W,o,o,o,o,W,W,O,o,o,o,W,n,n,n,W,n,W,n,n,n,W,o,o,o,o,O,o,o,o,W},
                {W,W,W,W,W,W,W,W,W,W,W,W,n,n,n,W,P,W,n,n,n,W,W,W,W,W,W,W,W,W,W}
        };

/**
 * Translates the current position of drawing to the bottom left corner of the map.
 */
void translateMapOrigin()
{
    glTranslatef(38.0f, 26.0f, 0.0f);
}

/**
 * Translate to a given (x,y) in map coordinates within the window
 *
 * @param x - x coordinate relative to game map
 * @param y - y coordinate relative to game map
 */
void translateMapCoords(float x, float y)
{
    glTranslatef(x * 8, y * 8, 0.0f);
}

/**
 * Get tile at given location in map
 *
 * @param x - X coordinate in map
 * @param y - Y coordinate in map
 * @return -  tile at given position
 */
tile getTile(int x, int y)
{
    return map[x][y];
}

/**
 * Set tile at given location in map to given type
 *
 * @param x - X coordinate in map to be updated
 * @param y - Y coordinate in map to be updated
 * @param t - tile to which position in map should be updated
 */
void setTile(int x, int y, tile t)
{
    map[x][y] = t;
}

/**
 * Return true if the given tile is impassible (a WALL or GATE)
 * @param t - tile for which to check passibility
 * @return -  bool, true if tile is impassible
 */
bool isImpassible(tile t)
{
    return t == W || t == G;
}

/**
 * Iterates through map array repopulating it with pills where they have been eaten
 */
void resetMap()
{
    for(int x=0;x<28;x++)
    {
        for(int y=0;y<31;y++)
        {
            switch(getTile(x,y))
            {
                case F:     // Fruits only spawn on empty pill tiles, not empty big pill tiles - if it still exists on map reset, reset to pill
                case e:
                    setTile(x,y,o); break;
                case E:
                    setTile(x,y,O); break;
            }
        }
    }
}

/**
 * When Pacman dies, remove any spawned fruits from the map
 */
void resetFruit()
{
    for(int x=0;x<28;x++)
    {
        for(int y=0;y<31;y++)
        {
            switch(getTile(x,y))
            {
                case F:     // Fruits only spawn on empty pill tiles, not empty big pill tiles - if it still exists on map reset, reset to pill
                    setTile(x,y,e); break;
            }
        }
    }
}

/**
 * Randomly spawn a fruit in the lower third of the map
 */
void spawnFruit()
{
    int x;
    int y;
    do
    {
        x = rand() % 27 + 1;    // Generate random X within the map (excluding outer walls)
        y = rand() % 10 + 1;    // Generate random Y within the lower third of the map (excluding outer walls)
    } while(getTile(x,y) != e); // Randomly selected tile must be empty

    // Once randomly selected tile is empty, spawn fruit
    setTile(x,y,F);
    fruitSpawned = true;
}

/**
 * Determine which fruit to draw based on how many have already been consumed
 */
void drawFruit(int x, int y)
{
    glPushMatrix();

    glTranslatef(-3.0f, -3.0f, 0.0f);   // Account for over-sized sprite (14x14 on 8x8 tile)

    // Determine which fruit sprite to draw from the array based on current fruit consumption count
    drawSprite(fruits_tex[fruits], 14, 14, 0);   // Draw fruit at current location

    glPopMatrix();
}

/**
 * Iterates through the map array, drawing all pills. Also draws map as a sprite.
 */
void drawMap()
{
    glPushMatrix();

    translateMapOrigin();               // Translate to map origin
    drawSprite(map_tex, 224, 248, 0);   // Draw map as a sprite
    for(int x=0;x<28;x++)               // Loop populates map
    {
        glPushMatrix();

        for(int y=0;y<31;y++)
        {
            switch(getTile(x,y))           // Draw pills as sprites
            {
                case o:
                    drawSprite(pill_tex, 8, 8, 0); break;
                case O:
                    if(ticks % 60 < 30)     // Set big pill sprite to flash depending on ticks
                        drawSprite(bigPill_tex, 8, 8, 0);
                    break;
                case F:
                    drawFruit(x,y); break;  // Determine which fruit should be drawn at current location
            }
            translateMapCoords(0,1);    // Increment Y pos
        }
        glPopMatrix();

        translateMapCoords(1,0);        // Increment X pos
    }
    glPopMatrix();
}

/**
 * Original map-drawing code; calls to each method were made in the drawMap() method.
 *
// Methods draw the given edge of the wall
void drawTop()
{
    static float vertex[2][2] =
            {
                    {0, 10},
                    {10, 10}
            };
    drawLine(vertex);
}
void drawRight()
{
    static float vertex[2][2] =
            {
                    {10, 0},
                    {10, 10}
            };
    drawLine(vertex);
}
void drawBottom()
{
    static float vertex[2][2] =
            {
                    {0, 0},
                    {10, 0}
            };
    drawLine(vertex);
}
void drawLeft()
{
    static float vertex[2][2] =
            {
                    {0, 0},
                    {0, 10}
            };
    drawLine(vertex);
}

// Calculate which sides of wall to draw; fill wall
void drawWall(int x, int y)
{
    // Wall tile background
    rgb(0,5,20);    // Set fill colour to very dark blue
    static float vertex[4][2] =
            {
                    {0, 0},
                    {0, 10},
                    {10, 10},
                    {10, 0}
            };

    glBegin(GL_POLYGON);        // Filled polygon gives wall blocks background
    for (size_t i=0;i<4;i++)
        glVertex2fv(vertex[i]);
    glEnd();

    // Wall tile edges
    glLineWidth(3.0f);
    rgb(34,49,184);     // Set drawing colour to blue (real game wall colour)
    if(y == 30 || map[x][y+1] != W)
        drawTop();
    if(x == 27 || map[x+1][y] != W)
        drawRight();
    if(y == 0 || map[x][y-1] != W)
        drawBottom();
    if(x == 0 || map[x-1][y] != W)
        drawLeft();
}

// Method draws the ghost spawn gate
void drawGate()
{
    glPushMatrix();
    rgb(255,255,255);    // Set drawing colour to white
    glTranslatef(0.0f, 5.0f, 0.0f);     // Transform to tile centre on Y axis
    glLineWidth(7.0f);
    static float vertex[2][2] =
            {
                    {0, 0},
                    {10, 0}
            };
    drawLine(vertex);
    glPopMatrix();
}
*/

#endif //COURSEWORK_MAP_H
