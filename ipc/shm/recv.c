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
    perror("shmget");
    exit(0);
  }

  char *addr = (char *)shmat(shmid, 0, 0);

  char rbuf[1024];
  char receveTime[1024];

  while (1) {
    if (addr[0] == 'a') {
      getTime(receveTime);
      addr[0] = '1';
      printf("%s\n", addr);
      printf("%s\n\n", receveTime);
    }
  }

  return 0;
}
