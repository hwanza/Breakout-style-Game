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


// 공이 벽돌에 충돌한 횟수
int		Bcollision_count = 0;


// 고정된 공의 반지름, 움직이는 공의 반지름
float	radius1, moving_ball_radius;
float	delta1_x, delta2_y, deltaP1_x, deltaP2_y;



// 패들 좌표(x, y)
// Paddle[0], Paddle[1] => 패들 좌하단 좌표
// Paddle[2], Paddle[3] => 패들 좌상단 좌표
// Paddle[4], Paddle[5] => 패들 우상단 좌표
// Paddle[6], Paddle[7] => 패들 우하단 좌표
float Paddle[8] = { PaddleX1, PaddleY1, PaddleX1, PaddleY2, PaddleX2, PaddleY2, PaddleX2, PaddleY1 };


// 첫번째 벽돌 좌표(아래기준, 0행 왼쪽에서 시작)
float brickX = 220.0, brickY = 380.0;
float brick[5][8] = {
	{brickX, brickY, brickX, brickY + 50.0, brickX + 50.0, brickY + 50.0, brickX + 50.0, brickY},
	{brickX + 70.0, brickY, brickX + 70.0, brickY + 50.0, brickX + 120.0, brickY + 50.0, brickX + 120.0, brickY},
	{brickX + 140.0, brickY, brickX + 140.0, brickY + 50.0, brickX + 190.0, brickY + 50.0, brickX + 190.0, brickY},
	{brickX + 210.0, brickY, brickX + 210.0, brickY + 50.0, brickX + 260.0, brickY + 50.0, brickX + 260.0, brickY},
	{brickX + 280.0, brickY, brickX + 280.0, brickY + 50.0, brickX + 330.0, brickY + 50.0, brickX + 330.0, brickY},
};


// 두번째 벽돌 좌표(0행 왼쪽에서 시작)
float brick2X = 220.0, brick2Y = 450.0;
float brick2[5][8] = {
	{brick2X, brick2Y, brick2X, brick2Y + 50.0, brick2X + 50.0, brick2Y + 50.0, brick2X + 50.0, brick2Y},
	{brick2X + 70.0, brick2Y, brickX + 70.0, brick2Y + 50.0, brickX + 120.0, brick2Y + 50.0, brickX + 120.0, brick2Y},
	{brickX + 140.0, brick2Y, brickX + 140.0, brick2Y + 50.0, brickX + 190.0, brick2Y + 50.0, brickX + 190.0, brick2Y},
	{brickX + 210.0, brick2Y, brickX + 210.0, brick2Y + 50.0, brickX + 260.0, brick2Y + 50.0, brickX + 260.0, brick2Y},
	{brickX + 280.0, brick2Y, brickX + 280.0, brick2Y + 50.0, brickX + 330.0, brick2Y + 50.0, brickX + 330.0, brick2Y},
};


// 구조체 Point
typedef struct _Point {
	float	x;
	float	y;
} Point;


Point moving_ball, velocity;
Point center;


// 초기화 함수
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


// 도형그리기 함수
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height); // mouse2()
}


// 패들 방정식
float StraightP(float u) {
	float x2 = 300.0, x1 = 490.0, y2 = 120.0, y1 = 120.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (u - x1) + y1;
	return y;
}


// 직선의 방정식(우상단)
float Straight(float u) {
	float x2 = 400.0, x1 = 800.0, y2 = 800.0, y1 = 400.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (u - x1) + y1;
	return y;
}


// 직선의 방정식(좌하단)
float Straight2(float i) {
	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 0.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (i - x1) + y1;
	return y;
}


// 직선의 방정식(좌상단)
float Straight3(float i) {
	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 800.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (i - x1) + y1;
	return y;
}


// 직선의 방정식(우하단)
float Straight4(float i) {
	float x2 = 400.0, x1 = 800.0, y2 = 0.0, y1 = 400.0, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (i - x1) + y1;
	return y;
}


// 원과 직선 사이의 거리 공식(패들)
float LinePaddle(float x1, float y1) {
	float distancePaddle;

	distancePaddle = std::abs((0 * x1) + (-1 * y1) + 120.0) / 1;
	return distancePaddle;
}


// 원과 직선 사이의 거리 공식(우상단)
float LineRightUp(float x1, float y1) {
	float distanceRUp;

	distanceRUp = std::abs((1 * x1) + (1 * y1) - 1200.0) / 1;
	return distanceRUp;
}


