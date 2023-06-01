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

float YellowBrick = 0;

float brickCollisionCnt = 0;

// ���⺤�� ����ȭ
float speed = 0.1;

// ������ ���� ������, �����̴� ���� ������
float	radius1, moving_ball_radius;
float	delta1_x, delta2_y, deltaP1_x, deltaP2_y;


// ��Ʈ����Ʈ �Լ�
void renderBitmapCharacter(float x, float y, void* font, char* string)
{
	char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}


// SUCCESS ���� �Լ�
void Draw_SUCCESS() {
	glColor3f(1.0, 1.0, 1.0);
	char buf[100] = { 0 };
	sprintf_s(buf, "SUCCESS");
	renderBitmapCharacter(350, 400, GLUT_BITMAP_TIMES_ROMAN_24, buf);
}


// �й�, �̸� �ؽ�Ʈ �Լ�
void Draw_NameText() {
	glColor3f(1.0, 1.0, 1.0);
	char buf[100] = { 0 };
	sprintf_s(buf, "20193125_LeeJuHwan");
	renderBitmapCharacter(635, 10, GLUT_BITMAP_9_BY_15, buf);
}

// ���� ���� ���� �ؽ�Ʈ �Լ�
void Draw_GameText() {
	glColor3f(0.0, 0.0, 0.0);
	char buf2[100] = { 0 };
	sprintf_s(buf2, "Breakout-Style-Game");
	renderBitmapCharacter(10, 10, GLUT_BITMAP_TIMES_ROMAN_24, buf2);
}

// �й�, �̸� �ڽ�
void Modeling_NameBox() {

	// ��Ű����
	glColor3f(0.25098, 0.878431, 0.815686);

	glBegin(GL_POLYGON);
	glVertex2f(630.0, 0.0);
	glVertex2f(630.0, 30.0);
	glVertex2f(800.0, 30.0);
	glVertex2f(800.0, 0.0);
	glEnd();
}


// �е� ������
//float StraightP(float u) {
//	float x2 = PaddleX1, x1 = PaddleX2, y2 = PaddleY2, y1 = PaddleY2, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (u - x1) + y1;
//	return y;
//}


//// ������ ������(����)
//float Straight(float u) {
//	float x2 = 400.0, x1 = 800.0, y2 = 800.0, y1 = 400.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (u - x1) + y1;
//	return y;
//}
//
//
//// ������ ������(���ϴ�)
//float Straight2(float i) {
//	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 0.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}
//
//
//// ������ ������(�»��)
//float Straight3(float i) {
//	float x2 = 0.0, x1 = 400.0, y2 = 400.0, y1 = 800.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}
//
//
//// ������ ������(���ϴ�)
//float Straight4(float i) {
//	float x2 = 400.0, x1 = 800.0, y2 = 0.0, y1 = 400.0, m, y;
//	m = (y2 - y1) / (x2 - x1);
//
//	y = m * (i - x1) + y1;
//	return y;
//}

// �е� ��ǥ(x, y)
// Paddle[0], Paddle[1] => �е� ���ϴ� ��ǥ
// Paddle[2], Paddle[3] => �е� �»�� ��ǥ
// Paddle[4], Paddle[5] => �е� ���� ��ǥ
// Paddle[6], Paddle[7] => �е� ���ϴ� ��ǥ


float Paddle[8] = { PaddleX1, PaddleY1, PaddleX1, PaddleY2, PaddleX2, PaddleY2, PaddleX2, PaddleY1 };

// ù��° ���� ��ǥ(�Ʒ�����, 0�� ���ʿ��� ����)
float brickX = 220.0, brickY = 380.0;
float brick[5][9] = {
	{brickX, brickY, brickX, brickY + 50.0, brickX + 50.0, brickY + 50.0, brickX + 50.0, brickY, 1.0},
	{brickX + 70.0, brickY, brickX + 70.0, brickY + 50.0, brickX + 120.0, brickY + 50.0, brickX + 120.0, brickY, 1.0},
	{brickX + 140.0, brickY, brickX + 140.0, brickY + 50.0, brickX + 190.0, brickY + 50.0, brickX + 190.0, brickY, 1.0},
	{brickX + 210.0, brickY, brickX + 210.0, brickY + 50.0, brickX + 260.0, brickY + 50.0, brickX + 260.0, brickY, 1.0},
	{brickX + 280.0, brickY, brickX + 280.0, brickY + 50.0, brickX + 330.0, brickY + 50.0, brickX + 330.0, brickY, 1.0},
};


