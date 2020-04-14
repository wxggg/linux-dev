#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
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

int connect_server(const char *domain_name) {
  int connect_fd;
  struct sockaddr_un srv_addr;
  int ret;

  // create client socket
  connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (connect_fd < 0) {
    perror("client create socket failed");
    return -1;
  }

  // set server sockaddr_un
  srv_addr.sun_family = AF_UNIX;
  strcpy(srv_addr.sun_path, domain_name);

  // connect to server
  ret = connect(connect_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
  if (ret == -1) {
    char buf[128];
    sprintf(buf, "pid %d, %s", getpid(), "connect to server failed!");
    // printf("pid %d\n", getpid());
    perror(buf);
    close(connect_fd);
    unlink(domain_name);
    return -1;
  }

  return connect_fd;
}

int main(int argc, char const *argv[]) {

  int fd = connect_server("/tmp/domain.socket");

  char wbuf[4096];

  while (1) {
    getTime(wbuf);
    write(fd, wbuf, 4096);
    sleep(1);
  }

  close(fd);

  return 0;
}
