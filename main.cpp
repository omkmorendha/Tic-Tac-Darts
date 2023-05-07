#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;

int menuScreenOption=1;

int status = 0;
int istat = 0;
int player = 1;
int rounds = 0;
int redscore = 0;
int bluescore = 0;
int v = 0;


void myinit(){
    glutInitWindowSize(1920, 1080);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("TIC-TAC-DARTS");

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float as = 1920.0 / 1080.0;
    gluPerspective(50, as, 0.1, 100);
}

class bullseye{
    public:
    float centrex, centrey, centrez;
    float radius = 0.2;
    int stat = 0;
    int score = 0;

    void draw(){
        float color[10][3], color1[10][3];
        float color0[][3] = {{1, 1, 0},
                            {1, 1, 0},
                            {1, 0, 0},
                            {1, 0, 0},
                            {0, 1, 1},
                            {0, 1, 1},
                            {0.2, 0.2, 0.2},
                            {0.2, 0.2, 0.2},
                            {1, 1, 1},
                            {1, 1, 1}};

        for(int i = 9; i >= 0; i--){
            if(i >= 10 - score){
                if(stat == 1){
                    color1[i][0] = 1;
                    color1[i][1] = 0;
                    color1[i][2] = 0;
                }
                else{
                    color1[i][0] = 0;
                    color1[i][1] = 0;
                    color1[i][2] = 1;
                }
            }
            else{
                color1[i][0] = 1;
                color1[i][1] = 1;
                color1[i][2] = 1;
            }
        }

        if(stat == 0){
            memcpy(color, color0, sizeof(color));
        }
        else{
            memcpy(color, color1, sizeof(color));
        }

        for(int j = 10; j > 0; j--){
            float r = radius * (float) j / 10;

            glColor3f(color[j - 1][0], color[j - 1][1], color[j - 1][2]);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(centrex, centrey, centrez - (float)j/1000);

                for(int i = 0; i < 360; i++)
                    glVertex3f(centrex + r * cos(i), centrey + r * sin(i), centrez - (float)j/1000);

            glEnd();

            glColor3f(0, 0, 0);
            glBegin(GL_POINTS);
                for(int i = 0; i < 720; i += 1)
                    glVertex3f(centrex + r * cos(i), centrey + r * sin(i), centrez + 0.001 - (float)j/1000);

            glEnd();
        }
    }

} bull[3][3];

int check(){
    if(rounds == 18){
        if(bluescore < redscore)
            return 1;
        else
            return 2;
    }

    for(int i = 0; i < 3; i++){
        if(bull[i][0].stat == bull[i][1].stat && bull[i][1].stat == bull[i][2].stat && bull[i][0].stat != 0){
            return bull[i][0].stat;
        }

        if(bull[0][i].stat == bull[1][i].stat && bull[1][i].stat == bull[2][i].stat && bull[0][i].stat != 0){
            return player;
        }

    }

    if(bull[0][0].stat == bull[1][1].stat && bull[1][1].stat == bull[2][2].stat  && bull[1][1].stat != 0)
        return bull[1][1].stat;

    if(bull[0][2].stat == bull[1][1].stat && bull[1][1].stat == bull[2][0].stat  && bull[1][1].stat != 0)
        return bull[1][1].stat;

    return 0;
}

void setbullco(){
    bull[0][0].centrex = -0.5;
    bull[0][0].centrey = 0.5;
    bull[0][0].centrez = 0;

    bull[0][1].centrex = 0;
    bull[0][1].centrey = 0.5;
    bull[0][1].centrez = 0;

    bull[0][2].centrex = 0.5;
    bull[0][2].centrey = 0.5;
    bull[0][2].centrez = 0;

    bull[1][0].centrex = -0.5;
    bull[1][0].centrey = 0;
    bull[1][0].centrez = 0;

    bull[1][1].centrex = 0;
    bull[1][1].centrey = 0;
    bull[1][1].centrez = 0;

    bull[1][2].centrex = 0.5;
    bull[1][2].centrey = 0;
    bull[1][2].centrez = 0;

    bull[2][0].centrex = -0.5;
    bull[2][0].centrey = -0.5;
    bull[2][0].centrez = 0;

    bull[2][1].centrex = 0;
    bull[2][1].centrey = -0.5;
    bull[2][1].centrez = 0;

    bull[2][2].centrex = 0.5;
    bull[2][2].centrey = -0.5;
    bull[2][2].centrez = 0;
}

void draw_all_bull(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++)
            bull[i][j].draw();
    }
}

void draw_board_border(){
    GLuint wood2_2d = SOIL_load_OGL_texture
    (
        "wood2.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glBindTexture(GL_TEXTURE_2D, wood2_2d);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(1, 1, 1);
        glTexCoord2f(1, 0); glVertex3f(1, 1, -1);
        glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

        glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);

        glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
        glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);

        glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
        glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
        glTexCoord2f(1, 1); glVertex3f(-1, -1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);

        glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(1, -1, -1);

        glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);
    glEnd();

    glDeleteTextures(1, &wood2_2d);
}

