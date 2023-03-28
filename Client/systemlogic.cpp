#include"system.h"
#include"../tcpSocket/tcpSocket.h"
System::System() {
	admin_num = 1;
	users_num = 0;
	packages_num = 0;
	courier_num = 0;
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
bool System::Log(int order,SOCKET&client_socket) {
	string password;
	bool flag = false;
	int times = 5;
	int judge;
	cout << "请输入密码:";
	cin >> password;
	send(client_socket, password.c_str(), password.length(), 0);
	while (times > 0)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge==1)
		{
			flag = true;
			cout << "登录成功" << endl;
			return flag;
		}
		else
		{
			cout << "密码错误，还有：" << times << "次可以尝试" << "请重新输入" << endl;
			cin >> password;
			send(client_socket, password.c_str(), password.length(), 0);
			times--;
		}
	}
	return flag;
}
bool System::Courierlog(string id,SOCKET&client_socket) {
	int times = 3;
	string t_password;
	int judge;
	cout << "请输入你的密码: ";
	cin >> t_password;
	send(client_socket, t_password.c_str(), t_password.length(), 0);
	while (times > 0)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			return true;
		}
		else
		{
			cout << "密码输入错误，还有" << times << "次机会" << endl;
			cout << "请重新输入：";
			cin >> t_password;
			send(client_socket, t_password.c_str(), t_password.length(), 0);
			times--;
		}
	}
	return false;
}
void System::Send(int sender, int receiver, SOCKET& client_socket) {//sender是寄件者的序号，receiver是收件者的序号
	string package_name, note;
	cout << "请输入您要寄送的快递类型：";
	cout << "1.普通快递(5元/kg) 2.易碎品(8元/kg) 3.图书(2元/本)" << endl;
	int choose;
	int judge;
	cin >> choose;
	send(client_socket, (char*)&choose, 4, 0);
	cout << "请输入您的快递重量：" ;
	double weight;
	cin >> weight;
	send(client_socket, (char*)&weight, 8, 0);
	if (choose == 1)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			cout << "请输入包裹的名称：" ;
			cin >> package_name;
			send(client_socket, package_name.c_str(), package_name.length(), 0);
			cout << "请输入快件的备注：" ;
			cin >> note;
			send(client_socket, note.c_str(), note.length(), 0);
		}
		else
		{
			cout << "余额不足" << endl;
		}
	}
	else if (choose == 2)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			cout << "请输入包裹的名称：" ;
			cin >> package_name;
			send(client_socket, package_name.c_str(), package_name.length(), 0);
			cout << "请输入快件的备注：" ;
			cin >> note;
			send(client_socket, note.c_str(), note.length(), 0);
		}
		else
		{
			cout << "余额不足" << endl;
		}
	}
	else if (choose == 3)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			cout << "请输入包裹的名称：";
			cin >> package_name;
			send(client_socket, package_name.c_str(), package_name.length(), 0);
			cout << "请输入快件的备注：";
			cin >> note;
			send(client_socket, note.c_str(), note.length(), 0);
		}
		else
		{
			cout << "余额不足" << endl;
		}
	}

}
void System::Receive(int receiver,SOCKET& client_socket) {
	int order;
	bool flag = true;;
	int num1;
	char buf[BUFSIZ] = { 0 };
	recv(client_socket, (char*)&num1, 4, 0);
	if (num1 == 0)
	{
		cout << "没有待接收快递。" << endl;
		flag = false;
	}
	else
	{
		cout << "待签收快递：" << endl;
		for (int i = 0; i <= num1 - 1; i++)
		{
			cout << i + 1 << " :" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
		}
	}
	if (flag)
	{
		int  choose = -1;
		cout << "请输入你要签收的快递序号：" ;
		cin >> choose;
		send(client_socket, (char*)&choose, 4, 0);
	}
}
void System::Show(int sender, SOCKET& client_socket)//查找该用户的所有快递信息
{
	int order;
	int num1, num2, num3;
	string recv_message;
	char buf[BUFSIZ] = { 0 };
	recv(client_socket, (char*)&num1, 4, 0);
	if (num1 == 0)
	{
		cout << "没有待接收快递。" << endl;
	}
	else
	{
		cout << "待接收快递：" << endl;
		for (int i = 0; i <= num1 - 1; i++) 
		{
			cout << i + 1 << " :" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
		}
	}
	cout << endl;
	recv(client_socket, (char*)&num2, 4, 0);
	if (num2 == 0)
	{
		cout << "没有已接收快递。" << endl;
	}
	else
	{
		cout << "已接收快递：" << endl;
		for (int i = 0; i <= num2 - 1; i++)
		{
			cout << i + 1 << " :" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
		}
	}
	cout << endl;
	recv(client_socket, (char*)&num3, 4, 0);
	if (num3 == 0)
	{
		cout << "没有发送的快递。" << endl;
	}
	else
	{
		cout << "已发送快递：" << endl;
		for (int i = 0; i <= num3 - 1; i++)
		{
			cout << i + 1 << " :" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
		}
	}
	cout << endl;
}
void System::Showbytime(int order,SOCKET &client_socket)const
{
	int i = 0;
	int year, month, day;
	int choose;
	int num = 0;
	char buf[BUFSIZ] = { 0 };
	string recv_message;
	cout << "请输入快递发送相应的时间(输入日期)" << endl;
	cout << "年：";
	cin >> year;
	cout << "月：";
	cin >> month;
	cout << "日：";
	cin >> day;
	int num1, num2, num3, judge;
	send(client_socket, (char*)&year, 4, 0);
	send(client_socket, (char*)&month, 4, 0);
	send(client_socket, (char*)&day, 4, 0);
	recv(client_socket, (char*)&num1, 4, 0);
	for (i; i <= num1 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			num++;
		}
	}
	if (num == 0)
	{
		cout << "当天没有任何发送过来的快递。" << endl;
	}
	num = 0;
	recv(client_socket, (char*)&num2, 4, 0);
	for (i = 0; i <= num2 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			memset(buf, 0, BUFSIZ);
			num++;
		}
	}
	if (num == 0)
	{
		cout << "当天没有任何签收的快递。" << endl;
	}
	num = 0;
	recv(client_socket, (char*)&num3, 4, 0);
	for (i = 0; i <= num3 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			memset(buf, 0, BUFSIZ);
			num++;
		}
	}
	if (num == 0)
	{
		cout << "当天没有寄出任何快递" << endl;
	}
}
void System::Showbynum(int order,SOCKET& client_socket)
{
	string num;
	int num1 = 0;//记录查询到的快递数量
	int choose;
	int num2, num3, num4, judge;
	string recv_message;
	char buf[BUFSIZ] = { 0 };
	cout << "请输入你的快递单号:" << endl;
	cin >> num;
	send(client_socket, num.c_str(), num.length(), 0);
	int i;
	recv(client_socket, (char*)&num2, 4, 0);
	for (i = 0; i <= num2 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			memset(buf,0,BUFSIZ);
			num1++;
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到待签收快递" << endl;
	}
	num1 = 0;
	recv(client_socket, (char*)&num3, 4, 0);
	for (i = 0; i <= num3 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			memset(buf, 0, BUFSIZ);
			num1++;
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到已签收快递" << endl;
	}
	num1 = 0;
	recv(client_socket, (char*)&num4, 4, 0);
	for (i = 0; i <= num4 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			memset(buf, 0, BUFSIZ);
			num1++;
		}
	}
	if (num1 == 0)
	{
		cout << "未查询到已发送快递" << endl;
	}
}
bool System::Adminlog(SOCKET& client_socket)
{
	int times = 3;
	int judge;
	string password;
	while (times > 0)
	{
		cout << "请输入你的密码:";
		cin >> password;
		send(client_socket, password.c_str(), password.length(), 0);
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			cout << "登录成功!" << endl;
			return true;
		}
		else
		{
			cout << "密码输入错误，你还有" << times << "次机会" << endl;
			times--;
		}
	}
	return false;

}
void System::Adshowbyuser(SOCKET&client_socket)
{
	string id;
	int order = -1;
	cout << "请输入用户的id:";
	cin >> id;
	char buf[BUFSIZ];
	send(client_socket, id.c_str(), id.length(), 0);
	recv(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		cout << "未查询到该用户" << endl;
	}
	else
	{
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		cout << buf << endl;
	}

}
void System::Adshowbyallusers(SOCKET&client_socket)const
{
	int i;
	int num;
	char buf[BUFSIZ] = { 0 };
	recv(client_socket, (char*)&num, 4, 0);
	for (i = 0; i <= num - 1; i++)
	{
		memset(buf, 0, BUFSIZ);
		recv(client_socket, buf, BUFSIZ, 0);
		cout << buf;
		cout << endl;
	}
}
void System::Adshowbyid(SOCKET&client_socket)
{
	int order = -1;
	string id;
	cout << "请输入用户的id：";
	cin >> id;
	send(client_socket, id.c_str(), id.length(), 0);
	recv(client_socket, (char*)&order, 4, 0);
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
	int num1 = 0;
	char buf[BUFSIZ] = { 0 };
	cout << "请输入要查询的时间（年月日）" << endl;
	cout << "年：";
	cin >> time[0];
	send(client_socket, (char*)&time[0], 4, 0);
	cout << "月：";
	cin >> time[1];
	send(client_socket, (char*)&time[1], 4, 0);
	cout << "日：";
	cin >> time[2];
	send(client_socket, (char*)&time[2], 4, 0);
	recv(client_socket, (char*)&num, 4, 0);
	for (int i = 0; i <= num - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
			num1++;
		}
	}
	if (num1 == 0)
	{
		cout << "当天没有快递" << endl;
	}
}
void System::Adshowbynum(SOCKET&client_socket)const
{
	string num;
	int num1 = 0;
	int num2;
	int judge;
	char buf[BUFSIZ] = { 0 };
	string recv_message;
	cout << "请输入要查询的快递单号：";
	cin >> num;
	send(client_socket, num.c_str(), num.length(), 0);
	recv(client_socket, (char*)&num2, 4, 0);
	for (int i = 0; i <= num2 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
			num1++;
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
	int num;
	char buf[BUFSIZ] = { 0 };
	cin >> choose;
	send(client_socket, (char*)&choose, 4, 0);
	if (choose == 1)
	{
		recv(client_socket, (char*)&num, 4, 0);
		if (num == 0)
		{
			cout << "无快递员" << endl;
		}
		else
		{
			for (int i = 0; i <= num - 1; i++)
			{
				memset(buf, 0, BUFSIZ);
				recv(client_socket, buf, BUFSIZ, 0);
				cout << buf;
				cout << endl;
			}
		}
	}
	else if (choose == 2)
	{
		cout << "请输入快递员id：" ;
		string id;
		int order = -1;
		cin >> id;
		send(client_socket, id.c_str(), id.length(), 0);
		recv(client_socket, (char*)&order, 4, 0);
		if (order == -1)
		{
			cout << "为查询到该快递员" << endl;
		}
		else
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf;
			cout << endl;
		}
	}
}
void System::Adshowcollect(SOCKET&client_socket)
{
	cout << "请输入快递员的id：" ;
	string id;
	cin >> id;
	send(client_socket, id.c_str(), id.length(), 0);
	int order;
	recv(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		cout << "该用户不存在" << endl;
	}
	else
	{
		cout << "请选择查询的方式：" << endl;
		cout << "1.全部显示 2.时间查询 3.快递单号查询 4.寄件人 5.收件人" << endl;
		int choose;
		cin >> choose;
		send(client_socket, (char*)&choose, 4, 0);
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
	cout << "请输入想要添加的id：" ;
	string id;
	cin >> id;
	send(client_socket, id.c_str(), id.length(), 0);
	recv(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		new_courier.user_name = id;
		cout << "请输入您的姓名：";
		cin >> new_courier.name;
		send(client_socket, new_courier.name.c_str(), new_courier.name.length(), 0);
		cout << "请输入电话：";
		cin >> new_courier.phone;
		send(client_socket, new_courier.phone.c_str(), new_courier.phone.length(), 0);
		cout << "请输入您的密码：";
		cin >> new_courier.password;
		send(client_socket, new_courier.password.c_str(), new_courier.password.length(), 0);
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
	cout << "请输入想要删除的快递员id：";
	cin >> id;
	send(client_socket, id.c_str(), id.length(), 0);
	order = Findcourier(id);
	recv(client_socket, (char*)&order, 4, 0);
	if (order == -1)
	{
		cout << "没有该快递员" << endl;
	}
}
void System::Assign(SOCKET&client_socket)
{
	int order;
	char buf[BUFSIZ] = { 0 };
	int num;
	int num1, num2;
	recv(client_socket, (char*)&num1, 4, 0);
	if (num1 > 0)
	{
		cout << "可选择的快递员:" << endl;
		for (int i = 0; i <= num1 - 1; i++)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
		}
		int choose;
		cout << "请输入您要分配的快递员：";
		cin >> choose;
		send(client_socket, (char*)&choose, 4, 0);
		if (choose >= 0 && choose <= num1)
		{
			cout << "待分配的快递:" << endl;
			recv(client_socket, (char*)&num2, 4, 0);
			for (int i = 0; i <= num2 - 1; i++)
			{
				cout << i + 1 << " "<<endl;
				memset(buf, 0, BUFSIZ);
				recv(client_socket, buf, BUFSIZ, 0);
				cout << buf << endl;
			}
			cout << "请输入你要分配的快递个数：";
			cin >> num;
			send(client_socket, (char*)&num, 4, 0);
			int choose1;
			if (num2 != 1 && num <= num2)
			{
				for (int i = 0; i <= num - 1; i++)
				{
					cout << "请输入你要分配的快递序号：";
					cin >> choose1;
					send(client_socket, (char*)&choose1, 4, 0);
					num2--;
					if (i != num - 1)
					{
						cout << "待分配的快递:" << endl;
						for (int i = 0; i <= num2 - 1; i++)
						{
							cout << i + 1 << " "<<endl;
							memset(buf, 0, BUFSIZ);
							recv(client_socket, buf, BUFSIZ, 0);
							cout << buf << endl;
						}
					}
				}
			}
			else if (num == 1 && num2 == 1)
			{
				cout << "已分配" << endl;
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
	char buf[BUFSIZ] = { 0 };
	recv(client_socket, (char*)&num1, 4, 0);
	if (num1 == 0)
	{
		cout << "无待揽收快递" << endl;
	}
	else
	{
		cout << "待揽收的快递："<<endl;
		for (int i = 0; i <= num1 - 1; i++)
		{
			cout << i + 1 << ":" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
		}
		cout << "请输入你要揽收的个数:";
		cin >> num;
		send(client_socket, (char*)&num, 4, 0);
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			for (int i = 0; i <= num - 1; i++)
			{
				cout << "请选择您要揽收的快递：";
				cin >> choose1;
				send(client_socket, (char*)&choose1, 4, 0);
				num1--;
				if (i != num - 1)
				{
					cout << "待揽收的快递：" << endl;
					for (int i = 0; i <= num1 - 1; i++)
					{
						cout << i + 1 << ":" << endl;
						memset(buf, 0, BUFSIZ);
						recv(client_socket, buf, BUFSIZ, 0);
						cout << buf << endl;
					}
				}
			}
		}
		else if (judge == 0 && num == 1)
		{
			cout << "已揽收" << endl;
		}
		else
		{
			cout << "无效输入" << endl;
			cin.clear();
			cin.ignore((std::numeric_limits< streamsize >::max)(), '\n');
		}
	}
}
void System::Coshow_not_collect(int order,SOCKET&client_socket)const
{
	int order1;
	char buf[BUFSIZ] = { 0 };
	int num1;
	recv(client_socket, (char*)&num1, 4, 0);
	if (num1 == 0)
	{
		cout << "无待揽收快递" << endl;
	}
	else
	{
		cout << "待揽收的快递：" << endl;
		for (int i = 0; i <= num1 - 1; i++)
		{
			cout << i + 1 << ":" << endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
		}
	}
}
void System::CoSearch(int order,SOCKET&client_socket)
{
	int order1;
	int num, judge;
	char buf[BUFSIZ] = { 0 };
	recv(client_socket, (char*)&num, 4, 0);
	if (num == 0)
	{
		cout << "无已揽收快递" << endl;
	}
	else
	{
		cout << "我揽收的快递：" << endl;
		for (int i = 0; i <= num - 1; i++)
		{
			cout << i + 1 << ":"<<endl;
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
		}
	}
	
}
void System::Coshowbynum(int order,SOCKET&client_socket)const
{
	cout << "请输入你要查询的快递单号：";
	string number;
	char buf[BUFSIZ] = { 0 };
	cin >> number;
	send(client_socket, number.c_str(), number.length(), 0);
	int i;
	int order1;
	int num = 0;
	int num1, judge;
	recv(client_socket, (char*)&num1, 4, 0);
	for (i = 0; i <= num1 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
			num++;
		}
	}
	if (num == 0)
	{
		cout << "未查询到该快递" << endl;
	}
}
void System::Coshowbyreceive(int order,SOCKET&client_socket)const
{
	cout << "请输入收件人id" ;
	string id;
	char buf[BUFSIZ] = { 0 };
	cin >> id;
	send(client_socket, id.c_str(), id.length(), 0);
	int i;
	int order1;
	int num = 0;
	int num1, judge;
	recv(client_socket, (char*)&num1, 4, 0);
	for (i = 0; i <= num1 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
			num++;
		}
	}
	if (num == 0)
	{
		cout << "未查询到该快递" << endl;
	}
}
void System::Coshowbysend(int order,SOCKET&client_socket)const
{
	cout << "请输入寄件人id：";
	string id;
	char buf[BUFSIZ] = { 0 };
	cin >> id;
	send(client_socket, id.c_str(), id.length(), 0);
	int i;
	int order1;
	int num = 0;
	int num1, judge;
	recv(client_socket, (char*)&num1, 4, 0);
	for (i = 0; i <= num1 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
			num++;
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
	char buf[BUFSIZ] = { 0 };
	cout << "请输入要查询的时间（年月日）" << endl;
	cout << "年：";
	cin >> time[0];
	send(client_socket, (char*)&time[0], 4, 0);
	cout << "月：";
	cin >> time[1];
	send(client_socket, (char*)&time[1], 4, 0);
	cout << "日：";
	cin >> time[2];
	send(client_socket, (char*)&time[2], 4, 0);
	int order1;
	int num = 0;
	int num1, judge;
	recv(client_socket, (char*)&num1, 4, 0);
	for (int i = 0; i <= num1 - 1; i++)
	{
		recv(client_socket, (char*)&judge, 4, 0);
		if (judge == 1)
		{
			memset(buf, 0, BUFSIZ);
			recv(client_socket, buf, BUFSIZ, 0);
			cout << buf << endl;
			num++;
		}
	}
	if (num == 0)
	{
		cout << "当天没有揽收任何快递" << endl;
	}
}