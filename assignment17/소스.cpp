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


float PaddleX1 = 300.0, PaddleX2 = 400.0;
float PaddleY1 = 100.0, PaddleY2 = 120.0;


float Line;

// 방향벡터 정규화
float speed = 0.2;


// 고정된 공의 반지름, 움직이는 공의 반지름
float	radius1, moving_ball_radius;
float	delta1_x, delta2_y, deltaP1_x, deltaP2_y;



// 패들 방정식
float StraightP(float u) {
	float x2 = PaddleX1, x1 = PaddleX2, y2 = PaddleY2, y1 = PaddleY2, m, y;
	m = (y2 - y1) / (x2 - x1);

	y = m * (u - x1) + y1;
	return y;
}


//// 직선의 방정식(우상단)
//float Straight(float u) {
//	float x2 = 400.0, x1 = 800.0, y2 = 800.0, y1 = 400.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (u - x1) + y1;
//	return y;
//}
//
//
//// 직선의 방정식(좌하단)
//float Straight2(float i) {
//	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 0.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}
//
//
//// 직선의 방정식(좌상단)
//float Straight3(float i) {
//	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 800.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}
//
//
//// 직선의 방정식(우하단)
//float Straight4(float i) {
//	float x2 = 400.0, x1 = 800.0, y2 = 0.0, y1 = 400.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}

// 패들 좌표(x, y)
// Paddle[0], Paddle[1] => 패들 좌하단 좌표
// Paddle[2], Paddle[3] => 패들 좌상단 좌표
// Paddle[4], Paddle[5] => 패들 우상단 좌표
// Paddle[6], Paddle[7] => 패들 우하단 좌표

float Paddle[8] = { PaddleX1, PaddleY1, PaddleX1, PaddleY2, PaddleX2, PaddleY2, PaddleX2, PaddleY1 };



// 첫번째 벽돌 좌표(아래기준, 0행 왼쪽에서 시작)
float brickX = 220.0, brickY = 380.0;
float brick[5][9] = {
	{brickX, brickY, brickX, brickY + 50.0, brickX + 50.0, brickY + 50.0, brickX + 50.0, brickY, 1.0},
	{brickX + 70.0, brickY, brickX + 70.0, brickY + 50.0, brickX + 120.0, brickY + 50.0, brickX + 120.0, brickY, 1.0},
	{brickX + 140.0, brickY, brickX + 140.0, brickY + 50.0, brickX + 190.0, brickY + 50.0, brickX + 190.0, brickY, 1.0},
	{brickX + 210.0, brickY, brickX + 210.0, brickY + 50.0, brickX + 260.0, brickY + 50.0, brickX + 260.0, brickY, 1.0},
	{brickX + 280.0, brickY, brickX + 280.0, brickY + 50.0, brickX + 330.0, brickY + 50.0, brickX + 330.0, brickY, 1.0},
};



// 두번째 벽돌 좌표(0행 왼쪽에서 시작)
float brick2X = 220.0, brick2Y = 450.0;
float brick2[5][9] = {
	{brick2X, brick2Y, brick2X, brick2Y + 50.0, brick2X + 50.0, brick2Y + 50.0, brick2X + 50.0, brick2Y, 1.0},
	{brick2X + 70.0, brick2Y, brickX + 70.0, brick2Y + 50.0, brickX + 120.0, brick2Y + 50.0, brickX + 120.0, brick2Y, 1.0},
	{brickX + 140.0, brick2Y, brickX + 140.0, brick2Y + 50.0, brickX + 190.0, brick2Y + 50.0, brickX + 190.0, brick2Y, 1.0},
	{brickX + 210.0, brick2Y, brickX + 210.0, brick2Y + 50.0, brickX + 260.0, brick2Y + 50.0, brickX + 260.0, brick2Y, 1.0},
	{brickX + 280.0, brick2Y, brickX + 280.0, brick2Y + 50.0, brickX + 330.0, brick2Y + 50.0, brickX + 330.0, brick2Y, 1.0},
};



typedef struct _Point {
	float	x;
	float	y;
} Point;


Point moving_ball, velocity;
Point center;


// 정규화 식
float normalize(float a, float b) {
	float value;
	value = sqrt(pow(a, 2) + pow(b, 2));
	return value;
}


// 점과 점 사이의 거리
float DotToDot(float x1, float y1, float x2, float y2) {
	float val;
	val = sqrt(pow((x2 - x2), 2) + pow((y2 - y1), 2));
	return val;
}



