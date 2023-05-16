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

float speed = 0.6;

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


Point moving_ball, velocity;
Point center;


// �ʱ�ȭ �Լ�
void init(void) {

	moving_ball_radius = 10.0;
	moving_ball.x = width / 2;
	moving_ball.y = height / 2;


	center.x = width / 2;
	center.y = height / 2;


	velocity.x = 0.3;
	velocity.y = 0.5;

	collision_count = 1;
}


// �����׸��� �Լ�
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height); // mouse2()
}


// ������ ������
//float Line1(float i) {
//	float x2 = 400.0, x1 = 800.0, y2 = 800.0, y1 = 400.0, m, x, y, distance;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}


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



//float Line2() {
//	float x1 = 0.0, x2 = 400.0, y1 = 400.0, y2 = 800.0, distance;
//	distance = sqrt(pow((x2 - x1), 2) - pow((y2 - y1), 2));
//
//	return distance;
//}



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


// y = x + 400; ���� ��
int Point_to_Straight(int a, int b) {
	int distance;

	if (moving_ball.y <= 600) {
		distance = std::abs(moving_ball.x + moving_ball.y) / 401;
	}

	return distance;
}


// ù��°�� ���� �׸���
void Modeling_brick() {

	for (int i = 0; i < 5; i++) {
		glColor3f(1.0, 1.0, 1.0);

		//// ���࿡ ù��° ���� 3��° �簢���̶��
		//if (i == 2) {
		//	// ���� �浹Ƚ���� 0�̶�� �Ͼ������ ��Ÿ����.
		//	if (Bcollision_count == 0) glColor3f(1.0, 1.0, 1.0);

		//	// ���� �浹Ƚ���� 0�� �ƴ϶�� ���������� ��Ÿ����.
		//	else glColor3f(0.0, 0.0, 0.0);
		//}
		//else if (i == 4) {
		//	if (Bcollision_count == 0) glColor3f(1.0, 1.0, 1.0);
		//	else glColor3f(0.0, 0.0, 0.0);
		//}

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
	/*if (Bcollision_count != 0) {
		for (int i = 0; i < 8; i += 2) {
			brick[i][8] = { 450.0 };
		}
	}*/
}


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

	// �е� �浹 ����
	float Pdistance1, Pdistance2, Pdistance3, Scalar;
	Point vector1, vector2, vector3;

	/*Pdistance1 = sqrt(pow((moving_ball.x - PaddleX1), 2) + pow((moving_ball.y - PaddleY1), 2));*/
	Pdistance1 = sqrt(pow((PaddleX2 - moving_ball.x), 2) + pow((PaddleY2 - moving_ball.y), 2));

	vector1.x = velocity.x / Pdistance1;
	vector1.y = velocity.y / Pdistance1;

	/*Scalar = sqrt(pow((CenterX - moving_ball.x), 2) + pow((CenterY - moving_ball.y), 2));*/

	Pdistance2 = sqrt(pow((CenterX - PaddleX2), 2) + pow((CenterY - PaddleY2), 2));

	vector2.x = velocity.x / Pdistance2;
	vector2.y = velocity.y / Pdistance2;

	if (Pdistance2 <= moving_ball_radius) {
		/*if (velocity.x >= PaddleX1 || velocity.x <= PaddleX2) {
			Bcollision_count++;
			velocity.y = -velocity.y;
		}*/

		//velocity.x = -velocity.x;
		//velocity.y = -velocity.y;

		vector2.x = -vector2.x;
		vector2.y = -vector2.y;
	}

}


// ���� ���� �浹������ �Լ�
void Collision_Detection_to_Walls(void) {

	float distanceP, distanceP2, distanceP3, brickX = 350.0, brickY = 380.0;
	float HX, HY;
	Point Vector1;


	// �������� n ����(x, y) ��ǥ
	Vector1.x = center.x;
	Vector1.y = center.y;

	/*distanceP = sqrt(pow(moving_ball.x - , 2) - pow(moving_ball.y - , 2));*/
	// distanceP = sqrt(pow(CenterX - Line1(moving_ball.x), 2) - pow(CenterY - Line1(moving_ball.y), 2));


	//Left = Point_to_Straight(moving_ball.x, moving_ball.y);
	//if (Left == moving_ball_radius) {
	//	velocity.y = -velocity.y;
	//}


	//if (moving_ball.y + 400.0 <= 800.0) {
	//	/*velocity.x = -velocity.x;*/
	//	velocity.y = -velocity.y;
	//}

	//if (distanceP == moving_ball_radius) {
	//	velocity.x = -velocity.x;
	//}


	// ����� ���� �� �浹
	float RightUp = LineRightUp(moving_ball.x, moving_ball.y);
	float LeftUp = LineLeftUp(moving_ball.x, moving_ball.y);
	/*printf("debug : %f\n", RightUp);*/
	if (RightUp < moving_ball_radius) {
		velocity.y = -velocity.y;
		velocity.x = -velocity.x;
		printf("RightUp : %f\n", RightUp);
	}
	else if (RightUp > width - moving_ball_radius) {
		velocity.y = -velocity.y;
		velocity.x = -velocity.x;
		printf("RightUp : %f\n", RightUp);
	}


	// ����� �»�� �� �浹
	/*printf("debug : %f\n", LeftUp);*/
	
	if (LeftUp < moving_ball_radius) {
		velocity.y =-(velocity.y);
		velocity.x = -(velocity.x);
		printf("LeftUp : %f\n", LeftUp);
	}
	else if (LeftUp > width - moving_ball_radius) {
		velocity.y = -velocity.y;
		velocity.x = -velocity.x;
		printf("LeftUp : %f\n", LeftUp);
	}
	
	


	/*width
	������ �� �浹*/
	if (moving_ball.x + moving_ball_radius > width) {
		/*Bcollision_count--;*/
		velocity.x = -velocity.x;
		//printf("velocity.x : %f\n", velocity.x);
		//printf("moving_ball.x : %f\n", moving_ball.x);
	}

	// width
	// ���� �� �浹
	if (moving_ball.y + moving_ball_radius > width) {
		/*Bcollision_count++;*/
		velocity.y = -velocity.y;
		//printf("velocity.x : %f\n", velocity.x);
		//printf("moving_ball.x : %f\n", moving_ball.x);
	}

	// 0
	// ���� �� �浹
	//if (moving_ball.x - moving_ball_radius < 0.0) {
	//	/*Bcollision_count++;*/
	//	velocity.x = -velocity.x;
	//	printf("LeftUp : %f\n", LeftUp);
	//	//printf("velocity.x : %f\n", velocity.x);
	//	//printf("moving_ball.x : %f\n", moving_ball.x);
	//}

	// �Ʒ��� ��
	//if (moving_ball.y - moving_ball_radius < 120.0) {
	//	/*Bcollision_count--;*/
	//	Bcollision_count++;
	//	velocity.y = -velocity.y;
	//	printf("LeftUp : %f\n", LeftUp);
	//	//printf("velocity.x : %f\n", velocity.x);
	//	//printf("moving_ball.x : %f\n", moving_ball.x);
	//}

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