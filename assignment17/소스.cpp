#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <ios>


#define	width 800
#define	height 800
#define	PI 3.1415
#define	polygon_num 50
#define	CenterX 400.0
#define	CenterY 400.0


int		left = 0;
int		bottom = 0;
int		right = 0;
int		top = 0;
int		collision_count = 0;


float PaddleX1 = 300.0, PaddleX2 = 400.0;
float PaddleY1 = 100.0, PaddleY2 = 120.0;


float Line;

int LeftTopLineX = 1, LeftTopLineY = 400;

float speed = 0.2;


// ���� ������ �浹�� Ƚ��
int		Bcollision_count = 0;


// ������ ���� ������, �����̴� ���� ������
float	radius1, moving_ball_radius;
float	delta1_x, delta2_y, deltaP1_x, deltaP2_y;



// �е� ��ǥ(x, y)
// Paddle[0], Paddle[1] => �е� ���ϴ� ��ǥ
// Paddle[2], Paddle[3] => �е� �»�� ��ǥ
// Paddle[4], Paddle[5] => �е� ���� ��ǥ
// Paddle[6], Paddle[7] => �е� ���ϴ� ��ǥ
float Paddle[8] = { PaddleX1, PaddleY1, PaddleX1, PaddleY2, PaddleX2, PaddleY2, PaddleX2, PaddleY1 };


// ù��° ���� ��ǥ(�Ʒ�����, 0�� ���ʿ��� ����)
float brickX = 220.0, brickY = 380.0;
float brick[5][8] = {
	{brickX, brickY, brickX, brickY + 50.0, brickX + 50.0, brickY + 50.0, brickX + 50.0, brickY},
	{brickX + 70.0, brickY, brickX + 70.0, brickY + 50.0, brickX + 120.0, brickY + 50.0, brickX + 120.0, brickY},
	{brickX + 140.0, brickY, brickX + 140.0, brickY + 50.0, brickX + 190.0, brickY + 50.0, brickX + 190.0, brickY},
	{brickX + 210.0, brickY, brickX + 210.0, brickY + 50.0, brickX + 260.0, brickY + 50.0, brickX + 260.0, brickY},
	{brickX + 280.0, brickY, brickX + 280.0, brickY + 50.0, brickX + 330.0, brickY + 50.0, brickX + 330.0, brickY},
};


// �ι�° ���� ��ǥ(0�� ���ʿ��� ����)
float brick2X = 220.0, brick2Y = 450.0;
float brick2[5][8] = {
	{brick2X, brick2Y, brick2X, brick2Y + 50.0, brick2X + 50.0, brick2Y + 50.0, brick2X + 50.0, brick2Y},
	{brick2X + 70.0, brick2Y, brickX + 70.0, brick2Y + 50.0, brickX + 120.0, brick2Y + 50.0, brickX + 120.0, brick2Y},
	{brickX + 140.0, brick2Y, brickX + 140.0, brick2Y + 50.0, brickX + 190.0, brick2Y + 50.0, brickX + 190.0, brick2Y},
	{brickX + 210.0, brick2Y, brickX + 210.0, brick2Y + 50.0, brickX + 260.0, brick2Y + 50.0, brickX + 260.0, brick2Y},
	{brickX + 280.0, brick2Y, brickX + 280.0, brick2Y + 50.0, brickX + 330.0, brick2Y + 50.0, brickX + 330.0, brick2Y},
};


// ����ü Point
typedef struct _Point {
	float	x;
	float	y;
} Point;


Point moving_ball, velocity;
Point center;


// �ʱ�ȭ �Լ�
void init(void) {

	moving_ball_radius = 10.0;
	moving_ball.x = width / 2;
	moving_ball.y = height / 2;

	center.x = width / 2;
	center.y = height / 2;

	velocity.x = 0.7;
	velocity.y = 0.2;

	collision_count = 1;
}

float normalize(float a, float b) {
	float value;
	value = sqrt(pow(a, 2) + pow(b, 2));
	return value;
}


// �����׸��� �Լ�
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height); // mouse2()
}


// �е� ������
float StraightP(float u) {
	float x2 = 300.0, x1 = 490.0, y2 = 120.0, y1 = 120.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (u - x1) + y1;
	return y;
}


// ������ ������(����)
float Straight(float u) {
	float x2 = 400.0, x1 = 800.0, y2 = 800.0, y1 = 400.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (u - x1) + y1;
	return y;
}


// ������ ������(���ϴ�)
float Straight2(float i) {
	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 0.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (i - x1) + y1;
	return y;
}


