#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

const int BUFFER_SIZE=64;

//文件被谁操作? 进程! 进程如何操作文件? 首先要把文件载入内存, 
//即打开文件, 获得一个文件描述符fd, 并基于此对文件进行操作
//如何打开一个文件?
//这里需要补充一个内核数据结构


//fd是什么? fd的本质

int Test() {
	//手动设置umask值, 在Shell中执行该程序时, 不会调整Shell的umask值, 该进程会继承父进程的0002(默认), 但被手动修改为0
	umask(0);
	//int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	int fd = open("log.txt", 
			O_RDWR | 		//可读可写
			O_CREAT | 		//若无则新建
			O_APPEND | 		//追加
			O_TRUNC, 		//如果文件允许写入(O_WRONLY, O_RDWR), 则会被截断为0
			0664
	);

	if (fd < 0) {
		printf("open error\n");
		return -1;
	}

	printf("open fd = %d\n", fd);

	char buffer[BUFFER_SIZE] = "1145141919810kasdfjk";

	//write(文件描述符, 写入源, 写多长);
	write(fd, buffer, 10);

	close(fd);

	return 0;
}

int main(void) {
	Test();
	
	return 0;
}