// �ι�° ���� ��ǥ(0�� ���ʿ��� ����)
float brick2X = 220.0, brick2Y = 450.0;
float brick2[5][9] = {
	{brick2X, brick2Y, brick2X, brick2Y + 50.0, brick2X + 50.0, brick2Y + 50.0, brick2X + 50.0, brick2Y, 1.0},
	{brick2X + 70.0, brick2Y, brickX + 70.0, brick2Y + 50.0, brickX + 120.0, brick2Y + 50.0, brickX + 120.0, brick2Y, 1.0},
	{brickX + 140.0, brick2Y, brickX + 140.0, brick2Y + 50.0, brickX + 190.0, brick2Y + 50.0, brickX + 190.0, brick2Y, 1.0},
	{brickX + 210.0, brick2Y, brickX + 210.0, brick2Y + 50.0, brickX + 260.0, brick2Y + 50.0, brickX + 260.0, brick2Y, 1.0},
	{brickX + 280.0, brick2Y, brickX + 280.0, brick2Y + 50.0, brickX + 330.0, brick2Y + 50.0, brickX + 330.0, brick2Y, 1.0},
};


// ����ü
typedef struct _Point {
	float	x;
	float	y;
} Point;


Point moving_ball, velocity;
Point center;


// ����ȭ ��
float normalize(float a, float b) {
	float value;
	value = sqrt(pow(a, 2) + pow(b, 2));
	return value;
}


// ���� �� ������ �Ÿ�
float DotToDot(float x1, float y1, float x2, float y2) {
	float val;
	val = sqrt(pow((x2 - x2), 2) + pow((y2 - y1), 2));
	return val;
}


// ���� -> ����
// ����ȭ�� �ش��ϴ� ����
// x ��ǥ : cos(2 * pi / 360 * ����)
// y ��ǥ : sin(2 * pi / 360 * ����)


// �ʱ�ȭ �Լ�
void init(void) {

	moving_ball_radius = 10.0;
	moving_ball.x = width / 4;
	moving_ball.y = height / 2;

	center.x = width / 4;
	center.y = height / 4;

	velocity.x = cos(2 * PI / 360 * 290);
	velocity.y = sin(2 * PI / 360 * 180);
}


// �����׸��� �Լ�
void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + width, bottom, bottom + height);
}


// ���� ���� ������ �Ÿ� ����(�е�)
float LinePaddle(float x1, float y1) {
	float distancePaddle;

	distancePaddle = std::abs((0 * x1) + (-1 * y1) + PaddleY2) / 1;
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
		if (brick[i][8] == 1.0) {
			// 3��° ���� Royal Blue �����
			if (i == 2) {
				glColor3f(0.254902, 0.411765, 0.882353);
			}

			// ������ ������ ���
			else glColor3f(1.0, 1.0, 1.0);

			// ��ǥ Ȯ���ؼ� ���� �����
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


// �ι�°�� ���� �׸���
void Modeling_brick2() {

	glColor3f(1.0, 1.0, 1.0);

	// �Ͼ�� �簢���� �����.
	for (int i = 0; i < 5; i++) {
		if (brick2[i][8] == 1.0) {
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_POLYGON);
			glVertex2f(brick2[i][0], brick2[i][1]);
			glVertex2f(brick2[i][2], brick2[i][3]);
			glVertex2f(brick2[i][4], brick2[i][5]);
			glVertex2f(brick2[i][6], brick2[i][7]);
			glEnd();
		}
		else glColor3f(0.0, 0.0, 0.0);
	}
}


// �е� �׸���
void Modeling_Paddle() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(Paddle[0], Paddle[1]);
	glVertex2f(Paddle[2], Paddle[3]);
	glVertex2f(Paddle[4], Paddle[5]);
	glVertex2f(Paddle[6], Paddle[7]);
	glEnd();
}