// ������ ������(�»��)
float Straight3(float i) {
	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 800.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (i - x1) + y1;
	return y;
}


// ������ ������(���ϴ�)
float Straight4(float i) {
	float x2 = 400.0, x1 = 800.0, y2 = 0.0, y1 = 400.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (i - x1) + y1;
	return y;
}


// ���� ���� ������ �Ÿ� ����(�е�)
float LinePaddle(float x1, float y1) {
	float distancePaddle;

	distancePaddle = std::abs((0 * x1) + (-1 * y1) + 120.0) / 1;
	return distancePaddle;
}


// ���� ���� ������ �Ÿ� ����(����)
float LineRightUp(float x1, float y1) {
	float distanceRUp;

	distanceRUp = std::abs((1 * x1) + (1 * y1) - 1200.0) / 1;
	return distanceRUp;
}


// ���� ���� ������ �Ÿ� ����(�»��)
float LineLeftUp(float x1, float y1) {
	float distanceLUp;

	distanceLUp = std::abs((1 * x1) + (-1 * y1) + 400.0) / 1;
	return distanceLUp;
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

	// ������ ������(800, 800)
	glVertex2f(0.0, 400.0);
	glVertex2f(400.0, 800.0);
	glVertex2f(800.0, 400.0);
	glVertex2f(400.0, 0.0);

	glEnd();
}