// 각도 -> 벡터
// 정규화된 해당하는 각도
// x 좌표 : cos(2 * pi / 360 * 각도)
// y 좌표 : sin(2 * pi / 360 * 각도)


// 초기화 함수
void init(void) {

	moving_ball_radius = 10.0;
	moving_ball.x = width / 4;
	moving_ball.y = height / 2;

	center.x = width / 2;
	center.y = height / 2;

	velocity.x = cos(2 * PI / 360 * 350);
	velocity.y = sin(2 * PI / 360 * 350);
}



// 도형그리기 함수
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height);
}


// 원과 직선 사이의 거리 공식(패들)
float LinePaddle(float x1, float y1) {
	float distancePaddle;

	distancePaddle = std::abs((0 * x1) + (-1 * y1) + PaddleY2) / 1;
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
		if (brick[i][8] == 1.0) {
			// 3번째 벽돌 노란색 만들기
			if (i == 2) glColor3f(1.0, 1.0, 0.0);

			// 나머지 벽돌은 흰색
			else glColor3f(1.0, 1.0, 1.0);

			// 좌표 확인해서 도형 만들기
			glBegin(GL_POLYGON);
			glVertex2f(brick[i][0], brick[i][1]);
			glVertex2f(brick[i][2], brick[i][3]);
			glVertex2f(brick[i][4], brick[i][5]);
			glVertex2f(brick[i][6], brick[i][7]);
			glEnd();
		}
		else glColor3f(0.0, 0.0, 0.0);
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
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(Paddle[0], Paddle[1]);
	glVertex2f(Paddle[2], Paddle[3]);
	glVertex2f(Paddle[4], Paddle[5]);
	glVertex2f(Paddle[6], Paddle[7]);
	glEnd();
}


// 패들 튕긴 후 벡터 구하기
//float vectorCollisionAngleY(float b) {
//	float vectorY;
//
//	vectorY = b - (2 * b);
//	return vectorY;
//}

// float brickX = 220.0, brickY = 380.0;
// {brickX + 70.0, brickY, brickX + 70.0, brickY + 50.0, brickX + 120.0, brickY + 50.0, brickX + 120.0, brickY, 1.0},


// 첫번째줄 2번째 벽돌[x, y][벽돌의 좌표] || 순서 -> 좌하단, 좌상단, 우상단, 우하단
// ===========================================================================
//float block2[2][4] = {
//	{290.0, 290.0, 340.0, 340.0},
//	{380.0, 430.0, 430.0, 380.0}
//};
//
//float block3EX[2][4] = {
//	{350.0, 350.0, 420.0, 420.0},
//	{370.0, 440.0, 440.0, 370.0}
//};
//
//float block2Top[2][4] = {
//	{360.0, 360.0, 410.0, 410.0},
//	{430.0, 440.0, 440.0, 430.0}
//};
//
//float block2Right[2][4] = {
//	{410.0, 410.0, 420.0, 420.0},
//	{380.0, 430.0, 430.0, 380.0}
//};
//
//float block2Bottom[2][4] = {
//	{360.0, 360.0, 410.0, 410.0},
//	{370.0, 380.0, 380.0, 370.0}
//};
//
//float block2Left[2][4] = {
//	{350.0, 350.0, 360.0, 360.0},
//	{380.0, 430.0, 430.0, 380.0}
//};
// ===========================================================================


// 첫번째줄 3번째 벽돌[x, y][벽돌의 좌표] || 순서 -> 좌하단, 좌상단, 우상단, 우하단
// ===========================================================================
float block3[2][4] = {
	{360.0, 360.0, 410.0, 410.0},
	{380.0, 430.0, 430.0, 380.0}
};

float block3EX[2][4] = {
	{350.0, 350.0, 420.0, 420.0},
	{370.0, 440.0, 440.0, 370.0}
};

float block3Top[2][4] = {
	{360.0, 360.0, 410.0, 410.0},
	{430.0, 440.0, 440.0, 430.0}
};

float block3Right[2][4] = {
	{410.0, 410.0, 420.0, 420.0},
	{380.0, 430.0, 430.0, 380.0}
};

float block3Bottom[2][4] = {
	{360.0, 360.0, 410.0, 410.0},
	{370.0, 380.0, 380.0, 370.0}
};

