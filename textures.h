/**
 * Header file responsible for binding, storing and drawing all textures.
 */

#ifndef COURSEWORK_TEXTURES_H
#define COURSEWORK_TEXTURES_H

/** Texture Storage **/
// Map Textures
unsigned int map_tex;           // Map
unsigned int pill_tex;          // Small Pill
unsigned int bigPill_tex[2];    // Array storing both big pill textures (flashes, changing size)
// Pacman Textures
unsigned int pac_0_tex;         // Open Mouth
unsigned int pac_1_tex;         // Half Mouth
unsigned int pac_2_tex;         // Closed Mouth
// Pacman Death Textures
unsigned int dead_tex[11];      // Array storing all death animation frames
// Ghost Textures
unsigned int ghost_r_tex[2];    // Array storing red        ghost sprites
unsigned int ghost_p_tex[2];    // Array storing pink       ghost sprites
unsigned int ghost_b_tex[2];    // Array storing blue       ghost sprites
unsigned int ghost_y_tex[2];    // Array storing yellow     ghost sprites
unsigned int ghost_f_tex[4];    // Array storing frightened ghost sprites
// Eye Textures
unsigned int eye_u_tex;         // Eyes facing UP
unsigned int eye_r_tex;         // Eyes facing RIGHT
unsigned int eye_d_tex;         // Eyes facing DOWN
unsigned int eye_l_tex;         // Eyes facing LEFT
// Fruit Textures
unsigned int fruits_tex[8];     // Array storing all fruit textures
unsigned int f_score_tex[8];    // Array storing all fruit score textures
// UI Textures
unsigned int num_0_tex;         // Number 0
unsigned int num_1_tex;         // Number 1
unsigned int num_2_tex;         // Number 2
unsigned int num_3_tex;         // Number 3
unsigned int num_4_tex;         // Number 4
unsigned int num_5_tex;         // Number 5
unsigned int num_6_tex;         // Number 6
unsigned int num_7_tex;         // Number 7
unsigned int num_8_tex;         // Number 8
unsigned int num_9_tex;         // Number 9
unsigned int g_scores_tex[4];   // Array of scores for eating a ghost
unsigned int one_up_tex;        // 1UP       tooltip
unsigned int score_tex;         // SCORE     tooltip
unsigned int ready_tex;         // READY!    tooltip
unsigned int gameover_tex;      // GAME OVER tooltip
unsigned int help_tex;          // HELP      tooltip
unsigned int quit_tex;          // QUIT      tooltip
unsigned int life_tex;          // Life counter
unsigned int pause_tex;         // PAUSE screen
unsigned int pause_alt_tex;     // PAUSE screen (alt)

/**
 * Loads and binds all textures on game init, improving performance over binding every time the world is drawn.
 * Note: glTexImage2D was changed in load_and_bind_texture.h to support RGBA
 */
