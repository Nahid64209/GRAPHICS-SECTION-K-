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
