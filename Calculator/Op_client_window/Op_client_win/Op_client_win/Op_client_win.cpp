﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment (lib,"ws2_32.lib")
#pragma warning(disable: 4996) // scanf 오류 무시
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void Handle_Error(const char* message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char opmsg[BUF_SIZE];
	int result, opndCnt, i;
	SOCKADDR_IN serv_address;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Handle_Error("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		Handle_Error("socket() Error");
	}

	memset(&serv_address, 0, sizeof(serv_address));
	serv_address.sin_family = AF_INET;
	serv_address.sin_addr.s_addr = inet_addr(argv[1]);
	serv_address.sin_port = htons(atoi(argv[2]));

	if(connect(hSocket, (SOCKADDR*)&serv_address, sizeof(serv_address)) == SOCKET_ERROR)
	{
		Handle_Error("connect() error!");
	}
	else
	{
		puts("Connected...............");
	}

	fputs("Operand count: ", stdout);
	scanf("%d", &opndCnt);
	opmsg[0] = (char)opndCnt;


	for (i = 0; i < opndCnt; i++)
	{
		printf("Operand %d: ", i + 1);
		scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
	}
	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opmsg[opndCnt*OPSZ + 1]);
	send(hSocket, opmsg, opndCnt*OPSZ + 2, 0);
	recv(hSocket, (char *)&result, RLT_SIZE, 0);

	printf("Operation result: %d\n", result);
	closesocket(hSocket);
	WSACleanup();
	return 0;

}

void Handle_Error(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}