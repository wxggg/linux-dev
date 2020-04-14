#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define KEY 1

#define SEM "/sem"

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

  sem_t *sem = sem_open(SEM, 0);
  if (sem == SEM_FAILED) {
    perror("sem_open");
  }

  char sendTime[1024];

  while (1) {
    getTime(sendTime);
    printf("%s\n\n", sendTime);
    sem_post(sem);
    sleep(1);
  }

  return 0;
}
