#include <stdio.h>
#include <conio.h> // _kbhit, _getch 사용
#include <stdlib.h> // syscls 사용
#include <Windows.h> // Sleep() 사용

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
* 상(오른쪽으로 회전)	 72
* 하(소프트 드랍)	 80
* 좌	(왼쪽으로 블럭 이동) 75
* 우(오른쪽으로 블럭 이동) 77
* 스페이스(하드 드랍) 32
* z(왼쪽으로 회전) 122
* x(오른쪽으로 회전) 120
* c(홀드) 99
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

/* 선언용 함수 */
void drawField();
void initField();
void game();
void input();
void clearScreen(int milliseconds);
void update();

int getInput();

void moveBlock(int dir);
void forwardRotateBlock(int block[4][4]);
void reverseRotateBlock(int block[4][4]);
void insertBlock();

/* 여기까지 선언용 함수*/

/* 기본 세팅 */
int startPosX = 4;
int startPosY = 0;

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
	drawField(); // 필드 그리기

	input(); // 입력 설정 관련
	update(); // 게임 관련 업데이트 등

	clearScreen(100); // 화면 지우기
}

void input() {
	int key = getInput();
	int direction = 0;

	switch (key) {
	case KEY_UP:
		printf("key : UP");
		break;
	case KEY_DOWN:
		printf("key : DOWN");
		break;
	case KEY_LEFT:
		direction = -1;
		moveBlock(direction);
		printf("key : LEFT");
		break;
	case KEY_RIGHT:
		direction = 1;
		moveBlock(direction);
		printf("key : RIGHT");
		break;
	case KEY_SPACE:
		printf("key : SPACE");
		break;
	case KEY_Z:
		printf("key : Z");
		break;
	case KEY_X:
		printf("key : X");
		break;
	case KEY_C:
		printf("key : C");
		break;
	default:
		break;
	}
}

void update() {
	insertBlock();
}

void drawField() { // 필드 그리기
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (field[y][x] == EMPTY) {
				printf("  ");
			}
			else if (field[y][x] == BLOCK) {
				printf("■");
			}
			else if (field[y][x] == WALL) {
				printf("▣");
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

void forwardRotateBlock(int block[4][4]) { // 정방향 회전(90도, 오른쪽 회전)
	int temp[4][4]; // 임시 배열

	// 90도 회전
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp[j][3 - i] = block[i][j];
		}
	}

	// block에 복사
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = temp[i][j];
		}
	}
}

void reverseRotateBlock(int block[4][4]) { // 역방향 회전 (-90도, 왼쪽 회전)
	int temp[4][4]; // 임시 배열

	// -90도 회전
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp[3 - i][j] = block[i][j];
		}
	}

	// block에 복사
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = temp[i][j];
		}
	}
}

void insertBlock() {
	int testBlock = 0; // I 블럭으로 고정

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			field[y + startPosY][x + startPosX] = blocks[testBlock][y][x];
		}
	}
}

void moveBlock(int dir) {
	
}