// 원과 직선 사이의 거리 공식(좌상단)
float LineLeftUp(float x1, float y1) {
	float distanceLUp;

	distanceLUp = std::abs((1 * x1) + (-1 * y1) + 400.0) / 1;
	return distanceLUp;
}


// 원 그리기(참고)
void	Modeling_Circle(float radius, Point CC) {
	float	delta;
	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius * cos(delta * i), CC.y + radius * sin(delta * i));
	glEnd();
}


// 게임판 그리기(도형)
void Modeling_GameGround() {

	// 검은색
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);

	// 윈도우 사이즈(800, 800)
	glVertex2f(0.0, 400.0);
	glVertex2f(400.0, 800.0);
	glVertex2f(800.0, 400.0);
	glVertex2f(400.0, 0.0);

	glEnd();
}


// 첫번째줄 벽돌 그리기
void Modeling_brick() {

	for (int i = 0; i < 5; i++) {
		glColor3f(1.0, 1.0, 1.0);

		// 나머지는 하얀색 사각형으로 만든다.
		glBegin(GL_POLYGON);
		glVertex2f(brick[i][0], brick[i][1]);
		glVertex2f(brick[i][2], brick[i][3]);
		glVertex2f(brick[i][4], brick[i][5]);
		glVertex2f(brick[i][6], brick[i][7]);
		glEnd();
	}

}


// 두번째줄 벽돌 그리기
void Modeling_brick2() {

	glColor3f(1.0, 1.0, 1.0);

	// 하얀색 사각형을 만든다.
	for (int i = 0; i < 5; i++) {
		glBegin(GL_POLYGON);
		glVertex2f(brick2[i][0], brick2[i][1]);
		glVertex2f(brick2[i][2], brick2[i][3]);
		glVertex2f(brick2[i][4], brick2[i][5]);
		glVertex2f(brick2[i][6], brick2[i][7]);
		glEnd();
	}
}


// 패들 그리기
void Modeling_Paddle() {
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(Paddle[0], Paddle[1]);
	glVertex2f(Paddle[2], Paddle[3]);
	glVertex2f(Paddle[4], Paddle[5]);
	glVertex2f(Paddle[6], Paddle[7]);
	glEnd();
}


// 공이 벽돌에 충돌했을때
void Collision_Detection_to_Brick(void) {
	float distanceP, distanceP2, distanceP3;
	float brickX, brickY = 0.0;
	//deltaP1_x = moving_ball.x - brickX;
	//deltaP2_y = moving_ball.y - brickY;

	distanceP = sqrt(deltaP1_x * deltaP1_x + deltaP2_y * deltaP2_y);


	// 움직이는 공이 벽돌에 충돌했을 때 벡터 y의 진행방향을 음의 방향으로 바꾼다.
	/*if (distanceP < moving_ball_radius) {
		Bcollision_count++;
		velocity.y = - velocity.y;
	}*/

	// 공이 충돌했을 때 벽돌의 위치를 바꾼다.
	/*if (Bcollision_count != 0) {
		for (int i = 0; i < 8; i += 2) {
			brick[i][8] = { brickY };
		}
	}*/
}

// 패들 부분 좀 더 생각