void draw_board(){

    GLuint wood_2d = SOIL_load_OGL_texture
    (
        "wood.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glBindTexture(GL_TEXTURE_2D, wood_2d);
    glTranslatef(0, 0, 0.001);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-1, 1, -0.1);
        glTexCoord2f(1, 0); glVertex3f(1, 1, -0.1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, -0.1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, -0.1);
    glEnd();

    glDeleteTextures(1, &wood_2d);
    glTranslatef(0, 1, 0);
    glScalef(1, 0.05, 0.25);

    draw_board_border();
    glTranslatef(0, -40, 0);
    draw_board_border();
    glTranslatef(0, 40, 0);
    glScalef(1, 20, 4);
    glScalef(0.05, 1, 0.25);
    glTranslatef(-20, -1, 0);
    draw_board_border();
    glTranslatef(40, 0, 0);
    draw_board_border();
    glTranslatef(-20, 1, 0);

    glScalef(20, 1, 4);
    glTranslatef(0, -1, 0);
}

void draw_scenery(){
    GLuint brick_2d = SOIL_load_OGL_texture
    (
        "brick.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    GLuint grass_2d = SOIL_load_OGL_texture
    (
        "grass.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    GLuint sky_2d = SOIL_load_OGL_texture
    (
        "sky.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS
    );

    glBindTexture(GL_TEXTURE_2D, sky_2d);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-8, 6, -0.11);
        glTexCoord2f(1, 0); glVertex3f(8, 6, -0.11);
        glTexCoord2f(1, 1); glVertex3f(8, 1, -0.11);
        glTexCoord2f(0, 1); glVertex3f(-8, 1, -0.11);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, brick_2d);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-1.2, 1.2, -0.1);
        glTexCoord2f(1, 0); glVertex3f(1.2, 1.2, -0.1);
        glTexCoord2f(1, 1); glVertex3f(1.2, -1.2, -0.1);
        glTexCoord2f(0, 1); glVertex3f(-1.2, -1.2, -0.1);

        glTexCoord2f(0, 0); glVertex3f(-1.2, 1.2, -0.1);
        glTexCoord2f(1, 0); glVertex3f(-1.2, -1.2, -0.1);
        glTexCoord2f(1, 1); glVertex3f(-1.2, -1.2, 6);
        glTexCoord2f(0, 1); glVertex3f(-1.2, 0.3, 6);

        glTexCoord2f(0, 0); glVertex3f(1.2, 1.2, -0.1);
        glTexCoord2f(1, 0); glVertex3f(1.2, -1.2, -0.1);
        glTexCoord2f(1, 1); glVertex3f(1.2, -1.2, 6);
        glTexCoord2f(0, 1); glVertex3f(1.2, 0.3, 6);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, grass_2d);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-1.2, -1.2, -0.1);
        glTexCoord2f(1, 0); glVertex3f(1.2, -1.2, -0.1);
        glTexCoord2f(1, 1); glVertex3f(1.2, -1.2, 6);
        glTexCoord2f(0, 1); glVertex3f(-1.2, -1.2, 6);
    glEnd();

    glDeleteTextures(1, &brick_2d);
    glDeleteTextures(1, &sky_2d);
    glDeleteTextures(1, &grass_2d);
}

float dartang_1 = 0, dartang_2 = 0;


void draw_dart(int p, float x, float y, float z){
    glTranslatef(x, y, z);
    GLuint red_dart_2d = SOIL_load_OGL_texture
    (
        "red_dart.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    GLuint blue_dart_2d = SOIL_load_OGL_texture
    (
        "blue_dart.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glScalef(0.03125, 0.03125, 0.5);
    glRotatef(180, 1, 0, 0);

    if(p == 1)
        glBindTexture(GL_TEXTURE_2D, red_dart_2d);
    else
        glBindTexture(GL_TEXTURE_2D, blue_dart_2d);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(1, 1, 1);
        glTexCoord2f(1, 0); glVertex3f(1, 1, -1);
        glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

        glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);

        glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
        glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);

        glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
        glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
        glTexCoord2f(1, 1); glVertex3f(-1, -1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);

        glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(1, -1, -1);

        glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
        glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
        glTexCoord2f(1, 1); glVertex3f(1, -1, -1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);
    glEnd();

    glColor3f(1, 1, 1);
    glDeleteTextures(1, &red_dart_2d);
    glDeleteTextures(1, &blue_dart_2d);

    glColor3f(0.66, 0.66, 0.66);
    glTranslatef(0, 0, 1);
    glutSolidCone(1, 0.8, 8, 8);
    glTranslatef(0, 0, -1);
    glRotatef(180, 1, 0, 0);
    glScalef(32, 32, 2);

    glTranslatef(0, 0, 2);
    glBegin(GL_TRIANGLES);
        glVertex3f(0, 0.1, -1.5);
        glVertex3f(0, -0.1, -1.5);
        glVertex3f(0, 0, -1.6);
    glEnd();
    glRotatef(90, 0, 0, 1);
    glBegin(GL_TRIANGLES);
        glVertex3f(0, 0.1, -1.5);
        glVertex3f(0, -0.1, -1.5);
        glVertex3f(0, 0, -1.6);
    glEnd();
    glRotatef(-90, 0, 0, 1);
    glTranslatef(0, 0, -2);
    glTranslatef(-x, -y, -z);
}
void draw_aim(float x, float y){
    glBegin(GL_POINTS);
    glColor3f(1, 1, 0);

    for(int i = 0; i < 1000; i++)
        glVertex3f(x + 0.25 * cos(M_PI * i / 500), y + 0.25 * sin(M_PI * i / 500), 0);

    for(int i = 0; i < 1000; i++)
        glVertex3f(x + 0.23 * cos(M_PI * i / 500), y + 0.23 * sin(M_PI * i / 500), 0);

    glEnd();
    glColor3f(1, 1, 1);
}

