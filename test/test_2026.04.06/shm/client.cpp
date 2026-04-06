#include "common.h"

int main(void) {

	key_t key = ftok(PATHNAME, PROJ_ID);
	assert(key != -1);

	int shmId = shmget(key, SIZE, 0);
	char * str = (char *)shmat(shmId, nullptr, 0);

	char start = 'a';
	while (start <= 'z') {
		str[start-'a'] = start;
		++start;
		sleep(3);
	}

	if (shmdt(str) == -1) {
		perror("shmdt");
		exit(-1);
	}

	return 0;
}
