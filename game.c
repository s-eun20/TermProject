#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>

// ���� ����
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define MAXENEMYBULLET 10
#define MAXENEMY 8
#define MAXENEMY2 1
#define MAXBULLET 10
#define MAXBALL 20
#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.
#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define Space 32
#define WIDTH 90
#define HEIGHT 50
#define UX 30  //���� ������ x��ǥ
#define UY 45  //���� ������ y��ǥ
#define ITEM "<H>"
#define LIFEITEM "<L>"

int stagecount = 0;

int items[WIDTH][HEIGHT] = { 0 };
int items_count = 10;
int items_use = 5; //������ ��Ÿ���� �ð�(��� ������)


int Lifeitems[WIDTH][HEIGHT] = { 0 };
int Lifeitems_count = 10;
int Lifeitems_use = 5; //������ ��Ÿ���� �ð�(���ǵ� ������)

int called = 0;

int oldx = UX, oldy = UY; // �÷��̾��� old ��ǥ
int newx = UX, newy = UY; //�÷��̾��� new ��ǥ
int keep_moving = 1;  //1:����̵�
int heart = 3 , score, square = 3;

struct {
	int exist;
	int x, y;
}Bullet[MAXBULLET];

struct {
	int exist;
	int x, y;
	int number;
	int move;
	int type;
}Enemy[MAXENEMY];


struct {
	int exist;
	int x, y;
}EnemyBullet[MAXENEMYBULLET];

char* Enemyunit[] = { "#*#","��@��","���Ģ�","��&��", "Bo$$"};
int nextstage[20];

int fx; //���� ��ǥ
int bx, by;
int mx = -1; my;
int Score;
int Delay = 10; int frame_count = 0; // game ���� frame count �� �ӵ� ���������� ���ȴ�.
int p1_frame_sync = 4; //�÷��̾��� �̵��ӵ�
int Enemy_count = 0;

void Enemyfall();
void playerfall();
void info();
void showitem();
void showitem2();
void moveitem();
void moveitem2();
void DrawBullet(int i);
void EraseBullet(int i);
void EraseBullet2();
void DrawBullet2();
void bulletmove();
void bulletmove2();
void playerdraw(int x, int y);
void playererase(int x, int y);
void playermove(unsigned char ch);
void showenemy();
void EnemyBulletshow();
void Enemymove();
void EnemyBulletdraw(int i);
void EnemyBulleterase(int i);
void EnemyBulletMove();
void nextStage();
void gamestart();
void gameClear();
void first(void);
int Menu(void);

void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos; // Windows.h �� ����
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showCursor(void) { // Ŀ���� ���̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void cls(int text_color, int bg_color) // ȭ�� �����
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}



void gameOver() {
	gotoxy(47, 25);
	textcolor(RED1, BLACK);
	printf("game over!");
	Sleep(1000);
	printf("              ");

}


void Enemyfall() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1)
			continue;
		if (Enemy[i].y == Bullet[i].y && abs(Enemy[i].x - Bullet[i].x) <= 7) {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("   ");
			Bullet[i].exist = FALSE;
			Enemy[i].type = -1;
			score += 100;
			Enemy_count++;
			break;
		}
			}
		}
void playerfall() {
	int i;
	for (i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == FALSE)
			continue;
		if (EnemyBullet[i].y == newy && abs(EnemyBullet[i].x - newx) <= 7) {
			EnemyBullet[i].exist = FALSE;
			gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
			printf("     ");
			square--;
		}
	}
}
void info() {
	textcolor(WHITE, BLACK);
	gotoxy(60, 48); printf("SCORE: %d", score);
	gotoxy(40, 48); printf("HP: ");
	gotoxy(20, 48); printf("LIFE:");
	if (square <= 0) {
		square = 3;
		gotoxy(45, 48); printf("        ");
		gotoxy(45, 48); printf("����");
		heart--;
	}
		else if (square == 3) {
			gotoxy(45, 48); printf("         ");
			gotoxy(45, 48); printf("����");
		}
		else if (square == 2) {
			gotoxy(45, 48); printf("         ");
			gotoxy(45, 48); printf("���");
		}
		else if (square == 1) {
			gotoxy(45, 48); printf("         ");
			gotoxy(45, 48); printf("��");
		}
		else {
		gotoxy(45, 48);
		printf("        ");
	}
	if (heart == 4) {
		heart = 3;
		gotoxy(25, 48); printf("           ");
		gotoxy(25, 48); printf("������");
	}
	else if (heart == 3) {
		gotoxy(25, 48); printf("           ");
		gotoxy(25, 48); printf("������");
	}
	else if (heart == 2) {
		gotoxy(25, 48); printf("        ");
		gotoxy(25, 48); printf("����");
	}
	else if (heart == 1) {
		gotoxy(25, 48); printf("        ");
		gotoxy(25, 48); printf("��");
	}
	else {
		gotoxy(25, 48);
		printf("           ");
	}
	}
	
