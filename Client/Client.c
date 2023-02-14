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
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		printf("Failed to init winsock 2.2 : error code %d", WSAGetLastError());
		return;
	}
	server.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
	server.sin_port = htons(12345);
	s = socket(server.sin_family, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("Failed to create socket : error code %d", WSAGetLastError());
		return;
	}
	printf("attempting a connection to the server\n");
	if (connect(s, (const struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Failed to connect to server : error code %d", WSAGetLastError());
		return;
	}
	printf("successfully connected, attempting to send \"Hello World !\"\n");
	PCSTR pszMSG = "Hello World !";
	if (send(s, pszMSG, strlen(pszMSG), 0) == SOCKET_ERROR)
	{
		printf("Failed to send message to server : error code %d", WSAGetLastError());
		return;
	}
	printf("Done.\n");
	system("pause>nul");
}
