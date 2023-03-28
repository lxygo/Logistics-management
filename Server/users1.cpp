#include"system1.h"
//实现users的成员函数
Users::Users() {
	new_users = true;
	money = 0;
	send_numbers = 0;
	receive_numbers = 0;
	not_re_nums = 0;
}
void Users::Register(string id,SOCKET& client_socket) {
	user_name = id;
	new_users = false;
	string tpassword;
	char buf[BUFSIZ] = { 0 };
	int judge;
	while (true)
	{
		cout << "请输入你的密码" << endl;
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		password = buf;
		cout << "请再次输入你的密码" << endl;
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		tpassword = buf;
		if (tpassword.compare(password) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			break;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "密码输入错误，请重新输入" << endl;
		}
	}
}
void Users::Change_password(SOCKET& client_socket) {
	cout << "请输入旧密码" << endl;
	string old_password;
	string new_password;
	int times = 3;
	int judge;
	char buf[BUFSIZ] = { 0 };
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	old_password = buf;
	while (times)
	{
		if (old_password.compare(password) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "请输入新的密码" << endl;
			memset(buf,0, BUFSIZ);
			recv(client_socket,buf, BUFSIZ, 0);
			password = buf;
			break;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "旧密码输入错误，请重新输入，还能尝试" << times-- << "次" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			old_password = buf;
		}
	}
}
void Users::Change_message(SOCKET& client_socket) {
	int judge;
	char buf[BUFSIZ] = { 0 };
	if (new_users)
	{
		judge = 1;
		send(client_socket, (char*)&judge, 4, 0);
		cout << "请输入用户姓名：";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		name = buf;
		cout << "请输入用户电话：";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		phone = buf;
		cout << "请输入用户地址：";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		address = buf;
	}
	else
	{
		judge = 0;
		send(client_socket, (char*)&judge, 4, 0);
		cout << "请选择你想修改的个人信息:" << endl;
		cout << "1.用户姓名 2.用户电话 3.用户地址" << endl;
		int choose;
		recv(client_socket, (char*)&choose, 4, 0);
		if (choose == 1)
		{
			cout << "请输入新的姓名：" ;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			name = buf;
		}
		else if (choose == 2)
		{
			cout << "请输入新的电话号码：" ;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			phone = buf;
		}
		else if (choose == 3)
		{
			cout << "请输入新的地址：";
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			address = buf;
		}
	}
}
void Users::Add_money(SOCKET& client_socket) {
	double add;
	cout << "请输入充值的金额：" ;
	recv(client_socket, (char*)&add, 8, 0);
	money = money + add;
}
void Users::Show_money(SOCKET& client_socket)const {
	send(client_socket, (char*)&money, 8, 0);
	cout << "余额:" << money << endl;
}
void Users::Storeusers(ofstream& out) {
	out << user_name << endl;
	out << password << endl;
	out << name << endl;
	out << phone << endl;
	out << address << endl;
	out << money << endl;
	out << new_users << endl;
	out << send_numbers << endl;	//发送的快递数量
	out << receive_numbers << endl;//接受的快递数量
	out << not_re_nums << endl;
	int i;
	for (i = 0; i <= send_numbers - 1; i++)
	{
		out << send_package[i] << " ";
	}
	out << endl;
	for (i = 0; i <= receive_numbers - 1; i++)
	{
		out << receive[i] << " ";
	}
	out << endl;
	for (i = 0; i <= not_re_nums - 1; i++)
	{
		out << not_receive[i] << " ";
	}
	out << endl;
}
void Users::Readusers(ifstream& in) {
	in >> user_name;
	in >> password;
	in >> name;
	in >> phone;
	in >> address;
	in >> money;
	in >> new_users;
	in >> send_numbers;	//发送的快递数量
	in >> receive_numbers;//接受的快递数量
	in >> not_re_nums;
	int order, i;
	for (i = 0; i <= send_numbers - 1; i++)
	{
		in >> order;
		send_package.push_back(order);
	}
	for (i = 0; i <= receive_numbers - 1; i++)
	{
		in >> order;
		receive.push_back(order);
	}
	for (i = 0; i <= not_re_nums - 1; i++)
	{
		in >> order;
		not_receive.push_back(order);
	}
}

