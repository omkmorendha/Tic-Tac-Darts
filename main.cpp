#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;

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

void draw_dart(){
    
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);

    setbullco();
    draw_scenery();
    draw_board();

    bull[0][1].stat = 1;
    bull[0][1].score = 2;

    bull[0][2].stat = 2;
    bull[0][2].score = 7;

    glTranslatef(0, 0, 0.001);
    draw_all_bull();
    glTranslatef(0, 0, -0.001);

    glLoadIdentity();
    draw_dart();

    glutSwapBuffers();
    glFlush();
}

void reshape(int width, int height){
    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective(50, aspect, 0.1, 100);  
}

int main(int argc, char** argv){
    glutInit(&argc,argv);

    myinit();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}