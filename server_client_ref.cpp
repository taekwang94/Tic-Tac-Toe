#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define BUFSIZE 1024

using namespace std;

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
		printf("listen() 성공\n");
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
	void close_() {
		closesocket(hSocket);
		WSACleanup();
	}

};

int main() {

	int mode = 0;

	while (1) {
		printf("Server는 1, client는 2를 입력 : ");
		cin >> mode;
		fflush(stdin);
		switch (mode) {
		case 1: {
			Server server;
			server.bind_();
			server.listen_();
			server.accept_();
			server.echo_();
		}

		case 2: {
			Client client;
			client.connect_();
			client.echo_test();
		}

		default: {
			printf("다시 입력하세요.\n");
		}

		}



	}



}