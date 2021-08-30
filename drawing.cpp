#include <windows.h>		// Header File For Windows

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <fstream>
#include <vector>
#include "resources.cpp"
#include "lib/Camera.cpp"
#include "lib/heightMap.h"
#include "lib/TgaLoader.h"
#include <cmath>
#include <stdlib.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
//#include "lib/3DTexture.h"
//#include "lib/Model_3DS.h"
//#include "lib/TgaLoader.h"
//#include"texture.cpp"
#include <time.h>       /* time */


#include <../include/glm-master/glm/glm.hpp>
#include<../include/texture.h>

//#include <../include/glm-masterglm/gtc/matrix_transform.hpp>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"GLAUX.LIB")

// linker --> input add --> legacy_stdio_definitions.lib
//Project --> property --> c/c++ --> preprocessor --> preprocessor definitions --> _CRT_SECURE_NO_WARNINGS
//Project --> property --> c/c++ --> preprocessor --> preprocessor definitions -->  _CRT_NONSTDC_NO_DEPRECATE


HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

PRAWINPUTDEVICE Rid;



#include<vector>
struct point { double x; double  y; double  z; };

Camera camera = Camera(0, 6, 0);
double Width = 1280, Height = 720;
// Relative X and Y movement each frame.
int relativeX = 0, relativeY = 0, fps = 30;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
														// Calculate The Aspect Ratio Of The Window
	gluPerspective(80.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


void centerCursor() {

	RECT window;
	GetWindowRect(hWnd, &window);
	POINT mouseCoords = { (window.left + window.right) / 2, (window.top + window.bottom) / 2 };

	SetCursorPos(mouseCoords.x, mouseCoords.y);
}

void updateCamera() {
	camera.updateAngle(relativeX, relativeY);

	if (relativeX || relativeY) {
		centerCursor();
		relativeX = relativeY = 0;
	}

	if (keys[VK_SHIFT]) camera.speedFactor *= 10; // sprint

												  // Front
	if (keys['W']) camera.move();

	// Right
	camera.updateAngleAbsolute(pi / 2, 0);
	if (keys['D']) camera.move();

	// Back
	camera.updateAngleAbsolute(pi / 2, 0);
	if (keys['S']) camera.move();

	// Left
	camera.updateAngleAbsolute(pi / 2, 0);
	if (keys['A']) camera.move();

	// Front again
	camera.updateAngleAbsolute(pi / 2, 0);

	if (keys[VK_SHIFT]) camera.speedFactor /= 10;

	if (keys[VK_ESCAPE]) exit(0);
}

void registerRawInputDevices() {
	Rid = (PRAWINPUTDEVICE)malloc(2 * sizeof(RAWINPUTDEVICE));
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = 0;
	RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));
}



double x = 0;











//float angle = 0;
/*GLfloat xRotated = 0, yRotated = 0, zRotated = 0, xTranslate = 0, yTranslate = 0, zTranslate = 0, vx = 0, xy = 0, vz = 0;
GLfloat m = 2, M = 1, v0 = 5, theta = 0;
GLfloat r = 0.4, a = 0.2, b = 0.2, c = sqrt(abs(r*r - a*a - b*b)),


f = 2 * m*v0 / (1 + (m / M) + ((5 / (2 * r*r))*(a*a + b*b*cos(theta)*cos(theta) + c*c*sin(theta)*sin(theta) - 2 * b*c*cos(theta)*sin(theta))));
GLfloat I = 2 * m*r*r / 5;
GLfloat wx = -1 * (c*f*sin(theta) + b*f*cos(theta)) / I, wy = -1 * a*f*cos(theta) / I, wz = b*f*cos(theta) / I;
*/



void drawSphere(double r, int lats, int longs, int id) {
	//The parameters lat defines how many horizontal lines you want to have in
	//your sphere and lon how many vertical lines.
	//r is the radius of your sphere.
	float M_PI = 3.14;
	int i, j;

	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			if (id == 21)
				glColor3f(0.5, 0.5, 0.5);
			else if (id % 2 == 0)
				glColor3f(1, 0, 0);
			else glColor3f(0, 0, 1);

			glVertex3f(r * x * zr0, r * y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, z1);
			if (id == 21)
				glColor3f(0, 0, 0);
			else glColor3f(1, 1, 1);

			glVertex3f(r * x * zr1, r * y * zr1, r * z1);
		}
		glEnd();
	}
	glColor3f(1, 1, 1);
}