// 공이 패들에 충돌했을때
void Collision_Detection_to_Paddle(void) {

	
	// 패들 충돌 변수
	float Pdistance1, Pdistance2, Pdistance3, Scalar;
	Point vector1, vector2, vector3; vector1.x, vector1.y, vector2.x, vector2.y, vector3.x, vector3.y= 0.0;


	if (LinePaddle(moving_ball.x, moving_ball.y) < moving_ball_radius) {
	if (moving_ball.x >= Paddle[0] || moving_ball.x <= Paddle[6]) {

		// 방향벡터
		vector1.x = moving_ball.x - center.x;
		vector1.y = StraightP(moving_ball.x) - center.y;

		vector1.x = vector1.x / normalize(vector1.x, vector1.y);
		vector1.y = vector1.y / normalize(vector1.x, vector1.y);

		// 법선벡터
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


// 공이 벽에 충돌했을때 함수
void Collision_Detection_to_Walls(void) {

	float distanceP, distanceP2, distanceP3;
	Point Vector1, Vector2, Vector3;
	Vector1.x = 0, Vector2.x = 0, Vector3.x = 0;
	Vector1.y = 0, Vector2.y = 0, Vector3.y = 0;


// 경기장 우상단 벽 충돌
	float RightUp = LineRightUp(moving_ball.x, moving_ball.y);
	float LeftUp = LineLeftUp(moving_ball.x, moving_ball.y);
	if (RightUp < moving_ball_radius) {

		// 벡터 정규화(벡터의 길이를 1로 만드는 것)
		// 벡터의 각 성분을 벡터의 크기로 나누는 것

		// 점과 점 사이의 거리(크기)

		// 방향벡터(시점이 경기장의 중앙 -> 충돌 지점)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight(moving_ball.x) - center.y;


		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터(시점이 충돌 지점 600, 600 종점 경기장의 중앙)
		Vector2.x = 0.0 - (width / 2);
		Vector2.y = (width / 2) - width;


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);


		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);

		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// 좌하단
	else if (RightUp > width - moving_ball_radius) {
		// 벡터 정규화(벡터의 길이를 1로 만드는 것)
		// 벡터의 각 성분을 벡터의 크기로 나누는 것


		// 방향벡터(시점이 경기장의 중앙, 충돌 지점)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight2(moving_ball.x) - center.y;


		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터(시점이 충돌 지점 600, 600 종점 경기장의 중앙)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = width - (width / 2);


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// 경기장 좌상단 벽 충돌
	if (LeftUp < moving_ball_radius) {
		// 벡터 정규화(벡터의 길이를 1로 만드는 것)
		// 벡터의 각 성분을 벡터의 크기로 나누는 것



		// 방향벡터(시점이 경기장의 중앙, 충돌 지점 600, 600)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight3(moving_ball.x) - center.y;

		distanceP = sqrt(pow(Vector1.x, 2) + pow(Vector1.y, 2));

		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터(시점이 충돌 지점 600, 600 종점 경기장의 중앙)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = 0.0 - (width / 2);


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// 경기장 우하단 충돌
	else if (LeftUp > width - moving_ball_radius) {
		// 벡터 정규화(벡터의 길이를 1로 만드는 것)
		// 벡터의 각 성분을 벡터의 크기로 나누는 것


		// 방향벡터(시점이 경기장의 중앙, 충돌 지점 600, 600)
		Vector1.x = moving_ball.x - center.x;
		Vector1.y = Straight4(moving_ball.x) - center.y;


		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터(시점이 충돌 지점 600, 600 종점 경기장의 중앙)
		Vector2.x = (width / 2) - width;
		Vector2.y = width - (width / 2);


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		Vector3.x = Vector1.x + (2 * Vector2.x);
		Vector3.y = Vector1.y + (2 * Vector2.y);

		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}
}


// 실행하는 함수
void RenderScene(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to Yellow
	glClear(GL_COLOR_BUFFER_BIT);


	Modeling_GameGround();
	Modeling_brick();
	Modeling_brick2();
	Modeling_Paddle();

	/*윈도우 중심의 위치에 고정된 공 그리기 */

	//glColor3f(1.0, 1.0, 0.0);
	//if (collision_count % 2)
	//	Modeling_Circle(radius1, fixed_ball);
	//else glColor3f(1.0, 1.0, 0.0);

	// 움직이는 공의 위치 변화
	moving_ball.x += velocity.x * speed;
	moving_ball.y += velocity.y * speed;

	// 충돌 처리 부분
	//Collision_Detection_Between_Balls(); // 공과 공의 충돌 함수
	/*Collision_Detection_to_Brick();*/
	Collision_Detection_to_Walls(); // 공과 벽의 충돌 함수
	Collision_Detection_to_Paddle();

	// 움직이는 공 그리기
	glColor3f(0.0, 1.0, 1.0);
	Modeling_Circle(moving_ball_radius, moving_ball);

	glutSwapBuffers();
	glFlush();
}


void MyKey(unsigned char key, int x, int y) {

	switch (key) {

		// 벡터 값 빼기
	case 'n':
		velocity.x -= 0.1;
		velocity.y -= 0.1;
		break;

	case 'b':
		velocity.x += 0.1;
		velocity.y += 0.1;
		break;

		// 벡터 방향 바꾸기
	case 'm':
		velocity.x = -velocity.x;
		velocity.y = -velocity.y;
		break;

		// 벡터 값 0으로 바꾸기
	case 'p':
		velocity.x = 0.0;
		velocity.y = 0.0;
		break;

	default: break;
	}
}


// 키보드 키를 누르는 함수
void SpecialKey(int key, int x, int y) {
	switch (key) {

		// 패들이 사각형 안에서 밖으로 안삐져나감(왼쪽 방향키)
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

		// 패들이 사각형 안에서 밖으로 안삐져나감(오른쪽 방향키)
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