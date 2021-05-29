#include<iostream>
#include<memory>
#include<vector>
#include<thread>
#include<mutex>
#include<future>
#include<condition_variable>
#include"pentagon.h"
#include"trapeze.h"
#include"rhombus.h"
#include"figure.h"
#include"factory.h"
#include"handler.h"
#include"hanlders.h"


void handle(std::vector<std::unique_ptr<figure>>& figures, int buffer_size, std::condition_variable& readed, std::condition_variable& handled, std::mutex& mtx, bool& Stop) {
	std::unique_lock<std::mutex> lock(mtx);
	handled.notify_all();
	std::vector<std::unique_ptr<handler>> handlers;

	handlers.push_back(std::make_unique<file_handler>());
	handlers.push_back(std::make_unique<console_handler>());
	while (!(Stop)) {
		readed.wait(lock);
		//std::cout << figures.size() << std::endl;
		for (int i = 0; i < handlers.size(); ++i) {
			handlers[i]->execute(figures);
		}
		figures.clear();
		handled.notify_all();
	}
	return;
}
int main(int argc, char* argv[]) {
	if (argc != 2)
		return 1;
	std::condition_variable readed;
	std::condition_variable handled;
	std::vector<std::unique_ptr<figure>> figures;
	std::unique_ptr<factory> my_factory;
	std::mutex mtx;
	std::unique_lock<std::mutex> lock(mtx);
	int buffer_size, menu;
	buffer_size = std::stoi(argv[1]);
	bool stop = false;
	std::thread handler(handle, std::ref(figures), buffer_size, std::ref(readed), std::ref(handled),ref(mtx), std::ref(stop));
	handled.wait(lock);
	while (true) {
		for (int i = 0; i < buffer_size; ++i) {
			std::cout << "1. Pentagon" << std::endl;
			std::cout << "2. Rhombus" << std::endl;
			std::cout << "3. Trapeze" << std::endl;
			std::cin >> menu;
			switch (menu) {
			case 1 :
				my_factory = std::make_unique<pentagon_factory>();
				figures.push_back(my_factory->build(std::cin));
				break;
			case 2 :
				my_factory = std::make_unique<rhombus_factory>();
				figures.push_back(my_factory->build(std::cin));
				break;
			case 3 :
				my_factory = std::make_unique<trapeze_factory>();
				figures.push_back(my_factory->build(std::cin));
				break;
			}
		}
		readed.notify_all();
		handled.wait(lock);
		std::cout << "Continue? 'y' - Yes 'n' - No" << std::endl;
		char answer;
		std::cin >> answer;
		if (answer != 'y')
			break;
	}
	stop = true;
	readed.notify_all();
	lock.unlock();
	handler.join();
	return 0;
}