void showitem() {
	int x, y;
	x = 7 + rand() % 40;
	y = 2;
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(ITEM);
	items[x][y] = 10;
	items_count++;
	textcolor(YELLOW2, BLACK);
}
void showitem2() {
	int x, y;
	x = 7 + rand() % 50;
	y = 2;
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(LIFEITEM);
	Lifeitems[x][y] = 10;
	Lifeitems_count++;
	textcolor(YELLOW2, BLACK);
}

void moveitem() {
	int x, y, dx, dy, nx, ny;
	int newitems[WIDTH][HEIGHT] = { 0 };
	if (items_count == 0)
		return;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (items[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = 1;
				nx = x + dx;
				ny = y + dy;
				if (nx == WIDTH - 5) nx = WIDTH - 6;
				if (nx < 7) nx = 8;
				if (ny < 1) ny = 1;
				if (ny > HEIGHT - 1) {
					gotoxy(x, y);
					printf("   ");
					items_count--;
				}
				else {
					gotoxy(x, y);
					textcolor(YELLOW2, BLACK);
					printf("   "); // erase gold
					gotoxy(nx, ny);
					printf(ITEM);
					newitems[nx][ny] = 1; // �̵��� ��������
					textcolor(YELLOW2, BLACK);
				}
			}
		}
	}
	memcpy(items, newitems, sizeof(newitems)); // �ѹ��� gold ��ġ�� �����Ѵ�.
}
void moveitem2() {
	int x, y, dx, dy, nx, ny;
	int newitems[WIDTH][HEIGHT] = { 0 };
	if (Lifeitems_count == 0)
		return;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (Lifeitems[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = 1;
				nx = x + dx;
				ny = y + dy;
				if (nx == WIDTH - 5) nx = WIDTH - 6;
				if (nx < 7) nx = 8;
				if (ny < 1) ny = 1;
				if (ny > HEIGHT - 1) {
					gotoxy(x, y);
					printf("   ");
					Lifeitems_count--;
				}
				else {
					gotoxy(x, y);
					textcolor(YELLOW2, BLACK);
					printf("   "); // erase gold
					gotoxy(nx, ny);
					printf(LIFEITEM);
					newitems[nx][ny] = 1; // �̵��� ��������
					textcolor(YELLOW2, BLACK);
				}
			}
		}
	}
	memcpy(Lifeitems, newitems, sizeof(newitems)); // �ѹ��� gold ��ġ�� �����Ѵ�.
}
void DrawBullet(int i) {
	textcolor(CYAN1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("��");
}
void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("     ");
}


void bulletmove() {
	int i;

	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);
			if (Bullet[i].y == 0) {
				Bullet[i].exist = FALSE;
			}
			else {
				Bullet[i].y--;
				DrawBullet(i);
			}
		}
	}
}


void playerdraw(int x, int y) {
	textcolor(CYAN1, BLACK);
	gotoxy(x, y);
	printf("<<=*=>>");
}
void playererase(int x, int y) {
	gotoxy(x, y);
	printf("            ");
}
void playermove(unsigned char ch) {

	int move_flag = 0;
	static unsigned char last_ch = 0;


	if (called == 0) {
		removeCursor();
		playerdraw(oldx, oldy);
		called = 1;
	}
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 25)
			newy = oldy - 1;
		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 3)
			newy = oldy + 1;
		move_flag = 1;
		break;
	case LEFT:
		if (oldx > 2)
			newx = oldx - 1;
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 6)
			newx = oldx + 1;
		move_flag = 1;
		break;
	}

	if (move_flag) {
		playererase(oldx, oldy);
		playerdraw(newx, newy); // ���ο� ��ġ���� �÷��̾� ǥ��
		oldx = newx; // ������ ��ġ�� ����Ѵ�.
		oldy = newy;
		if (items[newx][newy]) {
			square++;
			items[newx][newy] = 0;
			items_count--;
		}
		if (Lifeitems[newx][newy]) {
			heart++;
			Lifeitems[newx][newy] = 0;
			Lifeitems_count--;
		}
	}
}