class Physics {

public:
	double friction_coffecient;
	double felxibilty_coffecient;
	double Cr;
	double p;
	double g;
	double Fr;
	double dt = 0.16;


	Physics() {
		friction_coffecient = 0.25;
		felxibilty_coffecient = 0.98;
		Cr = 0.06;
		p = 0.00012;
		g = 9.8;
	}

	double calculate_air_resistance(double mass, double r, double speed_length) {
		double A = 3.14 * r * r;
		return  speed_length*speed_length * 0.5* Cr * p * A;
	}

	double calculate_friction(double mass) {
		return mass*g*friction_coffecient;
	}

	double calculate_W_Wr(double mass) {
		double W = mass * g;
		double Wr = -W;
		return W + Wr;
	}



};
Physics physics;

class Ball {
public:
	double mass;
	glm::vec3 center;
	glm::vec3 speed;
	glm::vec3 angularVelocity;
	double rotationAngular;
	glm::vec3  rotationAxis;
	int id;
	double r;

	bool tr = true, tl = true, tu = true, td = true;



	Ball(int id, glm::vec3 center, double r, double mass) {
		this->mass = mass;
		this->id = id;
		this->center = center;
		this->r = r;
	}

	void updateSpeed() {

		double speedLength1 = glm::length(speed);

		double forces = (physics.calculate_air_resistance(mass, r, speedLength1)
			+ physics.calculate_friction(mass)
			+ physics.calculate_W_Wr(mass));

		double acceleration = forces / mass;
		double speedLength2 = speedLength1 - acceleration * physics.dt;
		double dv = speedLength2 / speedLength1;

		if (dv>0.01)
			speed *= dv;
		else speed = glm::vec3(0);










		this->angularVelocity = speed;
		this->angularVelocity *= r;

		rotationAngular += glm::length(angularVelocity)*physics.dt;
		glm::vec3 nw = glm::normalize(angularVelocity);
		rotationAxis = glm::vec3(nw.z, nw.y, nw.x);


	}

	void moving() {

		this->updateSpeed();

		this->center[0] += this->speed[0] * 0.01*0.6;
		this->center[2] += this->speed[2] * 0.01*0.6;
	}

	void draw() {

		glPushMatrix();
		glTranslatef(this->center.x, r, this->center.z);
		if (glm::length(speed)>0)
			glRotatef(rotationAngular, rotationAxis[0], rotationAxis[1], rotationAxis[2]);


		drawSphere(this->r, 10, 10, this->id);
		glPopMatrix();
	}

	void go() {
		this->moving();
		this->draw();
	}


	void collision(Ball &b2) {

		glm::vec3 unitNormal = glm::normalize(center - b2.center);
		glm::vec3 unitTangent = glm::vec3(-unitNormal.z, 0, unitNormal.x);

		double v1n = glm::dot(unitNormal, speed);
		double v2n = glm::dot(unitNormal, b2.speed);

		double v1t = glm::dot(unitTangent, speed);
		double v2t = glm::dot(unitTangent, b2.speed);


		double v11n = (mass*v1n + b2.mass*v2n + b2.mass * physics.felxibilty_coffecient * (v2n - v1n)) / (mass + b2.mass);
		double v22n = (mass*v1n + b2.mass*v2n - mass * physics.felxibilty_coffecient * (v2n - v1n)) / (mass + b2.mass);


		glm::vec3 v1nv = glm::vec3(v11n * unitNormal.x, v11n*unitNormal.y, v11n*unitNormal.z);
		glm::vec3 v2nv = glm::vec3(v22n * unitNormal.x, v22n*unitNormal.y, v22n*unitNormal.z);

		glm::vec3 v1tv = glm::vec3(v1t * unitTangent.x, v1t*unitTangent.y, v1t*unitTangent.z);
		glm::vec3 v2tv = glm::vec3(v2t * unitTangent.x, v2t*unitTangent.y, v2t*unitTangent.z);


		speed = v1nv + v1tv;
		b2.speed = v2nv + v2tv;



	}

