#include <GL/glut.h>
#include <iostream>
using namespace std;

const int w = 800;	// 窗口宽度
const int h = 600;	// 窗口高度

					//各个曲面
GLUquadricObj *BASE = 0;
GLUquadricObj *LEG = 0;
GLUquadricObj *TABLE = 0;

//底座的高度和上下半径
const double BASE_HEIGHT = 1.5;
const double BASE_RADIUS1 = 3.0;
const double BASE_RADIUS2 = 0.5;

//桌腿的长度和半径
const double LEG_HEIGHT = 7.0;
const double LEG_RADIUS = 0.5;

//桌面的厚度和半径
const double TABLE_RADIUS = 4.5;
const double TABLE_HEIGHT = 0.5;

//旋转角度
double RotateAngle[3] = { 0.0, 0.0, 0.0 };

//旋转方向选择
double Control[2] = { 1.0, 0.0 };



//底座
void base()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(BASE, BASE_RADIUS1, BASE_RADIUS2, BASE_HEIGHT, 100, 100);
	glPopMatrix();
}

//桌腿
void leg()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(LEG, LEG_RADIUS, LEG_RADIUS, LEG_HEIGHT, 100, 100);
	glPopMatrix();
}

//关节
void joint()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluSphere(LEG, LEG_RADIUS, 100, 100);
	glPopMatrix();
}

//桌面侧面
void table1()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(TABLE, TABLE_RADIUS, TABLE_RADIUS, TABLE_HEIGHT, 100, 100);
	glPopMatrix();
}

//桌面双面
void table2()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluDisk(TABLE, 0, TABLE_RADIUS, 100, 100);
	glPopMatrix();
}


void draw(GLenum mode)
{
	glTranslatef(0.0, -2.0, 0.0);

	//绘制底座
	glRotatef(RotateAngle[0], 0.0, 1.0, 0.0);
	base();

	//绘制桌腿
	glTranslatef(0.0, BASE_HEIGHT, 0.0);
	glRotatef(RotateAngle[1], 0.0, 0.0, 1.0);
	joint();
	leg();
	glTranslatef(0.0, LEG_HEIGHT, 0.0);
	joint();

	//绘制桌面
	glRotatef(RotateAngle[2], Control[0], 0.0, Control[1]);
	table2();
	table1();
	glTranslatef(0.0, TABLE_HEIGHT, 0.0);
	table2();
	glFlush();
}



void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//控制桌子底座转动
	case 'a':
		RotateAngle[0] -= 5;
		break;
	case 'd':
		RotateAngle[0] += 5;
		break;
		//控制桌子腿转动
	case 'w':
		RotateAngle[1] -= 5;
		if (RotateAngle[1] <= -90)
			RotateAngle[1] = 90;
		break;
	case 's':
		RotateAngle[1] += 5;
		if (RotateAngle[1] >= 90)
			RotateAngle[1] = -90;
		break;
	default:
		return;
		break;
	}
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		RotateAngle[2] -= 5;
		if (RotateAngle[2] <= -90)
			RotateAngle[2] = 90;
		break;
	case GLUT_KEY_DOWN:
		RotateAngle[2] += 5;
		if (RotateAngle[2] >= 90)
			RotateAngle[2] = -90;
		break;
	case GLUT_KEY_LEFT:
		if (Control[1] < 1.05 && Control[1] > 0.95 && Control[0] > -0.95)
			Control[0] -= 0.1;
		if (Control[0] < -0.95 && Control[0] > -1.05 && Control[1] > -0.95)
			Control[1] -= 0.1;
		if (Control[1] < -0.95 && Control[1] > -1.05 && Control[0] < 0.95)
			Control[0] += 0.1;
		if (Control[0] < 1.05 && Control[0] > 0.95 && Control[1] < 0.95)
			Control[1] += 0.1;
		break;
	case GLUT_KEY_RIGHT:
		if (Control[0] < 1.05 && Control[0] > 0.95 && Control[1] > -0.95)
			Control[1] -= 0.1;
		if (Control[1] < -0.95 && Control[1] > -1.05 && Control[0] > -0.95)
			Control[0] -= 0.1;
		if (Control[0] < -0.95 && Control[0] > -1.05 && Control[1] < 0.95)
			Control[1] += 0.1;
		if (Control[1] < 1.05 && Control[1] > 0.95 && Control[0] < 0.95)
			Control[0] += 0.1;
		break;
	default:
		return;
		break;
	}
	glutPostRedisplay();
}

void init()
{
	// 各个二次曲面申请空间及填充方式
	BASE = gluNewQuadric();
	gluQuadricDrawStyle(BASE, GLU_FILL);
	LEG = gluNewQuadric();
	gluQuadricDrawStyle(LEG, GLU_FILL);
	TABLE = gluNewQuadric();
	gluQuadricDrawStyle(TABLE, GLU_FILL);

	// 光照
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.5, 0.05, 1.0 };
	GLfloat position[] = { 4.0, 4.0, 4.0, 0.0 };
	GLfloat lmodel_ambient[] = { 0.5, 0.4, 0.4, 1.0 };
	GLfloat local_view[] = { 0.0 };
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw(GL_RENDER);
	glutSwapBuffers();
}



void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-10.0, 10.0, -5.0 * (GLfloat)h / (GLfloat)w,
			15.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-10.0 * (GLfloat)w / (GLfloat)h,
			10.0 * (GLfloat)w / (GLfloat)h, -5.0, 15.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(" 郭奚源的变形桌子");
	init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glutMainLoop();
}
