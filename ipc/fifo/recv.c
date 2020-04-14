#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

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

  int fd = open("/tmp/fifo", O_RDONLY);

  printf("fd=%d\n", fd);

  char rbuf[1024];
  char receveTime[1024];

  int ret;

  while (1) {
    ret = read(fd, rbuf, 1024);
    getTime(receveTime);
    printf("%s\n", rbuf);
    printf("%s\n\n", receveTime);
  }

  printf("ret=%d\n", ret);
  close(fd);

  return 0;
}
