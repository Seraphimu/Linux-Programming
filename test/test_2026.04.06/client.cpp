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
	int fd = open(FIFO, O_WRONLY);
	// If expression is false assert() prints an error message to stan‐
	// dard error and terminates the program
	assert(fd >= 0);

	char buf[MAX];

	while (1)
	{
		std::cout << "Please Enter Message: ";
		fflush(stdout);
		// 从标准输入读入数据
		ssize_t r = read(0, buf, sizeof buf - 1);
		if (r > 0)
		{
			buf[r] = 0;
			write(fd, buf, r + 1);
		}
		else
		{
			perror("read");
			exit(-1);
		}
	}

	return 0;
}
