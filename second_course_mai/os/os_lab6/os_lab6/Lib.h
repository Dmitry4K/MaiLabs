#pragma once
#include<zmq.h>
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#define COMMAND_EXIT -1
#define COMMAND_DONE  0
#define PROCESS_EXIST 1
#define PUBLISHER 10001

inline bool space(char c);

inline bool notspace(char c);

//break a sentence into words
std::vector<std::string> split(const std::string& s);

struct Publisher {
		int SocketNum = -1;
		void* Context = nullptr;
		void* Socket = nullptr;
		void* GetContext = nullptr;
		void* GetSocket = nullptr;
		int ChildId = -1;
		HANDLE ChildHProcess;
		int Bind(const char* addres);
		void Publish(const std::string str);
		int NewProcess(std::string str);
		void ConnectTo(const char* addres);
		std::string GetMsg();
		Publisher();
		~Publisher();
};
struct Subscriber {
	Subscriber();
	~Subscriber();
	int NewProcess(int NewId, int NumSocket, int ParentId, int ParentSocket);
	int Id = -1;
	int SocketNumber = -1;
	int ParentId = -1;
	int ParentSocket = -1;
	void* PullContext = nullptr;
	void* PullSocket = nullptr;
	void* PublishContext = nullptr;
	void* PublishSocket = nullptr;
	void* SubContext = nullptr;
	void* SubSocket = nullptr;
	int LeftChildId = -1;
	int RightChildId = -1;
	HANDLE LeftChildHProcess = NULL;
	HANDLE RightChildHProcess = NULL;
	void Publish(const std::string str);
	void ConnectTo(const char* addres);
	int Bind(const char* addres);
	std::string GetMsg();
	void PullMsg(std::string str);
};

struct PullDumb {
	void* Context;
	void* Socket;
	PullDumb();
	std::string GetMsg();

};