// �е� ƨ�� �� ���� ���ϱ�
//float vectorCollisionAngleY(float b) {
//	float vectorY;
//
//	vectorY = b - (2 * b);
//	return vectorY;
//}


// ======================================================================================================================================================
// ù��°�� ���� ��ǥ��[���� ����][������ x��ǥ][������ y��ǥ]
float blockArr[5][2][4] = {
	{
		{220.0, 220.0, 270.0, 270.0},
		{380.0, 430.0, 430.0, 380.0},
	},
	{
		{290.0, 290.0, 340.0, 340.0},
		{380.0, 430.0, 430.0, 380.0},
	},
	{
		{360.0, 360.0, 410.0, 410.0},
		{380.0, 430.0, 430.0, 380.0},
	},
	{
		{430.0, 430.0, 480.0, 480.0},
		{380.0, 430.0, 430.0, 380.0},
	},
	{
		{500.0, 500.0, 550.0, 550.0},
		{380.0, 430.0, 430.0, 380.0},
	}
};


// ======================================================================================================================================================
// ù��°�� Ȯ��� ����[���� ����][������ x��ǥ][������ y��ǥ] || ���� -> ���ϴ�, �»��, ����, ���ϴ�

float blockEX[5][2][4] = {
	{
		{210.0, 210.0, 280.0, 280.0},
		{370.0, 440.0, 440.0, 370.0}
	},
	{
		{280.0, 280.0, 350.0, 350.0},
		{370.0, 440.0, 440.0, 370.0}
	},
	{
		{350.0, 350.0, 420.0, 420.0},
		{370.0, 440.0, 440.0, 370.0}
	},
	{
		{420.0, 420.0, 490.0, 490.0},
		{370.0, 440.0, 440.0, 370.0},
	},
	{
		{490.0, 490.0, 560.0, 560.0},
		{370.0, 440.0, 440.0, 370.0},
	},
};


// ======================================================================================================================================================
// ù��°�� 1��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block1Arr1[4][2][4] = {
	{
		// block1Top
		{220.0, 220.0, 280.0, 270.0},
		{430.0, 440.0, 440.0, 430.0}
	},
	{
		// block1Right
		{270.0, 270.0, 280.0, 280.0},
		{380.0, 430.0, 430.0, 380.0}
	},
	{
		// block1Bottom
		{210.0, 220.0, 270.0, 270.0},
		{380.0, 380.0, 380.0, 370.0}
	},
	{
		// block1Left
		{210.0, 210.0, 220.0, 220.0},
		{380.0, 430.0, 430.0, 380.0}
	}
};

// ù��°�� 1��° Ȯ��� ����
float block1Ex1[2][4] = {
	{210.0, 210.0, 280.0, 280.0},
	{370.0, 440.0, 440.0, 370.0}
};


// ======================================================================================================================================================
// ù��°�� 2��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block2Arr1[4][2][4] = {
	{
		// block2Top
		{290.0, 290.0, 340.0, 340.0},
		{430.0, 440.0, 440.0, 430.0}
	},
	{
		// block2Right
		{340.0, 340.0, 350.0, 350.0},
		{380.0, 430.0, 430.0, 380.0}
	},
	{
		// block2Bottom
		{280.0, 290.0, 340.0, 340.0},
		{370.0, 380.0, 380.0, 370.0}
	},
	{
		// block2Left
		{280.0, 280.0, 290.0, 290.0},
		{380.0, 430.0, 430.0, 380.0}
	}
};

// ù��°�� 2��° Ȯ��� ����
float block2Ex1[2][4] = {
	{280.0, 280.0, 350.0, 350.0},
	{370.0, 440.0, 440.0, 370.0}
};


// ======================================================================================================================================================
// ù��°�� 3��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block3Arr1[4][2][4] = {
	{
		// block3Top
		{360.0, 360.0, 410.0, 410.0},
		{430.0, 440.0, 440.0, 430.0}
	},
	{
		// block3Right
		{410.0, 410.0, 420.0, 420.0},
		{380.0, 430.0, 430.0, 380.0}
	},
	{
		// block3Bottom
		{360.0, 360.0, 410.0, 410.0},
		{370.0, 380.0, 380.0, 370.0}
	},
	{
		// block3Left
		{350.0, 350.0, 360.0, 360.0},
		{380.0, 430.0, 430.0, 380.0}
	}
};

