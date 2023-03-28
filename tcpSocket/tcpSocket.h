#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#define PORT 8080
#define err(errmsg) printf("[line:%d] %s falied code %d",__LINE__,errmsg, WSAGetLastError())
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<stdbool.h>
#include<iostream>
#include"stdio.h"
bool init_socket();
bool close_socket();
SOCKET creat_server_socket();
SOCKET creat_client_socket();
#endif // !_TCPSOCKET_H_
