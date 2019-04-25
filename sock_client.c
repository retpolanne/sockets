#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "sock.c"

int main(int argc, char *argv[]) {
  // Getting the sockaddr_in for a hostname
  char *hostname = argv[1];
  const int port = atoi(argv[2]);
  int sock = make_socket(hostname, port);
  if (sock < 0) {
    printf("Couldnt create sock\n");
    return 1;
  }

  int buflen = 255;
  char content[buflen];
  fgets(content, buflen, stdin);
  printf("EOF reached");
  if (write(sock, content, strlen(content)) < 0) {
    return 1;
  }
  char buffer[buflen];
  while(read(sock, buffer, buflen) != 0) {
    printf("%s", buffer);
  }
  close(sock);
  return 0;
}
