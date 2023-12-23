#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int win1,win2;
void Write(float x, float y, float z, float scale, char *s)
{
	int i, l = strlen(s);
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	for (i = 0; i < l; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
	glPopMatrix();
}
void frontsheet(void)
{
    glClearColor(0.5, 1.0, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	Write(-0.50, 0.9, 1, 0.0007, (char*)"SJB Institute Of Technology");
	Write(-0.55, 0.8, 1, 0.0006, (char*)"    Department of CSE");
	glColor3f(0.0, 0.0, 1.0);
	Write(-0.45, 0.6, 0.0, 0.0007, (char*) " Simulation of Helicopter");
	glColor3f(0.0, 0.0, 1.0);
	Write(-0.4, -0.8, 0.0, 0.0006, (char*) "Press 'C' to continue");
	glColor3f(1.0, 0.0, 0.0);
	Write(-1.0, 0.1, 0.0, 0.0007, (char*)" Submitted BY:");
	glColor3f(0.0, 0.0, 0.0);
	Write(-1.0, -0.03, 0.0, 0.0006, (char*) "1. Akanksha R Swamy: 1JB20CS003");
	Write(-1.0, -0.13, 0.0, 0.0006, (char*) "2. B POOJA:       1JB20CS018");
	glColor3f(1.0, 0.0, 0.0);
	Write(-1.0, -0.4, 0.0, 0.0007, (char*) " Under the guidance of: ");
	glColor3f(0.0, 0.0, 0.0);
	Write(0.15, -0.415, 0.0, 0.0006, (char*) " Dr Bindiya M K");
	//Write(0.15, -0.515, 0.0, 0.0006, (char*) " ");
	glFlush();
}
float theta = 0.0f, phi = 0.0f, r = 50.0f, eyeX = 0, eyeY = 0, eyeZ = 0, upX = 0,upY = 0,upZ = 0, windowHeight, windowWidth, zoom = 1.0f;
enum PolygonMode {WIREFRAME, SHADED};
enum ViewMode {PERSPECTIVE, ORTHOGRAPHIC};
PolygonMode polygonMode = SHADED;
ViewMode viewMode = PERSPECTIVE;
float ratio;
GLUquadricObj *obj = gluNewQuadric();
//Assignment 2 variables
float const PI = 3.14159265f;
float ro = -180.0f;
float curX = (float) 100*((cos(ro*PI/180) / (1 + sin(ro*PI/180)*sin(ro*PI/180))));
float curZ = (float) 100*((cos(ro*PI/180) * (sin(ro*PI/180))) / (1 + sin(ro*PI/180)*sin(ro*PI/180)));
float heliRot = PI,speed = 0.1f, fig8PointX = 0.0f, fig8PointZ = 0.0f;
bool moving = false;
float lx=-0.0f,lz=0.0f;// actual vector representing the camera's direction
float x=0.0f,z=5.0f, bladeAngle = 0.0f;// XZ position of the camera
enum ViewPerson {FIRST, THIRD};
ViewPerson viewPerson = THIRD;
// variables to compute frames per second
int frame;
long time, timebase;
char s[50];
GLfloat ltPosition[4] = {0, -3, 6.5, 1.0};
GLfloat ltDirection[4] = {0, 0, 1, 0.0};
float cutoffAngle = 60.0f, roo = -180, speed1 = 1000.0f, px, pz;
void setView(int w, int h) {// Prevent a divide by zero, when window is too short(you cant make a window of zero width).
    if (h == 0)
        h = 1;
    windowWidth = w;
    windowHeight = h;
    ratio =  w * 1.0 / h;
}
void drawBody(void) {
    //Main Body
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glScalef(1.0f, 1.0f, 2.0f);
    glutSolidCube(6.0f);
    glPopMatrix();
    //Right Side
    glPushMatrix();
    glScalef(1.0f, 3.0f, 1.0f);
    glTranslatef(3.0f, 0.0f, -6.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 1, 1, 12, 300, 200);
    glEnd();
    glPopMatrix();
    //Right Side Lid
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, 6.0f);
    glScalef(1.05f, 3.05f, 0.1f);
    glutSolidSphere(1.0, 100, 100);
    glPopMatrix();
    //Left Side
    glPushMatrix();
    glScalef(1.0f, 3.0f, 1.0f);
    glTranslatef(-3.0f, 0.0f, -6.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 1, 1, 12, 300, 200);
    glEnd();
    glPopMatrix();
    //Left Side Lid
    glPushMatrix();
    glTranslatef(-3.0f, 0.0f, 6.0f);
    glScalef(1.05f, 3.05f, 0.1f);
    glutSolidSphere(1.0, 100, 100);
    glPopMatrix();
    //Top Side
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, -6.0f);
    glScalef(3.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 1, 1, 12, 100, 100);
    glEnd();
    glPopMatrix();
}
void drawCockpit(void) {
    //Cockpit Windshield
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glScalef(1.33f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 6.0f);
    glutSolidSphere(3, 100, 100);
    glPopMatrix();
    //Cockpit Base
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 6.0f);
    glScalef(1.33f, 0.65f, 1.33f);
    glutSolidSphere(3, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}
