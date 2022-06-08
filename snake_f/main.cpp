#define STB_IMAGE_IMPLEMENTATION
#include<iostream>
//using namespace std;
#include<cmath>
#include <fstream>
#include<Windows.h>
#include<GL/stb_image.h>
#include<GL/glut.h>
#include<math.h>
#include<ctime> 
// For texture 
#define ROWS 40
#define COLUMNS 40
#define FPS 10 // approximitly 10 frames in one second 
//#define MAX 50
#define UP 1
#define RIGHT 2
#define DOWN -1
#define LEFT -2
#define MAX 20 // max length of snake 
GLfloat angleCube = 0.0f;
std::ofstream ofile;
std::ifstream ifile;
unsigned int texture;
int width, height, nrChannels;
unsigned char* data = NULL;
// For Grid
int gridX, gridY;
bool length_inc = false;
short sDirection = RIGHT; // default direction of snake 
bool gameOver = false;
bool food = true;
bool seedflag = false;
int foodX, foodY; // position of the food
int score = 0;
float wid, heigh, ratio, angle, eyey, eyez, eyex, upx, upz = -20; // camera settings 
void init();
void display_callback();
void keyboard(unsigned char, int, int); // asci, mouse
void mouse(int, int, int, int); // button , state, x, y
void special_keyboard(int, int, int);
void reshape_callback(int, int);
void timer_callback(int);
void initGrid(int, int);
void draw_snake();
void draw_food();
int random(int, int);
void draw_3d_food();
void draw2D();
void cubic3d(float, float);
void draw_texture();
void load(int imgnum);
void check(unsigned char* data);
bool rotate_flag = true;

//main function
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	// double for animation
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	wid = GetSystemMetrics(SM_CXSCREEN);
	heigh = GetSystemMetrics(SM_CYSCREEN);
	glutInitWindowSize(wid, heigh);

	glutInitWindowPosition(10, 10);
	// Trial
	//glutInitWindowSize(700, 700);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

	glutCreateWindow("SNAKE v1.0");
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	// For keyboard
	glutKeyboardFunc(keyboard);
	// special key in keyboard
	glutSpecialFunc(special_keyboard);
	glutTimerFunc(100, timer_callback, 0); // delay 100
	// For mouse Function
	glutMouseFunc(mouse);

	init();
	glutMainLoop();
	return 0;
}

void init()
{   // Background 
	glClearColor(0, 0, 0, 1);
	// for 3D
	glEnable(GL_DEPTH_TEST);// for z
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	initGrid(COLUMNS, ROWS);
	glEnable(GL_TEXTURE_2D); // for texture 

	// For 3d shape ..lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

//Callbacks

// Draw
void display_callback()
{
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// my draw
	draw_texture();
	// Convert to 2d 
	draw2D();
	// 3D
	draw_3d_food();
	glutSwapBuffers();

	if (gameOver)
	{
		ofile.open("score.dat", std::ios::trunc);
		ofile << score << std::endl;
		ofile.close();
		ifile.open("score.dat", std::ios::in);
		char a[4];
		ifile >> a;
		std::cout << a;
		char text[50] = "Your score : ";
		strcat_s(text, a);
		MessageBox(NULL, text, "Game Over", 0);
		exit(0);
	}
	
}

void draw_texture() {

	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// For camera 
	gluLookAt(eyex, eyey, eyez,
		upx, eyey, upz,
		0, 1, 0);

	glTranslatef(0, 0, -7);


	load(1); // wall 

	//glRotatef(angle,1,1,1);
	
	
	//top
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(4, 2, 10);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-4, 2, 10);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-4, 2, -10);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(4, 2, -10);

	glEnd();

	load(1);
	// The Base ..Ok
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(4, -2, 10);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-4, -2, 10);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-4, -2, -10);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(4, -2, -10);

	glEnd();

	load(1);//back //wall

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-4, -2, -10);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(4, -2, -10);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(4, 6, -10);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-4, 6, -10);

	glEnd();

	load(1);   //left

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-4, -2, -10);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-4, -2, 10);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(-4, 6, 10);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(-4, 6, -10);
	glEnd();

	load(1);//right

	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(4, -2, -10);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(4, -2, 10);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(4, 6, 10);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(4, 6, -10);
	glEnd();
}

void draw2D() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // save state of the matrix 
	glLoadIdentity();

	// Convert to 2D
	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//drawGrid();
	draw_snake();
	draw_food();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

}

void draw_3d_food() {
	cubic3d(-2, 1.5);
	cubic3d(2, -1.5);

	cubic3d(2, 1.5);
	cubic3d(-2, -1.5);

}

void cubic3d(float x, float y) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Reset the model-view matrix
	glTranslatef(x, y, -7.0f);  // Move right and into the screen
	glRotatef(angleCube, 0.0f, 0.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW]
	glScalef(0.2, 0.2, 0.2);

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	   // Top face (y = 1.0f)
	   // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	if (rotate_flag) {
		angleCube -= 0.7;
	}
	 
}

