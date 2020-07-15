#include<zmq.h>
#include<iostream>
#include<vector>
#include<thread>
#include<string>
#include<set>
#include<mutex>
#include<chrono>
#include"BinarySearchTree.h"
#include"Lib.h"


void PullDumbThreadFunction(MyBSTree& Tree) {
    PullDumb PD;
    bool isPingStart = false;
    std::string Buffer;
    std::set<int> Ids;
    auto t1 = std::chrono::high_resolution_clock::now();
    while (true) {
        Buffer = PD.GetMsg();
        if (Buffer == "exit")
            break;
        std::vector<std::string> Commands = split(Buffer);
        
        if (Commands[0] == "ping") {
            Ids.insert(std::stoi(Commands[1]));
            continue;
        }
        if(Commands[0] == "pend"){
            if (Ids.size() == Tree.Size()) {
                std::cout << "Ping: OK: -1\n";
            }
            else {
                std::cout << "Ping: OK: ";
                std::vector<int> Nodes = Tree.GetNodes();
                for (int i = 0; i < Nodes.size();++i) {
                        auto iter = Ids.find(Nodes[i]);
                        if (iter == Ids.end()) {
                            std::cout << Nodes[i] << " ";
                        }
                    }
                std::cout << std::endl;
            }
            Ids.clear();
        }
        else {
            std::cout << Buffer;
        }
    }
}

int main() {
    MyBSTree NodeMenedger;
    std::thread PullDumbThread(PullDumbThreadFunction,std::ref(NodeMenedger));
	Publisher Pub;
    Pub.SocketNum = 1;
    std::vector<std::string> Commands;
    std::string Line;
    bool exit = false;
    if (Pub.Bind("tcp://*:1")) {
        std::cout << "Binded\n";
    }
    while (true) {
        std::getline(std::cin, Line);
        Commands = split(Line);
        for (int i = 0; i < Commands.size(); ++i) {
            if (Commands[i] == "create") {
                ++i;
                IdAndSocket Temp{ stoi(Commands[i]), 0 };
                if (Pub.ChildId == -1) {//если узла нет, то создаем его
                    Temp = NodeMenedger.GenNewSocketNumber();//Генерируем сокет для узла
                    Temp.Id = stoi(Commands[i]);
                    NodeMenedger.Add(Temp);
                    std::cout << "OK: ";
                    std::cout<<Pub.NewProcess(Commands[i]+' '+std::to_string(Temp.SocketNumber)+' '+ std::to_string(PUBLISHER) +' '+std::to_string(Pub.SocketNum));//добавить аргумент
                    std::cout << std::endl;
                                                                                                                                                                   //новому процссу нужно передать его id и сокет и Id родителя и Сокет Родителя
                }
                else {
                    if (!NodeMenedger.Exist(Temp)) {//если узла нет, то отправляем сообщение.
                        Temp = NodeMenedger.GenNewSocketNumber();//Генерируем сокет для узла
                        Temp.Id = stoi(Commands[i]);
                        NodeMenedger.Add(Temp);
                        Pub.Publish(Commands[i - 1] + ' ' + Commands[i] + ' ' + std::to_string(Temp.SocketNumber));
                    }
                    else {
                        std::cout << "Exist\n";
                    }
                }
            }
            if (Commands[i] == "exit") {
                Pub.Publish(Commands[i]);
                exit = true;
                Subscriber s;
                s.PullMsg("exit");
                break;
            }
            if (Commands[i] == "remove") {
                ++i;
                if (Pub.ChildId == std::stoi(Commands[i])) {
                    Pub.ChildId = -1;
                    NodeMenedger.Destroy();
                    Pub.Publish("exit");
                }
                else {
                    if (NodeMenedger.Exist(IdAndSocket{ std::stoi(Commands[i]), -1 })) {
                        NodeMenedger.Delete(IdAndSocket{ std::stoi(Commands[i]), -1 });
                        Pub.Publish(Commands[i - 1] + ' ' + Commands[i]);
                    }
                    else {
                        std::cout << "Not Found\n";
                    }
                }
            }
            if (Commands[i] == "exec") {//exec id pattern file
                if (NodeMenedger.Exist(IdAndSocket{ std::stoi(Commands[i + 1]), -1 })) {
                    Pub.Publish(Commands[i] + ' ' + Commands[i+1] + ' ' + Commands[i + 2] + ' ' + Commands[i + 3]);
                    std::string address = "tcp://localhost:" + Commands[i + 1];
                    IdAndSocket Res = NodeMenedger.Find(IdAndSocket{ std::stoi(Commands[i + 1]), -1 });
                }
                else {
                    std::cout << "Not found\n";
                }
                i += 3;
            }
            if (Commands[i] == "pingall") {
                Subscriber s;
                Pub.Publish("pingall");
                Sleep(100);
                s.PullMsg("pend");
            }
        }
        if (exit)
            break;
    }
    WaitForSingleObject(Pub.ChildHProcess, INFINITE);
    PullDumbThread.join();
	return 0; 
}