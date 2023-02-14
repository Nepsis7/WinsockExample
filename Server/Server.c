#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

void Cleanup(SOCKET* pS)
{
	if (pS)
		closesocket(*pS);
	WSACleanup();
}

void main()
{
	struct WSAData wsadata;
	struct sockaddr_in server;
	SOCKET s;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR)
	{
		printf("Failed to init winsock 2.2 : error code %d", WSAGetLastError());
		Cleanup(&s);
		return;
	}
	server.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
	server.sin_port = htons(12345);
	s = socket(server.sin_family, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("Failed to create socket : error code %d", WSAGetLastError());
		Cleanup(&s);
		return;
	}
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed : error code %d", WSAGetLastError());
		Cleanup(&s);
		return;
	}
	listen(s, 0);
	printf("listening for connections\n");
	int cbServer = sizeof(server);
	SOCKET client = accept(s, (struct sockaddr*)&server, &cbServer);
	if (client == SOCKET_ERROR)
	{
		printf("Accept failed : error code %d", WSAGetLastError());
		Cleanup(&s);
		return;
	}
	printf("Connected with a client\n");
	printf("Receiving message\n");
	char buffer[1024];
	memset(buffer, 0, sizeof(char[1024]));
	int cReceivedBytes = recv(client, buffer, 1024, 0);
	closesocket(client);
	if (cReceivedBytes == SOCKET_ERROR)
	{
		printf("Failed to receive : error code %d", WSAGetLastError());
		Cleanup(&s);
		return;
	}
	printf("received msg : %s\n", buffer);
	Cleanup(&s);
	printf("Done.\n");
	system("pause>nul");
}
