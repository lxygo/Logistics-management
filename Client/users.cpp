#include"system.h"
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
	int judge;
	while (true)
	{
		cout << "请输入你的密码：" ;
		cin >> password;
		send(client_socket, password.c_str(), password.length(), 0);
		cout << "请再次输入你的密码：";
		cin >> tpassword;
		send(client_socket, tpassword.c_str(), tpassword.length(), 0);
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			break;
		}
		else
		{
			cout << "密码输入错误，请重新输入" << endl;
		}
	}
}
void Users::Change_password() {
	cout << "请输入旧密码" << endl;
	string old_password;
	string new_password;
	int times = 3;
	cin >> old_password;
	while (times)
	{
		if (old_password.compare(password) == 0)
		{
			cout << "请输入新的密码" << endl;
			cin >> new_password;
			while (new_password.compare(password) == 0)
			{
				cout << "两次密码不能重复,请重新输入" << endl;
				cin >> new_password;
			}
			password = new_password;
			break;
		}
		else
		{
			cout << "旧密码输入错误，请重新输入，还能尝试" << times-- << "次" << endl;
			cin >> old_password;
		}
	}
}
void Users::Change_message(SOCKET& client_socket) {
	int judge;
	recv(client_socket, (char*)&judge, 4, 0);
	if (judge == 1)
	{
		cout << "请输入用户姓名：";
		cin >> name;
		send(client_socket, name.c_str(), name.length(), 0);
		cout << "请输入用户电话：";
		cin >> phone;
		send(client_socket, phone.c_str(), phone.length(), 0);
		cout << "请输入用户地址：";
		cin >> address;
		send(client_socket, address.c_str(), address.length(), 0);
	}
	else
	{
		cout << "请选择你想修改的个人信息:" << endl;
		cout << "1.用户姓名 2.用户电话 3.用户地址" << endl;
		int choose;
		cin >> choose;
		send(client_socket, (char*)&choose, 4, 0);
		if (choose == 1)
		{
			cout << "请输入新的姓名" << endl;
			cin >> name;
			send(client_socket, name.c_str(), name.length(), 0);
		}
		else if (choose == 2)
		{
			cout << "请输入新的电话号码" << endl;
			cin >> phone;
			send(client_socket, phone.c_str(), phone.length(), 0);
		}
		else if (choose == 3)
		{
			cout << "请输入新的地址" << endl;
			cin >> address;
			send(client_socket, address.c_str(), address.length(), 0);
		}
	}
}
void Users::Add_money() {
	double add;
	cout << "请输入充值的金额" << endl;
	cin >> add;
	money = money + add;
}
void Users::Show_money()const {
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