void draw_pow(){
    glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glVertex3f(-0.9, 1.5, 0);
        glVertex3f(0.9, 1.5, 0);
        glVertex3f(0.9, 1.3, 0);
        glVertex3f(-0.9, 1.3, 0);

        glColor3f(1, 0, 0);
        glVertex3f(-0.75, 1.475, 0.001);
        glVertex3f(0.75, 1.475, 0.001);
        glVertex3f(0.75, 1.325, 0.001);
        glVertex3f(-0.75, 1.325, 0.001);

        glColor3f(1, 1, 0);
        glVertex3f(-0.4, 1.475, 0.002);
        glVertex3f(0.4, 1.475, 0.002);
        glVertex3f(0.4, 1.325, 0.002);
        glVertex3f(-0.4, 1.325, 0.002);

        glColor3f(0, 1, 0);
        glVertex3f(-0.1, 1.475, 0.003);
        glVertex3f(0.1, 1.475, 0.003);
        glVertex3f(0.1, 1.325, 0.003);
        glVertex3f(-0.1, 1.325, 0.003);
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_QUADS);
        glColor3f(0, 0, 0);
        glVertex3f(-0.9, 1.5, 0.001);
        glVertex3f(0.9, 1.5, 0.001);
        glVertex3f(0.9, 1.3, 0.001);
        glVertex3f(-0.9, 1.3, 0.001);

        glVertex3f(-0.75, 1.475, 0.002);
        glVertex3f(0.75, 1.475, 0.002);
        glVertex3f(0.75, 1.325, 0.002);
        glVertex3f(-0.75, 1.325, 0.002);

        glVertex3f(-0.4, 1.475, 0.003);
        glVertex3f(0.4, 1.475, 0.003);
        glVertex3f(0.4, 1.325, 0.003);
        glVertex3f(-0.4, 1.325, 0.003);

        glVertex3f(-0.1, 1.475, 0.004);
        glVertex3f(0.1, 1.475, 0.004);
        glVertex3f(0.1, 1.325, 0.004);
        glVertex3f(-0.1, 1.325, 0.004);
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float aim_x = ((float) rand()/RAND_MAX * 1.34) - 0.67;
float aim_y = ((float) rand()/RAND_MAX * 1.34) - 0.67;
float powline = 0;
int powflip = 1;

void draw_pow_line(){
    glColor3f(0, 0, 0);

    glTranslatef(powline, 0, 0);
    glBegin(GL_QUADS);
        glVertex3f(-0.05, 1.475, 0.005);
        glVertex3f(0.05, 1.475, 0.005);
        glVertex3f(0.05, 1.325, 0.005);
        glVertex3f(-0.05, 1.325, 0.005);
    glEnd();
    glTranslatef(-powline, 0, 0);
}

float dartx = 0, darty = -1, dartz = 5;

void collide(){
    int fi = -1, fj = - 1;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            float d = sqrt((bull[i][j].centrex - dartx) * (bull[i][j].centrex - dartx) + (bull[i][j].centrey - darty) * (bull[i][j].centrey - darty));

            if(d < 0.2){
                fi = i;
                fj = j;
            }

        }
    }

    if(fi != -1){
        float d = sqrt((bull[fi][fj].centrex - dartx) * (bull[fi][fj].centrex - dartx) + (bull[fi][fj].centrey - darty) * (bull[fi][fj].centrey - darty));

        d /= 0.02;
        int r = floor(d);

        if((10 - r) > bull[fi][fj].score){
            bull[fi][fj].score = 10 - r;
            bull[fi][fj].stat = player;

            if (player == 1){
                redscore += 10 - r;
                bluescore -= 10 - r;
            }
            else{
                redscore += 10 - r;
                bluescore -= 10 - r;
            }
        }
    }
}

