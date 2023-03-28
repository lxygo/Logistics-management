#include"system1.h"
#include"../tcpSocket/tcpSocket.h"
using namespace std;
System::System() {
	admin_num = 1;
	users_num = 0;
	packages_num = 0;
	courier_num = 0;
	not_assgin_num = 0;
}
void System::Store() {
	ofstream out;
	int i;
	out.open("用户.txt", ios::out);
	if (out.is_open())
	{
		out << users_num << endl;
		for (i = 0; i <= users_num - 1; i++)
		{
			allusers[i].Storeusers(out);
		}
		out.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
	out.open("快递.txt", ios::out);
	if (out.is_open())
	{
		out << packages_num << endl;
		out << not_assgin_num << endl;
		for (i = 0; i <= packages_num - 1; i++)
		{
			allpackages[i].StorePackage(out);
		}
		out << endl;
		for (i = 0; i <= not_assgin_num - 1; i++)
		{
			out << not_assign[i] << " ";
		}
		out << endl;
		out.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
	out.open("管理员.txt", ios::out);
	if (out.is_open())
	{
		out << admin_num << endl;
		alladmin[0].Storeadmin(out);
		out.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
	out.open("快递员.txt", ios::out);
	if (out.is_open())
	{
		out << courier_num << endl;
		for (i = 0; i <= courier_num - 1; i++)
		{
			allcouriers[i].Storecour(out);
		}
		out.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
}
void System::Read() {
	ifstream in;
	int i;
	in.open("用户.txt", ios::in);
	if (in.is_open())
	{
		in >> users_num;
		for (i = 0; i <= users_num - 1; i++)
		{
			Users new_user;
			new_user.Readusers(in);
			allusers.push_back(new_user);
		}
		in.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
	in.open("快递.txt", ios::in);
	if (in.is_open())
	{
		in >> packages_num;
		in >> not_assgin_num;
		for (i = 0; i <= packages_num - 1; i++)
		{
			Package new_package;
			new_package.ReadPackage(in);
			allpackages.push_back(new_package);
		}
		for (i = 0; i <= not_assgin_num - 1; i++)
		{
			int num;
			in >> num;
			not_assign.push_back(num);
		}
		in.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
	in.open("管理员.txt", ios::in);
	if (in.is_open())
	{
		in >> admin_num;
		Administrator new_admin;
		new_admin.Readadmin(in);
		alladmin.push_back(new_admin);
		in.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
	in.open("快递员.txt", ios::in);
	if (in.is_open())
	{
		in >> courier_num;
		for (i = 0; i <= courier_num - 1; i++)
		{
			Courier new_courier;
			new_courier.Readcour(in);
			allcouriers.push_back(new_courier);
		}
		in.close();
	}
	else
	{
		cout << "文件打开失败" << endl;
	}
}
int System::Finduser(string user_name) {
	int i;
	for (i = 0; i <= users_num - 1; i++)
	{
		if (user_name.compare(allusers[i].user_name) == 0)
		{
			return i;
		}
	}
	return -1;
}
int System::Findcourier(string courier_name) {
	int i;
	for (i = 0; i <= courier_num - 1; i++)
	{
		if (courier_name.compare(allcouriers[i].user_name) == 0)
		{
			return i;
		}
	}
	return -1;
}
bool System::Log(int order,SOCKET &client_socket) {
	string password;
	bool flag = false;
	int times = 5;
	char buf[BUFSIZ] = { 0 };
	int judge;
	cout << "请输入密码" << endl;
	recv(client_socket, buf, BUFSIZ, 0);
	password = buf;
	while (times > 0)
	{
		if (password.compare(allusers[order].password) == 0)
		{
			flag = true;
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "登录成功" << endl;
			return flag;
		}
		else
		{
			cout << "密码错误，还有：" << times << "次可以尝试" << "请重新输入" << endl;
			memset(buf, 0, BUFSIZ);
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			recv(client_socket, buf, BUFSIZ, 0);
			password = buf;
			times--;
		}
	}
	return flag;
}
bool System::Courierlog(string id,SOCKET&client_socket) {
	int times = 3;
	string t_password;
	char buf[BUFSIZ] = { 0 };
	int judge;
	cout << "请输入你的密码: ";
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	t_password = buf;
	int order = Findcourier(id);
	while (times > 0)
	{
		if (t_password.compare(allcouriers[order].password) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			return true;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "密码输入错误，还有" << times << "次机会" << endl;
			cout << "请重新输入：";
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			t_password = buf;
			times--;
		}
	}
	return false;
}
void System::Send(int sender, int receiver, SOCKET& client_socket) {//sender是寄件者的序号，receiver是收件者的序号
	cout << "请输入您要寄送的快递类型：";
	cout << "1.普通快递(5元/kg) 2.易碎品(8元/kg) 3.图书(2元/本)";
	int choose;
	int judge;
	recv(client_socket, (char*)&choose, 4, 0);
	cout << "请输入您的快递重量：" << endl;
	double weight;
	recv(client_socket, (char*)&weight, 8, 0);
	if (choose == 1)
	{
		Normal new_package;
		new_package.Getprice(weight);
		if (allusers[sender].money >= new_package.money)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			new_package.SendPackage(allusers[sender].user_name, allusers[receiver].user_name,client_socket);
			not_assign.push_back(packages_num);
			not_assgin_num++;
			allusers[sender].send_numbers++;
			allusers[sender].send_package.push_back(packages_num);
			new_package.Make_PackageNum();
			allpackages.push_back(new_package);
			packages_num++;
			allusers[sender].money = allusers[sender].money - new_package.money;
			alladmin[0].money = alladmin[0].money + new_package.money;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "余额不足" << endl;
		}
	}
	else if (choose == 2)
	{
		Fragile new_package;
		new_package.Getprice(weight);
		if (allusers[sender].money >= new_package.money)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			new_package.SendPackage(allusers[sender].user_name, allusers[receiver].user_name,client_socket);
			not_assign.push_back(packages_num);
			allusers[sender].send_numbers++;
			not_assgin_num++;
			allusers[sender].send_package.push_back(packages_num);
			new_package.Make_PackageNum();
			allpackages.push_back(new_package);
			packages_num++;
			allusers[sender].money = allusers[sender].money - new_package.money;
			alladmin[0].money = alladmin[0].money + new_package.money;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "余额不足" << endl;
		}
	}
	else if (choose == 3)
	{
		Book new_package;
		new_package.Getprice(weight);
		if (allusers[sender].money >= new_package.money)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			new_package.SendPackage(allusers[sender].user_name, allusers[receiver].user_name,client_socket);
			not_assign.push_back(packages_num);
			allusers[sender].send_numbers++;
			not_assgin_num++;
			allusers[sender].send_package.push_back(packages_num);
			new_package.Make_PackageNum();
			allpackages.push_back(new_package);
			packages_num++;
			allusers[sender].money = allusers[sender].money - new_package.money;
			alladmin[0].money = alladmin[0].money + new_package.money;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "余额不足" << endl;
		}
	}
}
void System::Receive(int receiver,SOCKET& client_socket) {
	int order;
	bool flag = true;
	int num1;
	num1 = allusers[receiver].not_re_nums;
	send(client_socket, (char*)&num1, 4, 0);
	if (allusers[receiver].not_re_nums == 0)
	{
		cout << "没有待接收快递" << endl;
		flag = false;
	}
	else
	{
		cout << "待签收快递：" << endl;
		for (int i = 0; i <= allusers[receiver].not_re_nums - 1; i++)
		{
			cout << i + 1 << " :" << endl;
			order = allusers[receiver].not_receive[i];
			allpackages[order].ShowPackage(client_socket);
		}
	}
	if (flag)
	{
		int  choose = -1;
		cout << "请输入你要签收的快递序号：" << endl;
		recv(client_socket, (char*)&choose, 4, 0);
		order = allusers[receiver].not_receive[choose - 1];
		allpackages[order].ReceivePackage();
		allusers[receiver].receive.push_back(order);
		if (allusers[receiver].not_re_nums == 1)
		{
			allusers[receiver].not_receive.pop_back();
		}
		else
		{
			for (int i = choose - 1; i < allusers[receiver].not_re_nums - 1; i++)
			{
				allusers[receiver].not_receive[i] = allusers[receiver].not_receive[i + 1];
			}
			allusers[receiver].not_receive.pop_back();
		}
		allusers[receiver].not_re_nums--;
		allusers[receiver].receive_numbers++;
	}
}
void System::Show(int sender, SOCKET& client_socket)//查找该用户的所有快递信息
{
	int order;
	string send_message;
	int num1, num2, num3;
	num1 = allusers[sender].not_re_nums;
	send(client_socket, (char*)&num1, 4, 0);
	if (allusers[sender].not_re_nums == 0)
	{
		cout << "没有待接收快递" << endl;
	}
	else
	{
		cout << "待接收快递：" << endl;
		for (int i = 0; i <= allusers[sender].not_re_nums - 1; i++)
		{
			order = allusers[sender].not_receive[i];
			cout << i + 1 << " :" << endl;
			allpackages[order].ShowPackage(client_socket);
		}
	}
	num2 = allusers[sender].receive_numbers;
	send(client_socket, (char*)&num2, 4, 0);
	if (allusers[sender].receive_numbers == 0)
	{
		cout << "没有已接收快递" << endl;
	}
	else
	{
		cout << "已接收快递：" << endl;
		for (int i = 0; i <= allusers[sender].receive_numbers - 1; i++)
		{
			order = allusers[sender].receive[i];
			cout << i + 1 << " :" << endl;
			allpackages[order].ShowPackage(client_socket);
		}
	}
	num3 = allusers[sender].send_numbers;
	send(client_socket, (char*)&num3, 4, 0);
	if (allusers[sender].send_numbers == 0)
	{
		cout << "没有发送的快递" << endl;
	}
	else
	{
		cout << "已发送快递：" << endl;
		for (int i = 0; i <= allusers[sender].send_numbers - 1; i++)
		{
			order = allusers[sender].send_package[i];
			cout << i + 1 << " :" << endl;
			allpackages[order].ShowPackage(client_socket);
		}
	}
}
void System::Showbytime(int order, SOCKET& client_socket)const
{
	int i = 0;
	int time[3];
	int choose;
	int num = 0;
	int num1, num2, num3;
	int judge = 0;
	cout << "请输入快递发送相应的时间(输入日期)" << endl;
	recv(client_socket, (char*)&time[0], 4, 0);
	recv(client_socket, (char*)&time[1], 4, 0);
	recv(client_socket, (char*)&time[2], 4, 0);
	num1 = allusers[order].not_re_nums - 1;
	send(client_socket, (char*)&num1, 4, 0);
	for (i=0; i <= allusers[order].not_re_nums - 1; i++)
	{
		choose = allusers[order].not_receive[i];
		if (allpackages[choose].send_time[0] == time[0] && allpackages[choose].send_time[1] == time[1] && allpackages[choose].send_time[2] == time[2])
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[choose].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num == 0)
	{
		cout << "当天没有任何发送过来的快递" << endl;
	}
	num = 0;
	num2 = allusers[order].receive_numbers;
	send(client_socket, (char*)&num2, 4, 0);
	for (i = 0; i <= allusers[order].receive_numbers - 1; i++)
	{
		choose = allusers[order].receive[i];
		if (allpackages[choose].receive_time[0] == time[0] && allpackages[choose].receive_time[1] == time[1] && allpackages[choose].receive_time[2] == time[2])
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[choose].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num == 0)
	{
		cout << "当天没有任何签收的快递" << endl;
	}
	num = 0;
	num3 = allusers[order].send_numbers;
	send(client_socket, (char*)&num3, 4, 0);
	for (i = 0; i <= allusers[order].send_numbers - 1; i++)
	{
		choose = allusers[order].send_package[i];
		if (allpackages[choose].send_time[0] == time[0] && allpackages[choose].send_time[1] == time[1] && allpackages[choose].send_time[2] == time[2])
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[choose].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num == 0)
	{
		cout << "当天没有寄出任何快递" << endl;
	}
}
void System::Showbynum(int order, SOCKET& client_socket)
{
	string num;
	int judge;
	int num1 = 0;//记录查询到的快递数量
	int choose;
	int num2, num3, num4;
	int i;
	char buf[BUFSIZ] = { 0 };
	cout << "请输入你的快递单号:";
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	num = buf;
	num2 = allusers[order].not_re_nums;
	send(client_socket, (char*)&num2, 4, 0);
	for (i = 0; i <= allusers[order].not_re_nums - 1; i++)
	{
		choose = allusers[order].not_receive[i];
		if (num.compare(allpackages[choose].package_num) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[choose].ShowPackage(client_socket);
			num1++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到待签收快递" << endl;
	}
	num1 = 0;
	num3 = allusers[order].receive_numbers;
	send(client_socket, (char*)&num3, 4, 0);
	for (i = 0; i <= allusers[order].receive_numbers - 1; i++)
	{
		choose = allusers[order].receive[i];
		if (num.compare(allpackages[choose].package_num) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[choose].ShowPackage(client_socket);
			num1++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到已签收快递" << endl;
	}
	num1 = 0;
	num4 = allusers[order].send_numbers;
	send(client_socket, (char*)&num4, 4, 0);
	for (i = 0; i <= allusers[order].send_numbers - 1; i++)
	{
		choose = allusers[order].send_package[i];
		if (num.compare(allpackages[choose].package_num) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[choose].ShowPackage(client_socket);
			num1++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到已发送快递" << endl;
	}
}
bool System::Adminlog(SOCKET&client_socket)
{
	int times = 3;
	string password;
	int judge1;
	char buf[BUFSIZ] = { 0 };
	while (times > 0)
	{
		cout << "请输入你的密码:";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		password = buf;
		if (password.compare(alladmin[0].password) == 0)
		{
			judge1 = 1;
			send(client_socket, (char*)&judge1, 4, 0);
			cout << "登录成功!" << endl;
			return true;
		}
		else
		{
			judge1 = 0;
			send(client_socket, (char*)&judge1, 4, 0);
			cout << "密码输入错误，你还有" << times << "次机会" << endl;
			times--;
		}
	}
	return false;

}
void System::Adshowbyuser(SOCKET&client_socket)
{
	string id;
	string send_message;
	char buf[BUFSIZ] = { 0 };
	int order = -1;
	cout << "请输入用户的id:";
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	order = Finduser(id);
	send(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		cout << "未查询到该用户" << endl;
	}
	else
	{
		cout << "用户的id：" << allusers[order].user_name << endl;
		send_message = "用户的id：" + allusers[order].user_name + "\n";
		cout << "用户的姓名：" << allusers[order].name << endl;
		send_message = send_message + "用户的姓名：" + allusers[order].name + "\n";
		cout << "用户的地址：" << allusers[order].address << endl;
		send_message = send_message + "用户的地址：" + allusers[order].address + "\n";
		cout << "用户的余额：" << allusers[order].money << endl;
		send_message = send_message + "用户的余额：" +to_string(allusers[order].money) + "\n";
		send(client_socket, send_message.c_str(), send_message.length(), 0);
	}

}
void System::Adshowbyallusers(SOCKET&client_socket)const
{
	int i;
	int num;
	string send_message;
	num = users_num;
	send(client_socket, (char*)&num, 4, 0);
	for (i = 0; i <= users_num - 1; i++)
	{
		cout << "用户的id：" << allusers[i].user_name << endl;
		send_message = "用户的id：" + allusers[i].user_name + "\n";
		cout << "用户的姓名：" << allusers[i].name << endl;
		send_message = send_message + "用户的姓名：" + allusers[i].name + "\n";
		cout << "用户的地址：" << allusers[i].address << endl;
		send_message = send_message + "用户的地址：" + allusers[i].address + "\n";
		cout << "用户的余额：" << allusers[i].money << endl;
		send_message = send_message + "用户的余额：" + to_string(allusers[i].money) + "\n";
		send(client_socket, send_message.c_str(), send_message.length(), 0);
		cout << endl;
		send_message.clear();
	}
}
void System::Adshowbyid(SOCKET&client_socket)
{
	int order = -1;
	string id;
	char buf[BUFSIZ] = { 0 };
	cout << "请输入用户的id：";
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	order = Finduser(id);
	send(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		cout << "用户不存在" << endl;
	}
	else
	{
		Show(order,client_socket);
	}
}
void System::Adshowbytime(SOCKET&client_socket)const
{
	int time[3];
	int num;
	int judge;
	cout << "请输入要查询的时间（年月日）" << endl;
	cout << "年：";
	recv(client_socket, (char*)&time[0], 4, 0);
	cout << "月：";
	recv(client_socket, (char*)&time[1], 4, 0);
	cout << "日：";
	recv(client_socket, (char*)&time[2], 4, 0);
	num = packages_num;
	send(client_socket, (char*)&num, 4, 0);
	for (int i = 0; i <= packages_num - 1; i++)
	{
		if (allpackages[i].send_time[0] == time[0] && allpackages[i].send_time[1] == time[1] && allpackages[i].send_time[2] == time[2])
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[i].ShowPackage(client_socket);
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
}
void System::Adshowbynum(SOCKET&client_socket)const
{
	string num;
	int num1 = 0;
	int num2;
	int judge;
	char buf[BUFSIZ] = { 0 };
	cout << "请输入要查询的快递单号：";
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	num = buf;
	num2 = packages_num;
	send(client_socket, (char*)&num2, 4, 0);
	for (int i = 0; i <= packages_num - 1; i++)
	{
		if (num.compare(allpackages[i].package_num) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[i].ShowPackage(client_socket);
			num1++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到该快递" << endl;
	}
}
void System::Adshowcourier(SOCKET&client_socket)
{
	cout << "请输入你要进行的服务:" << endl;
	cout << "1.显示所有快递员信息 2.用户名查询快递员" << endl;
	int choose;
	int num1, num2;
	string send_message;
	char buf[BUFSIZ] = { 0 };
	recv(client_socket, (char*)&choose, 4, 0);
	if (choose == 1)
	{
		num1 = courier_num;
		send(client_socket, (char*)&num1, 4, 0);
		if (courier_num == 0)
		{
			cout << "无快递员" << endl;
		}
		else
		{
			for (int i = 0; i <= courier_num - 1; i++)
			{
				cout << i + 1 << "  姓名：" << allcouriers[i].name << endl;
				send_message = to_string(i + 1) + "：\n" + "  姓名：" + allcouriers[i].name + "\n";
				cout << "  用户名：" << allcouriers[i].user_name << endl;
				send_message = send_message + "  用户名：" + allcouriers[i].user_name + "\n";
				cout << "  电话：" << allcouriers[i].phone << endl;
				send_message = send_message + "  电话：" + allcouriers[i].phone + "\n";
				cout << "  余额：" << allcouriers[i].money << endl;
				send_message = send_message + "  余额：" + to_string(allcouriers[i].money) + "\n";
				cout << endl;
				send(client_socket, send_message.c_str(), send_message.length(), 0);
				send_message.clear();
			}
		}
	}
	else if (choose == 2)
	{
		cout << "请输入快递员id：" << endl;
		string id;
		int order = -1;
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		id = buf;
		order = Findcourier(id);
		send(client_socket, (char*)&order, 4, 0);
		if (order == -1)
		{
			cout << "未查询到该快递员" << endl;
		}
		else
		{
			cout << "  姓名：" << allcouriers[order].name << endl;
			send_message = "  姓名：" + allcouriers[order].name + "\n";
			cout << "  电话：" << allcouriers[order].phone << endl;
			send_message = send_message + "  电话：" + allcouriers[order].phone + "\n";
			cout << "  余额：" << allcouriers[order].money << endl;
			send_message = send_message + "  余额：" + to_string(allcouriers[order].money) + "\n";
			cout << endl;
			send(client_socket, send_message.c_str(), send_message.length(), 0);
		}
	}
}
void System::Adshowcollect(SOCKET&client_socket)
{
	cout << "请输入快递员的id：" << endl;
	char buf[BUFSIZ] = { 0 };
	string id;
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	int order;
	order = Findcourier(id);
	send(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		cout << "该用户不存在" << endl;
	}
	else
	{
		cout << "请选择查询的方式：" << endl;
		cout << "1.全部显示 2.时间查询 3.快递单号查询 4.寄件人 5.收件人" << endl;
		int choose;
		recv(client_socket, (char*)&choose, 4, 0);
		if (choose == 1)
		{
			CoSearch(order,client_socket);
		}
		else if (choose == 2)
		{
			Coshowbytime(order,client_socket);
		}
		else if (choose == 3)
		{
			Coshowbynum(order,client_socket);
		}
		else if (choose == 4)
		{
			Coshowbysend(order,client_socket);
		}
		else if (choose == 5)
		{
			Coshowbyreceive(order,client_socket);
		}
	}
}
void System::Addcourier(SOCKET&client_socket)
{
	int order;
	Courier new_courier;
	cout << "请输入想要添加的id" << endl;
	string id;
	char buf[BUFSIZ] = { 0 };
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	order = Findcourier(id);
	send(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		new_courier.user_name = id;
		cout << "请输入您的姓名：";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		new_courier.name = buf;
		cout << "请输入电话：";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		new_courier.phone = buf;
		cout << "请输入您的密码：";
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		new_courier.password = buf;
		allcouriers.push_back(new_courier);
		courier_num++;
	}
	else
	{
		cout << "该用户名已注册" << endl;
	}
}
void System::Deletecourier(SOCKET&client_socket)
{
	string id;
	int order;
	char buf[BUFSIZ] = { 0 };
	cout << "请输入想要删除的快递员id：";
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	order = Findcourier(id);
	send(client_socket, (char*)&order, 4, 0);
	if (order != -1)
	{
		if (courier_num == 1)
		{
			allcouriers.pop_back();
			courier_num--;
		}
		else
		{
			for (int i = order; i < courier_num - 1; i++)
			{
				allcouriers[i] = allcouriers[i + 1];
			}
			allcouriers.pop_back();
			courier_num--;
		}
	}
	else
	{
		cout << "没有该快递员" << endl;
	}
}
void System::Assign(SOCKET&client_socket)
{
	int order;
	int num;
	string courier_message;
	int num1, num2;
	num1 = courier_num;
	send(client_socket, (char*)&num1, 4, 0);
	if (courier_num > 0)
	{
		cout << "可选择的快递员:" << endl;
		for (int i = 0; i <= courier_num - 1; i++)
		{
			cout << i + 1 << "  姓名：" << allcouriers[i].name << endl;
			courier_message = to_string(i + 1) + " 姓名：" + allcouriers[i].name + "\n";
			cout << "  电话：" << allcouriers[i].phone << endl;
			courier_message = courier_message + "  电话：" + allcouriers[i].phone + "\n";
			cout << "  余额：" << allcouriers[i].money << endl;
			courier_message = courier_message + "  余额：" +to_string(allcouriers[i].money) + "\n";
			cout << endl;
			send(client_socket, courier_message.c_str(), courier_message.length(), 0);
			courier_message.clear();
		}
		int choose;
		cout << "请输入您要分配的快递员：";
		recv(client_socket, (char*)&choose, 4, 0);
		if (choose >= 0 && choose <= courier_num)
		{
			cout << "待分配的快递:" << endl;
			num2 = not_assgin_num;
			send(client_socket, (char*)&num2, 4, 0);
			for (int i = 0; i <= not_assgin_num - 1; i++)
			{
				cout << i + 1 << ":"<<endl;
				order = not_assign[i];
				allpackages[order].ShowPackage(client_socket);
			}
			cout << "请输入你要分配的快递个数：";
			recv(client_socket,(char*)&num,4,0);
			int choose1;
			if (not_assgin_num != 1 && num <= not_assgin_num)
			{
				for (int i = 0; i <= num - 1; i++)
				{
					cout << "请输入你要分配的快递序号：";
					recv(client_socket, (char*)&choose1, 4, 0);
					allcouriers[choose - 1].not_collect.push_back(not_assign[choose1 - 1]);
					allcouriers[choose - 1].not_collect_num++;
					for (int j = choose1 - 1; j < not_assgin_num - 1; j++)
					{
						not_assign[j] = not_assign[j + 1];
					}
					not_assign.pop_back();
					not_assgin_num--;
					if (i != num - 1)
					{
						cout << "待分配的快递:" << endl;
						for (int i = 0; i <= not_assgin_num - 1; i++)
						{
							cout << i + 1 << ":"<<endl;
							order = not_assign[i];
							allpackages[order].ShowPackage(client_socket);
						}
					}
				}
			}
			else if (not_assgin_num == 1 && num == 1)
			{
				allcouriers[choose - 1].not_collect.push_back(not_assign[0]);
				allcouriers[choose - 1].not_collect_num++;
				not_assign.pop_back();
				not_assgin_num--;
			}
			else
			{
				cout << "无效输入" << endl;
				cin.clear();
				cin.ignore((std::numeric_limits< streamsize >::max)(), '\n');
			}

		}
	}
	else
	{
		cout << "无快递员" << endl;
	}
}
void System::Cocollect(int order,SOCKET&client_socket)
{
	int choose;
	int num;
	int choose1;
	int num1, judge;
	string name1;
	int order1;
	num1 = allcouriers[order].not_collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	if (allcouriers[order].not_collect_num == 0)
	{
		cout << "无待揽收快递" << endl;
	}
	else
	{
		cout << "待揽收的快递：";
		for (int i = 0; i <= allcouriers[order].not_collect_num - 1; i++)
		{
			cout << i + 1 << ":"<<endl;
			choose = allcouriers[order].not_collect[i];
			allpackages[choose].ShowPackage(client_socket);
		}
		cout << "请输入你要揽收的个数:";
		recv(client_socket, (char*)&num, 4, 0);
		if (allcouriers[order].not_collect_num == 1 && num == 1)
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			choose = allcouriers[order].not_collect[0];
			allpackages[choose].CollectPackage();
			name1 = allpackages[choose].receiver;
			order1 = Finduser(name1);
			allusers[order1].not_receive.push_back(choose);
			allusers[order1].not_re_nums++;
			allcouriers[order].collect.push_back(choose);
			allcouriers[order].collect_num++;
			allcouriers[order].not_collect_num--;
			allcouriers[order].not_collect.pop_back();
			allcouriers[order].money = allcouriers[order].money + allpackages[choose].money * 0.5;
			alladmin[0].money = alladmin[0].money - allpackages[choose].money * 0.5;
		}
		else if (allcouriers[order].not_collect_num > 1)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			for (int i = 0; i <= num - 1; i++)
			{
				cout << "请选择您要揽收的快递序号：" << endl;
				recv(client_socket, (char*)&choose1, 4, 0);
				choose = allcouriers[order].not_collect[choose1 - 1];
				allpackages[choose].CollectPackage();
				name1 = allpackages[choose].receiver;
				order1 = Finduser(name1);
				allusers[order1].not_receive.push_back(choose);
				allusers[order1].not_re_nums++;
				allcouriers[order].collect.push_back(choose);
				allcouriers[order].collect_num++;
				allcouriers[order].money = allcouriers[order].money + allpackages[choose].money * 0.5;
				alladmin[0].money = alladmin[0].money - allpackages[choose].money * 0.5;
				for (int j = 0; j < allcouriers[order].not_collect_num - 1; j++)
				{
					allcouriers[order].not_collect[j] = allcouriers[order].not_collect[j + 1];
				}
				allcouriers[order].not_collect.pop_back();
				allcouriers[order].not_collect_num--;
				if (i != num - 1)
				{
					cout << "待揽收的快递：";
					for (int i = 0; i <= allcouriers[order].not_collect_num - 1; i++)
					{
						cout << i + 1 << ":"<<endl;
						choose = allcouriers[order].not_collect[i];
						allpackages[choose].ShowPackage(client_socket);
					}
				}
			}
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
			cout << "无效输入" << endl;
			cin.clear();
			cin.ignore((std::numeric_limits< streamsize >::max)(), '\n');
		}
	}
}
void System::Coshow_not_collect(int order,SOCKET&client_socket)const
{
	int order1;
	int num1;
	num1 = allcouriers[order].not_collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	cout << "待揽收的快递：" << endl;
	for (int i = 0; i <= allcouriers[order].not_collect_num - 1; i++)
	{
		order1 = allcouriers[order].not_collect[i];
		cout << i + 1 << ":"<<endl;
		allpackages[order1].ShowPackage(client_socket);
	}
	if (allcouriers[order].not_collect_num == 0)
	{
		cout << "无待揽收快递" << endl;
	}
}
void System::CoSearch(int order,SOCKET&client_socket)
{
	int order1;
	int num1;
	num1 = allcouriers[order].collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	if (allcouriers[order].collect_num == 0)
	{
		cout << "无已揽收快递" << endl;
	}
	else
	{
		cout << "揽收的快递：" << endl;
		for (int i = 0; i <= allcouriers[order].collect_num - 1; i++)
		{
			order1 = allcouriers[order].collect[i];
			cout << i + 1 << ":"<<endl;
			allpackages[order1].ShowPackage(client_socket);
		}
	}
}
void System::Coshowbynum(int order,SOCKET&client_socket)const
{
	cout << "请输入你要查询的快递单号：";
	string number;
	char buf[BUFSIZ] = { 0 };
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	number = buf;
	int i;
	int order1;
	int num = 0;
	int num1, judge;
	num1 = allcouriers[order].collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	for (i = 0; i <= allcouriers[order].collect_num - 1; i++)
	{
		order1 = allcouriers[order].collect[i];
		if (number.compare(allpackages[order1].package_num) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[order1].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num == 0)
	{
		cout << "未查询到该快递" << endl;
	}
}
void System::Coshowbyreceive(int order,SOCKET&client_socket)const
{
	cout << "请输入收件人id";
	string id;
	char buf[BUFSIZ] = { 0 };
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	int i;
	int order1;
	int num = 0;
	int num1, judge;
	num1 = allcouriers[order].collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	for (i = 0; i <= allcouriers[order].collect_num - 1; i++)
	{
		order1 = allcouriers[order].collect[i];
		if (id.compare(allpackages[order1].receiver) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[order1].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);

		}
	}
	if (num == 0)
	{
		cout << "未查询到该快递" << endl;
	}
}
void System::Coshowbysend(int order,SOCKET&client_socket)const
{
	cout << "请输入寄件人id";
	string id;
	char buf[BUFSIZ] = { 0 };
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	id = buf;
	int i;
	int order1;
	int num = 0;
	int num1, judge;
	num1 = allcouriers[order].collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	for (i = 0; i <= allcouriers[order].collect_num - 1; i++)
	{
		order1 = allcouriers[order].collect[i];
		if (id.compare(allpackages[order1].sender) == 0)
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[order1].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num == 0)
	{
		cout << "未查询到该快递" << endl;
	}
}
void System::Coshowbytime(int order,SOCKET&client_socket)const
{
	int time[3];
	cout << "请输入要查询的时间（年月日）" << endl;
	cout << "年：";
	recv(client_socket, (char*)&time[0], 4, 0);
	cout << "月：";
	recv(client_socket, (char*)&time[1], 4, 0);
	cout << "日：";
	recv(client_socket, (char*)&time[2], 4, 0);
	int order1;
	int num = 0;
	int num1, judge;
	num1 = allcouriers[order].collect_num;
	send(client_socket, (char*)&num1, 4, 0);
	for (int i = 0; i <= allcouriers[order].collect_num - 1; i++)
	{
		order1 = allcouriers[order].collect[i];
		if (allpackages[order1].collect_time[0] == time[0] && allpackages[order1].collect_time[1] == time[1] && allpackages[order1].collect_time[2])
		{
			judge = 1;
			send(client_socket, (char*)&judge, 4, 0);
			allpackages[order1].ShowPackage(client_socket);
			num++;
		}
		else
		{
			judge = 0;
			send(client_socket, (char*)&judge, 4, 0);
		}
	}
	if (num == 0)
	{
		cout << "当天没有揽收任何快递" << endl;
	}
}