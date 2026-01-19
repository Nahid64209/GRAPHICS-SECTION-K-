//marged

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



// Spider-Man always visible
void drawHead(){
    glPushMatrix(); glTranslatef(0,1.0,0);
    glColor3f(0.9f,0.05f,0.05f); // bright red
    glutSolidSphere(0.2f,20,20);
    glColor3f(1,1,1); // eyes
    glPushMatrix(); glTranslatef(-0.08,0.05,0.18); glutSolidSphere(0.05,10,10); glPopMatrix();
    glPushMatrix(); glTranslatef(0.08,0.05,0.18); glutSolidSphere(0.05,10,10); glPopMatrix();
    glPopMatrix();
}

void drawBody(){
    glPushMatrix(); glTranslatef(0,0.6,0);
    glColor3f(0.9f,0.05f,0.05f); glutSolidCube(0.5); // torso red
    glColor3f(0.05f,0.1f,0.85f); // legs blue
    glPushMatrix(); glTranslatef(-0.15,-0.35,0); glutSolidCube(0.2); glPopMatrix();
    glPushMatrix(); glTranslatef(0.15,-0.35,0); glutSolidCube(0.2); glPopMatrix();
    glPopMatrix();
}

void drawArms(){
    glPushMatrix(); glTranslatef(0,0.7,0);
    glColor3f(0.9f,0.05f,0.05f); // red arms
    GLUquadric* quad=gluNewQuadric();
    glPushMatrix(); glTranslatef(-0.35,0,0); glRotatef(90,0,0,1); gluCylinder(quad,0.08,0.08,0.4,10,10); glPopMatrix();
    glPushMatrix(); glTranslatef(0.35,0,0); glRotatef(90,0,0,1); gluCylinder(quad,0.08,0.08,0.4,10,10); glPopMatrix();
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void drawSpiderMan(){
    glPushMatrix();
    glTranslatef(player.x,player.y,player.z);
    glDisable(GL_LIGHTING); // ensures always visible
    drawBody(); drawHead(); drawArms();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Web
void drawWeb(){
    if(!web.active) return;
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3f(web.startX,web.startY,web.startZ);
    glVertex3f(web.endX,web.endY,web.endZ);
    glEnd();
    glEnable(GL_LIGHTING);
}

// Collision
bool checkCollision(float x,float y,float z){
    for(auto &b:buildings){
        float halfW=b.width/2, halfD=b.depth/2;
        if(x>=b.x-halfW && x<=b.x+halfW && z>=b.z-halfD && z<=b.z+halfD && y<=b.height) return true;
    }
    return false;
}

// Move player with collision and range check
void movePlayer(float dt){
    float prevX = player.x;
    float prevY = player.y;
    float prevZ = player.z;

    float vx=0,vz=0;
    if(keyLeft) vx-=1; if(keyRight) vx+=1;
    if(keyUp) vz-=1; if(keyDown) vz+=1;

    float mag = sqrtf(vx*vx + vz*vz);
    if(mag > 0.001f){
        vx /= mag; vz /= mag;
        player.x += vx * player.speed * dt;
        player.z += vz * player.speed * dt;
        player.dirX = vx; player.dirZ = vz;
    }

    // Web movement
    if(web.active){
        float dx = web.endX - player.x;
        float dz = web.endZ - player.z;
        float dy = web.endY - player.y;
        float dist = sqrtf(dx*dx + dy*dy + dz*dz);
        if(dist > 0.1f){
            player.x += dx*0.05f;
            player.z += dz*0.05f;
            player.y += dy*0.05f;
        } else web.active = false;
    }

    // Out-of-range or building collision
    bool outOfRange = (player.x < -50 || player.x > 50 || player.z > 10 || player.z < -90 || player.y < 0 || player.y > 20);
    if(checkCollision(player.x, player.y, player.z) || outOfRange){
        player.x = prevX;
        player.y = prevY;
        player.z = prevZ;
    }
}

// Mini-map overlay (simplified, red background, yellow Spider-Man)
void drawMiniMap(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    int mapWidth = 150, mapHeight = 150;
    int mapX = 10, mapY = windowHeight - mapHeight - 10;

    // Red background
    glColor3f(0.6f,0.1f,0.1f);
    glBegin(GL_QUADS);
    glVertex2f(mapX, mapY);
    glVertex2f(mapX + mapWidth, mapY);
    glVertex2f(mapX + mapWidth, mapY + mapHeight);
    glVertex2f(mapX, mapY + mapHeight);
    glEnd();

    // Border
    glColor3f(1,0.5f,0.5f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(mapX, mapY);
    glVertex2f(mapX + mapWidth, mapY);
    glVertex2f(mapX + mapWidth, mapY + mapHeight);
    glVertex2f(mapX, mapY + mapHeight);
    glEnd();

    float scaleX = mapWidth / 100.0f;
    float scaleZ = mapHeight / 100.0f;

    // Draw buildings (white)
    glColor3f(1,1,1);
    for(auto &b: buildings){
        float bx = mapX + (b.x + 50) * scaleX;
        float bz = mapY + (b.z + 90) * scaleZ;
        float bw = b.width * scaleX;
        float bh = b.depth * scaleZ;
        glBegin(GL_QUADS);
        glVertex2f(bx - bw/2, bz - bh/2);
        glVertex2f(bx + bw/2, bz - bh/2);
        glVertex2f(bx + bw/2, bz + bh/2);
        glVertex2f(bx - bw/2, bz + bh/2);
        glEnd();
    }

    // Spider-Man (yellow)
    glColor3f(1,1,0);
    float px = mapX + (player.x + 50) * scaleX;
    float pz = mapY + (player.z + 90) * scaleZ;
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2f(px, pz);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Display
void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    gluLookAt(player.x,6,player.z+8, player.x,0.8,player.z-4, 0,1,0);
    drawSkyWithStars();

    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3f(0.9,0.9,0.95); // ground
    glVertex3f(-200,-0.01,-200); glVertex3f(200,-0.01,-200);
    glVertex3f(200,-0.01,50); glVertex3f(-200,-0.01,50);
    glEnd();
    glEnable(GL_LIGHTING);

    drawBuildings(); drawWeb(); drawSpiderMan();
    drawMiniMap(); // Mini-map overlay

    glutSwapBuffers();
}

// Reshape
void reshape(int w,int h){
    windowWidth=w; windowHeight=h; if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60,(float)w/h,0.1,500);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard
void handleKeyDown(unsigned char k,int x,int y){
    if(k==27) exit(0);
    if(k=='a'||k=='A') keyLeft=true;
    if(k=='d'||k=='D') keyRight=true;
    if(k=='w'||k=='W') keyUp=true;
    if(k=='s'||k=='S') keyDown=true;
}
void handleKeyUp(unsigned char k,int x,int y){
    if(k=='a'||k=='A') keyLeft=false;
    if(k=='d'||k=='D') keyRight=false;
    if(k=='w'||k=='W') keyUp=false;
    if(k=='s'||k=='S') keyDown=false;
}
void specialDown(int key,int x,int y){
    if(key==GLUT_KEY_LEFT) keyLeft=true;
    if(key==GLUT_KEY_RIGHT) keyRight=true;
    if(key==GLUT_KEY_UP) keyUp=true;
    if(key==GLUT_KEY_DOWN) keyDown=true;
}
void specialUp(int key,int x,int y){
    if(key==GLUT_KEY_LEFT) keyLeft=false;
    if(key==GLUT_KEY_RIGHT) keyRight=false;
    if(key==GLUT_KEY_UP) keyUp=false;
    if(key==GLUT_KEY_DOWN) keyDown=false;
}

// Mouse web
void mouseClick(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        GLint viewport[4]; GLdouble modelview[16], projection[16]; GLfloat winX,winY,winZ; GLdouble posX,posY,posZ;
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview); glGetDoublev(GL_PROJECTION_MATRIX,projection); glGetIntegerv(GL_VIEWPORT,viewport);
        winX=(float)x; winY=(float)viewport[3]-(float)y;
        glReadPixels(x,int(winY),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
        gluUnProject(winX,winY,winZ,modelview,projection,viewport,&posX,&posY,&posZ);
        web.active=true; web.startX=player.x; web.startY=player.y; web.startZ=player.z;
        web.endX=(float)posX; web.endY=(float)posY+2.0f; web.endZ=(float)posZ;
    }
}

// Timer
void timer(int value){
    static int prev=glutGet(GLUT_ELAPSED_TIME);
    int now=glutGet(GLUT_ELAPSED_TIME);
    float dt=(now-prev)/1000.0f; if(dt>0.1f) dt=0.1f; prev=now;
    movePlayer(dt); glutPostRedisplay(); glutTimerFunc(16,timer,0);
}

// Init scene
void initScene(){
    srand((unsigned)time(0));
    for(int i=0;i<25;i++){
        Building b; b.x=(rand()%81)-40; b.z=-10-(rand()%80);
        b.width=2.0f+(rand()%4); b.depth=2.0f+(rand()%4); b.height=6.0f+(rand()%24);
        buildings.push_back(b);
    }
    for(int i=0;i<numStars;i++) stars.emplace_back((rand()%100)-50,(rand()%60)-10);
}

// Init OpenGL
void initGL(){ glEnable(GL_DEPTH_TEST); glEnable(GL_COLOR_MATERIAL); glShadeModel(GL_SMOOTH); glClearColor(0.5f,0.8f,0.95f,1.0f); }

// Main
int main(int argc,char**argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Spider-Man OpenGL Visible Demo");
    initGL(); initScene();
    glutDisplayFunc(display); glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyDown); glutKeyboardUpFunc(handleKeyUp);
    glutSpecialFunc(specialDown); glutSpecialUpFunc(specialUp);
    glutMouseFunc(mouseClick); glutTimerFunc(16,timer,0);
    std::cout<<"Controls: WASD/Arrow keys to move, Click to shoot web, ESC to exit\n";
    glutMainLoop();
    return 0;
}