void drawBlade(void) {
    glPushMatrix();
    glRotatef(10., 0.0, 0.0, 1.0);
    glScalef(0.75, 0.2, 10.0);
    GLUquadric *blade = gluNewQuadric();
    gluCylinder(blade, 0.5, 0.5, 1, 16, 1);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    gluDisk(blade, 0.0, 0.5, 16, 1);
    glPopMatrix();
    glPopMatrix();
}
void drawBladeCap() {
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 1.0);
    GLUquadric *cap = gluNewQuadric();
    gluCylinder(cap, 0.5, 0.5, 1, 16, 1);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    gluDisk(cap, 0.0, 0.5, 16, 1);
    glPopMatrix();
    glPopMatrix();
}
void drawHelicopter(void) {
    glRotatef(180, 0.0f, 1.0f, 0.0f);
    glClearColor(0.49f, 0.75f, 0.93f, 1.0f);
    drawBody();
    //Tail
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -16.0f);
    glScalef(0.25f, 1.0f, 2.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 1.5, 1.5, 6, 100, 100);
    glEnd();
    glPopMatrix();
    //Back Body
    glPushMatrix();
    glScalef(1.33f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -6.0f);
    glutSolidSphere(3, 100, 100);
    glPopMatrix();
    drawCockpit();
    glColor3f(0.5f, 0.5f, 0.5f);
    //Front Left Leg
    glPushMatrix();
    glTranslatef(2.5f, -3.0f, 2.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 0.25, 0.25, 1, 100, 100);
    glEnd();
    glPopMatrix();
    //Back Left Leg
    glPushMatrix();
    glTranslatef(2.5f, -3.0f, -2.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 0.25, 0.25, 1, 100, 100);
    glEnd();

    glPopMatrix();
    //Front Right Leg
    glPushMatrix();
    glTranslatef(-2.5f, -3.0f, 2.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 0.25, 0.25, 1, 100, 100);
    glEnd();
    glPopMatrix();
    //Back Right Leg
    glPushMatrix();
    glTranslatef(-2.5f, -3.0f, -2.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 0.25, 0.25, 1, 100, 100);
    glEnd();
    glPopMatrix();

    glColor3f(0.25f, 0.25f, 0.25f);
    //Landing Ski Left
    glPushMatrix();
    glTranslatef(2.5f, -4.25f, 0.0f);
    glScalef(1.0f, 0.5f, 10.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    //Landing Ski Right
    glPushMatrix();
    glTranslatef(-2.5f, -4.25f, 0.0f);
    glScalef(1.0f, 0.5f, 10.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.5f, 0.5f, 0.5f);
    //Tail Fin
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, -15.0f);
    glScalef(0.5f, 6.0f, 3.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    /*Rotors drawn in separate methods in case we want to rotate them at some point.It would be easier to modify the code that way. */
    // Draw top rotor
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0, 4.0, 0.0);
    glRotatef(bladeAngle, 0.0, 1.0, 0.0);
    drawBladeCap();
    glTranslatef(0.0, 1.0, 0.0);
    drawBlade();
    glRotatef(120.0, 0.0, 1.0, 0.0);
    drawBlade();
    glRotatef(120.0, 0.0, 1.0, 0.0);
    drawBlade();
    glPopMatrix();
    // Draw tail rotor
    glPushMatrix();
    glTranslatef(0.5, 4.5, -15.0);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(-90.0, 0.0, 0.0, 1.0);
    glRotatef(bladeAngle, 0.0, 1.0, 0.0);
    drawBladeCap();
    glTranslatef(0.0, 0.5, 0.0);
    drawBlade();
    glRotatef(120.0, 0.0, 1.0, 0.0);
    drawBlade();
    glRotatef(120.0, 0.0, 1.0, 0.0);
    drawBlade();
    glPopMatrix();

    glColor3f(0.4f, 0.4f, 0.4f);
    //Left Wing
    glPushMatrix();
    glTranslatef(6.5f, 0.5f, -2.0f);
    glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 0.075f, 0.60f);
    glutSolidCube(8.0f);
    glPopMatrix();
    //Right Wing
    glPushMatrix();
    glTranslatef(-6.5f, 0.5f, -2.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 0.075f, 0.60f);
    glutSolidCube(8.0f);
    glPopMatrix();
    //Tail Wings
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -11.0f);
    glScalef(1.0f, 0.10f, 0.5f);
    glutSolidCube(4.0f);
    glPopMatrix();

    glColor3f(0.20f, 0.20f, 0.20f);
    //Missile Launcher Left
    glPushMatrix();
    glTranslatef(6.0f, -0.25f, -5.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 1, 1, 6, 100, 100);
    glEnd();
    glPopMatrix();
    //Missile Launcher Right
    glPushMatrix();
    glTranslatef(-6.0f, -0.25f, -5.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 1, 1, 6, 100, 100);
    glEnd();
    glPopMatrix();

    glColor3f(0.25f, 0.25f, 0.25f);
    //Left Side Missile Head
    glPushMatrix();
    glTranslatef(6.0f, -0.25f, 1.0f);
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
    //Right Side Missile Head
    glPushMatrix();
    glTranslatef(-6.0f, -0.25f, 1.0f);
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
    //Machine gun
    glPushMatrix();
    glTranslatef(0.0f, -2.5f, 9.0f);
    glBegin(GL_POLYGON);
    gluCylinder(obj, 0.25, 0.25, 2, 100, 100);
    glEnd();
    glPopMatrix();

}
void drawGround(float size, int numSegments) {
    float init = -size / 2.,d = size / numSegments,x = init;
    for (int i = 0; i <= numSegments; ++i) {
        float z = init;
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(0, 1, 0);
        glColor3f(0.8, 0.8, 0.8);
        float xd = x+d;
        for (int j = 0; j <= numSegments; ++j) {
            glVertex3f(x, 0.0, z);
            glVertex3f(xd, 0.0, z);
            z += d;
        }
        glEnd();
        x = xd;
    }
}
float lemniscate(float a, float t) {
    return a * cos(t) / (sin(t) * sin(t) + 1);
}
void drawTrack(float a, int numSegments) {
    const float width = 10.;
    float t = 0.0;
    float dt = 2.0 * PI / numSegments;
    // init values
    float lastX = lemniscate(a, t);
    float lastZ = lastX * sin(t);
    float curX, curZ;
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0, 1, 0);
    glColor3f(1.0, 1.0, 1.0);
    // loop and draw
    for (int i = 0; i <= numSegments; ++i) {
        // increment state
        t += dt;
        curX = lemniscate(a, t);
        curZ = curX * sin(t);
        float dX = curX - lastX;
        float dZ = curZ - lastZ;
        lastX = curX;
        lastZ = curZ;
         //normalize dX, dZ
        float factor = 1. / sqrt(dX*dX + dZ*dZ);
        //rotate dX, dZ by 90 deg, and apply track width
        float tX = dX * factor;
        dX = -(dZ*factor) * width;
        dZ = tX * width;
        //draw vertices
        glVertex3f(curX-dX, 0.0, curZ-dZ);
        glVertex3f(curX+dX, 0.0, curZ+dZ);
    }
    glEnd();
}
void moveHelicopter(void) {
    if (moving) {
        ro += speed;
        bladeAngle += 31.f;
    }
    if (ro >= 180)
        ro = -180;
    float prevX = curX,prevZ = curZ;
    curX = (float) 100*((cos(ro*PI/180) / (1 + sin(ro*PI/180)*sin(ro*PI/180))));
    curZ = (float) 100*((cos(ro*PI/180) * (sin(ro*PI/180))) / (1 + sin(ro*PI/180)*sin(ro*PI/180)));
    if (moving) {
        float magnitude = sqrt(curX*curX + curZ*curZ);
        heliRot = atan2f(((curX - prevX) / magnitude)*-1.0f, ((curZ - prevZ) / magnitude)*-1.0f);
    }
    glTranslatef(curX,0.0f,curZ);
}
void moveCamera(void) {//method for camera motion
    if (viewPerson == THIRD) {
        // Restrict the angles within 0~360 deg (optional)
        if(theta > 360)theta = fmod((double)theta,360.0);
        if(phi > 360)phi = fmod((double)phi,360.0);
        // Spherical to Cartesian conversion.
        // Degrees to radians conversion factor 0.0174532
        eyeX = r * sin(theta*0.0174532) * sin(phi*0.0174532);
        eyeY = r * cos(theta*0.0174532);
        eyeZ = r * sin(theta*0.0174532) * cos(phi*0.0174532);
        // Reduce theta slightly to obtain another point on the same longitude line on the sphere.
        GLfloat dt=1.0;
        // Connect these two points to obtain the camera's up vector.
        upX=(r * sin(theta*0.0174532-dt) * sin(phi*0.0174532))-eyeX;
        upY=(r * cos(theta*0.0174532-dt))-eyeY;
        upZ=(r * sin(theta*0.0174532-dt) * cos(phi*0.0174532))-eyeZ;
    }
}
void renderScene(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport (0, 0, (GLsizei) windowWidth, (GLsizei) windowHeight*0.9);
    if (viewMode == PERSPECTIVE)
        gluPerspective(45.0f*zoom, ratio, 1.0f, 1000.0f);
    else if (viewMode == ORTHOGRAPHIC)
        glOrtho((-windowWidth/50)*zoom, (windowWidth/50)*zoom, (-windowHeight/50)*zoom, (windowHeight/50)*zoom, 1.0f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    moveCamera();
    lx = (sin(heliRot+PI));
    lz = (cos(heliRot+PI));
    float camX = curX + lx * 10.0f;
    float camZ = curZ + lz * 10.0f;
    //Set camera
    if (viewPerson == THIRD)
        gluLookAt(eyeX+curX, eyeY, eyeZ+curZ,curX, 0.0f, curZ,upX, upY, upZ);
    else
        gluLookAt(camX, 1.5f, camZ,camX+lx, 1.5f, camZ+lz,0.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(0.0f, -10, 0.0f);
    drawGround(250, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -9.9, 0.0f);
    drawTrack(100.0, 100.0);
    glPopMatrix();

    glPushMatrix();
    moveHelicopter();
    glRotatef(heliRot/PI*180, 0.0f, 1.0f, 0.0f);
    drawHelicopter();
    glPopMatrix();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)//keyboard method
{
   switch(key) {
        case 's':
            moving = true;
            break;
        case 'e':
            moving = false;
            break;
        case 'a':
            if (moving && speed < 1.0f)
                speed += 0.1f;
            break;
        case 'A':
            if (moving && speed > 0.1f) {
                speed -= 0.1f;
                if (speed < 0.1f)
                    speed = 0.1f;
            }
            break;
        default:
            break;
    }
    moveCamera();
    glutPostRedisplay(); /* this redraws the scene without waiting for the display callback so that any changes appear instantly */
}
void specialKeyboard(int key, int x, int y) {//special keyboard method
    switch(key) {
        case GLUT_KEY_UP:
            if (viewPerson == THIRD)
                theta -= 4.0f;
            break;
        case GLUT_KEY_DOWN:
            if (viewPerson == THIRD)
                theta += 4.0f;
            break;
        case GLUT_KEY_RIGHT:
            if (viewPerson == THIRD)
                phi += 4.0f;
            break;
        case GLUT_KEY_LEFT:
            if (viewPerson == THIRD)
                phi -= 4.0f;
            break;
            default:
            break;
    }
    moveCamera();
    glutPostRedisplay();
}
void keyboards(unsigned char key, int x4, int y4)
{
	if (key == 'c' || key == 'C')
	{
		glutDestroyWindow(win1);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		win2 = glutCreateWindow("Simulation of Helicopter");
		glutDisplayFunc(renderScene);
		glutReshapeFunc(setView);
		glutKeyboardFunc(keyboard);
        glutSpecialFunc(specialKeyboard);
        glutIdleFunc(renderScene);
        moveCamera();
	}
}
int main(int argc, char **argv) {
    glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(700,600);
    win1=glutCreateWindow("Mini Project");
    glFlush();
    glutDisplayFunc(frontsheet);
    glutKeyboardFunc(keyboards);
    glutMainLoop();
    return 1;
}
