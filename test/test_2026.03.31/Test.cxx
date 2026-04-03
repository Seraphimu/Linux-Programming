#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <unistd.h>


const int PROCESS_NUM = 3;

class SubEp {
public:
	static int num;
	std::string name;
	pid_t subId;
	int writeFd;

	SubEp(pid_t subId, int writeFd) {
		this->subId = subId;
		this->writeFd = writeFd;
		char nameBuf[64];
		snprintf(nameBuf, sizeof nameBuf, "p%d_n%d_f%d", 
				this->subId, num++, this->writeFd);
		this->name = nameBuf;
	}
};

//The number id of process
int SubEp::num = 0;
//func_t指向无参数且返回void的函数
typedef void (*func_t)();

void createSubs(std::vector<SubEp> & subs, std::vector<func_t> & funcMap);
void loadTaskFunc(std::vector<func_t> * funcMap);
void downloadTask();
void ioTask();
void flushTask();

int main(void) {

//	std::vector<void (*)()> funcMap;
	std::vector<func_t> funcMap;
	loadTaskFunc(&funcMap);

	funcMap[0]();
	funcMap[1]();
	funcMap[2]();

//	std::vector<SubEp> subs;
//	createSubs(subs);
//
//	for (int i = 0; i < subs.size(); i++) {
//		std::cout << "subs name: "  << subs[i].name << std::endl;
//	}

	return 0;
}

void createSubs(std::vector<SubEp> & subs, std::vector<func_t> & funcMap) {

	for (int i = 0; i < PROCESS_NUM; i++) {
		//给每个子进程创建一个与父进程通信的专用管道
		int fds[2];
		int ret = pipe(fds);
		assert(ret == 0);
		// 消除"未使用变量n"的警告
		//assert()仅在debug模式下生效，如果要release的话，相当于assert()这句就没了
		void(ret);
		
		int id = fork();
		assert(id >= 0);

		//child close the write end 
		//不必纠结为什么会进程爆炸，只需要加上exit()防止进程爆炸即可。
		if (id == 0) {
			close(fds[1]);
			
			exit(0);
		}

		SubEp sub(id, fds[1]);
		subs.push_back(sub);

		//parent, close the read end
		close(fds[0]);

	}

}


void loadTaskFunc(std::vector<func_t> * funcMap) {
	assert(funcMap);
	funcMap->push_back(downloadTask);
	funcMap->push_back(ioTask);
	funcMap->push_back(flushTask);
}



//A set of print()
void downloadTask()
{
    std::cout << getpid() << ": 下载任务\n"
              << std::endl;
    sleep(1);
}

void ioTask()
{
    std::cout << getpid() << ": IO任务\n"
              << std::endl;
    sleep(1);
}

void flushTask()
{
    std::cout << getpid() << ": 刷新任务\n"
              << std::endl;
    sleep(1);
}