void loadBindTextures()
{
    // Enable blending, allowing transparency of PNG textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Bind map textures
    map_tex =           load_and_bind_texture("sprites/map/map.png");
    pill_tex =          load_and_bind_texture("sprites/map/pill.png");
    bigPill_tex[0] =    load_and_bind_texture("sprites/map/big-0.png");
    bigPill_tex[1] =    load_and_bind_texture("sprites/map/big-1.png");
    // Bind Pacman textures
    pac_0_tex =         load_and_bind_texture("sprites/pacman/0.png");
    pac_1_tex =         load_and_bind_texture("sprites/pacman/1.png");
    pac_2_tex =         load_and_bind_texture("sprites/pacman/2.png");
    // Bind Pacman Death textures
    dead_tex[0] =       load_and_bind_texture("sprites/pacman/d-0.png");
    dead_tex[1] =       load_and_bind_texture("sprites/pacman/d-1.png");
    dead_tex[2] =       load_and_bind_texture("sprites/pacman/d-2.png");
    dead_tex[3] =       load_and_bind_texture("sprites/pacman/d-3.png");
    dead_tex[4] =       load_and_bind_texture("sprites/pacman/d-4.png");
    dead_tex[5] =       load_and_bind_texture("sprites/pacman/d-5.png");
    dead_tex[6] =       load_and_bind_texture("sprites/pacman/d-6.png");
    dead_tex[7] =       load_and_bind_texture("sprites/pacman/d-7.png");
    dead_tex[8] =       load_and_bind_texture("sprites/pacman/d-8.png");
    dead_tex[9] =       load_and_bind_texture("sprites/pacman/d-9.png");
    dead_tex[10] =      load_and_bind_texture("sprites/pacman/d-10.png");
    // Bind ghost textures
    ghost_r_tex[0] =    load_and_bind_texture("sprites/ghosts/r-0.png");
    ghost_r_tex[1] =    load_and_bind_texture("sprites/ghosts/r-1.png");
    ghost_p_tex[0] =    load_and_bind_texture("sprites/ghosts/p-0.png");
    ghost_p_tex[1] =    load_and_bind_texture("sprites/ghosts/p-1.png");
    ghost_b_tex[0] =    load_and_bind_texture("sprites/ghosts/b-0.png");
    ghost_b_tex[1] =    load_and_bind_texture("sprites/ghosts/b-1.png");
    ghost_y_tex[0] =    load_and_bind_texture("sprites/ghosts/y-0.png");
    ghost_y_tex[1] =    load_and_bind_texture("sprites/ghosts/y-1.png");
    ghost_f_tex[0] =    load_and_bind_texture("sprites/ghosts/f-0.png");
    ghost_f_tex[1] =    load_and_bind_texture("sprites/ghosts/f-1.png");
    ghost_f_tex[2] =    load_and_bind_texture("sprites/ghosts/f-2.png");
    ghost_f_tex[3] =    load_and_bind_texture("sprites/ghosts/f-3.png");
    // Bind ghost eye textures
    eye_u_tex =         load_and_bind_texture("sprites/eyes/u.png");
    eye_r_tex =         load_and_bind_texture("sprites/eyes/r.png");
    eye_d_tex =         load_and_bind_texture("sprites/eyes/d.png");
    eye_l_tex =         load_and_bind_texture("sprites/eyes/l.png");
    // Bind fruit textures
    fruits_tex[0] =     load_and_bind_texture("sprites/fruits/cherry.png");
    fruits_tex[1] =     load_and_bind_texture("sprites/fruits/strawberry.png");
    fruits_tex[2] =     load_and_bind_texture("sprites/fruits/orange.png");
    fruits_tex[3] =     load_and_bind_texture("sprites/fruits/apple.png");
    fruits_tex[4] =     load_and_bind_texture("sprites/fruits/melon.png");
    fruits_tex[5] =     load_and_bind_texture("sprites/fruits/boss.png");
    fruits_tex[6] =     load_and_bind_texture("sprites/fruits/bell.png");
    fruits_tex[7] =     load_and_bind_texture("sprites/fruits/key.png");
    f_score_tex[0] =    load_and_bind_texture("sprites/ui/100.png");
    f_score_tex[1] =    load_and_bind_texture("sprites/ui/300.png");
    f_score_tex[2] =    load_and_bind_texture("sprites/ui/500.png");
    f_score_tex[3] =    load_and_bind_texture("sprites/ui/700.png");
    f_score_tex[4] =    load_and_bind_texture("sprites/ui/1000.png");
    f_score_tex[5] =    load_and_bind_texture("sprites/ui/2000.png");
    f_score_tex[6] =    load_and_bind_texture("sprites/ui/3000.png");
    f_score_tex[7] =    load_and_bind_texture("sprites/ui/5000.png");
    // Bind UI textures
    num_0_tex =         load_and_bind_texture("sprites/ui/0.png");
    num_1_tex =         load_and_bind_texture("sprites/ui/1.png");
    num_2_tex =         load_and_bind_texture("sprites/ui/2.png");
    num_3_tex =         load_and_bind_texture("sprites/ui/3.png");
    num_4_tex =         load_and_bind_texture("sprites/ui/4.png");
    num_5_tex =         load_and_bind_texture("sprites/ui/5.png");
    num_6_tex =         load_and_bind_texture("sprites/ui/6.png");
    num_7_tex =         load_and_bind_texture("sprites/ui/7.png");
    num_8_tex =         load_and_bind_texture("sprites/ui/8.png");
    num_9_tex =         load_and_bind_texture("sprites/ui/9.png");
    g_scores_tex[0] =   load_and_bind_texture("sprites/ui/200.png");
    g_scores_tex[1] =   load_and_bind_texture("sprites/ui/400.png");
    g_scores_tex[2] =   load_and_bind_texture("sprites/ui/800.png");
    g_scores_tex[3] =   load_and_bind_texture("sprites/ui/1600.png");
    one_up_tex =        load_and_bind_texture("sprites/ui/1up.png");
    score_tex =         load_and_bind_texture("sprites/ui/score.png");
    ready_tex =         load_and_bind_texture("sprites/ui/ready.png");
    gameover_tex =      load_and_bind_texture("sprites/ui/gameover.png");
    help_tex =          load_and_bind_texture("sprites/ui/help.png");
    quit_tex =          load_and_bind_texture("sprites/ui/quit.png");
    life_tex =          load_and_bind_texture("sprites/ui/life.png");
    pause_tex =         load_and_bind_texture("sprites/ui/pause.png");
    pause_alt_tex =     load_and_bind_texture("sprites/ui/pause_alt.png");
}

/**
 * Sets the drawing colour given RGB input (it's what I'm used to)
 *
 * @param r - RED   coefficient of colour
 * @param g - GREEN coefficient of colour
 * @param b - BLUE  coefficient of colour
 */
void rgb(float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
}

/**
 * Draws a given texture as a sprite of given length and height, applying a rotation of the given angle
 *
 * @param texture - unsigned int corresponding to the loaded and bound texture
 * @param length -  integer length of the sprite to be drawn
 * @param height -  integer height of the sprite to be drawn
 * @param angle -   rotate the drawn sprite by a given angle (float)
 */
void drawSprite(unsigned int texture, int length, int height, float angle)
{
    // Begin new transformation matrix
    glPushMatrix();
    rgb(255,255,255);   // Reset drawing colour to white, preventing texture discolouration

    int halfLength = length/2;
    int halfHeight = height/2;

    // Translate to center of sprite and rotate if necessary
    glTranslatef((float)halfLength,(float)halfHeight,0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Enable texturing and bind selected sprite
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw sprite shape as square to be textured of size (length,height)
    glBegin(GL_QUADS);
        glTexCoord2f (0.0f, 0.0f);      // Bottom left
        glVertex2i(-halfLength, -halfHeight);
        glTexCoord2f (1.0f, 0.0f);      // Bottom right
        glVertex2i(halfLength, -halfHeight);
        glTexCoord2f (1.0f, 1.0f);      // Top right
        glVertex2i(halfLength, halfHeight);
        glTexCoord2f (0.0f, 1.0f);      // Top left
        glVertex2i(-halfLength, halfHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Pop matrix to ignore above transformations on future objects
    glPopMatrix();
}

#endif //COURSEWORK_TEXTURES_H
