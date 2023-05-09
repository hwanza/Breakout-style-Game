#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>


#define	width 			700
#define	height			700
#define	PI				3.1415
#define	polygon_num		50


int		left = 0;
int		bottom = 0;
int		right = 0;
int		top = 0;
int		collision_count = 0;
float PaddleX1 = 300.0, PaddleX2 = 400.0;
// �ȳ��ϼ���

// ���� ������ �浹�� Ƚ��
int		Bcollision_count = 0;


// ������ ���� ������, �����̴� ���� ������
float	radius1, moving_ball_radius;
float	delta1_x, delta2_y, deltaP1_x, deltaP2_y;


// �е� ��ǥ(x, y)
float Paddle[8] = { PaddleX1, 100.0, PaddleX1, 120.0, PaddleX2, 120.0, PaddleX2, 100.0 };


// ù��° ���� ��ǥ(�Ʒ�����, 0�� ���ʿ��� ����)
float brick[5][8] = {
	{180.0, 380.0, 180.0, 430.0, 230.0, 430.0, 230.0, 380.0},
	{250.0, 380.0, 250.0, 430.0, 300.0, 430.0, 300.0, 380.0},
	{320.0, 380.0, 320.0, 430.0, 370.0, 430.0, 370.0, 380.0},
	{390.0, 380.0, 390.0, 430.0, 440.0, 430.0, 440.0, 380.0},
	{460.0, 380.0, 460.0, 430.0, 510.0, 430.0, 510.0, 380.0},
};


// �ι�° ���� ��ǥ(0�� ���ʿ��� ����)
float brick2[5][8] = {
	{180.0, 450.0, 180.0, 500.0, 230.0, 500.0, 230.0, 450.0},
	{250.0, 450.0, 250.0, 500.0, 300.0, 500.0, 300.0, 450.0},
	{320.0, 450.0, 320.0, 500.0, 370.0, 500.0, 370.0, 450.0},
	{390.0, 450.0, 390.0, 500.0, 440.0, 500.0, 440.0, 450.0},
	{460.0, 450.0, 460.0, 500.0, 510.0, 500.0, 510.0, 450.0},
};


// ����ü Point 
typedef struct _Point {
	float	x;
	float	y;
} Point;


Point	fixed_ball, moving_ball, velocity;
Point center;


// �ʱ�ȭ �Լ�
void init(void) {
	radius1 = 5.0;
	fixed_ball.x = width / 2;
	fixed_ball.y = height / 2;

	moving_ball_radius = 10.0;
	moving_ball.x = width / 2;
	moving_ball.y = 170.0;

	velocity.x = 0.4;
	velocity.y = 0.2;

	collision_count = 1;
}


// �����׸��� �Լ�
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height); // mouse2()
}


// �� �׸���(����)
void	Modeling_Circle(float radius, Point CC) {
	float	delta;
	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius * cos(delta * i), CC.y + radius * sin(delta * i));
	glEnd();
}


// ������ �׸���(����)
void Modeling_GameGround() {

	// ������
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);

	// ������ ������(700, 700)
	glVertex2f(0.0, 350.0);
	glVertex2f(350.0, 700.0);

	glVertex2f(350.0, 700.0);
	glVertex2f(700.0, 350.0);

	glVertex2f(700.0, 350.0);
	glVertex2f(350.0, 0.0);

	glVertex2f(350.0, 0.0);
	glVertex2f(0.0, 350.0);

	glEnd();
}


// ù��°�� ���� �׸���
void Modeling_brick() {

	for (int i = 0; i < 5; i++) {

		glColor3f(1.0, 1.0, 1.0);

		// ���࿡ ù��° ���� 3��° �簢���̶��
		if (i == 2) {
			// ���� �浹Ƚ���� 0�̶�� �Ͼ������ ��Ÿ����.
			if (Bcollision_count == 0) glColor3f(1.0, 1.0, 1.0);

			// ���� �浹Ƚ���� 0�� �ƴ϶�� ���������� ��Ÿ����.
			else glColor3f(0.0, 0.0, 0.0);
		}
		else if (i == 4) {
			if (Bcollision_count == 0) glColor3f(1.0, 1.0, 1.0);
			else glColor3f(0.0, 0.0, 0.0);
		}

		// �������� �Ͼ�� �簢������ �����.
		glBegin(GL_POLYGON);
		glVertex2f(brick[i][0], brick[i][1]);
		glVertex2f(brick[i][2], brick[i][3]);
		glVertex2f(brick[i][4], brick[i][5]);
		glVertex2f(brick[i][6], brick[i][7]);
		glEnd();
	}

}


// �ι�°�� ���� �׸���
void Modeling_brick2() {

	glColor3f(1.0, 1.0, 1.0);

	// �Ͼ�� �簢���� �����.
	for (int i = 0; i < 5; i++) {
		glBegin(GL_POLYGON);
		glVertex2f(brick2[i][0], brick2[i][1]);
		glVertex2f(brick2[i][2], brick2[i][3]);
		glVertex2f(brick2[i][4], brick2[i][5]);
		glVertex2f(brick2[i][6], brick2[i][7]);
		glEnd();
	}
}


// �е� �׸���
void Modeling_Paddle() {
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(Paddle[0], Paddle[1]);
	glVertex2f(Paddle[2], Paddle[3]);
	glVertex2f(Paddle[4], Paddle[5]);
	glVertex2f(Paddle[6], Paddle[7]);
	glEnd();
}

