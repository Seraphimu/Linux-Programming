#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

const int MAX = 1024;

void showString(char * str);
void testPipe();
void testPipeCreate();
void testPipeCommunication();
//管道常空，读端阻塞
void testReadGtWrite();
//管道写满，写端阻塞
void testWriteGtRead();
//读到EOF
//On success, the number of bytes read is returned (zero indicates end of file)
void testCloseWrite();
//关闭读端，被进程被13号信号杀死
void testCloseRead();
//创建
void testCreateNamedPipe();

int main(void) {
	//testPipeCreate();	
	//testPipe();
	//testPipeCommunication();
	//testReadGtWrite();
	//testWriteGtRead();
//	testCloseWrite();
	//testCloseRead();
	testCreateNamedPipe();

	return 0;
}

void testPipeCreate() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	std::cout << "fds[0] = " << fds[0] << std::endl;
	std::cout << "fds[1] = " << fds[1] << std::endl;

}

void testPipe() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	char buf[64] = "hello";
	showString(buf);
	write(fds[1], buf, strlen(buf));
	memset(buf, 0, sizeof(buf));
	
	showString(buf);
	ssize_t r = read(fds[0], buf, sizeof(buf) - 1);
	//std::cout << "buf: " << buf << std::endl;
}


//Test memset() function
void showString(char * str) {

	std::cout << "Content of String: " << std::endl;
	for (int i = 0; i < sizeof(str); i++) {
		std::cout << str[i];
	}
	std::cout << std::endl;
}


void testPipeCommunication() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	int id = fork();

	if (id == 0) {
		close(fds[0]);
		char buf[1024];
		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof (buf), "第%d次发送", cnt);
			write(fds[1], buf, strlen(buf));
			std::cout << "发送提示: " << cnt << "次" << std::endl;
			cnt++;
		}
	}
	close(fds[1]);
	char buf[1024];

	while (1) {
		ssize_t r = read(fds[0], buf, sizeof (buf) - 1);	

		std::cout << "buf: " << buf << std::endl;
		sleep(1);
	}
}

//写端快，读端慢，read阻塞式等待
void testReadGtWrite() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	pid_t id = fork();

	if (id == 0) {
		close(fds[0]);
		char buf[64];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "第%d次发消息", cnt);
			write(fds[1], buf, strlen(buf));
			sleep(5);
			cnt++;
		}
	}
	close(fds[1]);
	char buf[64];
	while (1) {
		ssize_t r = read(fds[0], buf, sizeof buf - 1);
		//防止随机字符
		if (r > 0) buf[r] = 0;
		std::cout << buf << std::endl;
		sleep(1);
	}

}

//
void testWriteGtRead() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	pid_t id = fork();

	if (id == 0) {
		close(fds[0]);
		char buf[MAX];

		int cnt = 1;
		while (1) {
			//sleep(1);
			snprintf(buf, sizeof buf, "第%d次发消息", cnt);
			write(fds[1], buf, strlen(buf));
			cnt++;
		}
	}
	close(fds[1]);
	char buf[MAX];
	while (1) {
		ssize_t r = read(fds[0], buf, sizeof buf - 1);
		//防止随机字符
		if (r > 0) buf[r] = 0;
		std::cout << buf << std::endl;
		sleep(5);
	}
}

void testCloseWrite() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	pid_t id = fork();

	if (id == 0) {
		close(fds[0]);
		char buf[MAX];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "第%d次发消息", cnt);
			write(fds[1], buf, strlen(buf));
			if (cnt == 5) close(fds[1]);
			sleep(1);
			cnt++;
		}
	}
	close(fds[1]);
	char buf[MAX];
	while (1) {
		ssize_t r = read(fds[0], buf, sizeof buf - 1);
		//防止随机字符
		if (r > 0) buf[r] = 0;
		else if (r == 0) std::cout << "读取大小为0" << std::endl;
		std::cout << buf << std::endl;
		sleep(1);
	}

	waitpid(id, nullptr, 0);
}

void testCloseRead() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);
	(void)ret;

	pid_t id = fork();

	if (id == 0) {
		close(fds[0]);
		char buf[MAX];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "第%d次发消息", cnt);
			write(fds[1], buf, strlen(buf));
			sleep(1);
			cnt++;
		}
	}
	close(fds[1]);
	char buf[MAX];
	int cnt = 1;
	while (1) {
		if (cnt > 5) {
			close(fds[0]);
			break;
		}
		ssize_t r = read(fds[0], buf, sizeof buf - 1);
		//防止随机字符
		if (r > 0) buf[r] = 0;
		else if (r == 0) std::cout << "读取大小为0" << std::endl;
		std::cout << buf << std::endl;
		sleep(1);
		cnt++;
	}
	
	int status = 0;
	pid_t w = waitpid(id, &status, 0);

	if (w > 0) {
		std::cout << "杀死进程的信号: " << (status & 0x7f) << std::endl;
	}

}

void testCreateNamedPipe() {
	umask(0);
	int ret = mkfifo("FifoByFunc", 0666);
	assert(ret == 0);
	(void)ret;

}
