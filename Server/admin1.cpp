#include"system1.h"

//实现管理员类的函数
Administrator::Administrator() {
	money = 0;
}
void Administrator::Changename(SOCKET& client_socket)
{
	char buf[BUFSIZ] = { 0 };
	cout << "请输入你的新名字" << endl;
	recv(client_socket, buf, BUFSIZ, 0);
	name = buf;
}
void Administrator::Changepassword(SOCKET& client_socket)
{
	int times = 3;
	int judge;
	string t_password;
	char buf[BUFSIZ] = { 0 };
	while (times > 0)
	{
		cout << "请输入你的旧密码:";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		t_password = buf;
		if (t_password.compare(password) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "请输入新的密码:";
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			password = buf;
			break;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "密码输入错误，还能输入" << times << "次";
			times--;
		}
	}
}
void Administrator::Readadmin(ifstream& in)
{
	in >> user_name;
	in >> name;
	in >> password;
	in >> money;
}
void Administrator::Storeadmin(ofstream& out)
{
	out << user_name << endl;
	out << name << endl;
	out << password << endl;
	out << money << endl;
}