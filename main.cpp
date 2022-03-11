#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <tuple>
#include <random>
#include <windows.h>
#include <conio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define BUFSIZE 1024
using namespace std;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"


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
class Server {
private:
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKADDR_IN servAddr;

	int clientLen;
	SOCKET hClientSock;
	SOCKADDR clientAddr;

public:
	Server() {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//소켓 생성
		hServSock = socket(PF_INET, SOCK_STREAM, 0);
		printf("socket 생성 성공\n");
		//바인딩 : 서버역할수행
		servAddr.sin_family = AF_INET;
		//IP설정 : INADDR_ANY 자신의 IP주소를 획득
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		//Port설정
		servAddr.sin_port = htons(5555);
	}

	void bind_() {
		bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
		printf("bind() 성공\n");
	}
	void listen_() {
		listen(hServSock, 5);
		printf("Client를 기다리는중 ..\n");
	}
	void accept_() {
		//accept : 접속자를 받기
		clientLen = sizeof(clientAddr);
		hClientSock = accept(hServSock, (SOCKADDR*)&clientAddr, &clientLen);
		printf("클라이언트 접속 성공");
	}
	void close_() {
		closesocket(hClientSock);
		WSACleanup();
	}
	void echo_() {
		int strLen = 0;
		char message[BUFSIZE] = "";
		while ((strLen = recv(hClientSock, message, BUFSIZE, 0)) != 0) {
			printf("recv, check : %s\n", message);
			send(hClientSock, message, strlen(message), 0);
			memset(message, 0, sizeof(message));
		}
	}
	void send_position(int y, int x, int mark) {
		char message[BUFSIZE] = "";
		char y_[10] = "";
		char x_[10] = "";
		char mark_[10] = "";
		sprintf(y_, "%d", y);
		sprintf(x_, "%d", x);
		sprintf(mark_, "%d", mark);
		
		strcat(message, y_);
		strcat(message, x_);
		strcat(message, mark_);
		printf("recv, check : %s\n", message);
		send(hClientSock, message, strlen(message), 0);
		memset(message, 0, sizeof(message));
	}
};

class Client {
private:
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

public:
	Client() {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1 자신의 ip 알아서 함
		servAddr.sin_port = htons(5555);
	}