void showenemy() {
	int i, location, direct, hieght;
	location = rand() % 2;
	direct = 5 + rand() % 55;
	hieght = 1 + rand() % 8;
		for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) {}
		if (i != MAXENEMY) {
			if (location == 1) {
				Enemy[i].x = direct;
				Enemy[i].y = hieght;
				Enemy[i].move = 1;
			}
			else {
				Enemy[i].x = direct;
				Enemy[i].y = hieght;
				Enemy[i].move = -1;
			}
			Enemy[i].type = rand()% 4;
			Enemy[i].exist = TRUE;
		}
	}
void showenemy2() {
	int i, location, direct, hieght;
	location = rand() % 2;
	direct = 5 + rand() % 55;
	hieght = 1 + rand() % 8;
	for (i = 0; i < MAXENEMY2 && Enemy[i].exist == TRUE; i++) {}
	if (i != MAXENEMY2) {
		if (location == 1) {
			Enemy[i].x = direct;
			Enemy[i].y = hieght;
			Enemy[i].move = 1;
		}
		else {
			Enemy[i].x = direct;
			Enemy[i].y = hieght;
			Enemy[i].move = -1;
		}
		Enemy[i].type = 4;
		Enemy[i].exist = TRUE;
	}
}
void EnemyBulletshow() {
	int j;
	int random = rand() % MAXENEMY;
	for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) {}
	if (j != MAXENEMYBULLET && Enemy[random].exist == TRUE) {
		EnemyBullet[j].x = Enemy[random].x + 2;
		EnemyBullet[j].y = Enemy[random].y + 1;
		EnemyBullet[j].exist = TRUE;
	}
}
void EnemyBulletshow2() {
	int j;
	int random = 1;
	for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) {}
	if (j != MAXENEMYBULLET && Enemy[random].exist == TRUE) {
		EnemyBullet[j].x = Enemy[random].x - 2;
		EnemyBullet[j].y = Enemy[random].y - 1;
		EnemyBullet[j].exist = TRUE;
	}
}

void Enemymove() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == TRUE) {
			if (Enemy[i].type == -1) {
				gotoxy(Enemy[i].x - 1, Enemy[i].y);
				printf("            ");
				Enemy[i].exist = FALSE;
				continue;
			}
			if (Enemy[i].x > 80) {
				Enemy[i].x -= 1;
				Enemy[i].move = -1;
				printf("     ");


			}
			else if (Enemy[i].x <= 3) {

				Enemy[i].move = 1;
				Enemy[i].x += 1;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("     ");

			}
			else {
				Enemy[i].x += Enemy[i].move;
				gotoxy(Enemy[i].x , Enemy[i].y);
				printf("     ");
				printf(Enemyunit[Enemy[i].type]);
				printf("     ");
			}
		}
	}
}
void Enemymove2() {
	int i;
	for (i = 0; i < MAXENEMY2; i++) {
		if (Enemy[i].exist == TRUE) {
			if (Enemy[i].type == -1) {
				gotoxy(Enemy[i].x - 1, Enemy[i].y);
				printf("            ");
				Enemy[i].exist = FALSE;
				continue;
			}
			if (Enemy[i].x > 80) {
				Enemy[i].x -= 1;
				Enemy[i].move = -1;
				printf("     ");


			}
			else if (Enemy[i].x <= 3) {

				Enemy[i].move = 1;
				Enemy[i].x += 1;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("     ");

			}
			else {
				Enemy[i].x += Enemy[i].move;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("     ");
				printf(Enemyunit[4]);
				printf("     ");
			}
		}
	}
}
void EnemyBulletdraw(int i) {
	textcolor(RED2, BLACK);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("*");
}
void EnemyBulleterase(int i) {
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("    ");
}
void EnemyBulletdraw2(int i) {
	textcolor(RED2, BLACK);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("&&");
}

