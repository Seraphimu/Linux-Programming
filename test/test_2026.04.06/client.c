#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO "./fifo"

int main()
{
  // 以写的方式打开管道文件
  int fd = open(FIFO, O_WRONLY);
  if (fd < 0){
    perror("open pipefile");
    exit(-1);
  }

  char buf[64];
  while (1){
    printf("Please Enter Message# ");
    fflush(stdout);

    // 使用read读取用户输入的数据
    ssize_t s = read(0, buf, sizeof(buf)/sizeof(buf[0])-1);
    if (s > 0){
      buf[s] = 0;
      write(fd, buf, s+1);
    }
    else{
      perror("read");
      exit(-1);
    }
  }
  return 0;
}

