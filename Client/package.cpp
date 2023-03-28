#include"system.h"
//实现package的成员函数
Package::Package() {
	sign = false;
	srand((int)time(0));//随机数种子的生成
	collect = false;
}
void Package::StorePackage(ofstream& out) {
	int i;
	out << package_num << endl;
	for (i = 0; i <= 5; i++)
	{
		out << send_time[i] << " ";
	}
	out << endl;
	for (i = 0; i <= 5; i++)
	{
		out << receive_time[i] << " ";
	}
	out << endl;
	for (i = 0; i <= 5; i++)
	{
		out << collect_time[i] << " ";
	}
	out << endl;
	out << package_name << endl;
	out << sender << endl;
	out << receiver << endl;
	out << sign << endl;
	out << note << endl;
	out << collect << endl;
	out << money << endl;
	out << type << endl;
}
void Package::ReadPackage(ifstream& in) {
	int i;
	in >> package_num;
	for (i = 0; i <= 5; i++)
	{
		in >> send_time[i];
	}
	for (i = 0; i <= 5; i++)
	{
		in >> receive_time[i];
	}
	for (i = 0; i <= 5; i++)
	{
		in >> collect_time[i];
	}
	in >> package_name;
	in >> sender;
	in >> receiver;
	in >> sign;
	in >> note;
	in >> collect;
	in >> money;
	in >> type;
}
void Package::SendPackage(string t_sender, string t_receiver) {
	receiver = t_receiver;
	sender = t_sender;
	cout << "请输入包裹的名称：" << endl;
	cin >> package_name;
	cout << "请输入快件的备注：" << endl;
	cin >> note;
	tm time1;
	time_t sendtime = time(0);
	localtime_s(&time1, &sendtime);
	send_time[0] = time1.tm_year + 1900;
	send_time[1] = time1.tm_mon + 1;
	send_time[2] = time1.tm_mday;
	send_time[3] = time1.tm_hour;
	send_time[4] = time1.tm_min;
	send_time[5] = time1.tm_sec;
}
void Package::CollectPackage()
{
	time_t collecttime = time(0);
	tm time1;
	localtime_s(&time1, &collecttime);
	collect_time[0] = time1.tm_year + 1900;
	collect_time[1] = time1.tm_mon + 1;
	collect_time[2] = time1.tm_mday;
	collect_time[3] = time1.tm_hour;
	collect_time[4] = time1.tm_min;
	collect_time[5] = time1.tm_sec;
	collect = true;
}
void Package::ReceivePackage() {
	time_t receivetime = time(0);
	tm time1;
	localtime_s(&time1, &receivetime);
	receive_time[0] = time1.tm_year + 1900;
	receive_time[1] = time1.tm_mon + 1;
	receive_time[2] = time1.tm_mday;
	receive_time[3] = time1.tm_hour;
	receive_time[4] = time1.tm_min;
	receive_time[5] = time1.tm_sec;
	sign = true;
}

void Package::Make_PackageNum() {
	package_num = 'L';
	for (int i = 0; i <= 10; i++)
	{
		package_num = package_num + (char)(rand() % 10 + 48);
	}
}
void Package::Getprice(double weight) {
	money = weight * 15;
}
void Fragile::Getprice(double weight) {
	money = weight * 8;
}
void Book::Getprice(double weight) {
	money = weight * 2;
}
void Normal::Getprice(double weight) {
	money = weight * 5;
}
