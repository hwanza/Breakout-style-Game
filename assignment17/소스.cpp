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
float brick[5][8] = {
	{180.0, 380.0, 180.0, 430.0, 230.0, 430.0, 230.0, 380.0},
	{250.0, 380.0, 250.0, 430.0, 300.0, 430.0, 300.0, 380.0},
	{320.0, 380.0, 320.0, 430.0, 370.0, 430.0, 370.0, 380.0},
	{390.0, 380.0, 390.0, 430.0, 440.0, 430.0, 440.0, 380.0},
	{460.0, 380.0, 460.0, 430.0, 510.0, 430.0, 510.0, 380.0},
};


// 두번째 벽돌 좌표(0행 왼쪽에서 시작)
float brick2[5][8] = {
	{180.0, 450.0, 180.0, 500.0, 230.0, 500.0, 230.0, 450.0},
	{250.0, 450.0, 250.0, 500.0, 300.0, 500.0, 300.0, 450.0},
	{320.0, 450.0, 320.0, 500.0, 370.0, 500.0, 370.0, 450.0},
	{390.0, 450.0, 390.0, 500.0, 440.0, 500.0, 440.0, 450.0},
	{460.0, 450.0, 460.0, 500.0, 510.0, 500.0, 510.0, 450.0},
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


	velocity.x = 0.3;
	velocity.y = 0.5;

	collision_count = 1;
}


// 도형그리기 함수
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height); // mouse2()
}


// 직선의 방정식
//float Line1(float i) {
//	float x2 = 400.0, x1 = 800.0, y2 = 800.0, y1 = 400.0, m, x, y, distance;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}


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



//float Line2() {
//	float x1 = 0.0, x2 = 400.0, y1 = 400.0, y2 = 800.0, distance;
//	distance = sqrt(pow((x2 - x1), 2) - pow((y2 - y1), 2));
//
//	return distance;
//}



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


// y = x + 400; 왼쪽 위
int Point_to_Straight(int a, int b) {
	int distance;

	if (moving_ball.y <= 600) {
		distance = std::abs(moving_ball.x + moving_ball.y) / 401;
	}

	return distance;
}


// 첫번째줄 벽돌 그리기
void Modeling_brick() {

	for (int i = 0; i < 5; i++) {
		glColor3f(1.0, 1.0, 1.0);

		//// 만약에 첫번째 줄의 3번째 사각형이라면
		//if (i == 2) {
		//	// 공의 충돌횟수가 0이라면 하얀색으로 나타낸다.
		//	if (Bcollision_count == 0) glColor3f(1.0, 1.0, 1.0);

		//	// 공의 충돌횟수가 0이 아니라면 검은색으로 나타낸다.
		//	else glColor3f(0.0, 0.0, 0.0);
		//}
		//else if (i == 4) {
		//	if (Bcollision_count == 0) glColor3f(1.0, 1.0, 1.0);
		//	else glColor3f(0.0, 0.0, 0.0);
		//}

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
	float distanceP, brickX = 350.0, brickY = 380.0;
	deltaP1_x = moving_ball.x - brickX;
	deltaP2_y = moving_ball.y - brickY;

	distanceP = sqrt(deltaP1_x * deltaP1_x + deltaP2_y * deltaP2_y);


	// 움직이는 공이 벽돌에 충돌했을 때 벡터 y의 진행방향을 음의 방향으로 바꾼다.
	/*if (distanceP < moving_ball_radius) {
		Bcollision_count++;
		velocity.y = - velocity.y;
	}*/

	// 공이 충돌했을 때 벽돌의 위치를 바꾼다.
	/*if (Bcollision_count != 0) {
		for (int i = 0; i < 8; i += 2) {
			brick[i][8] = { 450.0 };
		}
	}*/
}


// 공이 패들에 충돌했을때
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

	// 패들 충돌 변수
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


// 공이 벽에 충돌했을때 함수
void Collision_Detection_to_Walls(void) {

	float distanceP, distanceP2, distanceP3, brickX = 350.0, brickY = 380.0;
	float HX, HY;
	Point Vector1;


	// 법선벡터 n 벡터(x, y) 좌표
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


	// 경기장 우상단 벽 충돌
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


	// 경기장 좌상단 벽 충돌
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
	오른쪽 벽 충돌*/
	if (moving_ball.x + moving_ball_radius > width) {
		/*Bcollision_count--;*/
		velocity.x = -velocity.x;
		//printf("velocity.x : %f\n", velocity.x);
		//printf("moving_ball.x : %f\n", moving_ball.x);
	}

	// width
	// 위쪽 벽 충돌
	if (moving_ball.y + moving_ball_radius > width) {
		/*Bcollision_count++;*/
		velocity.y = -velocity.y;
		//printf("velocity.x : %f\n", velocity.x);
		//printf("moving_ball.x : %f\n", moving_ball.x);
	}

	// 0
	// 왼쪽 벽 충돌
	//if (moving_ball.x - moving_ball_radius < 0.0) {
	//	/*Bcollision_count++;*/
	//	velocity.x = -velocity.x;
	//	printf("LeftUp : %f\n", LeftUp);
	//	//printf("velocity.x : %f\n", velocity.x);
	//	//printf("moving_ball.x : %f\n", moving_ball.x);
	//}

	// 아래쪽 벽
	//if (moving_ball.y - moving_ball_radius < 120.0) {
	//	/*Bcollision_count--;*/
	//	Bcollision_count++;
	//	velocity.y = -velocity.y;
	//	printf("LeftUp : %f\n", LeftUp);
	//	//printf("velocity.x : %f\n", velocity.x);
	//	//printf("moving_ball.x : %f\n", moving_ball.x);
	//}

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