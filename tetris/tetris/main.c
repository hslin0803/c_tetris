#include <stdio.h>
#include <conio.h> // _kbhit, _getch 사용
#include <stdlib.h> // syscls 사용
#include <Windows.h> // Sleep() 사용

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

#define EMPTY 0
#define BLOCK 1
#define WALL 2

int field[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

int isGameOver = 0;

/* 선언용 함수 */
void drawField();
void initField();
void game();
void input();
void clearScreen(int milliseconds);
void update();

int getInput();
/* 여기까지 선언용 함수*/

int main() {
	initGame();

	while (!isGameOver)
	{
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

	clearScreen(300); // 화면 지우기
}

void input() {
	int ch = getInput();
	printf("key : %d", ch);
}

void update() {

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
		field[y][FIELD_WIDTH-1] = WALL;
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