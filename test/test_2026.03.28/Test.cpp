#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <sys/wait.h>


int test_fd_min();
int test_create_pipe();
int test_pipe_fork();
int test_read_block();
int test_end_read_quick();
int test_close_pipe_write();
int test_close_pipe_read();


int main(void) {

    //test_fd_min();
	//test_create_pipe();
	//test_pipe_fork();
	//test_read_block();
	//test_end_read_quick();
	//test_close_pipe_write();
	test_close_pipe_read();

    return 0;
}

//结论：文件会分配给未被占用的，最小的fd值
int test_fd_min() {
	//0: stdin
	//1: stdout
	//2: stderr
    //close(0);
	//如果close(1), 则会把标准输出关闭, 这样就看不到打印出来的结果了.
    //close(1);
    // close(2);

    int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (fd < 0) {
		std::cout << "open failed!" << std::endl;
        perror("open");
        return 1;
    }

	std::cout << "open fd = " << fd << std::endl;

	return 0;
}


//如何创建一个一个一个管道
int test_create_pipe() {
	int fds[2];

    // int pipe(int pipefd[2]);
	// On  success, zero is returned.  On error, -1 is returned
	// The array pipefd is used to return two file descriptors referring to the ends of the pipe.
	// pipefd[0] refers to the  read  end of  the pipe.  pipefd[1] refers to the write end of the pipe. 
	int ret = pipe(fds);
	assert(ret == 0);

	//创建出一个管道, 并通过fds数组返回读和写端的文件描述符 
	std::cout << "fds[0]" << fds[0] << std::endl;
	std::cout << "fds[1]" << fds[1] << std::endl;

	return 0;

}



int test_pipe_fork() {
	//创建管道
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	int id = fork();
	assert(id >= 0);

	//子进程
	if (0 == id) {
		//子进程关闭读端, 保留写端
		close(fds[0]);
		//准备写入管道中的信息
		char str[] = "Fuck you world!";

		////strlen()函数, 只检测有效字符的长度
		//std::cout << strlen(str) << std::endl;
		////sizeof 运算符, 只检测大小, 会考虑包括'\0' 这种字符
		//std::cout << sizeof(str) / sizeof(str[0]) << std::endl;

		char buf[1024];
		int cnt = 1;

		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次", str, cnt);

			//Test
			//std::cout << "son buf: " << buf << std::endl;
			//向写端写入该字符串, 将该字符串完整地写入到管道中
			//strlen(str)只是str字符串的长度, 我之前在缓冲区里又多写了几个汉字, 所以向fds[1]中拷贝的时候读不全
			write(fds[1], buf, strlen(str));
			sleep(1);
			cnt++;
		}

	}

	//父进程
	//父进程保留读端, 关闭写端
	close(fds[1]);
	while (1) {
		char buf[1024];
		ssize_t s = read(fds[0], buf, sizeof (buf) - 1);
		buf[s] = 0;
		std::cout << "子进程发给父进程的消息: " << buf << std::endl;
		sleep(1);
	}

	int status = 0;
	int wait = waitpid(id, &status, 0);
	std::cout << "status: " << status << std::endl;

	return 0;

}

int test_read_block() {

	//创建管道
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	//创建子进程

	int id = fork();
	assert(id >= 0);

	//子进程
	if (id == 0) {
		//关闭读端
		close(fds[0]);
		char str[] = "Hello, World!";
		char buf[1024];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次, 子进程pid: %d", str, cnt, getpid());
			//strlen(buf), 只写入有效字段, 即1023个字符
			write(fds[1], buf, strlen(buf));
			sleep(10);
			cnt++;
		}

	}
	//父进程
	close(fds[1]);

	char buf[1024];

	while (1) {
		//将管道的内容读入buf中, 读入大小为缓冲区的大小
		//🎇这里保留疑问
		std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
		//read()系统调用为阻塞等待, 即条件不满足时将进程阻塞
		ssize_t s = read(fds[0], buf, sizeof buf - 1);
		std::cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB" << std::endl;
		//读取成功, 则在缓冲区末尾添加 '\0'
		if (s > 0) buf[s] = 0;
		std::cout << "父进程:" << getpid() << " , 收到子进程传来的消息: " << buf << std::endl;
	}

	int status = 0;
	waitpid(id, &status, 0);
	std::cout << "status = " << status << std::endl;

	return 0;
}

//管道中的数据被读取之后就会消失吗? 应该是的, 否则就不可能有这后面的输入了
int test_end_read_quick() {

	const int MAX = 64;
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	int id = fork();
	assert(!(id < 0));

	if (id == 0) {
		close(fds[0]);
		char str[] = "Hello";
		char buf[MAX];

		int cnt = 1;

		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次", str, cnt);
			write(fds[1], buf, strlen(buf));
			std::cout << "第" << cnt << "次写入" << std::endl;
			cnt++;
		}
	}

	close(fds[1]);

	sleep(5);

	char buf[MAX];

	while (1) {
		ssize_t s = read(fds[0], buf, sizeof buf - 1);
		if (s > 0) buf[s] = 0;
		std::cout << "父进程接收到: " << buf << std::endl;
		//sleep(1);
	}
}

int test_close_pipe_write(){
	const int MAX = 64;

	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	int id = fork();
	assert(id >= 0);

	if (id == 0) {
		close(fds[0]);
		char str[] = "hello";
		char buf[MAX];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次", str, cnt);
			write(fds[1], buf, strlen(buf));
			cnt++;

			sleep(1);
			//关闭子进程的写端
			//close() returns zero on success.  On error, -1 is returned,
			if (0 == close(fds[1])) {
				std::cout << "子进程已关闭写端" << std::endl;
			}
		}
	}

	close(fds[1]);

	char buf[MAX];
	while (1) {
		size_t s = read(fds[0], buf, sizeof(buf) - 1);
		if (s > 0) buf[s] = 0;
		//不加这条判断, 会一直跳过s > 0然后打印这一条数据
		else if (s == 0) {
			//子进程关闭了写端, 且缓冲区数据读完
			std::cout << "read完: " << s << std::endl;
			break;
		}
		std::cout << "子进程给父进程发信息: " << buf << std::endl;
		sleep(1);
	}

	return 0;

}

int test_close_pipe_read() {
	const int MAX = 64;
	int fds[2];
	int ret = pipe(fds);
	assert(ret == 0);

	int id = fork();
	assert(id >= 0);

	if (id == 0) {
		close(fds[0]);
		char str[] = "hello";
		char buf[MAX];

		int cnt = 1;
		while (1) {
			snprintf(buf, sizeof buf, "%s, 第%d次", str, cnt);
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
			std::cout << "准备退出" << std::endl;
			break;
		}

		std::cout << "子进程发来贺电: " << buf << std::endl;
		sleep(1);

		close(fds[0]);
	}

	int status = 0;
	int wait = waitpid(id, &status, 0);

	return 0;
}

