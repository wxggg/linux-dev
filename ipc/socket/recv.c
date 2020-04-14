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

int init_listen_server(const char *domain_name) {
  int lsn_fd;
  struct sockaddr_un srv_addr;
  int ret;

  // create socket to bind local IP and PORT
  lsn_fd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (lsn_fd < 0) {
    perror("can't create communication socket!");
    return -1;
  }

  // create local IP and PORT
  srv_addr.sun_family = AF_UNIX;
  strncpy(srv_addr.sun_path, domain_name, sizeof(srv_addr.sun_path) - 1);
  unlink(domain_name);

  // bind sockfd and sockaddr
  ret = bind(lsn_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
  if (ret == -1) {
    perror("can't bind local sockaddr!");
    close(lsn_fd);
    unlink(domain_name);
    return -1;
  }

  // listen lsn_fd, try listen 1
  ret = listen(lsn_fd, 1);
  if (ret == -1) {
    perror("can't listen client connect request");
    close(lsn_fd);
    unlink(domain_name);
    return -1;
  }
  return lsn_fd;
}

int accept_client(int lsn_fd) {
  int apt_fd;
  struct sockaddr_un clt_addr;
  socklen_t clt_len;

  clt_len = sizeof(clt_addr);

  apt_fd = accept(lsn_fd, (struct sockaddr *)&clt_addr, &clt_len);
  if (apt_fd < 0) {
    return -1;
  }

  return apt_fd;
}

int main(int argc, char const *argv[]) {

  int listen_fd = init_listen_server("/tmp/domain.socket");
  int fd = accept_client(listen_fd);

  printf("fd=%d\n", fd);

  char rbuf[4096];
  char receveTime[1024];

  int ret;

  while (1) {
    ret = read(fd, rbuf, 4096);
    getTime(receveTime);
    printf("%s\n", rbuf);
    printf("%s\n\n", receveTime);
  }

  printf("ret=%d\n", ret);
  close(fd);

  return 0;
}
