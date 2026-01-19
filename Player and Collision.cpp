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