bool redColor = 1;
int blinkerIter = 0;
void outro(){
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float as = 1920.0 / 1080.0;
    gluPerspective(50, as, 0.1, 100);

    menuScreenOption = 5;

    char msgr[] = "RED PLAYER IS THE WINNER";
    char msgb[] = "BLUE PLAYER IS THE WINNER";

    // make it blink slow
    if(blinkerIter%3==0){
        redColor = !redColor;
    }

    if(player == 2){
        glColor3f(redColor,0,0);

        glLoadIdentity();
        glPushMatrix();
        glTranslatef(-0.45, 0.15, -0.3);
        glScalef(1/2600.0, 1/2000.0, 1/2600.0);
        for (int i=0;i<strlen(msgr);i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, msgr[i]);
        glPopMatrix();
    }
    else{
        glColor3f(0, 0, redColor);

        glLoadIdentity();
        glPushMatrix();
        glTranslatef(-0.45, 0.15, -0.3);
        glScalef(1/2600.0, 1/2000.0, 1/2600.0);
        for (int i=0;i<strlen(msgb);i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, msgb[i]);
        glPopMatrix();
    }


    char msg1[] = "Press H to go back to home, E to exit.";
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(-0.35, -0.65, -0.3);
    glScalef(1/3600.0, 1/3000.0, 1/3600.0);
    for (int i=0;i<strlen(msg1);i++)
      glutStrokeCharacter(GLUT_STROKE_ROMAN, msg1[i]);
    glPopMatrix();

    blinkerIter += 1;
    if(blinkerIter>100){
        blinkerIter = 0;
    }

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            bull[i][j].stat = 0;
            bull[i][j].score = 0;
        }
    }

    glutSwapBuffers();
    glFlush();
}

void display(){
    if(check() != 0){
        menuScreenOption = 5;
        glutDisplayFunc(outro);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);

        setbullco();
        draw_scenery();
        draw_board();

        glTranslatef(0, 0, 0.001);
        draw_all_bull();

        glLoadIdentity();
        gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);

        if(istat == 0)
            draw_aim(aim_x, aim_y);
        else if(istat == 1){
            draw_pow();
            draw_pow_line();
        }
        else if(istat == 2){
            draw_pow();
            draw_pow_line();
            draw_dart(1, -3, -1, 3); //Fake Dart
            draw_dart(player, dartx, darty, dartz);
        }

        glColor3f(1, 1, 1);
        glutSwapBuffers();
        glFlush();
    }
}

void reshape(int width, int height){
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective(50, aspect, 0.1, 100);
}

void update(int val){
    if(istat == 0){
        v++;
        if(v >= 40){
            istat = 1;
        }
    }
    else if(istat == 1){
        if(powline >= 0.67){
            powflip = -1;
            powline -= 0.1;
        }
        else if(powline <= -0.67){
            powflip = 1;
            powline += 0.1;
        }
        else if(powflip == 1)
            powline += 0.1;
        else if(powflip == -1)
            powline -= 0.1;
    }
    else if(istat == 2){
        if(dartz > 0){
            dartz -= 0.4;
            darty += exp(powline - 0.1) * (aim_y + 1) / 12.5;
            dartx += aim_x / 12.5;
        }
        else{
            collide();

            rounds++;
            player = (player == 1) ? 2 : 1;
            dartx = 0;
            darty = -1;
            dartz = 5;
            istat = 0;
            aim_x = (float) rand()/RAND_MAX * 0.67;
            aim_y = (float) rand()/RAND_MAX * 0.67;
            powline = 0;
            powflip = 1;
            v = 0;
        }
    }

    glutTimerFunc(100, update, 0);
    glutPostRedisplay();
}


