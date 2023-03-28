#include"system.h"

//实现管理员类的函数
Administrator::Administrator() {
	money = 0;
}
void Administrator::Changename()
{
	cout << "请输入你的新名字" << endl;
	cin >> name;
}
void Administrator::Changepassword()
{
	int times = 3;
	string t_password;
	while (times > 0)
	{
		cout << "请输入你的旧密码:";
		cin >> t_password;
		if (t_password.compare(password) == 0)
		{
			cout << "请输入新的密码:";
			cin >> password;
			break;
		}
		else
		{
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