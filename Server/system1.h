#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include"../tcpSocket/tcpSocket.h"
using namespace std;
class Users {  //用户类
public:
	string name;//用户的姓名
	string phone;//用户的电话
	double money;//用户余额
	string address;//用户地址
	string user_name;//用户名（保证唯一）
	string password;//密码
	bool new_users;//判断是否是新用户
	int send_numbers;	//发送的快递数量
	int receive_numbers;//接受的快递数量
	int not_re_nums;	//未接收的快递数量

	vector<int>send_package;//发送的快递
	vector<int>not_receive;//未签收的快递
	vector<int>receive;//接收快递

	Users();
	void Register(string id, SOCKET& client_socket);//注册
	void Change_password(SOCKET& client_socket);//修改密码
	void Change_message(SOCKET& client_socket);//修改个人信息
	void Add_money(SOCKET& client_socket);//充值
	void Show_money(SOCKET& client_socket)const;//查询余额
	void Storeusers(ofstream& out);//将信息写入文件
	void Readusers(ifstream& in);//读取用户信息
};
class Package {  //快递类
public:
	string package_num;//快递单号
	int send_time[6] = { 0 };//发送时间
	int receive_time[6] = { 0 };//接收时间
	int collect_time[6] = { 0 };//揽收时间
	double money;//快递价格
	string package_name;//包裹名称
	string type;//快递的类型
	string sender;//寄件用户
	string receiver;//收件用户
	bool sign;//签收状态
	bool collect;//揽收状态
	string note;//快递备注
	Package();
	virtual void Getprice(double weight);
	void StorePackage(ofstream& out);//存储快件信息
	void ReadPackage(ifstream& in);//读取快件信息
	void SendPackage(string t_sender, string t_receiver, SOCKET& client_socket);//记录寄件时间，寄件人，收件人，签收状态
	void CollectPackage();
	void ReceivePackage();//记录签收状态，签收时间
	void ShowPackage(SOCKET& client_socket)const;//查询快件状态
	void Make_PackageNum();//随机生成快递单号
};
class Fragile :public Package {
public:
	Fragile() :Package() {
		type = "易碎品";
	}
	virtual void Getprice(double weight);
};
class Book :public Package {
public:
	Book() :Package() {
		type = "图书";
	}
	virtual void Getprice(double weight);
};
class Normal :public Package {
public:
	Normal() :Package() {
		type = "普通快递";
	}
	virtual void Getprice(double weight);
};
class Administrator {  //管理员类
public:
	string user_name;
	string name;
	string password;
	double money;//账户余额
	vector<int>courier_order;
	Administrator();
	void Changename(SOCKET& client_socket);//修改管理员名称
	void Changepassword(SOCKET& client_socket);//修改密码
	void Storeadmin(ofstream& out);//存储信息
	void Readadmin(ifstream& in);//读取信息
};
class Courier { //快递员类
public:
	string user_name;//用户名
	string name;//姓名
	string phone;//电话
	double money;//余额
	string password;//密码
	vector<int>not_collect;//未揽收的快递
	vector<int>collect;//已揽收快递
	int not_collect_num;//未揽收的快递数量
	int collect_num;//已揽收的快递数量

	Courier();
	void Changepassword();
	void Showmoney();
	void Storecour(ofstream& out);
	void Readcour(ifstream& in);
};
class System {
public:
	vector<Users> allusers;//所有用户的集合
	vector<Package> allpackages;//所有快递的集合
	vector<Administrator>alladmin;//所有管理员的集合
	vector<Courier>allcouriers;//所有快递员的集合
	vector<int>not_assign;//未分配快递员的快件
	int users_num;//用户数量
	int packages_num;//快件数量
	int admin_num;//管理员数量
	int courier_num;//快递员的数量
	int not_assgin_num;//待分配的快件数量
	System();
	void Store();//存储系统信息
	void Read();//读取系统信息
	int Finduser(string user_name);//查找用户是否存在
	int Findcourier(string courier_name);//查找快递员是否存在
	bool Log(int order, SOCKET& client_socket);//是否登录成功
	bool Adminlog(SOCKET& client_socket);
	bool Courierlog(string id,SOCKET&client_socket);
	void Send(int sender, int receiver,SOCKET& client_socket);//寄件时对状态进行修改
	void Receive(int receiver, SOCKET& client_socket);//签收时进行相关状态的修改
	void Show(int sender, SOCKET& client_socket);//查询用户相关的快递
	void Showbytime(int order, SOCKET& client_socket)const;//用户通过时间来查找
	void Showbynum(int order, SOCKET& client_socket);//用户通过快递单号来查找
	void Adshowbyuser(SOCKET&client_socket);//管理员通过用户名查询用户信息
	void Adshowbyallusers(SOCKET&client_socket)const;//管理员输出所有用户的信息
	void Adshowbyid(SOCKET&client_socket);//管理员通过用户名来查询快递
	void Adshowbytime(SOCKET&client_socket)const;//管理员通过时间查询历史快递
	void Adshowbynum(SOCKET&client_sockeet)const;//管理员通过单号进行查询历史快递
	void Adshowcourier(SOCKET&client_socket);//管理员查询快递员的信息
	void Adshowcollect(SOCKET& client_socket);//管理员查询快递员的揽收信息
	void Addcourier(SOCKET&client_socket);//添加快递员
	void Deletecourier(SOCKET&client_socket);//删除快递员
	void Assign(SOCKET&client_socket);//分配快递
	void Coshow_not_collect(int order,SOCKET&client_socket)const;//快递员查询自己未揽收的快递
	void CoSearch(int order,SOCKET&client_socket);//快递员查询自己的揽收的所有快递信息
	void Coshowbysend(int order,SOCKET&client_socket)const;//快递员通过发送人查询快递
	void Coshowbyreceive(int order,SOCKET&client_socket)const;//快递员通过接收人查询快递
	void Coshowbytime(int order,SOCKET&client_socket)const;//快递员通过时间来查询快递信息
	void Coshowbynum(int order,SOCKET&client_socket)const;//快递员通过快递单号进行快递查询
	void Cocollect(int order,SOCKET&client_socket);//快递员揽收快件

};