void EnemyBulletMove() {
	int random;
	random = 1;
	for (int i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == TRUE) {
			EnemyBulleterase(i);
			if (EnemyBullet[i].y > HEIGHT - 3) {
				EnemyBullet[i].exist = FALSE;
			}
			else {
				if (random) {
					if (EnemyBullet[i].x <= newx)
						EnemyBullet[i].x++;
					else
						EnemyBullet[i].x--;
				}
				EnemyBullet[i].y++;
				EnemyBulletdraw(i);
			}
		}
	}
}
void EnemyBulletMove2() {
	int random;
	random = 1;
	for (int i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == TRUE) {
			EnemyBulleterase(i);
			if (EnemyBullet[i].y > HEIGHT - 3) {
				EnemyBullet[i].exist = FALSE;
			}
			else {
				if (random) {
					if (EnemyBullet[i].x <= newx)
						EnemyBullet[i].x++;
					else
						EnemyBullet[i].x--;
				}
				EnemyBullet[i].y++;
				EnemyBulletdraw2(i);
			}
		}
	}
}
void nextStage() {
	gotoxy(47, 25);
	textcolor(RED1, BLACK);
	printf("next stage!");
	Sleep(1000);
	gotoxy(47, 25);
	printf("               ");

}

void gamestart() {
	unsigned char ch;
	int i;
	system("cls");
	while (1) {
		removeCursor();
		if (_kbhit() == 1) {
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = _getch();
				switch (ch) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					playermove(ch);
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
				}
			}
			if (ch == Space) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 3;
					Bullet[i].y = 43;
					Bullet[i].exist = TRUE;
				}
			}
		}
		else {
			//�ӵ����� �Լ�
		}
		playerfall();
		Enemyfall();
		info();   
		textcolor(WHITE, BLACK);
		showenemy(); 
		
		if(frame_count%2==0)
			EnemyBulletshow();
	
		if (frame_count % 10 == 0) {
			moveitem();
			moveitem2();
		}
		if(frame_count % 5==0)
			bulletmove();
		if (frame_count %1200==0) {
			showitem();
		}
		if (frame_count % 900 == 0) {
			showitem2();
		}
		if (frame_count % 9 == 0) {
			textcolor(WHITE, BLACK);
			Enemymove();
			EnemyBulletMove();
		}
		
		if (Enemy_count==10) {
			nextStage();
			Enemy_count = 11; 
			square = 3;
			system("cls");
			return;
		}
		if (heart == 0) {
			gameOver();
			system("cls");
			textcolor(WHITE, BLACK);
			return;
		}
			Sleep(Delay); // Delay ���� ���̰�
			frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
		}
	}
void gamestart2() {
	unsigned char ch;
	int i;
	system("cls");
	removeCursor();
	while (1) {
		if (_kbhit() == 1) {
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = _getch();
				switch (ch) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					playermove(ch);
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
				}
			}
			if (ch == Space) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 3;
					Bullet[i].y = 43;
					Bullet[i].exist = TRUE;
				}
			}
		}
	
		else {
			//�ӵ����� �Լ�
		}
		playerfall();
		Enemyfall();
		info();
		showenemy();

		if (frame_count % 4 == 0) {
			bulletmove();
		}
		if (frame_count % 7 == 0) {
			textcolor(WHITE, BLACK);
			Enemymove();
			moveitem();
			moveitem2();
		}
		if (frame_count % 1200 == 0) {
			showitem();
		}
		if (frame_count % 900 == 0) {
			showitem2();
		}
		if (frame_count % 10 == 0) {
			EnemyBulletshow();

		}
		if (frame_count %  7== 0) {
			EnemyBulletMove();
		}

		if (Enemy_count == 21) {
			nextStage();
			heart = 3;
			square = 3;
			system("cls");
			return;
		}
		if (heart == 0) {
			gameOver();
			system("cls");
			textcolor(WHITE, BLACK);
			return;
		}
		Sleep(Delay); // Delay ���� ���̰�
		frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
	}
}
void gamestart3() {
	unsigned char ch;
	int i;
	system("cls");
	removeCursor();
	while (1) {
		if (_kbhit() == 1) {
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = _getch();
				switch (ch) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					playermove(ch);
					if (frame_count% p1_frame_sync == 0)
						playermove(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
				}
			}
			if (ch == Space) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 3;
					Bullet[i].y = 43;
					Bullet[i].exist = TRUE;
				}
			}
		}
		else {
			//�ӵ����� �Լ�
		}
		playerfall();
		Enemyfall();
		info();
		showenemy2();

		if(frame_count%3==0)
			bulletmove();
		if (frame_count % 10 == 0) {
				moveitem();
				moveitem2();
			}
		if (frame_count % 5 == 0) {
			bulletmove();
			EnemyBulletshow();
			Enemymove2();
		}
		if (frame_count % 1200 == 0) {
			showitem();
		}
		if (frame_count % 900 == 0) {
			showitem2();
		}
		if (frame_count % 9 == 0) {
			textcolor(WHITE, BLACK);
			EnemyBulletMove2();
		}

		if (Enemy_count == 31) {
			gameClear();
			textcolor(WHITE, BLACK);
			return; 
		}
		if (heart == 0) {
			gameOver();
			system("cls");
			textcolor(WHITE, BLACK);
			return;
		}
		Sleep(Delay); // Delay ���� ���̰�
		frame_count++;// frame_count ������ �ӵ� ������ �Ѵ�.
	}
}

