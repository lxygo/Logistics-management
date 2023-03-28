#include"../tcpSocket/tcpSocket.h"
#include"../Client/system.h"
using namespace std;
int main()
{
	init_socket();
	bool flag1 = true;
	int choose;
	SOCKET client_socket = creat_client_socket();
	char buf[BUFSIZ] = { 0 };
	string id;
	int order;
	bool flag;
	int service;
	int choose1;//局部变量不影响外部的choose
	System Logsystem;
	cout << "欢迎使用物流管理系统" << endl;
	while (flag1)
	{
		cout << "请选择你要登录的方式：" << endl;
		cout << "1.直接登录 2.新用户注册 3.管理员登录 4.快递员登录 5.退出系统" << endl;
		cin >> choose;//输入选择的方式
		send(client_socket, (char*)&choose , 4, 0);
		if (choose == 1)
		{
			cout << "请输入你的用户名:" ;
			cin >> id;
			send(client_socket, id.c_str(), id.length(), 0);
			recv(client_socket, (char*)&order, 4, 0);
			if (order == -1)
			{
				cout << "未查询到相关用户,请进行注册." << endl;
			}
			else
			{
				flag = Logsystem.Log(order,client_socket);
				while (flag)
				{
					cout << "\n请选择你需要的服务：" << endl;
					cout << "1.查询快递信息 2.寄送快递 3.签收快递 " << endl;
					cout << "4.修改账户密码 5.进行账户充值 6.账户余额查询 7.修改个人信息 8.退出系统" << endl;
					cin >> service;
					send(client_socket, (char*)&service, 4, 0);
					if (service == 1)
					{
						cout << "请选择你需要的查询服务：" << endl;
						cout << "1.查询全部快递信息 2.通过时间进行查询 3.通过快递单号进行查询" << endl;
						cin >> choose1;
						send(client_socket, (char*)&choose1, 4, 0);
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
						cin >> receiver;
						send(client_socket, receiver.c_str(), receiver.length(), 0);
						int  receive_order;
						recv(client_socket, (char*)&receive_order, 4, 0);
						Logsystem.Send(order, receive_order,client_socket);
					}
					else if (service == 3)
					{
						Logsystem.Receive(order, client_socket);
					}
					else if (service == 4)
					{
						cout << "请输入旧密码：" ;
						string old_password;
						string new_password;
						int times = 3;
						int judge;
						cin >> old_password;
						send(client_socket, old_password.c_str(), old_password.length(), 0);
						while (times)
						{
							recv(client_socket, (char*)&judge, 4, 0);
							if (judge == 1)
							{
								cout << "请输入新的密码：" ;
								cin >> new_password;
								send(client_socket, new_password.c_str(), new_password.length(), 0);
								break;
							}
							else
							{
								cout << "旧密码输入错误，请重新输入，还能尝试" << times-- << "次" << endl;
								cout << "请输入旧密码：";
								cin >> old_password;
								send(client_socket, old_password.c_str(), old_password.length(),0);
							}
						}
					}
					else if (service == 5)
					{
						double add;
						cout << "请输入充值的金额：";
						cin >> add;
						send(client_socket, (char*)&add, 8, 0);
					}
					else if (service == 6)
					{
						double money;
						recv(client_socket, (char*)&money, 8, 0);
						cout << "余额：" << money << endl;
					}
					else if (service == 7)
					{
						string name, phone, address;
						cout << "请选择你想修改的个人信息:" << endl;
						cout << "1.用户姓名 2.用户电话 3.用户地址" << endl;
						int choose;
						cin >> choose;
						send(client_socket, (char*)&choose, 4, 0);
						if (choose == 1)
						{
							cout << "请输入新的姓名：" ;
							cin >> name;
							send(client_socket, name.c_str(), name.length(), 0);
						}
						else if (choose == 2)
						{
							cout << "请输入新的电话号码：";
							cin >> phone;
							send(client_socket, phone.c_str(), phone.length(), 0);
						}
						else if (choose == 3)
						{
							cout << "请输入新的地址：" ;
							cin >> address;
							send(client_socket, address.c_str(), address.length(), 0);
						}
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
			cout << "请输入您想要id：" ;
			string id;
			id.clear();
			cin >> id;
			send(client_socket, id.c_str(), id.length(), 0);
			recv(client_socket, (char*)&order, 4, 0);
			if (order == -1)
			{
				Users new_user;
				new_user.Change_message(client_socket);
				new_user.Register(id,client_socket);
				double add;
				cout << "请输入充值的金额：";
				cin >> add;
				send(client_socket, (char*)&add, 8, 0);
				cout << endl;
			}
			else
			{
				cout << "该id已注册" << endl;
			}
		}
		else if (choose == 3)
		{
			int judge;
			cout << "请输入你的用户名：" ;
			cin >> id;
			send(client_socket, id.c_str(), id.length(), 0);
			recv(client_socket, (char*)&judge, 4, 0);
			if (judge == 1)
			{
				flag = Logsystem.Adminlog(client_socket);
				while (flag)
				{
					cout << "\n请选择你要进行的服务:" << endl;
					cout << "1.查询某一用户信息  2.查询所有用户信息  3.通过用户查询快递  4.通过时间查询快递  5.通过快递单号查询快递" << endl;
					cout << "6.修改管理员姓名  7.修改管理员密码  8.管理快递员 9.退出" << endl;
					cin >> service;
					send(client_socket, (char*)&service, 4, 0);
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
						string name;
						cout << "请输入你的新名字：" ;
						cin >> name;
						send(client_socket, name.c_str(), name.length(), 0);
					}
					else if (service == 7)
					{
						int times = 3;
						int judge;
						string t_password;
						string new_password;
						while (times > 0)
						{
							cout << "请输入你的旧密码：";
							cin >> t_password;
							send(client_socket, t_password.c_str(), t_password.length(), 0);
							recv(client_socket, (char*)&judge, 4, 0);
							if (judge == 1)
							{
								cout << "请输入新的密码：";
								cin >> new_password;
								send(client_socket, new_password.c_str(), new_password.length(), 0);
								break;
							}
							else
							{
								cout << "密码输入错误，还能输入" << times << "次"<<endl;
								times--;
							}
						}
					}
					else if (service == 8)
					{
						cout << "请选择你要进行的服务:"<<endl;
						cout << "1.查看快递员信息 2.查询其揽收的快递信息 3.添加快递员 4.删除快递员 5.分配揽收快递 " << endl;
						int choose1;
						cin >> choose1;
						send(client_socket, (char*)&choose1, 4, 0);
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
				cout << "该用户不是管理员" << endl;
			}
		}
		else if (choose == 4)
		{
			string id;
			int choose1;
			cout << "请输入你的用户名：";
			cin >> id;
			send(client_socket, id.c_str(), id.length(), 0);
			recv(client_socket, (char*)&order, 4, 0);
			if (order != -1)
			{
				flag = Logsystem.Courierlog(id,client_socket);
				while (flag)
				{
					cout << "请选择你要进行的服务："<<endl;
					cout << "1.查询待揽收的快递 2.查询所有揽收的快递 3.揽收快递 4.查询快递 5.退出系统";
					cout << endl;
					cin >> choose1;
					send(client_socket, (char*)&choose1, 4, 0);
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
							cin >> choose2;
							send(client_socket, (char*)&choose2, 4, 0);
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
	cout << "感谢您使用本物流管理系统" << endl;
	closesocket(client_socket);
	close_socket();
}