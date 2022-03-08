#include <iostream>
#include <stdio.h>
#include <random>
#include <tuple>
#include <random>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

using namespace std;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

char show_list[] = { ' ', 'O', 'X' };

void gotoxy(int x, int y) {
	HANDLE consolhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consolhandle, pos);
}

int keyConsol() {
	char temp = _getch();
	if (temp == 'w' || temp == 'W' || temp == 72) {
		return UP;
	}
	else if (temp == 'a' || temp == 'A' || temp == 75) {
		return LEFT;
	}
	else if (temp == 's' || temp == 'S' || temp == 80) {
		return DOWN;
	}
	else if (temp == 'd' || temp == 'D' || temp == 77) {
		return RIGHT;
	}
	else if (temp == ' ') {
		return SUBMIT;
	}
}

class Map {
private:
	int map[3][3];

public:
	Map() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				map[i][j] = 0;
			}
		}
	}
	void show_map() {
		for (int i = 0; i < 3; i++) {
			cout << "      ---|---|---" << endl;
			cout << "       " << show_list[map[i][0]] << " | " << show_list[map[i][1]] << " | " << show_list[map[i][2]] << endl;
		}
	}

	bool set_mark(int y, int x, int mark) {
		if (map[y][x] == 0) {
			this->map[y][x] = mark;
			return 1;
		}
		else {
			//cout << "옳바른 위치에 놓으세요" << endl;
			return 0;
		}
	}
	int get_position_mark(int y, int x) {
		return map[y][x];
	}
	int is_full() {
		int temp = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (map[i][j] == 0) temp++;
			}
		}
		return temp;
	}
	int check_map() {
		//가로
		for (int i = 0; i < 3; i++) {
			if (((map[i][0] == map[i][1]) && (map[i][1] == map[i][2]) && (map[i][0] == map[i][2])) && (map[i][0] != 0) && (map[i][1] != 0) && (map[i][2] != 0)) {
				return map[i][0];
			}
		}
		//세로
		for (int i = 0; i < 3; i++) {
			if (((map[0][i] == map[1][i]) && (map[1][i] == map[2][i]) && (map[0][i] == map[2][i])) && (map[0][i] != 0) && (map[1][i] != 0) && (map[2][i] != 0)) {
				return map[0][i];
			}
		}
		//대각선
		if ((map[0][0] == map[1][1]) && (map[1][1] == map[2][2]) && (map[0][0] != 0) && (map[1][1] != 0) && (map[2][2] != 0)
			|| (map[0][2] == map[1][1]) && (map[1][1] == map[2][0]) && (map[0][2] != 0) && (map[1][1] != 0) && (map[2][0] != 0)) {
			return map[1][1];
		}
		//없음
		return 0;
	}
};

class Player {
private:
	int mark; // O : 1, X : 2
	char mark_char; // O, X 
	bool is_ai; // 0 : player , 1 : AI
	int player_num; // 1P , 2P
public:
	Player() {
		mark = 0;
		mark_char = 'O';
		is_ai = false;
		player_num = 0;
	}
	Player(int mark, bool is_ai) {
		this->mark = mark;
		this->is_ai = is_ai;
	}
	void set_mark(int mark) {
		this->mark = mark;

		if (mark == 1) {
			this->mark_char = 'O';
		}
		else {
			this->mark_char = 'X';
		}
	}
	void set_is_ai(bool is_ai) {
		this->is_ai = is_ai;
	}
	bool get_is_ai() {
		return this->is_ai;
	}
	void set_player_num(int player_num) {
		this->player_num = player_num;
	}
	char get_mark_char() {
		return this->mark_char;
	}
	void set_mark_char(char a) {
		this->mark_char = a;
	}
	int get_player_num() {
		return this->player_num;
	}
	int get_mark() {
		return this->mark;
	}
	tuple<int, int, int> drop_mark() {
		int y, x;
		if (this->is_ai) {
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dis(0, 2);
			y = dis(gen);
			x = dis(gen);
		}
		else {
			cout << this->player_num << "P  " << "(y, x)좌표를 입력하세요 >> ";
			cin >> y >> x;
		}

		tuple<int, int, int> v = make_tuple(y, x, this->mark);
		return v;
	}
};