int Menu(void) {
	const x = 47;
	int y = 15;

	textcolor(BLUE1, BLACK);
	gotoxy(x, y);
	printf(">");
	printf(" PLAY");
	gotoxy(x, y + 1);
	printf("  END");
	gotoxy(x, y + 2);

	while (1)
	{

		gotoxy(x, y);
		printf("");
		int input = _getch();
		printf(" ");

		switch (input)
		{
		case UP:
			if (y > 14) y--;
			break;

		case DOWN:
			if (y < 16) y++;
			break;

		case Space:
			return y - 15;
		}
		gotoxy(x, y);
		printf(">");
	}
}

void gameClear() {
	gotoxy(47, 25);
	textcolor(RED1, BLACK);
	printf("game clear!");
	Sleep(1000);
	gotoxy(47, 25);
	printf("               ");

	}



void first(void) {
	int i, x, y;
	char buf[100];
	textcolor(CYAN1, BLACK);
	gotoxy(20, 5);
	printf("   ���      ��      ��     �� ��         �� ��     ��  ��  ��      \n");
	gotoxy(20, 6);
	printf(" ��    ��   ��      ��    ��      ��    ��     ��      ��           \n");
	gotoxy(20, 7);
	printf(" ��          ��      ��    ��     ��    ��     ��      ��           \n");
	gotoxy(20, 8);
	printf("   �� ��     ������   ��     ��    ��     ��      ��            \n");
	gotoxy(20, 9);
	printf("         ��  ��      ��    ��     ��    ��     ��      ��           \n");
	gotoxy(20, 10);
	printf("         ��   ��      ��  ��     ��    ��    ��       ��            \n");
	gotoxy(20, 11);
	printf(" �� �� ��    ��      ��    �� �� ��     ��  ��       ��             \n");
	gotoxy(20, 12);

	textcolor(WHITE, BLACK);
	gotoxy(10, 20);
	printf("��������������������   ��item  ����������������������\n");
	gotoxy(10, 21);
	printf("��                              ��\n");
	gotoxy(10, 22);
	printf("��  ��HP +1                     ��\n");
	gotoxy(10, 23);
	printf("��  ��Life +1                   ��\n");
	gotoxy(10, 24);
	printf("����������������������������������������������������������������\n"); {
	gotoxy(60, 20);
	printf("������������������   ��control  ������������������\n");
	gotoxy(60, 21);
	printf("��   ��: up         �� : right  ��\n");
	gotoxy(60, 22);
	printf("��   ��: down       �� : left   ��\n");
	gotoxy(60, 23);
	printf("��   space : shoot              ��\n");
	gotoxy(60, 24);
	printf("����������������������������������������������������������������\n");
	showCursor();
		while (1)
		{
			int SELECT = Menu();
			switch (SELECT)
			{
			case 0:
				system("cls");
				removeCursor();
				gamestart();
				if (Enemy_count == 11) {
					removeCursor();
					gamestart2();
				}
				if (Enemy_count == 21) {
					removeCursor();
					system("cls");
					gamestart3();
				}
				if (Enemy_count == 31) {
					removeCursor();
				}
			case 1:
				system("cls");
				return;
			}
		}
	}
}
int main()
{
	system("mode con:cols=100 lines=50");
	first();
	

	return 0;
}
