#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO "./FifoByFunc"

const int MAX = 1024;

int main(void)
{
	umask(0);
	//创建命名管道
	int ret = mkfifo(FIFO, 0666);
	assert(ret == 0);
	(void)ret;

	//打开管道文件
	int fd = open(FIFO, O_RDONLY);
	assert(fd >= 0);

	//现象是：打开Server的时候，什么输出都没有，只有client端连接过来的时候才会看到提示，这个时候它被什么玩意阻塞了吗？如果是被read()阻塞了的话，应该先把Wait Client打印出来才对。

	//那么，就只有可能是open()了。


	char buf[MAX];
	while (1)
	{
		std::cout << "Wait Client..." << std::endl;
		ssize_t r = read(fd, buf, sizeof buf - 1);
		if (r > 0)
		{
			buf[r] = 0;
			std::cout << "Cliend says: " << buf << std::endl;
		}
		else if (r == 0)
		{
			std::cout << "Server Exit" << std::endl;
			exit(0);
		}
		else
		{
			perror("read");
			exit(-1);
		}
	}

	return 0;
}