class Game {
private:
	int mode;
	int turn;
	int turn_count;
	Player p1;
	Player p2;
	Map map;
public:
	Game() {
		mode = 0;
		turn = 1;
		turn_count = 9;

		cout << endl;

	}

	void title_show() {
		printf("\n\n");
		printf("     #####   #    ####     #####     #      ####     #####    ###    #####     \n");
		printf("       #     #   #           #      # #    #           #     #   #   #         \n");
		printf("       #     #   #           #     #####   #           #     #   #   #####     \n");
		printf("       #     #   #           #     #   #   #           #     #   #   #         \n");
		printf("       #     #    ####       #     #   #    ####       #      ###    #####     \n");
		printf("----------------------------------------------------------------------------------\n");
	}
	int draw_player_choice() {
		title_show();
		int x = 24;
		int y = 12;
		gotoxy(x - 2, y - 1);
		cout << "<<  1P, 2P 선택  >>" << endl;
		gotoxy(x - 2, y);
		cout << "> 1인용";
		gotoxy(x, y + 1);
		cout << "2인용";
		gotoxy(x, y + 2);
		cout << endl;
		while (1) {
			int n = keyConsol();
			switch (n) {
			case UP: {
				if (y > 12) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, --y);
					printf(">");
				}
				break;
			}
			case DOWN: {
				if (y < 13) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, ++y);
					printf(">");
				}
				break;
			}
			case SUBMIT: {
				return y - 12;
			}
			}
		}
	}

	int draw_player_ox() {
		title_show();
		int x = 24;
		int y = 12;

		gotoxy(x - 2, y - 1);
		cout << "<<  O,X중 사용할것 선택  >>" << endl;
		gotoxy(x - 2, y);
		cout << "> O";
		gotoxy(x, y + 1);
		cout << "X";
		gotoxy(x, y + 2);
		cout << endl;
		while (1) {
			int n = keyConsol();
			switch (n) {
			case UP: {
				if (y > 12) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, --y);
					printf(">");
				}
				break;
			}
			case DOWN: {
				if (y < 13) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, ++y);
					printf(">");
				}
				break;
			}
			case SUBMIT: {
				return y - 12;
			}
			}
		}
	}

	tuple<int, int> drop_marker_consol(Player a) {
		int x = 9;
		int y = 9;
		int left_count = 0;
		int right_count = 0;
		int up_count = 0;
		int down_count = 0;
		char p = a.get_mark_char();
		system("cls");
		title_show();
		map.show_map();
		gotoxy(19, y);
		cout << p << " 차례 입니다" << endl;
		gotoxy(5, y + 9);
		cout << "진행중인 게임";
		gotoxy(0, y + 11);
		map.show_map();


		gotoxy(x - 2, y);
		cout << ">";
		while (1) {
			int n = keyConsol();
			switch (n) {
			case UP: {
				if (y > 9) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, ----y);
					printf(">");
					up_count--;
				}
				break;
			}
			case DOWN: {
				if (y < 13) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, ++++y);
					printf(">");
					down_count++;
				}
				break;
			}
			case LEFT: {
				if (x > 9) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(--------x - 2, y);
					printf(">");
					left_count--;
				}
				break;
			}
			case RIGHT: {
				if (x < 14) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(++++++++x - 2, y);
					printf(">");
					right_count++;
				}
				break;
			}
			case SUBMIT: {
				if (map.get_position_mark(up_count + down_count, left_count + right_count) == 0) {
					tuple<int, int> v = make_tuple(up_count + down_count, left_count + right_count);
					return v;
				}

			}
			}
		}
	}

	int draw_menu() {
		title_show();
		int x = 24;
		int y = 12;
		gotoxy(x - 2, y);
		cout << "> 게임시작";

		gotoxy(x, y + 1);
		cout << " 종료 ";
		cout << endl;
		while (1) {
			int n = keyConsol();
			switch (n) {
			case UP: {
				if (y > 12) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, --y);
					printf(">");
				}
				break;
			}
			case DOWN: {
				if (y < 13) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(x - 2, ++y);
					printf(">");
				}
				break;
			}
			case SUBMIT: {
				return y - 12;
			}
			}
		}
	}
	void set_mode() {
		int menu_code = draw_menu();
		while (1) {
			if (menu_code == 0) {
				system("cls");

				this->mode = draw_player_choice() + 1;
				system("cls");
				title_show();
				break;
			}
			else {
				exit(0);
				return;

			}

		}
	}
	int get_mode() {
		return this->mode;
	}
	void set_player_inform() {

		bool input_ox_check = 1;
		system("cls");
		//title_show();
		int temp = draw_player_ox();

		Player* p;
		if (temp == 0) {
			p = &p1;
			p->set_mark(1);
			p->set_is_ai(0);
			p->set_player_num(1);
			p->set_mark_char('O');
			p = &p2;
			p->set_mark(2);
			p->set_player_num(2);
			p->set_mark_char('X');
			if (this->mode == 1) {
				p->set_is_ai(1);
			}
			else {
				p->set_is_ai(0);
			}

		}
		else if (temp == 1) {
			p = &p1;
			p->set_mark(2);
			p->set_is_ai(0);
			p->set_player_num(1);
			p->set_mark_char('X');
			p = &p2;
			p->set_mark(1);
			p->set_player_num(2);
			p->set_mark_char('O');
			if (this->mode == 1) {
				p->set_is_ai(1);
			}
			else {
				p->set_is_ai(0);
			}
		}



	}
	void play() {
		int check;
		bool gaming = 1;
		if (!this->turn_count) {
			return;
		}
		set_mode();
		set_player_inform();
		system("cls");
		title_show();
		while (turn_count) {

			if (this->turn_count < 0) {
				break;
			}
			tuple<int, int, int> p;
			tuple<int, int> mark_xy;
			bool drop_success = 0;
			int is_full = 0;

			//P1
			mark_xy = drop_marker_consol(p1);
			map.set_mark(get<0>(mark_xy), get<1>(mark_xy), p1.get_mark());
			gotoxy(9 - 2 + 10 + 10, 9 + 10);

			check = map.check_map();
			is_full = map.is_full();
			if (check || !is_full) {
				if (check) {
					cout << "<< " << show_list[check] << "가 이겼습니다. >>" << endl;
					map.show_map();
				}
				else {
					cout << "<< " << "무승부 입니다. >>" << endl;
					map.show_map();
				}
				turn_count = 0;
				break;
			}

			//P2
			if (p2.get_is_ai()) {
				do {
					p = p2.drop_mark();
					drop_success = map.set_mark(get<0>(p), get<1>(p), get<2>(p));
				} while (!drop_success);
			}
			else {
				mark_xy = drop_marker_consol(p2);
				map.set_mark(get<0>(mark_xy), get<1>(mark_xy), p2.get_mark());
				gotoxy(9 - 2 + 10, 9 + 10);
				map.show_map();
			}
			gotoxy(9 - 2 + 10, 9 + 10);
			map.show_map();
			check = map.check_map();
			if (check) {
				cout << "<< " << show_list[check] << "가 이겼습니다. >>" << endl;
				turn_count = 0;
			}
			turn_count--;

		}
		char a = 0;
		cout << "아무 키나 눌러 타이틀로" << endl;
		a = _getch();
		if (a) {
			return;
		}

	}

};




int main() {
	//수정 테스트
	

	while (1) {
		system("cls");
		Game a;
		a.play();

	}


	return 0;
}