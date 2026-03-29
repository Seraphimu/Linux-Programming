#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>


void createPipe();
void forkPipe();

int main(void) {
	//createPipe();
	forkPipe();

	return 0;
}


void createPipe() {
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	std::cout << "fds[0]" << fds[0] << std::endl;
	std::cout << "fds[1]" << fds[1] << std::endl;
}

void forkPipe() {
	const int MAX = 64;
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	int id = fork();
	assert(!(id < 0));
	if (id == 0) {
		//子进程
		//关闭读端，保留写入端
		close(fds[0]);
		char str[] = "Hello";
		char * buf = (char *)malloc(MAX * sizeof(char));

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次", str, cnt);
			write(fds[1], buf, strlen(buf));
			std::cout << "写入，第" << cnt << "次。" << std::endl;
			cnt++;
		}
	}

	//父进程
	//关闭写端，保留读端
	close(fds[1]);
	char * buf = (char *)malloc(sizeof (char) * MAX);

	sleep(2);
	while (1) {
		ssize_t s = read(fds[0], buf, sizeof buf - 1);
		if (s > 0) buf[s] = 0;
		std::cout << "子进程发的消息：" << buf << std::endl;
	}
}
