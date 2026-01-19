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