void homeScreen(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.9,0.9,0.9,1);

    glLineWidth(1);

    glLoadIdentity();
    glColor3f(1,1,1); //to make image stay not coloured
    GLuint logo = SOIL_load_OGL_texture
    (
        "tic-tac-darts-logo.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glBindTexture(GL_TEXTURE_2D, logo);
    glTranslatef(0, 0, 0.001);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-0.18, 0.85, -0.3);
        glTexCoord2f(1, 1); glVertex3f(0.18, 0.85, -0.3);
        glTexCoord2f(1, 0); glVertex3f(0.18, 0.35, -0.3);
        glTexCoord2f(0, 0); glVertex3f(-0.18, 0.35, -0.3);
    glEnd();

    glPushMatrix();
    draw_dart(1, -3, -1, 3); //Fake Dart
    glPopMatrix();

    float padding = 0.02;
    float sub_segment_length = (1 - 4*padding)/3;
    float x1 = -0.5 + padding;
    float x2 = x1 + sub_segment_length + padding;
    float x3 = x2 + sub_segment_length + padding;

    if(menuScreenOption==1){
        glColor3f(0,0,1);
        glBegin(GL_LINE_LOOP);
            glVertex3f(-0.5, 0.21, -0.3);
            glVertex3f(x1 + 0.01, 0.32, -0.3);
            glVertex3f(x1 + sub_segment_length - 0.01, 0.32, -0.3);
            glVertex3f(x1 + sub_segment_length + 0.015, 0.21, -0.3);
            glVertex3f(0.5, 0.21, -0.3);
            glVertex3f(0.5, -0.38, -0.3);
            glVertex3f(-0.5, -0.38, -0.3);

            glVertex3f(-0.5, 0.21, -0.3);
            glVertex3f(x1 + sub_segment_length + 0.015, 0.21, -0.3);
        glEnd();
    }else if(menuScreenOption==2){
        glColor3f(0,0,1);
        glBegin(GL_LINE_LOOP);
            glVertex3f(x2 - 0.02, 0.21, -0.3);
            glVertex3f(x2 + 0.01, 0.32, -0.3);
            glVertex3f(x2 + sub_segment_length - 0.01, 0.32, -0.3);
            glVertex3f(x2 + sub_segment_length + 0.015, 0.21, -0.3);
            glVertex3f(0.5, 0.21, -0.3);
            glVertex3f(0.5, -0.38, -0.3);
            glVertex3f(-0.5, -0.38, -0.3);

            glVertex3f(-0.5, 0.21, -0.3);
            glVertex3f(x2 + sub_segment_length + 0.015, 0.21, -0.3);
        glEnd();
    }else if(menuScreenOption==3){
        glColor3f(0,0,1);
        glBegin(GL_LINE_LOOP);
            glVertex3f(x3 - 0.02, 0.21, -0.3);
            glVertex3f(x3 + 0.01, 0.32, -0.3);
            glVertex3f(x3 + sub_segment_length - 0.01, 0.32, -0.3);
            glVertex3f(x3 + sub_segment_length + 0.015, 0.21, -0.3);
            glVertex3f(0.5, 0.21, -0.3);
            glVertex3f(0.5, -0.38, -0.3);
            glVertex3f(-0.5, -0.38, -0.3);

            glVertex3f(-0.5, 0.21, -0.3);
            glVertex3f(x3 + sub_segment_length + 0.015, 0.21, -0.3);
        glEnd();
    }


    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex3f(x1 + 0.02, 0.3, -0.3);
        glVertex3f(x1 + sub_segment_length - 0.02, 0.3, -0.3);
        glVertex3f(x1 + sub_segment_length, 0.23, -0.3);
        glVertex3f(x1, 0.23, -0.3);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex3f(x2 + 0.02, 0.3, -0.3);
        glVertex3f(x2 + sub_segment_length - 0.02, 0.3, -0.3);
        glVertex3f(x2 + sub_segment_length, 0.23, -0.3);
        glVertex3f(x2, 0.23, -0.3);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex3f(x3 + 0.02, 0.3, -0.3);
        glVertex3f(x3 + sub_segment_length - 0.02, 0.3, -0.3);
        glVertex3f(x3 + sub_segment_length, 0.23, -0.3);
        glVertex3f(x3, 0.23, -0.3);
    glEnd();

    glColor3f(1,0,0);
    glRasterPos3f(x1+0.08,0.249,-0.3);
    char msg1[] = "(H) HOME";
    for(int i=0;i<strlen(msg1);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg1[i]);
    }

    glColor3f(1,0,0);
    glRasterPos3f(x2+0.025,0.249,-0.3);
    char msg2[] = "(I) INSTRUCTIONS";
    for(int i=0;i<strlen(msg2);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg2[i]);
    }

    glColor3f(1,0,0);
    glRasterPos3f(x3+0.06,0.249,-0.3);
    char msg3[] = "(C) CREDITS";
    for(int i=0;i<strlen(msg3);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg3[i]);
    }

    if(menuScreenOption==1){
        glColor3f(0,0,0);
        glRasterPos3f(-0.45,0.15,-0.3);
        char msg1[] = "WELCOME TO TIC TAC DARTS";
        for(int i=0;i<strlen(msg1);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1[i]);
        }
        glColor3f(0,0,0);
        glRasterPos3f(-0.45,0.05 + 0.03,-0.3);
        char msg1_0[] = "This is a 1v1 game whose is to control the 3 continuous bullseyes in a vertical, horizontal, or diagonal";
        for(int i=0;i<strlen(msg1_0);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_0[i]);
        }
        glRasterPos3f(-0.45,0.00 + 0.03,-0.3);
        char msg1_1[] = "direction just like Tic-Tac-Toe. The bullseyes can be captured by aiming the darts at it. The player";
        for(int i=0;i<strlen(msg1_1);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_1[i]);
        }
        glRasterPos3f(-0.45,-0.05 + 0.03,-0.3);
        char msg1_2[] = "would also be awarded a score from 1 to 10 indicating the dart�s closeness to the centre. A player";
        for(int i=0;i<strlen(msg1_2);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_2[i]);
        }
        glRasterPos3f(-0.45,-0.10 + 0.03,-0.3);
        char msg1_3[] = "can capture the opposing player�s bullseye as their own by having a higher score at the bullseye i.e";
        for(int i=0;i<strlen(msg1_3);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_3[i]);
        }
        glRasterPos3f(-0.45,-0.15 + 0.03,-0.3);
        char msg1_4[] = "by aiming it closer the centre than the opposing player.";
        for(int i=0;i<strlen(msg1_4);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_4[i]);
        }
        glRasterPos3f(-0.45,-0.20 + 0.03,-0.3);
        char msg1_5[] = "The player will have to first aim using the transparent circle, and then shoot according to the power";
        for(int i=0;i<strlen(msg1_5);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_5[i]);
        }
        glRasterPos3f(-0.45,-0.25 + 0.03,-0.3);
        char msg1_6[] = "slider mechanism, and ensure it is in the green region for max accuracy.";
        for(int i=0;i<strlen(msg1_6);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_6[i]);
        }
        glRasterPos3f(-0.45,-0.30 + 0.03,-0.3);
        char msg1_7[] = "If the game lasts more than 18 rounds, the game will be awarded to the player with the higher score,";
        for(int i=0;i<strlen(msg1_7);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_7[i]);
        }
        glRasterPos3f(-0.45,-0.35 + 0.03,-0.3);
        char msg1_8[] = "In the rare case of a tie, the player who started 2nd will be declared the winner.";
        for(int i=0;i<strlen(msg1_8);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_8[i]);
        }

    }else if(menuScreenOption==2){
        glColor3f(0,0,0);
        glRasterPos3f(-0.45,0.15,-0.3);
        char msg1[] = "HOW TO PLAY THE GAME";
        for(int i=0;i<strlen(msg1);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1[i]);
        }
        glColor3f(0,0,0);
        glRasterPos3f(-0.45,0.05 + 0.03,-0.3);
        char msg1_0[] = "-> Use the keys wherever specified in order to navigate the menu. Ex. 'H' for home, 'C' for credits";
        for(int i=0;i<strlen(msg1_0);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_0[i]);
        }
        glRasterPos3f(-0.45,0.00 + 0.03,-0.3);
        char msg1_1[] = "-> Two players will play in alternating fashion for a maximum of 18 rounds.";
        for(int i=0;i<strlen(msg1_1);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_1[i]);
        }
        glRasterPos3f(-0.45,-0.05 + 0.03,-0.3);
        char msg1_2[] = "-> Once you press 'X' and the game starts, you can use the 'W','A','S','D' keys to move your aim.";
        for(int i=0;i<strlen(msg1_2);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_2[i]);
        }
        glRasterPos3f(-0.45,-0.10 + 0.03,-0.3);
        char msg1_3[] = "-> The player will have to first aim using the transparent circle, and then shoot according to the power";
        for(int i=0;i<strlen(msg1_3);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_3[i]);
        }
        glRasterPos3f(-0.45,-0.15 + 0.03,-0.3);
        char msg1_4[] = "-> According to the slider at the top, adjust the force of your dart throw. Ensure that your throw";
        for(int i=0;i<strlen(msg1_4);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_4[i]);
        }
        glRasterPos3f(-0.45,-0.20 + 0.03,-0.3);
        char msg1_5[] = "happens when the slider is in the green region to throw your dart exactly where you aim.";
        for(int i=0;i<strlen(msg1_5);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_5[i]);
        }
        glRasterPos3f(-0.45,-0.25 + 0.03,-0.3);
        char msg1_6[] = "-> Press the spacebar' ' or 'M' to shoot your dart.";
        for(int i=0;i<strlen(msg1_6);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_6[i]);
        }
        glRasterPos3f(-0.45,-0.30 + 0.03,-0.3);
        char msg1_7[] = "-> If the game lasts more than 18 rounds, the game will be awarded to the player with the higher score,";
        for(int i=0;i<strlen(msg1_7);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_7[i]);
        }
        glRasterPos3f(-0.45,-0.35 + 0.03,-0.3);
        char msg1_8[] = "In the rare case of a tie, the player who started 2nd will be declared the winner.";
        for(int i=0;i<strlen(msg1_8);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_8[i]);
        }
    }else if(menuScreenOption==3){
        glColor3f(0,0,0);
        glRasterPos3f(-0.45,0.15,-0.3);
        char msg1[] = "CREDITS";
        for(int i=0;i<strlen(msg1);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1[i]);
        }glColor3f(0,0,0);
        glRasterPos3f(-0.45,0.05 + 0.03,-0.3);
        char msg1_0[] = "This game was made for the course CS364:Computer Graphics: 6th sem, IIIT Dharwad. ";
        for(int i=0;i<strlen(msg1_0);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_0[i]);
        }
        glRasterPos3f(-0.45,0.00 + 0.03,-0.3);
        char msg1_1[] = "We made this game entirely using GLUT in C++ with the addition of the SOIL library.";
        for(int i=0;i<strlen(msg1_1);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_1[i]);
        }
        glRasterPos3f(-0.45,-0.05 + 0.03,-0.3);
        char msg1_2[] = "All credits go to: ";
        for(int i=0;i<strlen(msg1_2);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_2[i]);
        }
        glRasterPos3f(-0.35,-0.10 + 0.01,-0.3);
        char msg1_3[] = "Om Morendha: 20BCS095";
        for(int i=0;i<strlen(msg1_3);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_3[i]);
        }
        glRasterPos3f(-0.35,-0.15 + 0.01,-0.3);
        char msg1_4[] = "Adithya Hegde: 20BCS006";
        for(int i=0;i<strlen(msg1_4);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_4[i]);
        }
        glRasterPos3f(-0.35,-0.20 + 0.01,-0.3);
        char msg1_5[] = "Rohan Singh: 20BCS113";
        for(int i=0;i<strlen(msg1_5);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_5[i]);
        }
        glRasterPos3f(-0.35,-0.25 + 0.01,-0.3);
        char msg1_6[] = "Parasa Sai Tarun: 20BCS096";
        for(int i=0;i<strlen(msg1_6);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,msg1_6[i]);
        }
        glRasterPos3f(-0.25,-0.33 + 0.01,-0.3);
        char msg1_7[] = "Thank you for playing. Have fun!";
        for(int i=0;i<strlen(msg1_7);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg1_7[i]);
        }
    }

    GLuint dart_board = SOIL_load_OGL_texture
    (
        "dart-board-plain.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glColor3f(1,1,1); //to make image stay not coloured
    glBindTexture(GL_TEXTURE_2D, dart_board);
    glTranslatef(0, 0, 0.001);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-0.9, 0.4, -0.3);
        glTexCoord2f(1, 1); glVertex3f(-0.7, 0.4, -0.3);
        glTexCoord2f(1, 0); glVertex3f(-0.7, 0.7, -0.3);
        glTexCoord2f(0, 0); glVertex3f(-0.9, 0.7, -0.3);
    glEnd();

    GLuint blue_board = SOIL_load_OGL_texture
    (
        "blue-board-red-dart.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glColor3f(1,1,1); //to make image stay not coloured
    glBindTexture(GL_TEXTURE_2D, blue_board);
    glTranslatef(0, 0, 0.001);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0); glVertex3f(0.8, 0.35, -0.3);
        glTexCoord2f(0, 0); glVertex3f(0.6, 0.35, -0.3);
        glTexCoord2f(0, 1); glVertex3f(0.6, 0.65, -0.3);
        glTexCoord2f(1, 1); glVertex3f(0.8, 0.65, -0.3);
    glEnd();

    GLuint x_o_board = SOIL_load_OGL_texture
    (
        "x-o-board.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glColor3f(1,1,1); //to make image stay not coloured
    glBindTexture(GL_TEXTURE_2D, x_o_board);
    glTranslatef(0, 0, 0.001);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0); glVertex3f(0.8, -0.4, -0.3);
        glTexCoord2f(0, 0); glVertex3f(0.6, -0.4, -0.3);
        glTexCoord2f(0, 1); glVertex3f(0.6, -0.7, -0.3);
        glTexCoord2f(1, 1); glVertex3f(0.8, -0.7, -0.3);
    glEnd();

    GLuint red_board = SOIL_load_OGL_texture
    (
        "red-board-blue-dart.jpeg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    glColor3f(1,1,1); //to make image stay not coloured
    glBindTexture(GL_TEXTURE_2D, red_board);
    glTranslatef(0, 0, 0.001);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3f(-0.9, -0.35, -0.3);
        glTexCoord2f(0, 1); glVertex3f(-0.7, -0.35, -0.3);
        glTexCoord2f(0, 0); glVertex3f(-0.7, -0.65, -0.3);
        glTexCoord2f(1, 0); glVertex3f(-0.9, -0.65, -0.3);
    glEnd();

    glColor3f(1,1,1); //to make image stay not coloured
    draw_dart(1, -3, -1, 3); //Fake Dart

    glColor3f(1,0,0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.33, -0.48 + 0.05, -0.3);
        glVertex3f(0.3, -0.48 + 0.05, -0.3);
        glVertex3f(0.3, -0.58 + 0.05, -0.3);
        glVertex3f(-0.33, -0.58 + 0.05, -0.3);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.34, -0.46 + 0.05, -0.3);
        glVertex3f(0.31, -0.46 + 0.05, -0.3);
        glVertex3f(0.31, -0.6 + 0.05, -0.3);
        glVertex3f(-0.34, -0.6 + 0.05, -0.3);
    glEnd();

    glColor3f(0,0,1);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.33, -0.63, -0.3);
        glVertex3f(0.3, -0.63, -0.3);
        glVertex3f(0.3, -0.73, -0.3);
        glVertex3f(-0.33, -0.73, -0.3);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3f(-0.34, -0.61, -0.3);
        glVertex3f(0.31, -0.61, -0.3);
        glVertex3f(0.31, -0.75, -0.3);
        glVertex3f(-0.34, -0.75, -0.3);
    glEnd();

    glColor3f(0,0,1);
    glRasterPos3f(-0.3,-0.55 + 0.05,-0.3);
    char msg6[] = "PLAY GAME: PRESS 'X' TO GET STARTED";
    for(int i=0;i<strlen(msg6);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg6[i]);
    }

    glColor3f(1,0,0);
    glRasterPos3f(-0.3,-0.7,-0.3);
    char msg7[] = "EXIT GAME: PRESS 'E' TO QUIT THE GAME";
    for(int i=0;i<strlen(msg7);i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg7[i]);
    }

    if(menuScreenOption == 4){

        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glColor3f(1,1,1);
        glLineWidth(3);
        glBegin(GL_POLYGON);
            glVertex3f(-0.43, 0.33, -0.3);
            glVertex3f(0.4, 0.33, -0.3);
            glVertex3f(0.4, -0.13, -0.3);
            glVertex3f(-0.43, -0.13, -0.3);
        glEnd();
        glColor3f(1,0,0);
        glBegin(GL_LINE_LOOP);
            glVertex3f(-0.44, 0.35, -0.3);
            glVertex3f(0.41, 0.35, -0.3);
            glVertex3f(0.41, -0.15, -0.3);
            glVertex3f(-0.44, -0.15, -0.3);
        glEnd();

        glColor3f(0,0,1);
        glRasterPos3f(-0.35,0.23,-0.2);
        char msg8[] = "ARE YOU SURE YOU WANT TO EXIT THE GAME? ";
        for(int i=0;i<strlen(msg8);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg8[i]);
        }

        glColor3f(1,0,0);
        glRasterPos3f(-0.4,0.03,-0.2);
        char msg9[] = "Yes, Exit game, Bye!(Y)";
        for(int i=0;i<strlen(msg9);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg9[i]);
        }
        glColor3f(0,0,1);
        glRasterPos3f(0.0,0.03,-0.2);
        char msg10[] = "No, I want to play more!(N)";
        for(int i=0;i<strlen(msg10);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,msg10[i]);
        }

    }

    glutSwapBuffers();
}