	void collisionWall(glm::vec3 wall) {


		if (wall.x > center.x || wall.x<center.x)
			speed.x *= -1;
		if (wall.z > center.z || center.z>wall.z)
			speed.z *= -1;



	}

	void detect_collision(Ball &b2) {
		float dist = glm::distance(center, b2.center);
		if (dist <= r + b2.r)
		{
			collision(b2);
		}


	}
};





class table {

public:
	point left_up;
	point right_down;
	table() {};
	table(point left_up, point right_down) {
		this->left_up = left_up;
		this->right_down = right_down;
	}


	void draw() {};


};

class controller {

private:
	std::vector <Ball> balls;
	table tb;


public:

	controller() {};

	void set_table(table tb) {
		this->tb = tb;
	}
	void add_ball(Ball& b) {
		balls.push_back(b);
	}

	void collisionWall(Ball &b) {

		if (b.tl&&b.center.x - b.r <= tb.left_up.x)
		{
			b.collisionWall(glm::vec3(tb.left_up.x, 0, b.center.z));
		}

		if (b.tr&&b.center.x + b.r >= tb.right_down.x)
		{
			b.collisionWall(glm::vec3(tb.right_down.x, 0, b.center.z));
		}

		if (b.tu&&b.center.z - b.r <= tb.left_up.z)
		{
			b.collisionWall(glm::vec3(b.center.x, 0, tb.left_up.z));
		}

		if (b.td&&b.center.z + b.r >= tb.right_down.z)
		{
			b.collisionWall(glm::vec3(b.center.x, 0, tb.right_down.z));
		}

	}
	void addForce(double angle, double forceLength) {
		double speedLength = forceLength / balls[0].mass;
		double angel = 2 * 3.14*angle / 360;
		balls[0].speed[0] = speedLength * glm::cos(angel);
		balls[0].speed[2] = speedLength * -glm::sin(angel);
		balls[0].angularVelocity = balls[0].speed;
		balls[0].angularVelocity *= balls[0].r;

	}

	void addForce(glm::vec3 froce) {
		balls[0].speed = froce;
		balls[0].speed /= balls[0].mass;
		balls[0].angularVelocity = balls[0].speed;
		balls[0].angularVelocity *= balls[0].r;

	}


	void go() {
		for (int i = 0; i<balls.size(); i++)
			balls[i].go();

		for (int i = 0; i < balls.size(); i++) {

			for (int j = i + 1; j < balls.size(); j++) {

				balls[i].detect_collision(balls[j]);
			}

			; collisionWall(balls[i]);


		}



	}

};



glm::vec3 speed1(-40, 0, 40);
glm::vec3 center1(-15, 1, -3);
double r1 = 1;
Ball ball1(1, center1, r1, 10);



glm::vec3 speed2(40, 0, 40);
glm::vec3 center2(20, 1, -15);
double r2 = 1;
Ball ball2(2, center2, r2, 1);



glm::vec3 speed3(40, 0, -40);
glm::vec3 center3(-22, 1, 5);
double r3 = 1;
Ball ball3(3, center3, r3, 1);



glm::vec3 speed4(20, 0, -5);
glm::vec3 center4(14, 1, 4);
double r4 = 1;
Ball ball4(4, center4, r4, 1);


glm::vec3 speed5(0, 0, 0);
glm::vec3 center5(-10, 1, -10);
double r5 = 1;
Ball ball5(5, center5, r5, 1);

glm::vec3 speed6(0, 0, 0);
glm::vec3 center6(0, 1, -15);
double r6 = 1;
Ball ball6(6, center6, r6, 1);




glm::vec3 speed20(0, 0, 0);
glm::vec3 center20(0, 1, 10);
double r20 = 1;
Ball ball20(20, center20, r20, 1);