float block3Left[2][4] = {
	{350.0, 350.0, 360.0, 360.0},
	{380.0, 430.0, 430.0, 380.0}
};
// =================================================================


// 확장된 사각형 그리기
//void Modeling_block3EX() {
//	glColor3f(1.0, 1.0, 1.0);
//	glBegin(GL_LINES);
//	glVertex2f(block3EX[0][0], block3EX[1][0]);
//	glVertex2f(block3EX[0][1], block3EX[1][1]);
//
//	glVertex2f(block3EX[0][1], block3EX[1][1]);
//	glVertex2f(block3EX[0][2], block3EX[1][2]);
//
//	glVertex2f(block3EX[0][2], block3EX[1][2]);
//	glVertex2f(block3EX[0][3], block3EX[1][3]);
//
//	glVertex2f(block3EX[0][3], block3EX[1][3]);
//	glVertex2f(block3EX[0][0], block3EX[1][0]);
//	glEnd();
//}


// 원과 직선의 방정식
float BrickRight(float x1) {
	float distancePaddle;

	distancePaddle = std::abs((1 * x1) + block3[1][2] + 10.0) / 1;
	return distancePaddle;
}


//float brick[5][8] = {
//	{brickX, brickY, brickX, brickY + 50.0, brickX + 50.0, brickY + 50.0, brickX + 50.0, brickY},
//	{brickX + 70.0, brickY, brickX + 70.0, brickY + 50.0, brickX + 120.0, brickY + 50.0, brickX + 120.0, brickY},
//	{brickX + 140.0, brickY, brickX + 140.0, brickY + 50.0, brickX + 190.0, brickY + 50.0, brickX + 190.0, brickY},
//	{brickX + 210.0, brickY, brickX + 210.0, brickY + 50.0, brickX + 260.0, brickY + 50.0, brickX + 260.0, brickY},
//	{brickX + 280.0, brickY, brickX + 280.0, brickY + 50.0, brickX + 330.0, brickY + 50.0, brickX + 330.0, brickY},
//};


//float brickX = 220.0, brickY = 380.0;


//float block3EX[2][4] = {
//	{350.0, 350.0, 420.0, 420.0},
//	{370.0, 440.0, 440.0, 370.0}
//};

//float block3Top[2][4] = {
//	{360.0, 360.0, 410.0, 410.0},
//	{430.0, 440.0, 440.0, 430.0}
//};
//
//float block3Right[2][4] = {
//	{410.0, 410.0, 420.0, 420.0},
//	{380.0, 430.0, 430.0, 380.0}
//};
//
//float block3Bottom[2][4] = {
//	{360.0, 360.0, 410.0, 410.0},
//	{370.0, 380.0, 380.0, 370.0}
//};
//
//float block3Left[2][4] = {
//	{350.0, 350.0, 360.0, 360.0},
//	{380.0, 430.0, 430.0, 380.0}
//};