// ù��°�� 3��° Ȯ��� ����
float block3EX1[2][4] = {
	{280.0, 280.0, 350.0, 350.0},
	{370.0, 440.0, 440.0, 370.0}
};


// ======================================================================================================================================================
// ù��°�� 4��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block4Arr1[4][2][4] = {
	{
		// block4Top
		{430.0, 430.0, 480.0, 480.0},
		{430.0, 440.0, 440.0, 430.0}
	},
	{
		// block4Right
		{480.0, 480.0, 490.0, 490.0},
		{380.0, 430.0, 430.0, 380.0}
	},
	{
		// block4Bottom
		{430.0, 430.0, 480.0, 480.0},
		{370.0, 380.0, 380.0, 370.0}
	},
	{
		// block4Left
		{420.0, 420.0, 430.0, 430.0},
		{380.0, 430.0, 430.0, 380.0}
	}
};

// ù��°�� 4��° Ȯ��� ����
float block4EX1[2][4] = {
	{420.0, 420.0, 490.0, 490.0},
	{370.0, 440.0, 440.0, 370.0}
};


// ======================================================================================================================================================
// // ù��°�� 5��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block5Arr1[4][2][4] = {
	{
		// block5Top
		{500.0, 500.0, 550.0, 550.0},
		{430.0, 440.0, 440.0, 430.0}
	},
	{
		// block5Right
		{550.0, 550.0, 560.0, 560.0},
		{380.0, 430.0, 430.0, 380.0}
	},
	{
		// block5Bottom
		{500.0, 500.0, 550.0, 550.0},
		{370.0, 380.0, 380.0, 370.0}
	},
	{
		// block5Left
		{490.0, 490.0, 500.0, 500.0},
		{380.0, 430.0, 430.0, 380.0}
	}
};

// ù��°�� 5��° Ȯ��� ����
float block5EX1[2][4] = {
	{490.0, 490.0, 560.0, 560.0},
	{370.0, 440.0, 440.0, 370.0}
};

