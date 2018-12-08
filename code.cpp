#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>

int WIDTH = 1280;
int HEIGHT = 720;
int HeroX = 0;
int HeroZ = 0;
bool LookLeft = false;
bool LookRight = false;
bool LookForward = false;
bool LookBackward = false;

//Enemies movement variables
int Enemy1BackForward = 0;		//Flag for Choose if the Enemy moving forward or backward
int Enemy2BackForward = 0;
int Enemy3BackForward = 0;
int Enemy4BackForward = 0;

float MoveEnemy1X = 5;
float MoveEnemy2X = 5;
float MoveEnemy3X = 5;
float MoveEnemy4X = 12;

float MoveEnemy1Z = 6;
float MoveEnemy2Z = 3;
float MoveEnemy3Z = 9;
float MoveEnemy4Z = 4;

int RotationAngleEnemi1 = 0;
GLuint tex;
char title[] = "3D Model Loader Sample";
void Anim();
// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = ((GLdouble)WIDTH / (GLdouble)HEIGHT);
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_boss;
Model_3DS model_enemy1;
Model_3DS model_enemy2;
Model_3DS model_enemy3;
Model_3DS model_enemy4;
// Textures
GLTexture tex_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw Ground
	RenderGround();

	// Draw Tree Model
	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	// Draw house Model
	/*glPushMatrix();
	glRotatef(90.f, 1, 0, 0);
	model_house.Draw();
	glPopMatrix();*/

	glPushMatrix();
	////glRotatef(90.f, 1, 0, 0);
	//glScaled(1.1,1.1,1.1);
	glTranslated(HeroX, 0, HeroZ);
	if (LookLeft) {
		glRotated(90, 0, 1, 0);
	}
	if (LookRight) {
		glRotated(-90, 0, 1, 0);
	}
	if (LookForward) {
		glRotated(0, 0, 1, 0);
	}
	if (LookBackward) {
		glRotated(180, 0, 1, 0);
	}
	model_boss.Draw();
	glPopMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslated(MoveEnemy1X, 0, MoveEnemy1Z);
	glRotated(RotationAngleEnemi1, 0, 1, 0);
	glScaled(0.07, 0.07, 0.07);
	model_enemy1.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(MoveEnemy2X, 0, MoveEnemy2Z);
	glScaled(0.07, 0.07, 0.07);
	model_enemy2.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(MoveEnemy3X, 0, MoveEnemy3Z);
	glScaled(0.07, 0.07, 0.07);
	model_enemy3.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(MoveEnemy4X, 0, MoveEnemy4Z);
	glScaled(0.07, 0.07, 0.07);
	model_enemy4.Draw();
	glPopMatrix();


	//sky box
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();



	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'g':
		LookLeft = true;
		LookRight = false;
		LookForward = false;
		LookBackward = false;
		if (HeroX != 20) {
			HeroX += 1;
		}
		printf("The location is (g movment) %d\n", HeroX);
		break;
	case 'h':
		LookLeft = false;
		LookBackward = false;
		LookForward = false;
		LookRight = true;
		if (HeroX != -20) {
			HeroX -= 1;
		}
		printf("The location is (h movment) %d\n", HeroX);
		break;
	case 'z':
		LookForward = true;
		LookLeft = false;
		LookRight = false;
		LookBackward = false;
		if (HeroZ != 20) {
			HeroZ += 1;
		}
		printf("The location is (z movment) %d\n", HeroZ);

		break;
	case 'x':
		LookForward = false;;
		LookLeft = false;
		LookRight = false;
		LookBackward = true;
		if (HeroZ != -20) {
			HeroZ -= 1;
		}
		printf("The location is (x movment) %d\n", HeroZ);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}
void Anim() {
	if (Enemy1BackForward == 0)
		MoveEnemy1Z += 0.1;
	else
		MoveEnemy1Z -= 0.1;

	if (MoveEnemy1Z >= 17) {
		Enemy1BackForward = 1;
		RotationAngleEnemi1 = -180;
	}
	else if (MoveEnemy1Z <= -17) {
		Enemy1BackForward = 0;
		RotationAngleEnemi1 = 0;
	}
	printf("%f\n", MoveEnemy1Z);

	glutPostRedisplay();
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/Tree/Tree1.3ds");
	model_boss.Load("Models/Muham/Muhammer.3DS");
	model_enemy1.Load("Models/femalezombie/Zumbi_Female.3ds");
	//model_enemy2.Load("Models/femalezombie/Zumbi_Female.3ds");
	//model_enemy3.Load("Models/femalezombie/Zumbi_Female.3ds");
	//model_enemy4.Load("Models/femalezombie/Zumbi_Female.3ds");

	//model_wolf.Load("Models/wolf/wolf/wolf.3ds");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutIdleFunc(Anim);
	glutMainLoop();
}