// 공이 벽돌에 충돌했을때
void Collision_Detection_to_Brick(void) {
	Point vector1, vector2, vector3;
	float distanceP, distanceP2, distanceP3;

	// 첫번째 2번째 벽돌
	if ((moving_ball.x >= block3EX[0][0] && moving_ball.x <= block3EX[0][2]) && (moving_ball.y >= block3EX[1][0] && moving_ball.y <= block3EX[1][1])) {
		//	float block3[2][4] = {
		//{360.0, 360.0, 410.0, 410.0},
		//{380.0, 430.0, 430.0, 380.0}
		//};

		if (brick[2][8] == 1.0) {

			for (int i = 0; i < 4; i++) {
				if (i % 2 == 0) {
					// 좌하단, 우상단
					if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][i], block3EX[1][i]) < moving_ball_radius) {
						velocity.y = -velocity.y;
					}
				}
				else {
					// 좌상단, 우하단
					if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][i], block3EX[1][i]) < moving_ball_radius) {
						velocity.x = -velocity.x;
					}
				}
			}
			//// 좌하단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][0], block3EX[1][0]) < moving_ball_radius) {
			//	velocity.y = -velocity.y;
			//}

			//// 좌상단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][1], block3EX[1][1]) < moving_ball_radius) {
			//	velocity.x = -velocity.x;
			//}

			//// 우상단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][2], block3EX[1][2]) < moving_ball_radius) {
			//	velocity.y = -velocity.y;
			//	// velocity.x = -velocity.x;
			//}

			//// 우하단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][3], block3EX[1][3]) < moving_ball_radius) {
			//	velocity.x = -velocity.x;
			//}

			// 공의 중심 좌표가 확장된 Top 사각형 안에 있으면
			if (moving_ball.x >= block3Top[0][0] && moving_ball.x <= block3Top[0][2] && moving_ball.y >= block3Top[1][0] && moving_ball.y <= block3Top[1][1]) {
				velocity.y = -velocity.y;
			}
			// Right 사각형 안에 있으면
			else if (moving_ball.x >= block3Right[0][0] && moving_ball.x <= block3Right[0][2] && moving_ball.y >= block3Right[1][0] && moving_ball.y <= block3Right[1][1]) {
				velocity.x = -velocity.x;
			}
			// Bottom 사각형 안에 있으면
			else if (moving_ball.x >= block3Bottom[0][0] && moving_ball.x <= block3Bottom[0][2] && moving_ball.y >= block3Bottom[1][0] && moving_ball.y <= block3Bottom[1][1]) {
				velocity.x = -velocity.x;
			}

			// Left 사각형 안에 있으면
			else if (moving_ball.x >= block3Left[0][0] && moving_ball.x <= block3Left[0][2] && moving_ball.y >= block3Left[1][0] && moving_ball.y <= block3Left[1][1]) {
				velocity.x = -velocity.x;
			}

		}

		brick[2][8] = 0.0;
	}


	// 첫번째 3번째 벽돌
	// 공의 중심 좌표가 확장된 사각형 안에 있나?
	if ((moving_ball.x >= block3EX[0][0] && moving_ball.x <= block3EX[0][2]) && (moving_ball.y >= block3EX[1][0] && moving_ball.y <= block3EX[1][1])) {
		//	float block3[2][4] = {
		//{360.0, 360.0, 410.0, 410.0},
		//{380.0, 430.0, 430.0, 380.0}
		//};

		if (brick[2][8] == 1.0) {

			for (int i = 0; i < 4; i++) {
				if (i % 2 == 0) {
					// 좌하단, 우상단
					if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][i], block3EX[1][i]) < moving_ball_radius) {
						velocity.y = -velocity.y;
					}
				}
				else {
					// 좌상단, 우하단
					if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][i], block3EX[1][i]) < moving_ball_radius) {
						velocity.x = -velocity.x;
					}
				}
			}
			//// 좌하단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][0], block3EX[1][0]) < moving_ball_radius) {
			//	velocity.y = -velocity.y;
			//}

			//// 좌상단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][1], block3EX[1][1]) < moving_ball_radius) {
			//	velocity.x = -velocity.x;
			//}

			//// 우상단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][2], block3EX[1][2]) < moving_ball_radius) {
			//	velocity.y = -velocity.y;
			//	// velocity.x = -velocity.x;
			//}

			//// 우하단 모서리
			//if (DotToDot(moving_ball.x, moving_ball.y, block3EX[0][3], block3EX[1][3]) < moving_ball_radius) {
			//	velocity.x = -velocity.x;
			//}

			// 공의 중심 좌표가 확장된 Top 사각형 안에 있으면
			if (moving_ball.x >= block3Top[0][0] && moving_ball.x <= block3Top[0][2] && moving_ball.y >= block3Top[1][0] && moving_ball.y <= block3Top[1][1]) {
				velocity.y = -velocity.y;
			}
			// Right 사각형 안에 있으면
			else if (moving_ball.x >= block3Right[0][0] && moving_ball.x <= block3Right[0][2] && moving_ball.y >= block3Right[1][0] && moving_ball.y <= block3Right[1][1]) {
				velocity.x = -velocity.x;
			}
			// Bottom 사각형 안에 있으면
			else if (moving_ball.x >= block3Bottom[0][0] && moving_ball.x <= block3Bottom[0][2] && moving_ball.y >= block3Bottom[1][0] && moving_ball.y <= block3Bottom[1][1]) {
				velocity.x = -velocity.x;
			}

			// Left 사각형 안에 있으면
			else if (moving_ball.x >= block3Left[0][0] && moving_ball.x <= block3Left[0][2] && moving_ball.y >= block3Left[1][0] && moving_ball.y <= block3Left[1][1]) {
				velocity.x = -velocity.x;
			}

		}

		brick[2][8] = 0.0;
	}

}


