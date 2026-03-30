#include <iostream>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

const int MAX = 64;

//关闭父进程的读端，让子进程继续写入，观察现象
void pipeCloseRead();

int main(void) {

	pipeCloseRead();

	return 0;
}

void pipeCloseRead() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	int id = fork();
	assert(id >= 0);

	if (id == 0) {
		close(fds[0]);
		char str[] = "Helo";
		char buf[MAX];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次", str, cnt);
			//std::cout << "子进程发出：" << buf << std::endl;
			write(fds[1], buf, strlen(buf));
			cnt++;
			sleep(1);
		}
	}
	close(fds[1]);

	char buf[MAX];
	while (1) {
		ssize_t s = read(fds[0], buf, sizeof buf - 1);
		if (s > 0) buf[s] = 0;
		else if (s == 0) {
			std::cout << "读取失败" << std::endl;
			break;
		}
		std::cout << "父进程收到：" << buf << std::endl;

		//只读一次就关闭读端
		close(fds[0]);
		//并退出循环
		break;
		sleep(1);
	}

	int status = 0;
	pid_t retProcess = waitpid(id, &status, 0);
	assert(retProcess > 0);
	//取低7位，表示信号值
	//这里会看到13号信号： 13) SIGPIPE
	std::cout << "child exit singal is " << (status & 0x7f) << std::endl;
	//看看coredump标记:
	std::cout << "core dump: " << ((status >> 7) &0x01) << std::endl;
	//看看高8位：
	std::cout << "high 8 bit: " << (status >> 8) << std::endl;
}
