#include"../tcpSocket/tcpSocket.h"
#include"../Server/system1.h"
using namespace std;
int main()
{
	init_socket();
	bool flag = true;
	SOCKET server_socket = creat_server_socket();
	char buf[BUFSIZ] = { 0 };
	int choose1;//局部变量不影响外部的choose
	cout << "wait client" << endl;
	SOCKET client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		err("accept");
	}//完成通信的连接

	cout << "欢迎使用物流管理系统" << endl;
	System Logsystem;
	Logsystem.Read();
	string id;
	string password;
	int order = -1;//序号
	int service;//选择的服务方式
	int choose;
	int i;
	bool flag1 = true;

	while (flag1)
	{
		cout << "请选择你要登录的方式：" << endl;
		cout << "1.直接登录 2.新用户注册 3.管理员登录 4.快递员登录 5.退出系统" << endl;
		recv(client_socket,(char*)&choose, 4, 0);
		cout << choose;//添加输出
		if (choose == 1)
		{
			cout << "请输入你的用户名:";
			memset(buf, 0, BUFSIZ);
			recv(client_socket,buf, BUFSIZ, 0);
			id = buf;
			order = Logsystem.Finduser(id);
			send(client_socket, (char*)&order,4, 0);
			if (order == -1)
			{
				cout << "未查询到相关用户,请进行注册" << endl;
			}
			else
			{
				flag = Logsystem.Log(order,client_socket);
				while (flag)
				{
					cout << "\n请选择你需要的服务：" << endl;
					cout << "1.查询快递信息 2.寄送快递 3.签收快递 " << endl;
					cout << "4.修改账户密码 5.进行账户充值 6.账户余额查询 7.修改个人信息 8 退出系统" << endl;
					recv(client_socket, (char*)&service, 4, 0);
					cout << service << endl;//hsuchu
					if (service == 1)
					{
						cout << "请选择你需要的查询服务：" << endl;
						cout << "1.查询全部快递信息 2.通过时间进行查询 3.通过快递单号进行查询" << endl;
						recv(client_socket, (char*)&choose1, 4, 0);
						if (choose1 == 1)
						{
							Logsystem.Show(order,client_socket);
						}
						else if (choose1 == 2)
						{
							Logsystem.Showbytime(order,client_socket);
						}
						else if (choose1 == 3)
						{
							Logsystem.Showbynum(order,client_socket);
						}
					}
					else if (service == 2)
					{
						string receiver;
						cout << endl;
						cout << "请输入收件人id：";
						memset(buf, 0, BUFSIZ);
						recv(client_socket, buf, BUFSIZ, 0);
						receiver = buf;
						int  receive_order;
						receive_order = Logsystem.Finduser(receiver);
						send(client_socket, (char*)&receive_order, 4, 0);
						Logsystem.Send(order, receive_order, client_socket);
					}
					else if (service == 3)
					{
						Logsystem.Receive(order, client_socket);
					}
					else if (service == 4)
					{
						Logsystem.allusers[order].Change_password(client_socket);
					}
					else if (service == 5)
					{
						Logsystem.allusers[order].Add_money(client_socket);
					}
					else if (service == 6)
					{
						Logsystem.allusers[order].Show_money(client_socket);
					}
					else if (service == 7)
					{
						Logsystem.allusers[order].Change_message(client_socket);
					}
					else if (service == 8)
					{
						flag = false;
						flag1 = false;
					}
					else
					{
						cout << "无效输入" << endl;
						cin.clear();
						cin.ignore((std::numeric_limits< streamsize >::max)(), '\n');
					}
					cout << endl;
				}
			}
		}
		else if (choose == 2)
		{
			cout << "请输入您想要id：";
			string id;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			id = buf;
			order = Logsystem.Finduser(id);
			send(client_socket, (char*)&order, 4, 0);
			if (order == -1)
			{
				Users new_user;
				new_user.Change_message(client_socket);
				new_user.Register(id,client_socket);
				new_user.Add_money(client_socket);
				order = Logsystem.users_num;
				Logsystem.allusers.push_back(new_user);
				Logsystem.users_num++;
			}
			else
			{
				cout << "该id已注册" << endl;
			}
		}
		else if (choose == 3)
		{
			int judge;
			cout << "请输入你的用户名：";
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			id = buf;
			if (id.compare(Logsystem.alladmin[0].user_name) == 0)
			{
				judge = 1;
				send(client_socket, (char*)&judge, 4, 0);
				flag = Logsystem.Adminlog(client_socket);
				while (flag)
				{
					cout << "\n请选择你要进行的服务:" << endl;
					cout << "1.查询某一用户信息  2.查询所有用户信息  3.通过用户查询快递  4.通过时间查询快递  5.通过快递单号查询快递" << endl;
					cout << "6.修改管理员姓名  7.修改管理员密码  8.管理快递员 9.退出" << endl;
					recv(client_socket, (char*)&service, 4, 0);
					if (service == 1)
					{
						Logsystem.Adshowbyuser(client_socket);
					}
					else if (service == 2)
					{
						Logsystem.Adshowbyallusers(client_socket);
					}
					else if (service == 3)
					{
						Logsystem.Adshowbyid(client_socket);
					}
					else if (service == 4)
					{
						Logsystem.Adshowbytime(client_socket);
					}
					else if (service == 5)
					{
						Logsystem.Adshowbynum(client_socket);
					}
					else if (service == 6)
					{
						Logsystem.alladmin[0].Changename(client_socket);
					}
					else if (service == 7)
					{
						Logsystem.alladmin[0].Changepassword(client_socket);
					}
					else if (service == 8)
					{
						cout << "请选择你要进行的服务:";
						cout << "1.查看快递员信息 2.查询其揽收的快递信息 3.添加快递员 4.删除快递员 5.分配揽收快递 " << endl;
						int choose1;
						recv(client_socket, (char*)&choose1, 4, 0);
						if (choose1 == 1)
						{
							Logsystem.Adshowcourier(client_socket);
						}
						else if (choose1 == 2)
						{
							Logsystem.Adshowcollect(client_socket);
						}
						else if (choose1 == 3)
						{
							Logsystem.Addcourier(client_socket);
						}
						else if (choose1 == 4)
						{
							Logsystem.Deletecourier(client_socket);
						}
						else if (choose1 == 5)
						{
							Logsystem.Assign(client_socket);
						}
					}
					else if (service == 9)
					{
						flag = false;
						flag1 = false;
					}
					else
					{
						cout << "无效输入" << endl;
						cin.clear();
						cin.ignore((std::numeric_limits< streamsize >::max)(), '\n');
					}
					cout << endl;
				}
			}
			else
			{
				judge = 0;
				send(client_socket, (char*)&judge, 4, 0);
				cout << "该用户不是管理员" << endl;
			}
		}
		else if (choose == 4)
		{
			string id;
			int choose1;
			cout << "请输入你的用户名：";
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			id = buf;
			order = Logsystem.Findcourier(id);
			send(client_socket, (char*)&order, 4, 0);
			if (order != -1)
			{
				flag = Logsystem.Courierlog(id,client_socket);
				while (flag)
				{
					cout << "登录成功，欢迎回来" << endl;
					cout << "请选择你要进行的服务：";
					cout << "1.查询待揽收的快递 2.查询所有揽收的快递 3.揽收快递 4.查询快递 5.退出系统";
					cout << endl;
					recv(client_socket, (char*)&choose1, 4, 0);
					if (choose1 == 1)
					{
						Logsystem.Coshow_not_collect(order,client_socket);
					}
					else if (choose1 == 2)
					{
						Logsystem.CoSearch(order,client_socket);
					}
					else if (choose1 == 3)
					{
						Logsystem.Cocollect(order,client_socket);
					}
					else if (choose1 == 4)
					{
						int choose2;
						flag = true;
						while (flag)
						{
							cout << "请输入你要查询的方式：" << endl;
							cout << "1.快递单号查询  2.时间查询  3.寄件人查询  4.收件人查询  5.退出" << endl;
							recv(client_socket, (char*)&choose2, 4, 0);
							if (choose2 == 1)
							{
								Logsystem.Coshowbynum(order,client_socket);
							}
							else if (choose2 == 2)
							{
								Logsystem.Coshowbytime(order,client_socket);
							}
							else if (choose2 == 3)
							{
								Logsystem.Coshowbysend(order,client_socket);
							}
							else if (choose2 == 4)
							{
								Logsystem.Coshowbyreceive(order,client_socket);
							}
							else if (choose2 == 5)
							{
								flag = false;
							}
						}
					}
					else if (choose1 == 5)
					{
						flag = false;
						flag1 = false;
					}

				}
			}
		}
		else if (choose == 5)
		{
			flag1 = false;
		}
		else
		{
			cout << "无效输入" << endl;
			cin.clear();
			cin.ignore((std::numeric_limits< streamsize >::max)(), '\n');
		}
	}
	Logsystem.Store();
	cout << "感谢您使用本物流管理系统" << endl;
	closesocket(client_socket);
	closesocket(server_socket);
	close_socket();
}