// 공이 패들에 충돌했을때
void Collision_Detection_to_Paddle(void) {

	// 패들 충돌 변수
	float Pdistance1, Pdistance2, Pdistance3, Scalar;
	Point vector1, vector2, vector3; vector1.x, vector1.y, vector2.x, vector2.y, vector3.x, vector3.y = 0.0;

	// 패들 좌표 간 충돌
	if (LinePaddle(moving_ball.x, moving_ball.y) < moving_ball_radius) {
		if (moving_ball.x >= Paddle[0] && moving_ball.x <= Paddle[6]) {

			// 방향벡터
			vector1.x = velocity.x;
			vector1.y = velocity.y;


			vector2.x = vector1.x;
			vector2.y = vector1.y - (2 * vector1.y);

			/*vector2.y = vectorCollisionAngleY(vector1.y);*/

			velocity.x = vector2.x;
			velocity.y = vector2.y;
		}
	}
}


// 공이 벽에 충돌했을때 함수
void Collision_Detection_to_Walls(void) {

	float distanceP, distanceP2, distanceP3;
	Point Vector1, Vector2, Vector3;
	Vector1.x = 0, Vector2.x = 0, Vector3.x = 0;
	Vector1.y = 0, Vector2.y = 0, Vector3.y = 0;

	/*printf("velocity.x : %f\n", velocity.x);*/
	//printf("x : %f\n", moving_ball.x);
	//printf("y : %f\n", moving_ball.y);
	//printf("f : %f\n", moving_ball.x);
	/*printf("d : %f\n", distanceDot(moving_ball.x, block3[0][2] + 10.0, moving_ball.y, 405.0));*/
	/*printf("Dot : %f\n", distanceDot(brickX + 190.0, brickY, brickX + 190.0, brickY + 50.0));*/

	//printf("moving : %f\n", moving_ball.x);
	//printf("x : %f\n", BrickRight(moving_ball.x));

	// 경기장 우상단 벽 충돌
	float RightUp = LineRightUp(moving_ball.x, moving_ball.y);
	float LeftUp = LineLeftUp(moving_ball.x, moving_ball.y);


	if (RightUp < moving_ball_radius) {

		// 벡터 정규화(벡터의 길이를 1로 만드는 것)
		// 벡터의 각 성분을 벡터의 크기로 나누는 것
		// 점과 점 사이의 거리(크기)
		// 방향벡터(시점이 경기장의 중앙 -> 충돌 지점)

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;

		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터 : 시점(400, 800) , 종점(0, 400)
		Vector2.x = 0.0 - (width / 2);
		Vector2.y = (width / 2) - width;


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		// 바뀐 벡터
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);

		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);


		// 정규화
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.x = Vector3.x;
		velocity.y = Vector3.y;


		//// 새로운 방법(패들의 충돌 사용)
		//Vector2.x = Vector1.x - (2 * Vector1.x);
		//Vector2.y = Vector1.y;


		//velocity.x = Vector2.x;
		//velocity.y = Vector2.y;
	}


	// 좌하단
	else if (RightUp > width - moving_ball_radius) {

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;


		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터 : 시점 (0, 400) , 종점 (400, 800)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = width - (width / 2);


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		// 바뀐 벡터
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);


		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);

		// 정규화
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// 경기장 좌상단 벽 충돌
	if (LeftUp < moving_ball_radius) {

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;


		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터 : 시점 (0, 400) , 종점 (400, 0)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = 0.0 - (width / 2);


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);

		// 바뀐 벡터
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);

		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);


		// 정규화
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// 경기장 우하단 충돌
	else if (LeftUp > width - moving_ball_radius) {

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;


		// 정규화
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// 법선벡터 : 시점(800, 400) , 종점(400, 800)
		Vector2.x = (width / 2) - width;
		Vector2.y = width - (width / 2);


		// 정규화
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		// 바뀐 벡터
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);

		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);


		// 정규화
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


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
	/*Modeling_brick2();*/
	Modeling_Paddle();


	/*Modeling_block3EX();*/


	// 움직이는 공의 위치 변화
	moving_ball.x += velocity.x * speed;
	moving_ball.y += velocity.y * speed;


	// 충돌 처리 부분
	//Collision_Detection_Between_Balls(); // 공과 공의 충돌 함수
	Collision_Detection_to_Brick(); // 공과 벽돌의 충돌 함수
	Collision_Detection_to_Walls(); // 공과 벽의 충돌 함수
	Collision_Detection_to_Paddle(); // 공과 패들의 충돌 함수
	

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