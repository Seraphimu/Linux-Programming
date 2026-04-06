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
  umask(0);
  // 创建管道
  int ret = mkfifo(FIFO, 0666);
  if (ret == -1){
    perror("make fifo");
    exit(-1);
  }
  
  // 以读的方式打开管道
  int fd = open(FIFO, O_RDONLY);
  if (fd < 0){
    perror("open fail");
    exit(-1);
  }
 
  char buf[64];
  while (1){
    printf("wait client...\n");
    ssize_t s = read(fd, buf, sizeof(buf)/sizeof(buf[0])-1);
    if (s > 0){
      // 正常读取
      buf[s] = '\0';
      printf("client say# %s", buf);
    }
    else if (s == 0){
      // 客服端写端关闭，服务器读端读到文件末尾
      printf("server exit...\n");
      exit(0);
    }
    else{
      // 读错误
      perror("read");
      exit(-1);
    }
  }

  return 0;
}

