/**
 * Header file responsible for storing and drawing the game map.
 *
 * Initially, map was drawn using GL_LINE_LOOPs etc. but textures provided a more authentic look.
 */

#ifndef PACMAN_MAP_H
#define PACMAN_MAP_H

// Allow access of ticks and fruit count from globals.h
extern int ticks;
extern int fruits;
extern bool fruitSpawned;

// Fruit timer incremented with each frame - fruit is removed after approx 10s if not eaten
int fruitTimer = -1;

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
 * When Pac-Man dies, remove any spawned fruits from the map
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

    // Once randomly selected tile is empty, spawn fruit and set timer to 0
    setTile(x,y,F);
    fruitSpawned = true;
    fruitTimer = 0;
}

/**
 * Determine which fruit to draw based on how many have already been consumed
 * If the timer exceeds 600 ticks (approx 15s) remove the fruit and reset the timer
 */
void drawFruit(int x, int y)
{
    if(fruitTimer <= 900)
    {
        glPushMatrix();

        glTranslatef(-3.0f, -3.0f, 0.0f);   // Account for over-sized sprite (14x14 on 8x8 tile)

        // Determine which fruit sprite to draw from the array based on current fruit consumption count
        drawSprite(fruits_tex[fruits], 14, 14, 0);  // Draw fruit at current location

        glPopMatrix();
        fruitTimer++;
    }
    else
    {
        setTile(x,y,e);
        fruitTimer = -1;
    }
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
            // Determine size of big pills to draw depending on ticks
            int bigPill = floor(ticks % 40 / 20);

            switch(getTile(x,y))    // Draw pills as sprites
            {
                case o:
                    drawSprite(pill_tex, 8, 8, 0); break;
                case O:
                    drawSprite(bigPill_tex[bigPill], 8, 8, 0); break;   // Draw big pill of determined size
                case F:
                    drawFruit(x,y); break;  // Method to determine which fruit should be drawn at current location
            }
            translateMapCoords(0,1);    // Increment Y pos
        }
        glPopMatrix();

        translateMapCoords(1,0);        // Increment X pos
    }
    glPopMatrix();
}

#endif //PACMAN_MAP_H