// ======================================================================================================================================================
// �ι�°�� ���� ��ǥ��[���� ����][������ x��ǥ][������ y��ǥ]
float blockArr2[5][2][4] = {
	{
		{220.0, 220.0, 270.0, 270.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		{290.0, 290.0, 340.0, 340.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		{360.0, 360.0, 410.0, 410.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		{430.0, 430.0, 480.0, 480.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		{500.0, 500.0, 550.0, 550.0},
		{450.0, 500.0, 500.0, 450.0},
	}
};

// ======================================================================================================================================================
// �ι�°�� Ȯ��� ����[���� ����][������ x��ǥ][������ y��ǥ] || ���� -> ���ϴ�, �»��, ����, ���ϴ�

float blockEX2[5][2][4] = {
	{
		{210.0, 210.0, 280.0, 280.0},
		{440.0, 510.0, 510.0, 440.0},
	},
	{
		{280.0, 280.0, 350.0, 350.0},
		{440.0, 510.0, 510.0, 440.0},
	},
	{
		{350.0, 350.0, 420.0, 420.0},
		{440.0, 510.0, 510.0, 440.0},
	},
	{
		{420.0, 420.0, 490.0, 490.0},
		{440.0, 510.0, 510.0, 440.0},
	},
	{
		{490.0, 490.0, 560.0, 560.0},
		{440.0, 510.0, 510.0, 440.0},
	},
};

// ======================================================================================================================================================
// �ι�°�� 1��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block1Arr2[4][2][4] = {
	{
		// block2Top
		{220.0, 220.0, 280.0, 270.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block2Right
		{270.0, 270.0, 280.0, 280.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block2Bottom
		{210.0, 220.0, 270.0, 270.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block2Left
		{210.0, 210.0, 220.0, 220.0},
		{450.0, 500.0, 500.0, 450.0},
	}
};

/*�ι�°�� 1��° Ȯ��� ����*/
float block1Ex2[2][4] = {
	{210.0, 210.0, 280.0, 280.0},
	{440.0, 510.0, 510.0, 440.0},
};

// ======================================================================================================================================================
// �ι�°�� 2��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block2Arr2[4][2][4] = {
	{
		// block2Top
		{290.0, 290.0, 340.0, 340.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block2Right
		{340.0, 340.0, 350.0, 350.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block2Bottom
		{280.0, 290.0, 340.0, 340.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block2Left
		{280.0, 280.0, 290.0, 290.0},
		{450.0, 500.0, 500.0, 450.0},
	}
};

// �ι�°�� 2��° Ȯ��� ����
float block2Ex2[2][4] = {
	{280.0, 280.0, 350.0, 350.0},
	{440.0, 510.0, 510.0, 440.0},
};

// ======================================================================================================================================================
// �ι�°�� 3��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block3Arr2[4][2][4] = {
	{
		// block3Top
		{360.0, 360.0, 410.0, 410.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block3Right
		{410.0, 410.0, 420.0, 420.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block3Bottom
		{360.0, 360.0, 410.0, 410.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block3Left
		{350.0, 350.0, 360.0, 360.0},
		{450.0, 500.0, 500.0, 450.0},
	}
};

// �ι�°�� 3��° Ȯ��� ����
float block3EX2[2][4] = {
	{280.0, 280.0, 350.0, 350.0},
	{440.0, 510.0, 510.0, 440.0},
};

// ======================================================================================================================================================
// �ι�°�� 4��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block4Arr2[4][2][4] = {
	{
		// block4Top
		{430.0, 430.0, 480.0, 480.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block4Right
		{480.0, 480.0, 490.0, 490.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block4Bottom
		{430.0, 430.0, 480.0, 480.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block4Left
		{420.0, 420.0, 430.0, 430.0},
		{450.0, 500.0, 500.0, 450.0},
	}
};

// �ι�°�� 4��° Ȯ��� ����
float block4EX2[2][4] = {
	{420.0, 420.0, 490.0, 490.0},
	{440.0, 510.0, 510.0, 440.0},
};

// ======================================================================================================================================================
// // �ι�°�� 5��° ���� [���� ����][������ x��ǥ][������ y��ǥ] Top, Right, Bottom, Left ������ �ִ� �簢��(���ϴ�, �»��, ����, ���ϴ�)
float block5Arr2[4][2][4] = {
	{
		// block5Top
		{500.0, 500.0, 550.0, 550.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block5Right
		{550.0, 550.0, 560.0, 560.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block5Bottom
		{500.0, 500.0, 550.0, 550.0},
		{450.0, 500.0, 500.0, 450.0},
	},
	{
		// block5Left
		{490.0, 490.0, 500.0, 500.0},
		{450.0, 500.0, 500.0, 450.0},
	}
};

// �ι�°�� 5��° Ȯ��� ����
float block5EX2[2][4] = {
	{490.0, 490.0, 560.0, 560.0},
	{440.0, 510.0, 510.0, 440.0},
};


// Ȯ��� �簢�� �׸���
void Modeling_blockEX() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(block3EX1[0][0], block3EX1[1][0]);
	glVertex2f(block3EX1[0][1], block3EX1[1][1]);

	glVertex2f(block3EX1[0][1], block3EX1[1][1]);
	glVertex2f(block3EX1[0][2], block3EX1[1][2]);

	glVertex2f(block3EX1[0][2], block3EX1[1][2]);
	glVertex2f(block3EX1[0][3], block3EX1[1][3]);

	glVertex2f(block3EX1[0][3], block3EX1[1][3]);
	glVertex2f(block3EX1[0][0], block3EX1[1][0]);
	glEnd();
}


// ���� ������ �浹������
void Collision_Detection_to_Brick(void) {
	Point vector1, vector2, vector3;
	float distanceP, distanceP2, distanceP3;

	// ù��°�� ���� �浹

	for (int i = 0; i < 5; i++) {
		// ù��°�� Ȯ��� ���� �浹 ���� Ȯ��
		if ((moving_ball.x >= blockEX[i][0][0] && moving_ball.x <= blockEX[i][0][2]) && (moving_ball.y >= blockEX[i][1][0] && moving_ball.y <= blockEX[i][1][1])) {

			// ù��°�� 1��° ����
			if (i == 0) {
				if (brick[0][8] == 1.0) {
					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ���� �𼭸�
							if (DotToDot(moving_ball.x, moving_ball.y, block1Ex1[0][i], block1Ex1[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ� �𼭸�
							if (DotToDot(moving_ball.x, moving_ball.y, block1Ex1[0][i], block1Ex1[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					/*���� ���� ���� ���� ���� �簢�� Top, Right, Bottom, Left*/
					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block1Arr1[i][0][0] && moving_ball.x <= block1Arr1[i][0][2] && moving_ball.y >= block1Arr1[i][1][0] && moving_ball.y <= block1Arr1[i][1][1]) {
							
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) {
								velocity.y = -velocity.y;
								/*velocity.y += 0.8;*/
							}

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else {
								velocity.x = -velocity.x;
								/*velocity.x += 0.8;*/
							}
						}
					}
					brick[0][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}

			// ù��°�� 2��° ����
			if (i == 1) {
				if (brick[1][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block2Ex1[0][i], block2Ex1[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block2Ex1[0][i], block2Ex1[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block2Arr1[i][0][0] && moving_ball.x <= block2Arr1[i][0][2] && moving_ball.y >= block2Arr1[i][1][0] && moving_ball.y <= block2Arr1[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.x = -velocity.x;
							/*velocity.y = -velocity.y;*/

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick[1][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}


			else if (i == 2) {
				// ù��°�� 3��° ���� (����� ���� -> �������� ��� ������)
				if (brick[2][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block3EX1[0][i], block3EX1[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
								/*velocity.y += 0.8;*/
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block3EX1[0][i], block3EX1[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
								/*velocity.x += 0.8;*/
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block3Arr1[i][0][0] && moving_ball.x <= block3Arr1[i][0][2] && moving_ball.y >= block3Arr1[i][1][0] && moving_ball.y <= block3Arr1[i][1][1]) {

							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) {
								velocity.x = -velocity.x;
								/*velocity.y = -velocity.y;*/
								/*velocity.y += 0.8;*/
							}

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else {
								velocity.x = -velocity.x;
								/*velocity.x += 0.8;*/
							}
						}
					}
					brick[2][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}

			}

			// ù��°�� 4��° ����
			else if (i == 3) {
				if (brick[3][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block4EX1[0][i], block4EX1[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block4EX1[0][i], block4EX1[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block4Arr1[i][0][0] && moving_ball.x <= block4Arr1[i][0][2] && moving_ball.y >= block4Arr1[i][1][0] && moving_ball.y <= block4Arr1[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.y = -velocity.y;

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick[3][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}

			// ù��°�� 5��° ����
			else if (i == 4) {

				if (brick[4][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block5EX1[0][i], block5EX1[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block5EX1[0][i], block5EX1[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block5Arr1[i][0][0] && moving_ball.x <= block5Arr1[i][0][2] && moving_ball.y >= block5Arr1[i][1][0] && moving_ball.y <= block5Arr1[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.y = -velocity.y;

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick[4][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}
		}
	}

	// �ι�°�� ���� �浹

	for (int i = 0; i < 5; i++) {
		// ù��°�� Ȯ��� ���� �浹 ���� Ȯ��
		if ((moving_ball.x >= blockEX2[i][0][0] && moving_ball.x <= blockEX2[i][0][2]) && (moving_ball.y >= blockEX2[i][1][0] && moving_ball.y <= blockEX2[i][1][1])) {

			// ù��°�� 1��° ����
			if (i == 0) {
				if (brick2[0][8] == 1.0) {
					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ���� �𼭸�
							if (DotToDot(moving_ball.x, moving_ball.y, block1Ex2[0][i], block1Ex2[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ� �𼭸�
							if (DotToDot(moving_ball.x, moving_ball.y, block1Ex2[0][i], block1Ex2[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					/*���� ���� ���� ���� ���� �簢�� Top, Right, Bottom, Left*/
					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block1Arr2[i][0][0] && moving_ball.x <= block1Arr2[i][0][2] && moving_ball.y >= block1Arr2[i][1][0] && moving_ball.y <= block1Arr2[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.y = -velocity.y;

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick2[0][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}

			// ù��°�� 2��° ����
			if (i == 1) {
				if (brick2[1][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block2Ex2[0][i], block2Ex2[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block2Ex2[0][i], block2Ex2[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block2Arr2[i][0][0] && moving_ball.x <= block2Arr2[i][0][2] && moving_ball.y >= block2Arr2[i][1][0] && moving_ball.y <= block2Arr2[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.y = -velocity.y;

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick2[1][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}


			else if (i == 2) {
				// ù��°�� 3��° ���� (����� ���� -> �������� ��� ������)
				if (brick2[2][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block3EX2[0][i], block3EX2[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
								/*velocity.y += 0.8;*/
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block3EX2[0][i], block3EX2[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
								/*velocity.x += 0.8;*/
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block3Arr2[i][0][0] && moving_ball.x <= block3Arr2[i][0][2] && moving_ball.y >= block3Arr2[i][1][0] && moving_ball.y <= block3Arr2[i][1][1]) {

							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) {
								velocity.x = -velocity.x; 

								/*velocity.y += 0.8;*/
							}

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else {
								velocity.x = -velocity.x;
								/*velocity.x += 0.8;*/
							}
						}
					}
					brick2[2][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}

			}

			// ù��°�� 4��° ����
			else if (i == 3) {
				if (brick2[3][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block4EX2[0][i], block4EX2[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block4EX2[0][i], block4EX2[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block4Arr2[i][0][0] && moving_ball.x <= block4Arr2[i][0][2] && moving_ball.y >= block4Arr2[i][1][0] && moving_ball.y <= block4Arr2[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.y = -velocity.y;

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick2[3][8] = 0.0;
					brickCollisionCnt++;
				}
			}

			// ù��°�� 5��° ����
			else if (i == 4) {

				if (brick2[4][8] == 1.0) {

					for (int i = 0; i < 4; i++) {
						if (i % 2 == 0) {
							// ���ϴ�, ����
							if (DotToDot(moving_ball.x, moving_ball.y, block5EX2[0][i], block5EX2[1][i]) < moving_ball_radius) {
								velocity.y = -velocity.y;
							}
						}
						else {
							// �»��, ���ϴ�
							if (DotToDot(moving_ball.x, moving_ball.y, block5EX2[0][i], block5EX2[1][i]) < moving_ball_radius) {
								velocity.x = -velocity.x;
							}
						}
					}

					for (int i = 0; i < 4; i++) {
						if (moving_ball.x >= block5Arr2[i][0][0] && moving_ball.x <= block5Arr2[i][0][2] && moving_ball.y >= block5Arr2[i][1][0] && moving_ball.y <= block5Arr2[i][1][1]) {
							// Top �簢�� �ȿ� �ִ� ���
							if (i == 0) velocity.y = -velocity.y;

							// Right, Bottom, Left �簢�� �ȿ� �ִ� ���
							else velocity.x = -velocity.x;
						}
					}
					brick2[4][8] = 0.0;
					brickCollisionCnt++;
					printf("���� �浹 ���� : %f\n", brickCollisionCnt);
				}
			}
		}
	}
}


// ���� �е鿡 �浹������
void Collision_Detection_to_Paddle(void) {

	// �е� �浹 ����
	float Pdistance1, Pdistance2, Pdistance3, Scalar;
	Point vector1, vector2, vector3; vector1.x, vector1.y, vector2.x, vector2.y, vector3.x, vector3.y = 0.0;

	// �е� ��ǥ �� �浹
	if (LinePaddle(moving_ball.x, moving_ball.y) < moving_ball_radius) {
		if (moving_ball.x >= Paddle[0] && moving_ball.x <= Paddle[6]) {

			// ���⺤��
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

		printf("���� x : %f\n", velocity.x);
		printf("���� y : %f\n", velocity.y);

		// ���� ����ȭ(������ ���̸� 1�� ����� ��)
		// ������ �� ������ ������ ũ��� ������ ��
		// ���� �� ������ �Ÿ�(ũ��)
		// ���⺤��(������ ������� �߾� -> �浹 ����)

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;

		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// �������� : ����(400, 800) , ����(0, 400)
		Vector2.x = 0.0 - (width / 2);
		Vector2.y = (width / 2) - width;


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		// �ٲ� ����
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);

		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);


		// ����ȭ
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.x = Vector3.x;
		velocity.y = Vector3.y;


		//// ���ο� ���(�е��� �浹 ���)
		//Vector2.x = Vector1.x - (2 * Vector1.x);
		//Vector2.y = Vector1.y;


		//velocity.x = Vector2.x;
		//velocity.y = Vector2.y;
	}


	// ���ϴ�
	else if (RightUp > width - moving_ball_radius) {

		//printf("���ϴ� x : %f\n", velocity.x);
		//printf("���ϴ� y : %f\n", velocity.y);

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;


		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// �������� : ���� (0, 400) , ���� (400, 800)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = width - (width / 2);


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		// �ٲ� ����
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);


		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);

		// ����ȭ
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// ����� �»�� �� �浹
	if (LeftUp < moving_ball_radius) {

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;

		printf("�»�� x : %f\n", velocity.x);
		printf("�»�� y : %f\n", velocity.y);

		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// �������� : ���� (0, 400) , ���� (400, 0)
		Vector2.x = (width / 2) - 0.0;
		Vector2.y = 0.0 - (width / 2);


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);

		// �ٲ� ����
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);

		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);


		// ����ȭ
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}


	// ����� ���ϴ� �浹
	else if (LeftUp > width - moving_ball_radius) {

		Vector1.x = velocity.x;
		Vector1.y = velocity.y;


		// ����ȭ
		Vector1.x = Vector1.x / normalize(Vector1.x, Vector1.y);
		Vector1.y = Vector1.y / normalize(Vector1.x, Vector1.y);


		// �������� : ����(800, 400) , ����(400, 800)
		Vector2.x = (width / 2) - width;
		Vector2.y = width - (width / 2);


		// ����ȭ
		Vector2.x = Vector2.x / normalize(Vector2.x, Vector2.y);
		Vector2.y = Vector2.y / normalize(Vector2.x, Vector2.y);


		// �ٲ� ����
		Vector3.x = Vector1.x + 2 * Vector2.x * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);
		Vector3.y = Vector1.y + 2 * Vector2.y * (-Vector1.x * Vector2.x + -Vector1.y * Vector2.y);


		//Vector3.x = Vector1.x + 2 * (Vector2.x);
		//Vector3.y = Vector1.y + 2 * (Vector2.y);


		// ����ȭ
		Vector3.x = Vector3.x / normalize(Vector3.x, Vector3.y);
		Vector3.y = Vector3.y / normalize(Vector3.x, Vector3.y);


		velocity.y = Vector3.y;
		velocity.x = Vector3.x;
	}
}


// �����ϴ� �Լ�
void RenderScene(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to Yellow
	glClear(GL_COLOR_BUFFER_BIT);


	Modeling_NameBox();
	Draw_NameText();
	Draw_GameText();

	Modeling_GameGround();
	Modeling_brick();
	Modeling_brick2();
	Modeling_Paddle();
	

	/*Modeling_blockEX();*/


	// �����̴� ���� ��ġ ��ȭ
	moving_ball.x += velocity.x * speed;
	moving_ball.y += velocity.y * speed;


	// �浹 ó�� �κ�
	Collision_Detection_to_Brick(); // ���� ������ �浹 �Լ�
	Collision_Detection_to_Walls(); // ���� ���� �浹 �Լ�
	Collision_Detection_to_Paddle(); // ���� �е��� �浹 �Լ�

	// �����̴� �� �׸��� (īŰ��)
	/*glColor3f(0.741176, 0.717647, 0.419608);*/
	if (brickCollisionCnt == 10) {
		velocity.x = 0;
		velocity.y = 0;
		Draw_SUCCESS();
	}
	glColor3f(0.941176, 0.901961, 0.54902);
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
		if (Paddle[0] <= 280.0) {
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
		if (Paddle[0] >= 420.0) {
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