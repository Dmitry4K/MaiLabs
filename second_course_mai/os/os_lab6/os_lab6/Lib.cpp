#include<zmq.h>
#include<string>
#include<iostream>
#include<malloc.h>
#include<Windows.h>
#include<vector>
#include"Lib.h"

inline bool space(char c) {
	return std::isspace(c);
}

inline bool notspace(char c) {
	return !std::isspace(c);
}

//break a sentence into words
std::vector<std::string> split(const std::string& s) {
	typedef std::string::const_iterator iter;
	std::vector<std::string> ret;
	iter i = s.begin();
	while (i != s.end()) {
		i = std::find_if(i, s.end(), notspace); // find the beginning of a word
		iter j = std::find_if(i, s.end(), space); // find the end of the same word
		if (i != s.end()) {
			ret.push_back(std::string(i, j)); //insert the word into vector
			i = j; // repeat 1,2,3 on the rest of the line.
		}
	}
	return ret;
}

Publisher::Publisher() {
	Context = zmq_ctx_new();
	Socket = zmq_socket(Context, ZMQ_PUB);
	GetContext = zmq_ctx_new();
	GetSocket = zmq_socket(GetContext, ZMQ_SUB);
	ChildHProcess = nullptr;
}
Publisher::~Publisher() {
	zmq_close(Socket);
	zmq_ctx_destroy(Context);
}

int Publisher::Bind(const char* addres) {
	return zmq_bind(Socket, addres);
}
void Publisher::Publish(const std::string str) {
	zmq_msg_t message;
	zmq_msg_init_size(&message, str.length()+1 );
	memcpy(zmq_msg_data(&message), str.c_str(), str.length()+1);
	zmq_msg_send(&message, Socket, 0);
	zmq_msg_close(&message);
}


std::string Publisher::GetMsg() {
	std::string Buffer;
	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, GetSocket, 0);
	char* str = new char[zmq_msg_size(&reply)];
	memcpy(str, zmq_msg_data(&reply), zmq_msg_size(&reply));
	zmq_msg_close(&reply);
	Buffer = std::string(str);
	delete[] str;
	return Buffer;
}

void Publisher::ConnectTo(const char* addres) {
	zmq_connect(GetSocket, addres);
	zmq_setsockopt(GetSocket, ZMQ_SUBSCRIBE, 0, 0);
}
int Publisher::NewProcess(std::string str) {
		std::string ProcessName = "subscriber " + str;
		char* lpszClientPath = (char*)ProcessName.c_str();

		PROCESS_INFORMATION ProcessInfo;
		ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
		STARTUPINFO StartupInfo;
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof(STARTUPINFO);
		int process = CreateProcess(NULL,
			lpszClientPath,
			//lpszClientPath,//процесс
			NULL, NULL, true, //CREATE_NO_WINDOW| CREATE_SUSPENDED
			CREATE_NO_WINDOW,// 
			NULL, NULL,
			&StartupInfo,
			&ProcessInfo);
		ChildId = std::stoi(str);
		ChildHProcess = ProcessInfo.hProcess;
		return ProcessInfo.dwProcessId;
}


Subscriber::Subscriber() {
	SubContext = zmq_ctx_new();
	SubSocket = zmq_socket(SubContext, ZMQ_SUB);
	PublishContext = zmq_ctx_new();
	PublishSocket = zmq_socket(PublishContext, ZMQ_PUB);
	PullContext = zmq_ctx_new();
	PullSocket = zmq_socket(PullContext, ZMQ_PUSH);
	if(zmq_connect(PullSocket, "tcp://localhost:2") != 0)
		std::cout<<"Error\n";
}
Subscriber::~Subscriber() {
	zmq_close(PublishSocket);
	zmq_ctx_destroy(PublishContext);
	zmq_close(SubSocket);
	zmq_ctx_destroy(SubContext);
}
void Subscriber::ConnectTo(const char* addres) {
	zmq_connect(SubSocket, addres);
	zmq_setsockopt(SubSocket, ZMQ_SUBSCRIBE, 0, 0);
}

int Subscriber::Bind(const char* addres) {
	return zmq_bind(PublishSocket, addres);
}
std::string Subscriber::GetMsg() {
	std::string Buffer;
	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, SubSocket, 0);
	char* str = new char[zmq_msg_size(&reply)];
	memcpy(str, zmq_msg_data(&reply), zmq_msg_size(&reply));
	zmq_msg_close(&reply);
	Buffer = std::string(str);
	delete[] str;
	return Buffer;
}

int Subscriber::NewProcess(int NewId, int NumSocket, int ParentId, int ParentSocket) {
	
	//новому процссу нужно передать его id и сокет и Id родителя и Сокет Родителя
	std::string ProcessName = "subscriber " + std::to_string(NewId) + ' ' + std::to_string(NumSocket) + ' '+ std::to_string(ParentId) + ' ' + std::to_string(ParentSocket);
	char* lpszClientPath = (char*)ProcessName.c_str();

	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	int process = CreateProcess(NULL,
		lpszClientPath,
		//lpszClientPath,//процесс
		NULL, NULL, true, //CREATE_NO_WINDOW| CREATE_SUSPENDED
		CREATE_NO_WINDOW ,// CREATE_NEW_CONSOLE
		NULL, NULL,
		&StartupInfo,
		&ProcessInfo);
	//ChildId = std::stoi(str);
	if (NewId > Id) {
		RightChildId = NewId;
		RightChildHProcess = ProcessInfo.hProcess;
	}
	else {
		LeftChildId = NewId;
		LeftChildHProcess = ProcessInfo.hProcess;
	}
	return ProcessInfo.dwProcessId;
}

void Subscriber::Publish(const std::string str) {
	zmq_msg_t message;
	zmq_msg_init_size(&message, str.length() + 1);
	memcpy(zmq_msg_data(&message), str.c_str(), str.length() + 1);
	zmq_msg_send(&message, PublishSocket, 0);
	zmq_msg_close(&message);
}

PullDumb::PullDumb() {
	std::string Adress = "tcp://*:2";
	Context = zmq_ctx_new();
	Socket = zmq_socket(Context, ZMQ_PULL);
	if(zmq_bind(Socket, Adress.c_str()) != 0)
		std::cout<<"Error";
}

std::string PullDumb::GetMsg() {
	std::string Buffer;
	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, Socket, 0);
	char* str = new char[zmq_msg_size(&reply)];
	memcpy(str, zmq_msg_data(&reply), zmq_msg_size(&reply));
	zmq_msg_close(&reply);
	Buffer = std::string(str);
	delete[] str;
	return Buffer;
}

void Subscriber::PullMsg(std::string str) {
	zmq_msg_t message;
	zmq_msg_init_size(&message, str.length() +  1);
	memcpy(zmq_msg_data(&message), str.c_str(), str.length() + 1);
	zmq_msg_send(&message, PullSocket, 0);
	zmq_msg_close(&message);
}