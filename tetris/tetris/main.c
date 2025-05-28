#include <stdio.h>
#include <conio.h> // _kbhit, _getch 사용
#include <stdlib.h> // syscls 사용
#include <Windows.h> // Sleep() 사용

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

#define EMPTY 0
#define BLOCK 1
#define FIXED_BLOCK 2
#define WALL 3

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
int isBlockSpawn = 0; // 블럭이 스폰되면 1, 아니면 0
int isStop = 0; // 움직일수 있으면 0, 아닐경우 1

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
int nowBlocks[4][4];

int nowBlock;

/* 선언용 함수 */
void drawField();
void initField();
void game();
void input();
void clearScreen(int milliseconds);
void fallBlock();
void changeFixedBlock();
void copyBlock();
void spawnBlock();
void checkLine();
void deleteLine(int lineY);
void moveLine(int lineY);

int initGame();

int getInput();

void moveBlock(int dirX, int dirY);
void clearBlock(int prevPosX, int prevPosY);
void forwardRotateBlock(int block[4][4]);
void reverseRotateBlock(int block[4][4]);
int checkWall(int dirX, int dirY);

/* 여기까지 선언용 함수 */

/* 이전 위치 */
int prevPosX = 0;
int prevPosY = 0;

/* 현재 위치 */
int nowPosX = 4;
int nowPosY = 0;

int gravityCounter = 0;
int dropInterval = 20; // 20프레임마다 낙하

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
	spawnBlock(); // 블럭 스폰

	input(); // 입력 설정 관련

	gravityCounter++;
	if (gravityCounter >= dropInterval) {
		fallBlock();
		gravityCounter = 0;
	}
	checkLine();

	clearScreen(100); // 화면 지우기
}

void input() {
	int key = getInput();

	int dirX = 0;
	int dirY = 0;

	int isWall = 0;

	// 이전위치 저장
	prevPosX = nowPosX;
	prevPosY = nowPosY;

	switch (key) {
	case KEY_UP:
		clearBlock(prevPosX, prevPosY); // 이전위치 지우기
		forwardRotateBlock(nowBlocks); // 블럭 회전
		printf("key : UP");
		break;
	case KEY_DOWN:
		dirY = 1;
		isWall = checkWall(dirX, dirY); // 벽 충돌 확인
		if (isWall == 1) {
			clearBlock(prevPosX, prevPosY); // 이전위치 지우기
			moveBlock(dirX, dirY); // 블럭 이동
			printf("key : DOWN");
		}
		break;
	case KEY_LEFT:
		dirX = -1;
		isWall = checkWall(dirX, dirY); // 벽 충돌 확인
		if (isWall == 1) {
			clearBlock(prevPosX, prevPosY); // 이전위치 지우기
			moveBlock(dirX, dirY); // 블럭 이동
			printf("key : LEFT");
		}
		break;
	case KEY_RIGHT:
		dirX = 1;
		isWall = checkWall(dirX, dirY); // 벽 충돌 확인
		if (isWall == 1) {
			clearBlock(prevPosX, prevPosY); // 이전위치 지우기
			moveBlock(dirX, dirY); // 블럭 이동
			printf("key : RIGHT");
		}
		break;
	case KEY_SPACE:
		printf("key : SPACE");
		break;
	case KEY_Z:
		clearBlock(prevPosX, prevPosY); // 이전위치 지우기
		reverseRotateBlock(nowBlocks); // 블럭 회전
		printf("key : Z");
		break;
	case KEY_X:
		clearBlock(prevPosX, prevPosY); // 이전위치 지우기
		forwardRotateBlock(nowBlocks); // 블럭 회전
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
			else if (field[y][x] == FIXED_BLOCK) {
				printf("♠");
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
			temp[3 - j][i] = block[i][j];
		}
	}

	// block에 복사
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = temp[i][j];
		}
	}
}

void moveBlock(int dirX, int dirY) {
	// 현재 위치 갱신
	nowPosX += dirX;
	nowPosY += dirY;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (nowBlocks[y][x] == BLOCK) {
				field[y + nowPosY][x + nowPosX] = BLOCK;
			}
		}
	}
}

void clearBlock(int prevPosX, int prevPosY) { // 이전 위치 블럭 지우기
	for (int y = 0; y < 4;y++) {
		for (int x = 0; x < 4; x++) {
			if (nowBlocks[y][x] == BLOCK) {
				if (field[y + prevPosY][x + prevPosX] == BLOCK) {
					field[y + prevPosY][x + prevPosX] = EMPTY;
				}
			}
		}
	}
}

int checkWall(int dirX, int dirY) { // 이동하기전 다음 위치 벽인지 확인
	int nextPosX = nowPosX + dirX;
	int nextPosY = nowPosY + dirY;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (nowBlocks[y][x] == BLOCK) {
				int fieldX = nextPosX + x;
				int fieldY = nextPosY + y;

				// 배열 범위 초과 방지
				if (fieldY >= FIELD_HEIGHT || fieldY < 0 || fieldX >= FIELD_WIDTH || fieldX < 0) {
					printf("\n 배열 범위 초과! (%d, %d)\n", fieldX, fieldY);
					return -1;
				}

				if (field[fieldY][fieldX] == WALL || field[fieldY][fieldX] == FIXED_BLOCK) {
					printf("\n 불가능! (%d, %d)\n", fieldX, fieldY);
					return -1;
				}
			}
		}
	}
	return 1;
}

void fallBlock() {
	int dirX = 0;
	int dirY = 1;

	if (checkWall(dirX, dirY) == 1) {
		clearBlock(nowPosX, nowPosY);
		moveBlock(dirX, dirY);
	}
	else {
		changeFixedBlock();
		printf("\n 블럭 고정! \n");
	}
}

void changeFixedBlock() {
	if (isStop == 1) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				if (nowBlocks[y][x] == BLOCK) {
					field[y + nowPosY][x + nowPosX] = FIXED_BLOCK;
					isStop = 0;
					isBlockSpawn = 0;
				}
			}
		}
	}
}

void copyBlock() {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			nowBlocks[y][x] = blocks[nowBlock][y][x];
		}
	}
}

void spawnBlock() {
	if (isBlockSpawn == 0) { // 0이 아니면 블럭 생성 후 카피
		nowBlock = rand() % 7; // 임시로 세팅(추후 알고리즘 통해서 tetrio처럼 넥스트 작업 예정)
		nowPosX = 4;
		nowPosY = 0;
		copyBlock();

		isStop = 0;
		isBlockSpawn = 1;
	}
	else {
		isBlockSpawn = 1;
		isStop = 1;
		return;
	}
}

void checkLine() {
	for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
		int blockCount = 0;
		for (int x = 1; x < FIELD_WIDTH - 1; x++) {
			if (field[y][x] == FIXED_BLOCK) {
				blockCount++;
				if (blockCount == 10) {
					// 줄 삭제후 그 아래까지 이동
					deleteLine(y);
					moveLine(y);
				}
			}
		}
	}
}

void deleteLine(int lineY) {
	for (int x = 1; x < FIELD_WIDTH - 1; x++) {
		field[lineY][x] = EMPTY;
	}
}

void moveLine(int lineY) {
	for (int y = lineY; y > 0; y--) {
		for (int x = 0;x < FIELD_WIDTH - 1; x++) {
			field[y][x] = field[y - 1][x];
		}
	}

	for (int x = 1; x < FIELD_WIDTH - 1; x++) {
		field[0][x] = EMPTY;
	}
}