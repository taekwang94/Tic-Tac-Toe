#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
#include <time.h>
#include <queue>
#include <stack>


#define BUFSIZE 1024

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

using namespace std;

int dy[] = { -1,0,1,0 };
int dx[] = { 0,1,0,-1 };
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

struct Position {
	int x;
	int y;
};

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
		//printf("socket 생성 성공\n");
		//바인딩 : 서버역할수행
		servAddr.sin_family = AF_INET;
		//IP설정 : INADDR_ANY 자신의 IP주소를 획득
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		//Port설정
		servAddr.sin_port = htons(5555);
	}

	void init_() {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//소켓 생성
		hServSock = socket(PF_INET, SOCK_STREAM, 0);
		//printf("socket 생성 성공\n");
		//바인딩 : 서버역할수행
		servAddr.sin_family = AF_INET;
		//IP설정 : INADDR_ANY 자신의 IP주소를 획득
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		//Port설정
		servAddr.sin_port = htons(5555);
	}

	int bind_() {
		int flag;
		flag = bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
		if (!flag) {
			printf("bind() 성공\n");
			return 1;
		}
		else {
			printf("bind() 실패\n");
			return 0;
		}

	}
	int listen_() {
		int flag;
		flag = listen(hServSock, 5);
		if (!flag) {
			printf("listen() 성공\n");
			printf("Client를 기다리는중 ..\n");
			return 1;
		}
		else {
			printf("listen() 실패\n");
			return 0;
		}

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

	SOCKET get_client_sock() {
		return this->hClientSock;
	}

	void send_message() {
		int strLen = 0;
		char message[BUFSIZE] = "";
		//fputs("전송할 메세지를 입력하세요. (q to quit) : ", stdout);
		//fgets(message, BUFSIZE, stdin);
		while ((strLen = recv(hClientSock, message, BUFSIZE, 0)) != 0) {
			printf("recv, check : %s\n", message);
			cin >> message;
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
		strcat(message, "\0");
		printf("send, check : %s\n", message);
		int a;
		a = send(hClientSock, message, strlen(message), 0);
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

	void init_() {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1 자신의 ip 알아서 함
		servAddr.sin_port = htons(5555);
	}
	void set_ip(const char* ip) {
		servAddr.sin_addr.s_addr = inet_addr(ip);
	}
	void set_port(int port_num) {
		servAddr.sin_port = htons(port_num);
	}
	SOCKET get_sock() {
		return this->hSocket;
	}
	int connect_() {
		int c = -1;
		c = connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));
		if (c == 0) {
			cout << "접속 성공! " << endl;
			return 1;
		}
		else {
			cout << "접속 실패" << endl;
			return 0;
		}

	}

	void echo_test() {
		char message[BUFSIZE] = "";
		while (1) {

			//fputs("전송할 메세지를 입력하세요. (q to quit) : ", stdout);
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
		strLen = recv(hSocket, message, BUFSIZE, 0);
		message[strLen] = 0; // 여기서 예외 발생 , strLen : -1 messsage 못받고 있는듯
		printf("서버로 부터 전송된 메세지 :  %s \n", message);
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
		int a;
		a = send(hSocket, message, strlen(message), 0);
		cout << a;
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
	bool visited[3][3];

public:
	Map() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				map[i][j] = 0;
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				visited[i][j] = 0;
			}
		}
	}

	void map_init() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				map[i][j] = 0;
			}
		}
	}

	void visited_init() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				visited[i][j] = 0;
			}
		}
	}

	int get_map_to_arr_x(int x) {
		if (x == 9) return 0;
		else if (x == 13) return 1;
		else if (x == 17) return 2;
	}
	int get_map_to_arr_y(int y) {
		if (y == 9) return 0;
		else if (y == 11) return 1;
		else if (y == 13) return 2;
	}
	int get_arr_to_map_x(int x) {
		if (x == 0) return 9;
		else if (x == 1)return 13;
		else if (x == 2)return 17;
	}
	int get_arr_to_map_y(int y) {
		if (y == 0) return 9;
		else if (y == 1) return 11;
		else if (y == 2)return 13;
	}

	Position find_closest_point_sequential(int y, int x) {
		Position temp;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (!map[i][j]) {
					temp.y = i;
					temp.x = j;
					return temp;
				}
			}
		}
		temp.x = x;
		temp.y = y;
		return temp;
	}
	
	//수정할것
	Position find_closest_point(int y, int x) {
		Position temp;
		int y_;
		int x_;
		stack<pair<int, int>> st;
		st.push(make_pair(y, x));
		visited[y][x] = 1;

		while (!st.empty()) {
			y_ = st.top().first;
			x_ = st.top().second;
			st.pop();
			for (int i = 0; i < 4; i++) {
				y_ = y_ + dy[i];
				x_ = x_ + dx[i];
				if (y_ >= 0 && y_ < 3 && x_ >= 0 && x_ < 3) {
					if (map[y_][x_] != 0 ) {
						visited[y_][x_] = 1;
						st.push(make_pair(y_, x_));
					}
					else {
						temp.x = x_;
						temp.y = y_;
						return temp;
					}
				}
			}
		}
		
		

	}

	void show_map() {
		for (int i = 0; i < 3; i++) {
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
				if (i == 2) {
					printf("%s%c%s\n", GREEN, show_list[map[i][2]], RESET);
				}
				else {
					printf("%s%c%s\n", GREEN, show_list[map[i][2]], RESET);
					//printf("%s%c%s\n", GREEN, show_list[map[i][2]], RESET);
				}
				
			}
			else {
				if (i == 2) {
					printf("%s%c%s\n", RED, show_list[map[i][2]], RESET);
				}
				else {
					printf("%s%c%s\n", RED, show_list[map[i][2]], RESET);
				}
				
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
	void init() {
		mark = 0;
		mark_char = 'O';
		is_ai = false;
		is_server = false;
		player_num = 0;
		position.x = 9;
		position.y = 9;
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

		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis(0, 2);
		y = dis(gen);
		x = dis(gen);


		tuple<int, int, int> v = make_tuple(y, x, this->mark);
		return v;
	}
};

class Game {
private:
	int mode; // 1인, 2인 모드
	int turn; 
	int turn_count; // 9개 공간 차는것 체크
	int play_count; // 몇번 게임 진행했는지 확인. server client 연결 끊겼을 시 0으로 초기화
	bool continue_2p_game; // server client 게임 계속 진행 여부
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
		play_count = 0;
		continue_2p_game = 0;

	}
	~Game() {

	}
	void init_() {
		mode = 0;
		turn = 1;
		turn_count = 9;
		play_count = 0;
		continue_2p_game = 0;
		p1.init();
		p2.init();
		server.init_();
		client.init_();
		map.map_init();
	}
	void player_position_init() {
		p1.set_position(9, 9);
		p2.set_position(9, 9);
	}
	void set_turn_count(int a) {
		this->turn_count = a;
	}
	void set_continue_2p_game(int a) {
		this->continue_2p_game = a;
	}
	bool get_continue_2p_game() {
		return this->continue_2p_game;
	}
	void title_show() {
		printf("\n");
		printf("     #####   #    ####     #####    ###     ####     #####    ###    #####     \n");
		printf("       #     #   #           #     #   #   #           #     #   #   #         \n");
		printf("       #     #   #           #     #####   #           #     #   #   #####     \n");
		printf("       #     #   #           #     #   #   #           #     #   #   #         \n");
		printf("       #     #    ####       #     #   #    ####       #      ###    #####     \n");
		printf("----------------------------------------------------------------------------------\n");
		printf("                                                     ←→↑↓ 및 스페이스바 사용  \n");

	}

	void ip_setting() { // server client 연결하기 위한 client ip 세팅
		title_show();
		int x = 30;
		int y = 12;
		char ip[100] = "";
		int port_num;
		int success = 0;
		while (1) {
			system("cls");
			title_show();
			cout << " IP를 입력하세요 >> ";
			cin >> ip;
			port_num = 5555;

			client.set_ip(ip);
			client.set_port(port_num);
			success = client.connect_();
			cout << endl;
			if (success) {
				break;
			}
		}

	}

	int draw_player_choice() { // 1p, 2p 정하는 함수
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

	int draw_player_ox() { // o, x 선택창
		title_show();
		int x = 30;
		int y = 12;

		gotoxy(x - 2, y - 1);
		cout << "<<  O,X중 사용할것 선택  >>" << endl;
		gotoxy(x - 2, y);
		cout << "> O : Server";
		gotoxy(x, y + 1);
		cout << "X : Client";
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

	tuple<int, int> drop_marker_consol(Player& player) { //선택된 위치에 돌 놓기
		int x = 9;
		int y = 9;
		int left_count = 0;
		int right_count = 0;
		int up_count = 0;
		int down_count = 0;
		char p = player.get_mark_char();
		int player_num = player.get_player_num();

		system("cls");
		title_show();
		map.show_map();
		gotoxy(19, y);
		//cout << "내 차례 입니다" << endl;
		printf("%s내 차례 입니다%s", YELLOW, RESET);
		//gotoxy(5, y + 9);
		
		Position curr;

		int x_arr, y_arr;
		x = player.get_position().x;
		y = player.get_position().y;
		
		x_arr = map.get_map_to_arr_x(x);
		y_arr = map.get_map_to_arr_y(y);
		//curr = map.find_closest_point(y_arr, x_arr);
		curr = map.find_closest_point_sequential(y_arr, x_arr);
		x = map.get_arr_to_map_x(curr.x);
		y = map.get_arr_to_map_y(curr.y);

		gotoxy(x - 2, y);
		cout << ">";
		int x_ = x;
		int y_ = y;
		while (1) {
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
					if (this->mode == 2 && p1.get_mark() == 2) {
						printf("X");
					}
					else {
						printf("%c", p);
					}
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
					if (this->mode == 2 && p1.get_mark() == 2) {
						printf("X");
					}
					else {
						printf("%c", p);
					}
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
					if (this->mode == 2 && p1.get_mark() == 2) {
						printf("X");
					}
					else {
						printf("%c", p);
					}
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
					if (this->mode == 2 && p1.get_mark() == 2) {
						printf("X");
					}
					else {
						printf("%c", p);
					}
					right_count++;
					x_ += 4;
				}
				break;
			}
			case SUBMIT: {
				int y_temp, x_temp;
				y_temp = map.get_map_to_arr_y(y);
				x_temp = map.get_map_to_arr_x(x);
				if (map.get_position_mark(y_temp, x_temp) == 0) {
					tuple<int, int> v = make_tuple(y_temp, x_temp);
					player.set_position(x_, y_);
					return v;
				}
				else {
					gotoxy(19, 13);
					cout << "이미 놓여진 곳입니다." << endl;
				}
			}
			}
		}
	}

	int draw_menu() { // 메인메뉴
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
	void set_mode() { // 모드 선택
		int menu_code = draw_menu();
		while (1) {
			if (menu_code == 0) {
				system("cls");
				this->mode = draw_player_choice() + 1; 
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
	bool set_player_inform() {
		bool input_ox_check = 1;
		system("cls");
		int ox_01 = -10;
		if (play_count == 0) { // 처음 게임 시에만 ip 세팅용
			ox_01 = draw_player_ox();
			if (ox_01 == 1 and mode == 2) { // X 선택한 경우 ip 입력
				ip_setting();
			}
		}
		if (play_count != 0 and mode == 1) {
			ox_01 = draw_player_ox();
		}

		system("cls");
		title_show();
		Player* p;
		if (ox_01 == 0) { // 'O'선택 시 server 역할.
			//여기서 server on
			if (this->get_mode() == 2) {
				int a, b;
				a = server.bind_();
				b = server.listen_();
				if (a == 1 && b == 1) { // bind, reset 성공 시 accept 대기
					server.accept_();
				}
				else {
					cout << "서버 세팅 오류" << endl;
					return 0;
				}
			}
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
		else if (ox_01 == 1) {
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

		return 1;

	}

	bool checker_p1() { // 종료 체크
		int is_full = 0;
		int check;
		check = map.check_map();
		is_full = map.is_full();
		if (check || !is_full) {
			if (check) {
				gotoxy(0, 0);
				title_show();
				map.show_map();
				cout << "  << " << show_list[check] << "가 이겼습니다. >>" << endl;
				return 0;
			}
			else {
				gotoxy(0, 0);
				title_show();
				map.show_map();
				cout << "  << " << "무승부 입니다. >>" << endl;
				return 0;
			}
			turn_count = 0;
			return 1;
		}
	}

	bool checker_p2() { // 후공 종료 체크 
		int check;
		check = map.check_map();
		if (check) {
			cout << "  << " << show_list[check] << "가 이겼습니다. >>" << endl;
			gotoxy(0, 0);
			title_show();
			map.show_map();
			turn_count = 0;
			return 0;
		}
		return 1;
	}

	void play() { // 게임 진행 함수
		bool gaming = 1;
		bool end_checker = 1;
		bool server_setting = 0;

		if (!this->turn_count) {
			return;
		}
		if (!continue_2p_game) {
			set_mode();
			server.close_();
			client.close_();
			WSACleanup();
			server.init_();
			client.init_();
			server_setting = set_player_inform(); // server client connection 완료	
		}
		else {
			server_setting = 1;
		}
		system("cls");
		title_show();
		map.map_init();
		switch (this->get_mode()) {
		case 1: // 1인용
		{
			while (turn_count) {
				if (this->turn_count < 0) {
					return;
				}
				tuple<int, int, int> p;
				tuple<int, int> mark_xy;
				bool drop_success = 0;
				char a = ' ';
				//P1 , p1 이 서버
				mark_xy = drop_marker_consol(p1);
				int temp_y = get<0>(mark_xy);
				int temp_x = get<1>(mark_xy);
				int temp_mark = p1.get_mark();
				map.set_mark(temp_y, temp_x, temp_mark);
				//server.send_position(temp_y, temp_x, temp_mark);
				gotoxy(9 - 5, 9 + 5);

				end_checker = checker_p1();
				if (!end_checker) {
					cout << "\n  아무 키나 눌러 타이틀로.." << endl;
					a = _getch();
					if (a) {
						return;
					}
					return;
				}
				//P2
				do {
					p = p2.drop_mark();
					drop_success = map.set_mark(get<0>(p), get<1>(p), get<2>(p));
				} while (!drop_success);

				end_checker = checker_p2();
				if (!end_checker) {
					cout << "\n  아무 키나 눌러 타이틀로.." << endl;
					a = _getch();
					if (a) {
						return;
					}
					return;
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
		case 2: // 2인용
		{
			if (server_setting) {
				tuple<int, int, int> p;
				tuple<int, int> mark_xy;  //.
				int temp_y, temp_x, temp_mark;
				bool drop_success = 0;

				if (p1.get_mark() == 1) { // p1 server mark : "O"
					int strLen = 0;
					char a = 0;
					char message[BUFSIZE] = "";
					char winning_check[3] = "";
					system("cls");
					title_show();
					map.show_map();
					gotoxy(19, 9);
					cout << " 상대방을 기다리는 중입니다" << endl;
					while (1) {
						if (!(strLen = recv(server.get_client_sock(), message, BUFSIZE, 0)) != 0) {
							break;
						}
						if (strLen == -1) {
							system("cls");
							title_show();
							gotoxy(19, 9);
							cout << " 상대방 접속이 끊어졌습니다. 상대를 기다리려면 q, 타이틀로 가려면 아무키나 누르세요" << endl;
							server.close_();
							client.close_();
							WSACleanup();
							server.init_();
							client.init_();
							a = _getch();
							if (a == 'q') {
								int p, q;
								p = server.bind_();
								q = server.listen_();
								if (p == 1 && q == 1) {
									server.accept_();
								}
								system("cls");
								map.map_init();
								title_show();
								map.show_map();
								gotoxy(19, 9);
								cout << " 상대방을 기다리는 중입니다" << endl;

								continue;
							}
							else if (a != 'q' and a) {
								play_count = 0;
								continue_2p_game = 0;
								return;
							}

						}
						memset(winning_check, 0, sizeof(winning_check));
						// recv로 map 세팅.
						map.set_mark(message[0] - '0', message[1] - '0', message[2] - '0');
						memset(message, 0, sizeof(message));
						system("cls");
						//map.show_map();

						// end check
						end_checker = checker_p1();
						if (!end_checker || (message[3] == '1')) {
							char a = 0;
							cout << "\n 재대결을 원하면 q, 아니면 아무 키나 눌러 타이틀로.." << endl;
							a = _getch();
							if (a == 'q') {
								memset(message, 0, sizeof(message));
								strcat(message, "q");
								send(server.get_client_sock(), message, strlen(message), 0);
								cout << "상대방 응답을 기다리는 중입니다" << endl;
								while ((strLen = recv(server.get_client_sock(), message, BUFSIZE, 0)) != 0) {	
									
									if (!strcmp(message, "q")) { // 재대결
										continue_2p_game = 1;
										return;
									}
									else {
										continue_2p_game = 0;
										cout << "상대방이 나갔습니다." << endl;
										play_count = 0;
										cout << "아무키나 눌러 타이틀로.." << endl;
										a = _getch();
										
										if (a) {
											return;
										}
									}
								}
							}
							else {
								send(server.get_client_sock(), "상대방이 나갔습니다.", strlen("상대방이 나갔습니다."), 0);
								continue_2p_game = 0;
								play_count = 0;
								cout << "아무키나 눌러 타이틀로.." << endl;
								a = _getch();
								
								if (a) {
									return;
								}
							}
						}
						memset(message, 0, sizeof(message));
						mark_xy = drop_marker_consol(p1);

						temp_y = get<0>(mark_xy);
						temp_x = get<1>(mark_xy);
						temp_mark = p1.get_mark();
						map.set_mark(temp_y, temp_x, temp_mark);

						//end check
						end_checker = checker_p1();
						if (!end_checker) {
							strcat(winning_check, "1");
						}
						else {
							strcat(winning_check, "0");
							gotoxy(19, 9);
							cout << " 상대방을 기다리는 중입니다" << endl;
						}

						char y_[10] = "";
						char x_[10] = "";
						char mark_[10] = "";
						sprintf(y_, "%d", temp_y);
						sprintf(x_, "%d", temp_x);
						sprintf(mark_, "%d", temp_mark);

						strcat(message, y_);
						strcat(message, x_);
						strcat(message, mark_);
						strcat(message, winning_check);

						send(server.get_client_sock(), message, strlen(message), 0);
						if (!end_checker || message[3] == '1') {
							char a = 0;
							cout << "\n 재대결을 원하면 q, 아니면 아무 키나 눌러 타이틀로.." << endl;
							a = _getch();
							if (a == 'q') {
								memset(message, 0, sizeof(message));
								strcat(message, "q");
								send(server.get_client_sock(), message, strlen(message), 0);
								cout << "상대방 응답을 기다리는 중입니다" << endl;
								while ((strLen = recv(server.get_client_sock(), message, BUFSIZE, 0)) != 0) {
									
									if (!strcmp(message, "q")) { // 재대결
										continue_2p_game = 1;
										return;
									}
									else {
										continue_2p_game = 0;
										cout << "상대방이 나갔습니다." << endl;
										play_count = 0;
										cout << "아무키나 눌러 타이틀로.." << endl;
										a = _getch();
										
										if (a) {
											return;
										}
									}
								}
							}
							else {
								send(server.get_client_sock(), "상대방이 나갔습니다.", strlen("상대방이 나갔습니다."), 0);
								continue_2p_game = 0;
								play_count = 0;
								cout << "아무키나 눌러 타이틀로.." << endl;
								a = _getch();
								
								if (a) {
									return;
								}
							}

						}
						memset(message, 0, sizeof(message));
					}
				}
				else {                    // client mark : "X"
					char message[BUFSIZE] = "";
					char winning_check[3] = "";
					char a = 0;
					while (1) {
						memset(winning_check, 0, sizeof(winning_check));
						mark_xy = drop_marker_consol(p1);
						temp_y = get<0>(mark_xy);
						temp_x = get<1>(mark_xy);
						temp_mark = p1.get_mark();
						map.set_mark(temp_y, temp_x, temp_mark);
						end_checker = checker_p1();
						if (!end_checker) {
							strcat(winning_check, "1");
						}
						else {
							strcat(winning_check, "0");
							gotoxy(19, 9);
							cout << " 상대방을 기다리는 중입니다" << endl;
						}

						char y_[10] = "";
						char x_[10] = "";
						char mark_[10] = "";
						sprintf(y_, "%d", temp_y);
						sprintf(x_, "%d", temp_x);
						sprintf(mark_, "%d", temp_mark);

						strcat(message, y_);
						strcat(message, x_);
						strcat(message, mark_);
						strcat(message, winning_check);


						//전송
						int strLen = send(client.get_sock(), message, strlen(message), 0);

						// end check
						if (!end_checker || (message[3] == '1')) {
							char a = 0;
							cout << "\n 재대결을 원하면 q, 아니면 아무 키나 눌러 타이틀로.." << endl;
							a = _getch();
							if (a == 'q') {
								memset(message, 0, sizeof(message));
								strcat(message, "q");
								send(client.get_sock(), message, strlen(message), 0);
								cout << "상대방 응답을 기다리는 중입니다" << endl;
								while ((strLen = recv(client.get_sock(), message, BUFSIZE - 1, 0)) != 0) {							
									if (!strcmp(message, "q")) { // 재대결
										continue_2p_game = 1;
										return;
									}
									else {
										continue_2p_game = 0;
										cout << "상대방이 나갔습니다." << endl;
										play_count = 0;
										cout << "아무키나 눌러 타이틀로.." << endl;
										a = _getch();	
										if (a) {
											return;
										}
									}
								}
							}
							else {
								send(client.get_sock(), "상대방이 나갔습니다.", strlen("상대방이 나갔습니다."), 0);
								continue_2p_game = 0;
								play_count = 0;
								cout << "아무키나 눌러 타이틀로.." << endl;
								a = _getch();
								if (a) {
									return;
								}
							}
						}
						memset(message, 0, sizeof(message));
						//받기
						strLen = recv(client.get_sock(), message, BUFSIZE - 1, 0); // 동기, 받은게 message로 들어감
						if (strLen == -1) { // 접속 끊김.
							system("cls");
							title_show();
							gotoxy(19, 9);
							cout << " 서버와의 연결이 끊어졌습니다. 아무키나 눌러 타이틀로.." << endl;
							client.close_();
							server.close_();
							WSACleanup();
							server.init_();
							client.init_();
							a = _getch();
							if (a) {
								continue_2p_game = 0;
								play_count = 0;
								return;
							}
						}
						map.set_mark(message[0] - '0', message[1] - '0', message[2] - '0');
						system("cls");

						// end check
						end_checker = checker_p1();
						if (!end_checker || (message[3] == '1')) {
							char a = 0;
							cout << "\n 재대결을 원하면 q, 아니면 아무 키나 눌러 타이틀로.." << endl;
							a = _getch();
							if (a == 'q') {
								memset(message, 0, sizeof(message));
								strcat(message, "q");
								send(client.get_sock(), message, strlen(message), 0);
								cout << "상대방 응답을 기다리는 중입니다" << endl;
								while ((strLen = recv(client.get_sock(), message, BUFSIZE - 1, 0)) != 0) {
									
									if (!strcmp(message, "q")) { // 재대결
										continue_2p_game = 1;
										return;
									}
									else {
										continue_2p_game = 0;
										cout << "상대방이 나갔습니다." << endl;
										play_count = 0;
										cout << "아무키나 눌러 타이틀로.." << endl;
										a = _getch();	
										if (a) {
											return;
										}
										
									}
								}
							}
							else { 
								send(client.get_sock(), "상대방이 나갔습니다.", strlen("상대방이 나갔습니다."), 0);
								continue_2p_game = 0;
								cout << "아무키나 눌러 타이틀로.." << endl;
								a = _getch();					
								if (a) {
									return;
								}
								
							}
						}
						message[strLen] = 0;
						//printf("서버로 부터 전송된 메세지 :  %s \n", message);
						memset(message, 0, sizeof(message));
						if (a) {
							play_count++;
							return;
						}
					}
					server.close_();
					client.close_();
					WSACleanup();

				}
			}
			else {
				cout << "error" << endl;
				return;
			}
		}
		}
	}

};




int main() {
	system("cls");
	Game game;
	bool is_continue = 0;
	while (1) {
		game.play();
		system("cls");
		game.set_turn_count(9);
		game.player_position_init();
		is_continue = game.get_continue_2p_game();
	}
	return 0;
}
