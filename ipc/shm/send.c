#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define KEY 1

void getTime(char *buf) {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  char *p = buf;
  const char *sec = "number:sec:";
  memcpy(p, sec, strlen(sec));
  p += strlen(sec);

  sprintf(p, "%d", tv.tv_sec);

  p = buf + strlen(buf);

  const char *usec = "|number:usec:";
  memcpy(p, usec, strlen(usec));
  p += strlen(usec);

  sprintf(p, "%d", tv.tv_usec);
}

int main(int argc, char const *argv[]) {
  int shmid = shmget(KEY, 0, 0666);
  if (shmid < 0) {
    shmid = shmget(KEY, 4096, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0) {
      perror("shmget");
      exit(0);
    }
  }

  char *addr = (char *)shmat(shmid, 0, 0);
  if (addr == (void *)-1) {
    perror("shmat");
    exit(0);
  }

  while (1) {
    getTime(addr);
    addr[0] = 'a';
    sleep(1);
  }

  return 0;
}