// ù��°�� ���� �׸���
void Modeling_brick() {

	for (int i = 0; i < 5; i++) {
		glColor3f(1.0, 1.0, 1.0);

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


// ���� ������ �浹������
void Collision_Detection_to_Brick(void) {
	float distanceP, distanceP2, distanceP3;
	float brickX, brickY = 0.0;
	//deltaP1_x = moving_ball.x - brickX;
	//deltaP2_y = moving_ball.y - brickY;

	distanceP = sqrt(deltaP1_x * deltaP1_x + deltaP2_y * deltaP2_y);


	// �����̴� ���� ������ �浹���� �� ���� y�� ��������� ���� �������� �ٲ۴�.
	/*if (distanceP < moving_ball_radius) {
		Bcollision_count++;
		velocity.y = - velocity.y;
	}*/

	// ���� �浹���� �� ������ ��ġ�� �ٲ۴�.
	/*if (Bcollision_count != 0) {
		for (int i = 0; i < 8; i += 2) {
			brick[i][8] = { brickY };
		}
	}*/
}

// �е� �κ� �� �� ����

// ���� �е鿡 �浹������
void Collision_Detection_to_Paddle(void) {

	
	// �е� �浹 ����
	float Pdistance1, Pdistance2, Pdistance3, Scalar;
	Point vector1, vector2, vector3; vector1.x, vector1.y, vector2.x, vector2.y, vector3.x, vector3.y= 0.0;


	if (LinePaddle(moving_ball.x, moving_ball.y) < moving_ball_radius) {
	if (moving_ball.x >= Paddle[0] || moving_ball.x <= Paddle[6]) {

		// ���⺤��
		vector1.x = moving_ball.x - center.x;
		vector1.y = StraightP(moving_ball.x) - center.y;

		vector1.x = vector1.x / normalize(vector1.x, vector1.y);
		vector1.y = vector1.y / normalize(vector1.x, vector1.y);

		// ��������
		vector2.x = (width / 2) - 300.0;
		vector2.y = width - 300.0;

		vector2.x = vector2.x / normalize(vector2.x, vector2.y);
		vector2.y = vector2.y / normalize(vector2.x, vector2.y);

		vector3.x = vector1.x + (2 * vector2.x);
		vector3.y = vector1.x + (2 * vector2.y);

		vector3.x = vector3.x / normalize(vector3.x, vector3.y);
		vector3.y = vector3.y / normalize(vector3.x, vector3.y);

		printf("LinePaddle : %f\n", LinePaddle(moving_ball.x, moving_ball.y));

		velocity.x = vector3.x;
		velocity.y = vector3.y;
		}

		else {
			velocity.x = 0;
			velocity.y = 0;
		}
	}

	
}


// ���� ���� �浹������ �Լ�
void Collision_Detection_to_Walls(void) {

	float distanceP, distanceP2, distanceP3;
	Point Vector1, Vector2, Vector3;
	Vector1.x = 0, Vector2.x = 0, Vector3.x = 0;
	Vector1.y = 0, Vector2.y = 0, Vector3.y = 0;


// ����� ���� �� �浹
	float RightUp = LineRightUp(moving_ball.x, moving_ball.y);
	float LeftUp = LineLeftUp(moving_ball.x, moving_ball.y);
	if (RightUp < moving_ball_radius) {

		// ���� ����ȭ(������ ���̸� 1�� ����� ��)
		// ������ �� ������ ������ ũ��� ������ ��

		// ���� �� ������ �Ÿ�(ũ��)

		// ���⺤��(������ ������� �߾� -> �浹 ����)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight(moving_ball.x) - center.y;


		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// ��������(������ �浹 ���� 600, 600 ���� ������� �߾�)
		Vector2.x = 0.0 - (width / 2);
		Vector2.y = (width / 2) - width;


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);


		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);

		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// ���ϴ�
	else if (RightUp > width - moving_ball_radius) {
		// ���� ����ȭ(������ ���̸� 1�� ����� ��)
		// ������ �� ������ ������ ũ��� ������ ��


		// ���⺤��(������ ������� �߾�, �浹 ����)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight2(moving_ball.x) - center.y;


		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// ��������(������ �浹 ���� 600, 600 ���� ������� �߾�)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = width - (width / 2);


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// ����� �»�� �� �浹
	if (LeftUp < moving_ball_radius) {
		// ���� ����ȭ(������ ���̸� 1�� ����� ��)
		// ������ �� ������ ������ ũ��� ������ ��



		// ���⺤��(������ ������� �߾�, �浹 ���� 600, 600)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight3(moving_ball.x) - center.y;

		distanceP = sqrt(pow(Vector1.x, 2) + pow(Vector1.y, 2));

		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// ��������(������ �浹 ���� 600, 600 ���� ������� �߾�)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = 0.0 - (width / 2);


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// ����� ���ϴ� �浹
	else if (LeftUp > width - moving_ball_radius) {
		// ���� ����ȭ(������ ���̸� 1�� ����� ��)
		// ������ �� ������ ������ ũ��� ������ ��


		// ���⺤��(������ ������� �߾�, �浹 ���� 600, 600)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight4(moving_ball.x) - center.y;


		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// ��������(������ �浹 ���� 600, 600 ���� ������� �߾�)
		Vector2.x = (width / 2) - width;
		Vector2.y = width - (width / 2);


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);

		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
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
	moving_ball.x += velocity.x * speed;
	moving_ball.y += velocity.y * speed;

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


void MyKey(unsigned char key, int x, int y) {

	switch (key) {

		// ���� �� ����
	case 'n':
		velocity.x -= 0.1;
		velocity.y -= 0.1;
		break;

	case 'b':
		velocity.x += 0.1;
		velocity.y += 0.1;
		break;

		// ���� ���� �ٲٱ�
	case 'm':
		velocity.x = -velocity.x;
		velocity.y = -velocity.y;
		break;

		// ���� �� 0���� �ٲٱ�
	case 'p':
		velocity.x = 0.0;
		velocity.y = 0.0;
		break;

	default: break;
	}
}


// Ű���� Ű�� ������ �Լ�
void SpecialKey(int key, int x, int y) {
	switch (key) {

		// �е��� �簢�� �ȿ��� ������ �Ȼ�������(���� ����Ű)
	case GLUT_KEY_LEFT:
		if (Paddle[0] <= 300.0) {
			Paddle[0] += 10.0;
			Paddle[2] += 10.0;
			Paddle[4] += 10.0;
			Paddle[6] += 10.0;
			break;
		}
		else {
			Paddle[0] -= 10.0;
			Paddle[2] -= 10.0;
			Paddle[4] -= 10.0;
			Paddle[6] -= 10.0;
			break;
		}

		// �е��� �簢�� �ȿ��� ������ �Ȼ�������(������ ����Ű)
	case GLUT_KEY_RIGHT:
		if (Paddle[0] >= 400.0) {
			Paddle[0] -= 10.0;
			Paddle[2] -= 10.0;
			Paddle[4] -= 10.0;
			Paddle[6] -= 10.0;
			break;
		}
		else {
			printf("Paddle[0] : %f\n", Paddle[0]);
			printf("Paddle[6] : %f\n", Paddle[6]);

			Paddle[0] += 10.0;
			Paddle[2] += 10.0;
			Paddle[4] += 10.0;
			Paddle[6] += 10.0; break;
		}

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
	glutReshapeFunc(MyReshape);
	glutKeyboardFunc(MyKey);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(RenderScene);
	glutMainLoop();
}