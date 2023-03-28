#include "tcpSocket.h"
using namespace std;
bool init_socket()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		printf("WSAStartup falied code %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool close_socket()
{
	if (WSACleanup() != 0)
	{
		printf("WSACleanup falied code %d", WSAGetLastError());
		return false;
	}
	return true;
}

SOCKET creat_server_socket()
{
	//创建socket
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		err("socket");
	}
	//绑定ip和端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	int r = bind(server,(struct sockaddr *) &addr, sizeof(addr));
	if (r != 0)
	{
		err("bind");
	}
	//监听
	listen(server, 10);
	return server;
}

SOCKET creat_client_socket()
{
	//创建socket
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET)
	{
		err("socket");
	}
	//建立连接
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int r = connect(client_socket,(struct sockaddr *)&addr, sizeof(addr));
	if (r != 0)
	{
		err("connect");
	}
	return client_socket;
}