//{320.0, 380.0, 320.0, 430.0, 370.0, 430.0, 370.0, 380.0},


// ���� ������ �浹������
void Collision_Detection_to_Brick(void) {
	float distanceP, brickX = 350.0, brickY = 380.0;
	deltaP1_x = moving_ball.x - brickX;
	deltaP2_y = moving_ball.y - brickY;

	distanceP = sqrt(deltaP1_x * deltaP1_x + deltaP2_y * deltaP2_y);


	// �����̴� ���� ������ �浹���� �� ���� y�� ��������� ���� �������� �ٲ۴�.
	/*if (distanceP < moving_ball_radius) {
		Bcollision_count++;
		velocity.y = - velocity.y;
	}*/

	// ���� �浹���� �� ������ ��ġ�� �ٲ۴�.
	if (Bcollision_count != 0) {
		for (int i = 0; i < 8; i += 2) {
			brick[i][8] = { 450.0 };
		}
	}
}

//float brick[5][8] = {
//	{180.0, 380.0, 180.0, 430.0, 230.0, 430.0, 230.0, 380.0},
//	{250.0, 380.0, 250.0, 430.0, 300.0, 430.0, 300.0, 380.0},
//	{320.0, 380.0, 320.0, 430.0, 370.0, 430.0, 370.0, 380.0},
//	{390.0, 380.0, 390.0, 430.0, 440.0, 430.0, 440.0, 380.0},
//	{460.0, 380.0, 460.0, 430.0, 510.0, 430.0, 510.0, 380.0},
//};

//float Paddle[8] = { 300.0, 100.0, 300.0, 120.0, 400.0, 120.0, 400.0, 100.0 };


// ���� �е鿡 �浹������
void Collision_Detection_to_Paddle(void) {
	//if (moving_ball.y - moving_ball_radius < 120.0) {
	//	/*if (moving_ball.y - moving_ball_radius < 0.0) {*/
	//	velocity.y = std::abs(velocity.y);
	//	/*}*/
	//}

	//if (std::abs(velocity.y) + moving_ball_radius < 120.0) {
	//	/*velocity.x = -velocity.x;*/
	//	/*velocity.y = -velocity.y;*/
	//}

	if (moving_ball.y - moving_ball_radius < 120.0) {
		if (moving_ball.x >= PaddleX1 || moving_ball.x <= PaddleX2) {
			/*Bcollision_count--;*/
			Bcollision_count++;
			velocity.y = -velocity.y;
		}
	}

}


// ���� ���� �浹������ �Լ�
void Collision_Detection_to_Walls(void) {

	float distanceP, brickX = 350.0, brickY = 380.0;
	deltaP1_x = moving_ball.x - brickX;
	deltaP2_y = moving_ball.y - brickY;

	distanceP = sqrt(deltaP1_x * deltaP1_x + deltaP2_y * deltaP2_y);


	// �Ʒ��� ��
	//if (moving_ball.y - moving_ball_radius < 120.0) {
	//	/*Bcollision_count--;*/
	//	Bcollision_count++;
	//	velocity.y = -velocity.y;
	//}

	// width
	// ������ �� �浹
	if (moving_ball.x + moving_ball_radius > width) {
		/*Bcollision_count--;*/
		velocity.x = -velocity.x;
	}

	// width
	// ���� �� �浹
	if (moving_ball.y + moving_ball_radius > width) {
		/*Bcollision_count++;*/
		velocity.y = -velocity.y;
	}

	// 0
	// ���� �� �浹
	if (moving_ball.x - moving_ball_radius < 0.0) {
		/*Bcollision_count++;*/
		velocity.x = -velocity.x;
	}

}


// �����ϴ� �Լ�
void RenderScene(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to Yellow
	glClear(GL_COLOR_BUFFER_BIT);


	Modeling_GameGround();
	Modeling_brick();
	Modeling_brick2();
	Modeling_Paddle();

	/*������ �߽��� ��ġ�� ������ �� �׸��� */

	//glColor3f(1.0, 1.0, 0.0);
	//if (collision_count % 2)
	//	Modeling_Circle(radius1, fixed_ball);
	//else glColor3f(1.0, 1.0, 0.0);

	// �����̴� ���� ��ġ ��ȭ
	moving_ball.x += velocity.x;
	moving_ball.y += velocity.y;

	// �浹 ó�� �κ�
	//Collision_Detection_Between_Balls(); // ���� ���� �浹 �Լ�
	/*Collision_Detection_to_Brick();*/
	Collision_Detection_to_Walls(); // ���� ���� �浹 �Լ�
	Collision_Detection_to_Paddle();

	// �����̴� �� �׸��� 
	glColor3f(0.0, 1.0, 1.0);
	Modeling_Circle(moving_ball_radius, moving_ball);

	glutSwapBuffers();
	glFlush();
}


// Ű���� Ű�� ������ �Լ�
void SpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		Paddle[0] -= 10.0;
		Paddle[2] -= 10.0;
		Paddle[4] -= 10.0;
		Paddle[6] -= 10.0; break;
	case GLUT_KEY_RIGHT:
		Paddle[0] += 10.0;
		Paddle[2] += 10.0;
		Paddle[4] += 10.0;
		Paddle[6] += 10.0; break;
	default: break;
	}
}


void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Breakout-style Game");
	init();
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(RenderScene);
	glutMainLoop();
}