	void connect_() {
		connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));
		cout << "접속 성공! "<<endl;
	}

	void echo_test() {
		char message[BUFSIZE] = "";
		while (1) {

			fputs("전송할 메세지를 입력하세요. (q to quit) : ", stdout);
			//fgets(message, BUFSIZE, stdin);
			cin >> message;
			//message[strlen(message) - 1] = '\0';

			//q 입력 시 무한루프 탈출
			if (!strcmp(message, "q")) {
				this->close_();
				break;
			}

			//전송
			printf("전송하는 메세지 : %s\n", message);
			int strLen = send(hSocket, message, strlen(message), 0);
			//받기
			strLen = recv(hSocket, message, BUFSIZE - 1, 0); // 동기, 받은게 message로 들어감
			//printf("%d\n", strLen);
			message[strLen] = 0;
			printf("서버로 부터 전송된 메세지 :  %s \n", message);
		}
	}
	void recv_position() {
		char message[BUFSIZE] = "";
		int strLen = 0;
		while ((strLen = recv(hSocket, message, BUFSIZE, 0)) != 0) {
			message[strLen] = 0;
			printf("서버로 부터 전송된 메세지 :  %s \n", message);
		}	
	}
	void send_position(int y, int x, int mark) {
		char message[BUFSIZE] = "";
		char y_[10] = "";
		char x_[10] = "";
		char mark_[10] = "";
		sprintf(y_, "%d", y);
		sprintf(x_, "%d", x);
		sprintf(mark_, "%d", mark);

		strcat(message, y_);
		strcat(message, x_);
		strcat(message, mark_);
		printf("recv, check : %s\n", message);
		send(hSocket, message, strlen(message), 0);
		memset(message, 0, sizeof(message));
	}

	void close_() {
		closesocket(hSocket);
		WSACleanup();
	}

};


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
			//cout <<"      ---|---|---" << endl;
			//cout <<"       " << show_list[map[i][0]] << " | " << show_list[map[i][1]] << " | " << show_list[map[i][2]] << endl;
			printf("      ---|---|---\n");
			if (show_list[map[i][0]] == 'O') {
				printf("       %s%c%s", GREEN, show_list[map[i][0]], RESET);
			}
			else {
				printf("       %s%c%s", RED, show_list[map[i][0]], RESET);
			}

			printf(" | ");
			if (show_list[map[i][1]] == 'O') {
				printf("%s%c%s", GREEN, show_list[map[i][1]], RESET);
			}
			else {
				printf("%s%c%s", RED, show_list[map[i][1]], RESET);
			}
			printf(" | ");
			if (show_list[map[i][2]] == 'O') {
				printf("%s%c%s\n", GREEN, show_list[map[i][2]], RESET);
			}
			else {
				printf("%s%c%s\n", RED, show_list[map[i][2]], RESET);
			}
		}
	}

	bool set_mark(int y, int x, int mark) { // 돌놓기
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

struct Position {
	int x;
	int y;
};

class Player {
private:
	int mark; // O : 1, X : 2
	char mark_char; // O, X 
	bool is_ai; // 0 : player , 1 : AI
	bool is_server;
	int player_num; // 1P , 2P
	Position position;
public:
	Player() {
		mark = 0;
		mark_char = 'O';
		is_ai = false;
		is_server = false;
		player_num = 0;
		position.x = 9;
		position.y = 9;
	}
	Player(int mark, bool is_ai) {
		this->mark = mark;
		this->is_ai = is_ai;
	}
	void set_position(int x, int y) {
		this->position.x = x;
		this->position.y = y;
	}
	void set_is_server(bool is_server) {
		this->is_server = is_server;
	}
	bool get_is_server() {
		return this->is_server;
	}
	Position get_position() {
		return this->position;
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
	Server server;
	Client client;
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
		int x = 30;
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
		int x = 30;
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

	tuple<int, int> drop_marker_consol(Player& a) {
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

		x = a.get_position().x;
		y = a.get_position().y;

		gotoxy(x - 2, y);
		cout << ">";
		int x_ = x;
		int y_ = y;
		while (1) {
			//gotoxy(26, 26);
			//cout << "(" << x << " " << y << ")" << endl;
			//cout << "(" << x_ << " " << y_ << ")";
			int n = keyConsol();
			switch (n) {
			case UP: { // 
				if (y > 9) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(0, 0);
					title_show();
					map.show_map();
					gotoxy(x - 2, ----y);

					printf("%c", p);
					up_count--;
					y_ -= 2;

				}
				break;
			}
			case DOWN: {
				if (y < 13) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(0, 0);
					title_show();
					map.show_map();
					gotoxy(x - 2, ++++y);
					printf("%c", p);
					down_count++;
					y_ += 2;
				}
				break;
			}
			case LEFT: {
				if (x > 9) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(0, 0);
					title_show();
					map.show_map();
					gotoxy(--------x - 2, y);
					printf("%c", p);
					left_count--;
					x_ -= 4;
				}
				break;
			}
			case RIGHT: {
				if (x < 14) {
					gotoxy(x - 2, y);
					printf(" ");
					gotoxy(0, 0);
					title_show();
					map.show_map();
					gotoxy(++++++++x - 2, y);
					printf("%c", p);
					right_count++;
					x_ += 4;
				}
				break;
			}
			case SUBMIT: {
				int y_mapping[18] = { 0, };
				int x_mapping[18] = { 0, };
				y_mapping[9] = 0;
				y_mapping[11] = 1;
				y_mapping[13] = 2;
				x_mapping[9] = 0;
				x_mapping[13] = 1;
				x_mapping[17] = 2;

				if (map.get_position_mark(y_mapping[y], x_mapping[x]) == 0) {
					tuple<int, int> v = make_tuple(y_mapping[y], x_mapping[x]);
					a.set_position(x_, y_);
					return v;
				}

			}
			}
		}
	}

	int draw_menu() {
		title_show();
		int x = 30;
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

				

				this->mode = draw_player_choice() + 1; // 모드 선택.
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
	

		int temp = draw_player_ox();

		Player* p;
		if (temp == 0) {
			//여기서 server on
			server.bind_();
			server.listen_();
			server.accept_();

			p = &p1;
			p->set_mark(1);
			p->set_is_ai(0);
			p->set_player_num(1);
			p->set_mark_char('O');

			p->set_is_server(1); // 1P가 Server 함 

			p = &p2;
			p->set_mark(2);
			p->set_player_num(2);
			p->set_mark_char('X');

			p->set_is_server(0); // 2P는 Client


			if (this->mode == 1) {
				p->set_is_ai(1);
			}
			else {
				p->set_is_ai(0);
				
			}
			 
		}
		else if (temp == 1) {
			client.connect_(); // 예외처리 할것.
			p = &p1;
			p->set_mark(2);
			p->set_is_ai(0);
			p->set_player_num(1);
			p->set_mark_char('X');
			p->set_is_server(1); // 1P가 Server 함 
			p = &p2;
			p->set_mark(1);
			p->set_player_num(2);
			p->set_mark_char('O');
			p->set_is_server(0); // 2P는 Client
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
		set_player_inform(); // server client connection 완료
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

			//P1 , p1 이 서버
			if (p1.get_is_server()) {
				mark_xy = drop_marker_consol(p1);
				int temp_y = get<0>(mark_xy);
				int temp_x = get<1>(mark_xy);
				int temp_mark = p1.get_mark();
				map.set_mark(temp_y, temp_x, temp_mark);
				server.send_position(temp_y, temp_x, temp_mark);
				
				break;
			}
			
			
			gotoxy(9 - 5, 9 + 5);

			check = map.check_map();
			is_full = map.is_full();
			if (check || !is_full) {
				if (check) {
					cout << "<< " << show_list[check] << "가 이겼습니다. >>" << endl;
					gotoxy(0, 0);
					title_show();
					map.show_map();
				}
				else {
					cout << "<< " << "무승부 입니다. >>" << endl;
					gotoxy(0, 0);
					title_show();
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
				if (!p2.get_is_server()) {
					client.recv_position();
				}
				mark_xy = drop_marker_consol(p2);
				map.set_mark(get<0>(mark_xy), get<1>(mark_xy), p2.get_mark());
				gotoxy(9 - 5, 9 + 5);
				//map.show_map();
			}
			//gotoxy(9 - 2 + 10, 9 + 10);
			//map.show_map();
			check = map.check_map();
			if (check) {
				cout << "<< " << show_list[check] << "가 이겼습니다. >>" << endl;
				gotoxy(0, 0);
				title_show();
				map.show_map();
				turn_count = 0;
				break;



			}
			turn_count--;

		}

		char a = 0;
		cout << "\n  아무 키나 눌러 타이틀로.." << endl;
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
