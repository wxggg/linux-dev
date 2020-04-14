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
  unlink("/tmp/fifo");
  mkfifo("/tmp/fifo", 0666);

  int fd = open("/tmp/fifo", O_WRONLY);

  char wbuf[1024];

  while (1) {
    getTime(wbuf);
    write(fd, wbuf, 1024);
    sleep(1);
  }

  close(fd);

  return 0;
}
