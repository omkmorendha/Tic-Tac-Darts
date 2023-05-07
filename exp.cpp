#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

int status = 1;
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
    
    glClearColor(0, 0, 0, 1);
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
    glLoadIdentity();
    gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);

    glRotatef(dartang_1, 0, 1, 0);
    glRotatef(dartang_2, 0, 0, 1);
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
    glRotatef(-dartang_2, 0, 0, 1);
    glRotatef(-dartang_1, 0, 1, 0);
    glColor3f(1, 1, 1);
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

int menuScreenOption;
bool redColor = 1;
int i = 0;

void outro(){
    void display();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);

    menuScreenOption = 5;

    char msgr[] = "RED PLAYER IS THE WINNER";
    char msgb[] = "BLUE PLAYER IS THE WINNER";

    // make it blink slow
    if(i%3==0){
        redColor = !redColor;
    }

    if(player == 1){
        glColor3f(redColor,0,0);

        glPushMatrix();
        glTranslatef(-0.45, 0.15, 0);
        glScalef(1/2600.0, 1/2000.0, 1/2600.0);
        for (int i=0;i<strlen(msgr);i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, msgr[i]);
        glPopMatrix();
    }
    else{
        glColor3f(0, 0, redColor);

        glPushMatrix();
        glTranslatef(-0.45, 0.15, 0);
        glScalef(1/2600.0, 1/2000.0, 1/2600.0);
        for (int i=0;i<strlen(msgb);i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, msgb[i]);
        glPopMatrix();
    }


    char msg1[] = "Press H to go back to home, E to exit.";
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(-0.35, -0.65, 0);
    glScalef(1/3600.0, 1/3000.0, 1/3600.0);
    for (int i=0;i<strlen(msg1);i++)
      glutStrokeCharacter(GLUT_STROKE_ROMAN, msg1[i]);
    glPopMatrix();

    i += 1;
    if(i>100){
        menuScreenOption = 1;
        glutDisplayFunc(display);
    }

    glutSwapBuffers();
}

void display(){
    if(status == 2){
        outro();
    }
    else
    {
        void aim_key(unsigned char key, int x, int y);
        void display();

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

        if(check() == 1 || check() == 2){
            status = 2;
            myinit();
            glutDisplayFunc(display);
            glutKeyboardFunc(aim_key);
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

void aim_key(unsigned char key, int x, int y){
    if(status == 1){
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
    else if(status == 2){
        if(key == 'h' || key == 'H'){
            status = 1;
            myinit();
            glutDisplayFunc(display);
            glutKeyboardFunc(aim_key);
        }
        else if(key == 'e' || key == 'E')
            exit(0);   
    }
}

int main(int argc, char** argv){
    glutInit(&argc,argv);
    srand(time(0));

    myinit();
    glutReshapeFunc(reshape);
    
    glutDisplayFunc(display);

    glutKeyboardFunc(aim_key);
    glutTimerFunc(100, update, 0);
    glutMainLoop();
    return 0;
}