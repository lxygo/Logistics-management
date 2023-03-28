#include"system1.h"
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
void Package::SendPackage(string t_sender, string t_receiver, SOCKET& client_socket) {
	char buf[BUFSIZ] = { 0 };
	receiver = t_receiver;
	sender = t_sender;
	cout << "请输入包裹的名称：" << endl;
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	package_name = buf;
	cout << "请输入快件的备注：" << endl;
	memset(buf, 0, BUFSIZ);
	recv(client_socket, buf, BUFSIZ, 0);
	note = buf;
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
void Package::ShowPackage(SOCKET& client_socket)const
{
	string send_message;
	cout << "快递单号：" << package_num << endl;
	send_message = "快递单号：" + package_num + "\n";
	cout << "快递名称：" << endl;
	send_message = send_message + "快递名称：" + package_name + "\n";
	cout << "寄件人：" << sender << endl;
	send_message = send_message + "寄件人：" + sender + "\n";
	cout << "收件人：" << receiver << endl;
	send_message = send_message + "收件人：" + receiver + "\n";
	cout << "快递金额：" << money << endl;
	send_message = send_message + "快递金额：" +to_string(money) + "\n";
	cout << "快递备注：" << note << endl;
	send_message = send_message + "快递备注：" + note + "\n";
	if (collect)
	{
		cout << "已揽收" << endl;
		send_message = send_message + "已揽收" + "\n";
		cout << "寄件时间：" << send_time[0] << "年" << send_time[1] << "月" << send_time[2] << "日 " << send_time[3] << ":" << send_time[4] << ":" << send_time[5] << endl;
		send_message = send_message + "寄件时间：" + to_string(send_time[0]) + "年" + to_string(send_time[1]) + "月" + to_string(send_time[2]) + "日" + to_string(send_time[3]) + ":" + to_string(send_time[4]) + ":" + to_string(send_time[5]) + "\n";
		cout << "揽收时间：" << collect_time[0] << "年" << collect_time[1] << "月" << collect_time[2] << "日 " << collect_time[3] << ":" << collect_time[4] << ":" << collect_time[5] << endl;
		send_message = send_message + "揽件时间：" + to_string(collect_time[0]) + "年" + to_string(collect_time[1]) + "月" + to_string(collect_time[2]) + "日" + to_string(collect_time[3]) + ":" + to_string(collect_time[4]) + ":" + to_string(collect_time[5]) + "\n";
	}
	else if (collect == false)
	{
		cout << "待揽收" << endl;
		send_message = send_message + "待揽收" + "\n";
		cout << "寄件时间：" << send_time[0] << "年" << send_time[1] << "月" << send_time[2] << "日 " << send_time[3] << ":" << send_time[4] << ":" << send_time[5] << endl;
		send_message = send_message + "寄件时间：" + to_string(send_time[0]) + "年" + to_string(send_time[1]) + "月" + to_string(send_time[2]) + "日" + to_string(send_time[3]) + ":" + to_string(send_time[4]) + ":" + to_string(send_time[5]) + "\n";
	}
	if (sign)
	{
		cout << "已签收 " << endl;
		send_message = send_message + "已签收" + "\n";
		cout << "寄件时间：" << send_time[0] << "年" << send_time[1] << "月" << send_time[2] << "日 " << send_time[3] << ":" << send_time[4] << ":" << send_time[5] << endl;
		send_message = send_message + "寄件时间：" + to_string(send_time[0]) + "年" + to_string(send_time[1]) + "月" + to_string(send_time[2]) + "日" + to_string(send_time[3]) + ":" + to_string(send_time[4]) + ":" + to_string(send_time[5]) + "\n";
		cout << "签收时间：" << receive_time[0] << "年" << receive_time[1] << "月" << receive_time[2] << "日 " << receive_time[3] << ":" << receive_time[4] << ":" << receive_time[5] << endl;
		send_message = send_message + "签收时间：" + to_string(receive_time[0]) + "年" + to_string(receive_time[1]) + "月" + to_string(receive_time[2]) + "日" + to_string(receive_time[3]) + ":" + to_string(receive_time[4]) + ":" + to_string(receive_time[5]) + "\n";
	}
	else if ((sign == false) && (collect == true))
	{
		cout << "待签收" << endl;
		send_message = send_message + "待签收" + "\n";
		cout << "寄件时间：" << send_time[0] << "年" << send_time[1] << "月" << send_time[2] << "日 " << send_time[3] << ":" << send_time[4] << ":" << send_time[5] << endl;
		send_message = send_message + "寄件时间：" + to_string(send_time[0]) + "年" + to_string(send_time[1]) + "月" + to_string(send_time[2]) + "日" + to_string(send_time[3]) + ":" + to_string(send_time[4]) + ":" + to_string(send_time[5]) + "\n";
	}
	send(client_socket, send_message.c_str(), send_message.length(), 0);
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
