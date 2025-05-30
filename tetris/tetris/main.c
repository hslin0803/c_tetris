#include <stdio.h>
#include <conio.h> // _kbhit, _getch ���
#include <stdlib.h> // syscls ���
#include <Windows.h> // Sleep() ���

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 22

#define EMPTY 0
#define BLOCK 1
#define FIXED_BLOCK 2
#define WALL 3
#define GHOST_BLOCK 4

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_Z 122
#define KEY_X 120
#define KEY_C 99

#define NEXT_SIZE 3

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

int nextQueue[14]; // 7���� 2��Ʈ
int queueIndex = 0;
int queueCount = 0;

int isGameOver = 0;
int isBlockSpawn = 0; // ���� �����Ǹ� 1, �ƴϸ� 0
int isStop = 0; // �����ϼ� ������ 0, �ƴҰ�� 1
int isHold = 0; // Ȧ�� ������ �� 0, Ȧ�带 �̹� ���� ��� 1

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
int holdBlock = -1;

/* ����� �Լ� */
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
void insertGhostBlock();
void hardDrop();
void addQueueBag();
void drawNextBlocks();
void changeHold();
void drawHoldBlock();

int getNextBlock();

int initGame();

int getInput();

void moveBlock(int dirX, int dirY);
void clearBlock();
void forwardRotateBlock(int block[4][4]);
void reverseRotateBlock(int block[4][4]);
int checkWall(int dirX, int dirY);
void gotoxy(int x, int y);
void hideCursor();

/* ������� ����� �Լ� */

/* ���� ��ġ */
int prevPosX = 0;
int prevPosY = 0;

/* ���� ��ġ */
int nowPosX = 4;
int nowPosY = 0;

// ��Ʈ ��ġ
int ghostY;
int ghostX;

int gravityCounter = 0;
int dropInterval = 20; // 20�����Ӹ��� ����

int main() {
	hideCursor(); // �ܼ� Ŀ�� �����
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
	clearBlock(); // �� �����
	spawnBlock(); // �� ����
	insertGhostBlock(); // ��Ʈ�� ����
	moveBlock(0, 0);
	drawField(); // �ʵ� �׸���
	drawNextBlocks(); // �ؽ�Ʈ ���
	drawHoldBlock(); // Ȧ�� �� ���
	input(); // �Է� ���� ����

	gravityCounter++;
	if (gravityCounter >= dropInterval) {
		fallBlock();
		gravityCounter = 0;
	}
	checkLine();

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
		forwardRotateBlock(nowBlocks); // �� ȸ��
		printf("key : UP");
		break;
	case KEY_DOWN:
		dirY = 1;
		isWall = checkWall(dirX, dirY); // �� �浹 Ȯ��
		if (isWall == 1) {
			moveBlock(dirX, dirY); // �� �̵�
			printf("key : DOWN");
		}
		break;
	case KEY_LEFT:
		dirX = -1;
		isWall = checkWall(dirX, dirY); // �� �浹 Ȯ��
		if (isWall == 1) {
			moveBlock(dirX, dirY); // �� �̵�
			printf("key : LEFT");
		}
		break;
	case KEY_RIGHT:
		dirX = 1;
		isWall = checkWall(dirX, dirY); // �� �浹 Ȯ��
		if (isWall == 1) {
			moveBlock(dirX, dirY); // �� �̵�
			printf("key : RIGHT");
		}
		break;
	case KEY_SPACE:
		hardDrop();
		printf("key : SPACE");
		break;
	case KEY_Z:
		reverseRotateBlock(nowBlocks); // �� ȸ��
		printf("key : Z");
		break;
	case KEY_X:
		forwardRotateBlock(nowBlocks); // �� ȸ��
		printf("key : X");
		break;
	case KEY_C:
		changeHold(); // Ȧ��
		printf("key : C");
		break;
	default:
		break;
	}
	//printf("\nnowPosX = %d, nowPosY = %d\nprevPosX = %d, prevPosY = %d", nowPosX, nowPosY, prevPosX, prevPosY);
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
			else if (field[y][x] == FIXED_BLOCK) {
				printf("��");
			}
			else if (field[y][x] == GHOST_BLOCK) {
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
	/*Sleep(milliseconds);
	system("cls");*/
	gotoxy(0, 0);
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
			temp[3 - j][i] = block[i][j];
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
			if (nowBlocks[y][x] == BLOCK) {
				field[y + nowPosY][x + nowPosX] = BLOCK;
			}
		}
	}
}

void clearBlock() { // ���� ��, ��Ʈ �� �����
	for (int y = 0; y < FIELD_HEIGHT;y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (field[y][x] == BLOCK || field[y][x] == GHOST_BLOCK) {
				field[y][x] = EMPTY;
			}
		}
	}
}

