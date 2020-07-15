#include"../os_lab6/Lib.h"
#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<Windows.h>

std::vector<int> function(std::string& Pattern, std::string& Str){ 
        // i-с какого места строки  ищем
        // j-с какого места образца ищем
    std::vector<int> res;
    size_t n = 0, from = 0;
    while ((n = Str.find(Pattern, from)) != std::string::npos) {
        res.push_back(n);
        from = n + Pattern.size();
    }
    return res;
}
int main(int argc, char* argv[]) {
    //нужно считать присвоенный id , сокет, id и сокет родителя.
	Subscriber sub;
	sub.Id = std::stoi(argv[1]);
    sub.SocketNumber = std::stoi(argv[2]);
    sub.ParentId = std::stoi(argv[3]);
    sub.ParentSocket = std::stoi(argv[4]);
    std::cout << "Id: " << sub.Id << " PSocket: " << sub.SocketNumber << " ParentId: " << sub.ParentId << " ParentSocket: " << sub.ParentSocket << std::endl;
    std::vector<std::string> Commands;
    std::string Line;
    bool exit = false;
    std::string Adress = "tcp://*:";
    Adress += std::to_string(sub.SocketNumber);
    if (sub.Bind(Adress.c_str())) {
        std::cout << "Binded\n";
    }
    std::cout << Adress << std::endl;
    Adress = "tcp://localhost:" + std::to_string(sub.ParentSocket);
    std::cout << Adress << std::endl;
    sub.ConnectTo(Adress.c_str());
    while (true) {
        Line = sub.GetMsg();
        std::cout << Line << std::endl;
        Commands = split(Line);
        for (int i = 0; i < Commands.size(); ++i) {
            if (Commands[i] == "create") {
                ++i;
                int NewId = std::stoi(Commands[i]);
                int NewSocket = std::stoi(Commands[i+1]);
                std::string msg = Commands[i - 1] + ' ' + Commands[i] + ' ' + Commands[i + 1];
                if (sub.ParentId == PUBLISHER) {
                    if ((sub.LeftChildId == -1) && (sub.Id > NewId)) {
                        int PID = sub.NewProcess(NewId, NewSocket, sub.Id, sub.SocketNumber);
                        sub.PullMsg("OK: " + std::to_string(PID) + '\n');
                    }
                    else if ((sub.RightChildId == -1) && (sub.Id < NewId)) {
                        int PID = sub.NewProcess(NewId, NewSocket, sub.Id, sub.SocketNumber);
                        sub.PullMsg("OK: " + std::to_string(PID)+'\n');
                    }
                    else {
                        sub.Publish(msg);
                       }
                    ++i;
                    continue;
                }
                if (NewId > sub.ParentId) {
                    if (sub.Id > sub.ParentId) {
                        if ((sub.LeftChildId == -1) && (sub.Id > NewId)) {
                            int PID = sub.NewProcess(NewId, NewSocket, sub.Id, sub.SocketNumber);
                            sub.PullMsg("OK: " + std::to_string(PID) + '\n');
                        }
                        else if ((sub.RightChildId == -1) && (sub.Id < NewId)) {
                            int PID = sub.NewProcess(NewId, NewSocket, sub.Id, sub.SocketNumber);
                            sub.PullMsg("OK: " + std::to_string(PID) + '\n');
                        }
                        else {
                            sub.Publish(msg);
                        }
                    }
                    else {
                        ++i;
                    }
                }
                else if (sub.Id < sub.ParentId) {
                    if ((sub.LeftChildId == -1) && (sub.Id > NewId)) {
                        int PID = sub.NewProcess(NewId, NewSocket, sub.Id, sub.SocketNumber);
                        sub.PullMsg("OK: " + std::to_string(PID) + '\n');
                    }
                    else if ((sub.RightChildId == -1) && (sub.Id < NewId)) {
                        int PID = sub.NewProcess(NewId, NewSocket, sub.Id, sub.SocketNumber);
                        sub.PullMsg("OK: " + std::to_string(PID) + '\n');
                    }
                    else {
                        sub.Publish(msg);
                    }
                }
                else {
                    ++i;
                }
            }
            if (Commands[i] == "exit") {
                sub.Publish(Commands[i]);
                exit = true;
                break;
            }
            if (Commands[i] == "exec") {//exec Id pattern file
                ++i;
                if (sub.Id == std::stoi(Commands[i])) {
                    //function
                    std::vector<int> indexes = function(Commands[i + 1], Commands[i + 2]);
                    i += 2;
                    std::string res = "Result: " + std::to_string(indexes.size())+ ' ';
                    for (int i = 0; i < indexes.size(); ++i) {
                        res += std::to_string(indexes[i])+ ' ';
                    }
                    std::cout << res << std::endl;
                    sub.PullMsg(res + '\n');
                    //sub.Publish(res);
                    //publish
                }
                else {
                    DWORD StatusProcess1;
                    GetExitCodeProcess(sub.LeftChildHProcess, &StatusProcess1);
                    DWORD StatusProcess2;
                    GetExitCodeProcess(sub.RightChildHProcess, &StatusProcess2);
                    //std::cout << (StatusProcess1 == STILL_ACTIVE) << (StatusProcess2 == STILL_ACTIVE) << std::endl;
                    std::cout << std::stoi(Commands[i]) << sub.LeftChildId << sub.RightChildId << std::endl;
                    if((std::stoi(Commands[i]) == sub.LeftChildId) && (StatusProcess1 != STILL_ACTIVE)){
                            sub.PullMsg("Error: Node is unavaible\n");
                            i += 2;
                        }
                    else if ((std::stoi(Commands[i]) == sub.RightChildId) && (StatusProcess2 != STILL_ACTIVE)) {
                            sub.PullMsg("Error: Node is unavaible\n");
                            i += 2;
                        }
                        else {
                        sub.Publish(Commands[i - 1] + ' ' + Commands[i] + ' ' + Commands[i + 1] + ' ' + Commands[i + 2]);//добавить параметры

                            i += 2;
                        }
                    }
                }
            if (Commands[i] == "remove") {
                ++i;
                if (sub.LeftChildId == std::stoi(Commands[i])) {
                    sub.LeftChildId = -1;
                }
                if (sub.RightChildId == std::stoi(Commands[i])) {
                    sub.RightChildId = -1;
                }
                if (sub.Id == std::stoi(Commands[i])) {
                    sub.Publish("exit");
                    exit = true;
                    break;
                }
                else {
                    sub.Publish(Commands[i - 1] + ' ' + Commands[i]);
                }
            }
            if (Commands[i] == "pingall") {
                sub.PullMsg("ping " + std::to_string(sub.Id));
                sub.Publish("pingall");
            }
        }
        if (exit)
            break;
    }
    std::cout << "Closing\n";
    WaitForSingleObject(sub.LeftChildHProcess, INFINITE);
    WaitForSingleObject(sub.RightChildHProcess, INFINITE);
	return 0;
}