glm::vec3 speed21(0, 0, -100);
glm::vec3 center21(0, 1, 20);
double r21 = 1;
Ball ball21(21, center21, r21, 1);

point p1 = { -50,0,-25 };
point p2 = { 50,0,25 };
table tb(p1, p2);
controller C;




int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{

	loadTextures();
	loadModels();

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.50f);					// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations


														//	for (int j = 0; j < 100; j++)a[i][j] = 0;


														//glEnable(GL_LIGHTING);
	C.set_table(tb);
	C.add_ball(ball21);
	C.add_ball(ball20);
	C.add_ball(ball5);
	C.add_ball(ball6);
	C.add_ball(ball4);
	C.add_ball(ball2);
	C.add_ball(ball3);
	C.add_ball(ball1);

	//ambientLighting();
	registerRawInputDevices();



	return TRUE;										// Initialization Went OK
}

double cowAngel = 0;
double cowForce = 0;
bool draw_cow = true;
void drawCow() {
	if (!draw_cow)return;
	glPushMatrix();
	glTranslatef(0, 0, 20);
	glRotatef(cowAngel, 0, 1, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 2, 0);
	glVertex3f(cowForce, 2, 0);
	glColor3f(1, 1, 1);
	glEnd();
	glPopMatrix();
}



void drawland(float x, float y, float z)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, wall2);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(x, -y, -z);
	glTexCoord2f(0, 25);
	glVertex3d(x, -y, z);
	glTexCoord2f(25, 0);
	glVertex3f(-x, -y, z);
	glTexCoord2f(25, 25);
	glVertex3f(-x, -y, -z);
	glEnd();
	glPopMatrix();
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, wall1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, wall1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, wall1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, wall1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, wall1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, wall1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	//glColor3f(1, 1, 1);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);

}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();



	ShowCursor(FALSE);

	updateCamera();
	gluLookAt(camera.posX, camera.posY, camera.posZ, camera.posX + camera.dirX, camera.posY, camera.posZ + camera.dirZ, 0, 1, 0);

	Draw_Skybox(0, 0, 0, 200, 100, 200);
	//drawland(100,5,100);
	//drawbilliord();
	Table.pos.y = -15;
	Table.Draw();
	C.go();

	if (keys['L'])
	{
		cowAngel += 1;
	}
	if (keys['J'])
	{
		cowAngel -= 1;
	}
	if (keys['K'])
	{
		cowForce += 1;
	}
	if (keys['M'])
	{
		cowForce -= 1;
	}

	if (keys['P'])
	{
		C.addForce(cowAngel, 10 * cowForce);
		draw_cow = false;
	}
	//	ball20.go();

	//wall2();
	//drawSphere(2, 10, 10);
	drawCow();







	glFlush();											// Done Drawing The Quad

	SwapBuffers(hDC);
	return TRUE;
}




GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	static PAINTSTRUCT ps;

	switch (uMsg)									// Check For Windows Messages
	{
	case WM_INPUT:
	{
		UINT dwSize = sizeof(RAWINPUT);
		BYTE lpb[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			relativeX += raw->data.mouse.lLastX;
			relativeY += raw->data.mouse.lLastY;
		}
		else if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			if ((raw->data.keyboard.Flags & RI_KEY_MAKE) == RI_KEY_MAKE) keys[raw->data.keyboard.VKey] = true;
			if ((raw->data.keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK) keys[raw->data.keyboard.VKey] = false;
		}

		break;
	}
	case WM_PAINT:
		DrawGLScene();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		DrawGLScene();
		return 0;

	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			active = TRUE;						// Program Is Active
		}
		else
		{
			active = FALSE;						// Program Is No Longer Active
		}

		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
		case SC_SCREENSAVE:					// Screensaver Trying To Start?
		case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

														// Ask The User Which Screen Mode They Prefer
														//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)

														// Create Our OpenGL Window
														//(LPCWSTR)
	if (!CreateGLWindow("NeHe Template", Width, Height, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}


	UINT timer = SetTimer(hWnd, 0, 1000 / fps, (TIMERPROC)NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}