// Reshape 
void reshape_callback(int w, int h)
{
	float ratio = w / (float)h;
	glViewport(0, 0, (GLfloat)w, GLfloat(h));
	// New Cordinate.. 
	glMatrixMode(GL_PROJECTION); // edit in camera property
	glLoadIdentity();       
	//glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0); // 2D coordinates
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

// timer 
void timer_callback(int) // Animation 
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer_callback, 0);
}

// keyboard
void keyboard(unsigned char key, int x, int y) {

	angle += 4;
	if (key == 27) {
		exit(0);
	}
	if (key == 'a') // Left
	{
		upx -= .9 * cos(.1);
		upz -= .9 * sin(.1);
	}
	if (key == 'd') { // Right
		upx += .9 * cos(.1);
		upz += .9 * sin(.1);
	}
	if (key == 'w') // UP
	{
		eyez -= 5;
		eyey = .3 * abs(cos(angle));
	}
	if (key == 's') { // down 
		eyez += 5;
		eyey = .3 * abs(cos(angle)); // 0.3 factor
	}
}


// special keyboard 
void special_keyboard(int key, int x, int y) // (key , mouse position)
{
	switch (key) {

	case GLUT_KEY_UP:
		if (sDirection != DOWN) // turn only 90 degree
			sDirection = UP;
		break;
	case GLUT_KEY_DOWN:
		if (sDirection != UP)
			sDirection = DOWN;
		break;
	case GLUT_KEY_RIGHT:
		if (sDirection != LEFT)
			sDirection = RIGHT;
		break;
	case GLUT_KEY_LEFT:
		if (sDirection != RIGHT)
			sDirection = LEFT;
		break;
	}
}

//mouse event
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Show player score 
		if (rotate_flag) {
			rotate_flag = false;
		}
		else {
			rotate_flag = true;
		}
	}
}

void initGrid(int x, int y) {
	gridX = x;
	gridY = y;
}

int snake_length = 5;
int posx[60] = { 20,20,20,20,20 };// position of first 5 element
int posy[60] = { 20,90,18,17,16 };

//int posx, posy = 20;

// draw snake
void draw_snake()
{
	// position will be swipped always 
	for (int i = snake_length - 1; i > 0; i--)
	{
		posx[i] = posx[i - 1];
		posy[i] = posy[i - 1];
	}
	if (sDirection == UP)
		posy[0]++;
	else if (sDirection == DOWN)
		posy[0]--;
	else if (sDirection == RIGHT)
		posx[0]++;
	else if (sDirection == LEFT)
		posx[0]--;

	for (int i = 0; i < snake_length; i++) { // loop to draw elements 
		if (i == 0)
			glColor3f(0.9, 1.0, 0.0);
		else
			glColor3f(1.0, 0.1, 0.0);
		glRectd(posx[i], posy[i], posx[i] + 1, posy[i] + 1);
	}
	// validate over head only 
	
	if (posx[0] == 0 || posx[0] == gridX - 1 || posy[0] == 0 || posy[0] == gridY - 1)
		gameOver = true;

	//when eating 
	else if (posx[0] == foodX && posy[0] == foodY) {
		food = true;
		score++;
		if (snake_length <= MAX)
			length_inc = true;
		if (snake_length == MAX)
			MessageBox(NULL, "You Win\nYou can still keep playing but the snake will not grow.", "Awsome", 0);
	}

	// game over when snake hit itself
	for (int j = 1; j < snake_length; j++)
	{
		if (posx[j] == posx[0] && posy[j] == posy[0])
			gameOver = true;
	}

	//inreasing length of snake
	if (length_inc)
	{
		snake_length++;
		length_inc = false;
	}
		////////////////////////
		// handle crash in block 1

		 /*if (posx[0] == 5 && posy[0] == 5 || posx[0] == 6 && posy[0] == 6) {
			gameOver = true;
		}
		// handle crash in block 2
		if (posx[0] == 35 && posy[0] == 35 || posx[0] == 36 && posy[0] == 35) {
			gameOver = true;
		}*/
	}

void draw_food()
{
	if (food)
	{
		foodX = random(2, gridX - 3);
		foodY = random(2, gridY - 3);
		//std::cout << foodX << foodY << std::endl;
		food = false;
	}
	glColor3f(0.2, 0.6, 0.4);
	glBegin(GL_QUADS);
	glVertex2d(foodX, foodY);
	glVertex2d(foodX + 1, foodY);
	glVertex2d(foodX + 1, foodY + 1); 
	glVertex2d(foodX, foodY + 1);
	glEnd();
}
////random position of food 
int random(int _min, int _max)
{
	if (!seedflag)
	{
		srand(time(NULL));
		seedflag = true;
	}
	else
		seedflag = false;
	return _min + rand() % (_max - _min);
}


// Helper function
// load image function 

void load(int imgnum) {
	if (imgnum == 1) {

		data = stbi_load("ww.png", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 2) {

		data = stbi_load("LS.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
}

// Check data function
void check(unsigned char* data) {
	if (data)
	{
		glGenTextures(1, &texture); // number of texture you want to created ..&texture index
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// repeat on T(y) , S(x) axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// constructor of method
	stbi_image_free(data);
}