void keys(unsigned char key,int x,int y){
    if(menuScreenOption == 1 || menuScreenOption == 2 || menuScreenOption == 3){
        if(key == 'X' || key == 'x'){
            menuScreenOption = 0;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            float as = 1920.0 / 1080.0;
            gluPerspective(50, as, 0.1, 100);
            glColor3f(1,1,1);
            glutDisplayFunc(display);
        }
    }
    if(menuScreenOption == 1 || menuScreenOption == 2 || menuScreenOption == 3){
        if(key == 'h' || key == 'H'){
            menuScreenOption = 1;
            glutDisplayFunc(homeScreen);
        }
        if(key == 'i' || key == 'I'){
            menuScreenOption = 2;
            glutDisplayFunc(homeScreen);
        }
        if(key == 'c' || key == 'C'){
            menuScreenOption = 3;
            glutDisplayFunc(homeScreen);
        }
        if(key == 'e' || key == 'E'){
            menuScreenOption = 4;
            glutDisplayFunc(homeScreen);
        }
    }
    if(menuScreenOption == 4){
        if(key == 'y' || key == 'Y'){
            menuScreenOption = 4;
            exit(0);
        }
        if(key == 'n' || key == 'N'){
            menuScreenOption = 1;
            glutDisplayFunc(homeScreen);
        }
        if(key == 'o' || key =='O'){
            menuScreenOption = 5;
            glutDisplayFunc(outro);
        }
    }
    if(menuScreenOption == 5){
        if(key == 'h' || key == 'H'){
            menuScreenOption = 1;
            glutDisplayFunc(homeScreen);
        }
        if(key == 'e' || key == 'E'){
            menuScreenOption = 4;
            glutDisplayFunc(homeScreen);
        }

    }
    if(istat == 0){
        if((key == 'w' || key == 'W') && aim_y < 0.65)
            aim_y += 0.075;
        else if((key == 's' || key == 'S')  && aim_y > -0.65)
            aim_y -= 0.075;
        else if((key == 'a' || key == 'A')  && aim_x > -0.65)
            aim_x -= 0.075;
        else if((key == 'd' || key == 'D')  && aim_x < 0.65)
            aim_x += 0.075;
        else if(key == 'm' || key == 'M' || key == ' ')
            istat = 1;
    }
    else if(istat == 1){
        if(key == 'm' || key == 'M' || key == ' '){
            istat = 2;

            dartang_2 = exp(powline) * (aim_y + 1) / 0.4;

            dartang_1 = acos(5 / sqrt(25 + (aim_x - dartx) * (aim_x - dartx))) * 180 / M_PI;
            if (aim_x > 0)
                dartang_1 *= -1;
        }
    }
}

int main(int argc, char** argv){
    glutInit(&argc,argv);

    myinit();
    glutReshapeFunc(reshape);

    glutDisplayFunc(homeScreen);
    glutKeyboardFunc(keys);
    glutTimerFunc(100, update, 0);
    glutMainLoop();
    return 0;
}
