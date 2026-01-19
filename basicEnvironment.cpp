#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

int windowWidth = 800;
int windowHeight = 600;

// Player
struct Player {
    float x=0.0f, z=-5.0f, y=0.6f;
    float speed=6.0f;
    float dirX=0.0f, dirZ=-1.0f;
} player;

bool keyLeft=false,keyRight=false,keyUp=false,keyDown=false;

// Stars
std::vector<std::pair<float,float>> stars;
int numStars=120;

// Buildings
struct Building {
    float x,z,width,depth,height;
};
std::vector<Building> buildings;

// Web
struct Web {
    bool active=false;
    float startX,startY,startZ;
    float endX,endY,endZ;
} web;

// Cube utility
void drawCube(float w,float h,float d){
    float hw=w/2, hh=h/2, hd=d/2;
    glBegin(GL_QUADS);
    // Front
    glVertex3f(-hw,-hh, hd); glVertex3f(hw,-hh, hd); glVertex3f(hw, hh, hd); glVertex3f(-hw, hh, hd);
    // Back
    glVertex3f(-hw,-hh, -hd); glVertex3f(-hw, hh, -hd); glVertex3f(hw, hh, -hd); glVertex3f(hw,-hh, -hd);
    // Left
    glVertex3f(-hw,-hh, -hd); glVertex3f(-hw,-hh, hd); glVertex3f(-hw, hh, hd); glVertex3f(-hw, hh, -hd);
    // Right
    glVertex3f(hw,-hh, -hd); glVertex3f(hw, hh, -hd); glVertex3f(hw, hh, hd); glVertex3f(hw,-hh, hd);
    // Top
    glVertex3f(-hw, hh, -hd); glVertex3f(-hw, hh, hd); glVertex3f(hw, hh, hd); glVertex3f(hw, hh, -hd);
    // Bottom
    glVertex3f(-hw, -hh, -hd); glVertex3f(hw, -hh, -hd); glVertex3f(hw, -hh, hd); glVertex3f(-hw, -hh, hd);
    glEnd();
}

// Sky
void drawSkyWithStars(){
    glDisable(GL_LIGHTING);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
    glColor3f(0.5f,0.8f,0.95f); // light sky
    glVertex3f(-50,-10,-100); glVertex3f(50,-10,-100); glVertex3f(50,50,-100); glVertex3f(-50,50,-100);
    glEnd();
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    for(auto &s:stars) glVertex3f(s.first,s.second,-90);
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// Buildings with white windows
void drawBuildings(){
    for(auto &b:buildings){
        glPushMatrix();
        glTranslatef(b.x, b.height/2, b.z);

        // Building wall color (slightly gray for depth)
        glColor3f(0.7f, 0.7f, 0.7f);
        drawCube(b.width, b.height, b.depth);

        // Draw windows (all white)
        int rows = int(b.height);
        int cols = int(b.width * 2);
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                float wx = -b.width/2 + j*0.5f + 0.1f;
                float wy = -b.height/2 + i*0.5f + 0.1f;
                float wz = b.depth/2 + 0.01f;

                glColor3f(1.0f,1.0f,1.0f); // white windows
                glBegin(GL_QUADS);
                glVertex3f(wx, wy, wz);
                glVertex3f(wx + 0.3f, wy, wz);
                glVertex3f(wx + 0.3f, wy + 0.3f, wz);
                glVertex3f(wx, wy + 0.3f, wz);
                glEnd();
            }
        }
        glPopMatrix();
    }
}

