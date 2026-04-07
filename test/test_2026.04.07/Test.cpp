#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>


//使用Ctrl+C去终止这个死循环
void testCtrlC(); 

//信号捕捉
void testSignal();
void handerCtrlC(int sig);

//
void testKill(int argc, char * argv[]);
void handerKill(int sig);

void testRaise();
void testRaiseHandler(int sig);

void testAbort();
void testAbortHandler(int sig);


//这两种打印方式下，怎么对cnt的值差了这么多！
//因为方式1一直在打印，每一次cnt++都要有一次I/O
//但是方式2直到最后打印才产生了一次I/O
void testAlarm();
void testAlarm1();
void testAlarm14();
void testAlarm14Handler(int sig);
long long int cnt = 1;


void testHardwareExp();
void testMMUExp();
void testMMUExpHandler(int sig);

int main(int argc, char *argv[])
{
	//testCtrlC();
	//testSignal();
	
	//testKill(argc, argv);
	//testRaise();
	//testAbort();

	//testAlarm();
	//testAlarm1();
	//testAlarm14();

	//testHardwareExp();
	testMMUExp();

	return 0;
}



void testCtrlC() {
	while (1){
		printf("I am a process,I am waitting signal\n");
		sleep(1);
	}
}

void testSignal() {
	//提前绑定好遇到2信号的处理方式
	//又因为绑定了处理方式，结果本来的退出进程变成了打印一条消息
	//与Qt的信号槽机制高度相似，从这也能看出来信号与信号量半角钱关系都没有
	//信号就是信号，只要触发了，就会有处理方式，不处理本身就是一种处理方式
	//而信号量就是表示资源数量的量
	signal(2, &handerCtrlC);
	testCtrlC();	
}
void handerCtrlC(int sig) {
	printf("recevie the signal: %d\n", sig);
}



void testKill(int argc, char * argv[]){
	//9号信号绑定处理函数
	signal(9, &handerKill);
	if (argc == 3) {
		//进程号，发送信号
		// int kill(pid_t pid, int sig);
		// On success, zero is returned.  On error, -1 is returned
		kill(atoi(argv[1]), atoi(argv[2]));
	}
}
void handerKill(int sig) {
	printf("捕捉到信号：%d\n", sig);
}

// raise函数相当于kill(getpid(), sig)
void testRaise() {
	signal(2, &testRaiseHandler);
	while (1) {
		raise(2);
		sleep(1);
	}
}

void testRaiseHandler(int sig) {
	printf("recevie the signal: %d\n", sig);
}


void testAbort() {
	//void abort(void);
	signal(6, &testAbortHandler);
	while (1) {
		//If  the SIGABRT signal is ignored, or caught by a handler that returns, 
		//the abort() function will still terminate the process. 
		abort();
	}
}

void testAbortHandler(int sig) {
	printf("recevie the signal: %d\n", sig);
}

void testAlarm() {
	//unsigned alarm(unsigned seconds);
	alarm(5);

	while (1) {
		std::cout << "cnt: " << cnt << std::endl;
		cnt++;
		sleep(1);
	}
}


void testAlarm1() {
	alarm(1);
	while (1) {
		std::cout << "cnt: " << cnt << std::endl;
		cnt++;
	}
}

void testAlarm14() {
	signal(14, &testAlarm14Handler);
	alarm(1);
	while (1) {
		cnt++;
	}
}

void testAlarm14Handler(int sig) {
	std::cout << "cnt: " << cnt << std::endl;
	exit(0);
}

void testHardwareExp() {
	int a = 100;
	int b = 0;

	int c = a/b;
}


void testMMUExp() {
	//signal(11, &testMMUExpHandler);
	int * p = nullptr;
	std::cout << *p << std::endl;
}

void testMMUExpHandler(int sig) {
	std::cout << "receive " << sig << std::endl;
}