int checkWall(int dirX, int dirY) { // �̵��ϱ��� ���� ��ġ ������ Ȯ��
	int nextPosX = nowPosX + dirX;
	int nextPosY = nowPosY + dirY;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (nowBlocks[y][x] == BLOCK) {
				int fieldX = nextPosX + x;
				int fieldY = nextPosY + y;

				// �迭 ���� �ʰ� ����
				if (fieldY >= FIELD_HEIGHT || fieldY < 0 || fieldX >= FIELD_WIDTH || fieldX < 0) {
					printf("\n �迭 ���� �ʰ�! (%d, %d)\n", fieldX, fieldY);
					return -1;
				}

				if (field[fieldY][fieldX] == WALL || field[fieldY][fieldX] == FIXED_BLOCK) {
					printf("\n �Ұ���! (%d, %d)\n", fieldX, fieldY);
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
		moveBlock(dirX, dirY);
	}
	else {
		changeFixedBlock();
		isHold = 0;
		printf("\n �� ����! \n");
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
	if (isBlockSpawn == 0) { // 0�� �ƴϸ� �� ���� �� ī��
		nowBlock = getNextBlock();
		//nowBlock = rand() % 7; // �ӽ÷� ����(���� �˰��� ���ؼ� tetrioó�� �ؽ�Ʈ �۾� ����)
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
					// �� ������ �� �Ʒ����� �̵�
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

void insertGhostBlock() {
	int minDrop = FIELD_HEIGHT;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (nowBlocks[y][x] == BLOCK) {
				int startY = nowPosY + y;
				int fieldX = nowPosX + x;
				int drop = 0;

				while (1) {
					int newY = startY + drop + 1;
					if (newY >= FIELD_HEIGHT || field[newY][fieldX] == WALL || field[newY][fieldX] == FIXED_BLOCK) {
						break;
					}
					drop++;
				}

				if (drop < minDrop) {
					minDrop = drop;
				}
			}
		}
	}

	// ��Ʈ �� ���
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (nowBlocks[y][x] == BLOCK) {
				int ghostY = nowPosY + y + minDrop;
				int ghostX = nowPosX + x;

				if (field[ghostY][ghostX] == EMPTY) {
					field[ghostY][ghostX] = GHOST_BLOCK;
				}
			}
		}
	}
}

void hardDrop() {
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (field[y][x] == GHOST_BLOCK) {
				field[y][x] = FIXED_BLOCK;
			}
		}
	}

	isBlockSpawn = 0;
	isStop = 0;
	isHold = 0;
}

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);   // ���� Ŀ�� ���� ��������
	cursorInfo.bVisible = FALSE;                   // Ŀ�� �� ���̰� ����
	SetConsoleCursorInfo(hConsole, &cursorInfo);   // ���� ����
}

void addQueueBag() {
	int temp[7];

	// �ʱ� ������ ������ ���� �ʱ�ȭ
	for (int i = 0; i < 7; i++) {
		temp[i] = i;
	}

	// ������ �̿��ؼ� ����
	for (int i = 6; i > 0; i--) {
		int j = rand() % (i + 1);
		int tmp = temp[i];
		temp[i] = temp[j];
		temp[j] = tmp;
	}

	// nextQueue�� �߰�
	for (int i = 0; i < 7; i++) {
		nextQueue[queueIndex + queueCount + i] = temp[i];
	}
	queueCount += 7;
}

int getNextBlock() {
	// �ؽ�Ʈ ������ �Ѽ�Ʈ �߰�
	if (queueCount < NEXT_SIZE + 1) {
		addQueueBag();
	}

	int block = nextQueue[queueIndex++];
	queueCount--;

	return block;
}

void drawNextBlocks() {
	int baseX = 25; // �ʵ� ������ ���� ǥ��
	int baseY = 2;  // Y ��ġ�� ���� (�ʿ�� ����)

	gotoxy(baseX, baseY - 1);
	printf("[ Next ]");

	for (int i = 0; i < NEXT_SIZE; i++) {
		int blockType = nextQueue[queueIndex + i];

		for (int y = 0; y < 4; y++) {
			gotoxy(baseX, baseY + i * 5 + y); // �� �� ���� 5��
			for (int x = 0; x < 4; x++) {
				if (blocks[blockType][y][x] == BLOCK)
					printf("��");
				else
					printf("  ");
			}
		}
	}
}

void changeHold() {
	if (isHold == 0) {
		if (holdBlock == -1) {
			holdBlock = nowBlock;

			isBlockSpawn = 0;
			isStop = 0;
			isHold = 1;
		}
		else if (holdBlock != -1) {
			int temp;

			temp = holdBlock;
			holdBlock = nowBlock;
			nowBlock = temp;

			nowPosX = 4;
			nowPosY = 0;
			copyBlock();
			isStop = 0;
			isBlockSpawn = 1;
			isHold = 1;
		}
	}
}

void drawHoldBlock() {
	int baseX = 40; // �ʵ� ������ ���� ǥ��
	int baseY = 2;  // Y ��ġ�� ���� (�ʿ�� ����)

	gotoxy(baseX, baseY - 1);
	printf("[ Hold ]");

	int blockType = holdBlock;

	if (blockType != -1) {
		for (int y = 0; y < 4; y++) {
			gotoxy(baseX, baseY + y); // �� �� ���� 5��
			for (int x = 0; x < 4; x++) {
				if (blocks[blockType][y][x] == BLOCK)
					printf("��");
				else
					printf("  ");
			}
		}
	}
}