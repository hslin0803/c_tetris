#include <stdio.h>
#include <conio.h> // _kbhit, _getch ���
#include <stdlib.h> // syscls ���
#include <Windows.h> // Sleep() ���

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

#define EMPTY 0
#define BLOCK 1
#define WALL 2

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_Z 122
#define KEY_X 120
#define KEY_C 99

/*
* ��(���������� ȸ��)	 72
* ��(����Ʈ ���)	 80
* ��	(�������� �� �̵�) 75
* ��(���������� �� �̵�) 77
* �����̽�(�ϵ� ���) 32
* z(�������� ȸ��) 122
* x(���������� ȸ��) 120
* c(Ȧ��) 99
*/

int field[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

int isGameOver = 0;

/* Block */
int blocks[7][4][4] = {
	// BLOCK_I
	{
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	// BLOCK_O
	{
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	// BLOCK_T
	{
		{0, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	// BLOCK_S
	{
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	// BLOCK_Z
	{
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	// BLOCK_J
	{
		{1, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	// BLOCK_L
	{
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	}
};

/* ����� �Լ� */
void drawField();
void initField();
void game();
void input();
void clearScreen(int milliseconds);
void update();
int initGame();

int getInput();

void moveBlock(int dirX, int dirY);
void clearBlock(int prevPosX, int prevPosY);
void forwardRotateBlock(int block[4][4]);
void reverseRotateBlock(int block[4][4]);
int checkWall(int dirX, int dirY);

/* ������� ����� �Լ�*/

/* ���� ��ġ */
int prevPosX = 0;
int prevPosY = 0;

/* ���� ��ġ */
int nowPosX = 4;
int nowPosY = 0;

int testBlock = 0; // I ������ ����

int main() {
	initGame();

	while (!isGameOver) {
		game();
	}

	return 0;
}

int initGame() {
	initField();

	return 0;
}

void game() {
	drawField(); // �ʵ� �׸���

	input(); // �Է� ���� ����
	update(); // ���� ���� ������Ʈ ��

	clearScreen(100); // ȭ�� �����
}

void input() {
	int key = getInput();

	int dirX = 0;
	int dirY = 0;

	int isWall = 0;

	// ������ġ ����
	prevPosX = nowPosX;
	prevPosY = nowPosY;

	switch (key) {
	case KEY_UP:
		// forwardRotateBlock(); // �� ȸ��
		printf("key : UP");
		break;
	case KEY_DOWN:
		dirY = 1;
		isWall = checkWall(dirX, dirY); // �� �浹 Ȯ��
		if (isWall == 1) {
			clearBlock(prevPosX, prevPosY); // ������ġ �����
			moveBlock(dirX, dirY); // �� �̵�
			printf("key : DOWN");
		}
		break;
	case KEY_LEFT:
		dirX = -1;
		isWall = checkWall(dirX, dirY); // �� �浹 Ȯ��
		if (isWall == 1) {
			clearBlock(prevPosX, prevPosY); // ������ġ �����
			moveBlock(dirX, dirY); // �� �̵�
			printf("key : LEFT");
		}
		break;
	case KEY_RIGHT:
		dirX = 1;
		isWall = checkWall(dirX, dirY); // �� �浹 Ȯ��
		if (isWall == 1) {
			clearBlock(prevPosX, prevPosY); // ������ġ �����
			moveBlock(dirX, dirY); // �� �̵�
			printf("key : RIGHT");
		}
		break;
	case KEY_SPACE:
		printf("key : SPACE");
		break;
	case KEY_Z:
		// forwardRotateBlock(); // �� ȸ��
		printf("key : Z");
		break;
	case KEY_X:
		// forwardRotateBlock(); // �� ȸ��
		printf("key : X");
		break;
	case KEY_C:
		printf("key : C");
		break;
	default:
		break;
	}
	printf("\nnowPosX = %d, nowPosY = %d\nprevPosX = %d, prevPosY = %d", nowPosX, nowPosY, prevPosX, prevPosY);
}

void update() {

}

void drawField() { // �ʵ� �׸���
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (field[y][x] == EMPTY) {
				printf("  ");
			}
			else if (field[y][x] == BLOCK) {
				printf("��");
			}
			else if (field[y][x] == WALL) {
				printf("��");
			}
		}
		printf("\n");
	}
}

void initField() {
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		field[y][0] = WALL;
		field[y][FIELD_WIDTH - 1] = WALL;
	}

	for (int x = 0; x < FIELD_WIDTH; x++) {
		field[FIELD_HEIGHT - 1][x] = WALL;
	}
}

int getInput() {
	if (_kbhit()) {
		int ch = _getch();

		if (ch == 0 || ch == 224) {
			ch = _getch();
		}
		return ch;
	}
	return -1;
}

void clearScreen(int milliseconds) {
	Sleep(milliseconds);
	system("cls");
}

void forwardRotateBlock(int block[4][4]) { // ������ ȸ��(90��, ������ ȸ��)
	int temp[4][4]; // �ӽ� �迭

	// 90�� ȸ��
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp[j][3 - i] = block[i][j];
		}
	}

	// block�� ����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = temp[i][j];
		}
	}
}

void reverseRotateBlock(int block[4][4]) { // ������ ȸ�� (-90��, ���� ȸ��)
	int temp[4][4]; // �ӽ� �迭

	// -90�� ȸ��
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp[3 - i][j] = block[i][j];
		}
	}

	// block�� ����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = temp[i][j];
		}
	}
}

void moveBlock(int dirX, int dirY) {
	// ���� ��ġ ����
	nowPosX += dirX;
	nowPosY += dirY;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (blocks[testBlock][y][x] == BLOCK) {
				field[y + nowPosY][x + nowPosX] = BLOCK;
			}
		}
	}
}

void clearBlock(int prevPosX, int prevPosY) { // ���� ��ġ �� �����
	for (int y = 0; y < 4;y++) {
		for (int x = 0; x < 4; x++) {
			if (blocks[testBlock][y][x] == BLOCK) {
				if (field[y + prevPosY][x + prevPosX] == BLOCK) {
					field[y + prevPosY][x + prevPosX] = EMPTY;
				}
			}
		}
	}
}

int checkWall(int dirX, int dirY) { // �̵��ϱ��� ���� ��ġ ������ Ȯ��
	int nextPosX = nowPosX + dirX;
	int nextPosY = nowPosY + dirY;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (blocks[testBlock][y][x] == BLOCK) {
				int fieldX = nextPosX + x;
				int fieldY = nextPosY + y;

				// �迭 ���� �ʰ� ����
				if (fieldY >= FIELD_HEIGHT || fieldY < 0 || fieldX >= FIELD_WIDTH || fieldX < 0) {
					printf("\n �迭 ���� �ʰ�! (%d, %d)\n", fieldX, fieldY);
					return -1;
				}

				if (field[fieldY][fieldX] == WALL) {
					printf("\n �Ұ���! (%d, %d)\n", fieldX, fieldY);
					return -1;
				}
			}
		}
	}
	return 1;
}