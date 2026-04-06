#include "common.h"

int main(void) {
	
	key_t key = ftok(PATHNAME, PROJ_ID);
	assert(key != -1);

	printf("key:%p\n", key);

	 // 创建内存空间
  // IPC_CREAT 要创建的共享内存如果存在，就打开返回，不存在就创建
  // IPC_EXCL  如果底层共享内存已经存在就出错返回
  // 结合使用可以创建一个全新的共享内存

	int shmId = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0664);
	assert(shmId >= 0);

	std::cout << "Shm Id: " << shmId << std::endl;

	char * str = (char *)shmat(shmId, nullptr, 0);
	while (1) {
		std::cout << "Client Say: " << str;
		sleep(1);
	}

  // 取消关联
  	if (shmdt(str) == -1){
  	  perror("shmdt");
  	  exit(-1);
  	}
  	
  	// 删除共享内存段
  	shmctl(shmId, IPC_RMID, NULL);

	return 0;
}
