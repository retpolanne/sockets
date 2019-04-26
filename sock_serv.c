#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sock.c"

int main(int argc, char *argv[]) {
  int buflen = 255;
  char *hostname = argv[1];
  const int port = atoi(argv[2]);
  
  // Step 1: create a socket.
  int sock = make_serv_socket(port);
  if (sock < 0) {
    printf("Couldnt create socket\n");
    return 1;
  }

  // Step 2: read the stdin for commands and write to the socket.
  char content[buflen];
  fgets(content, buflen, stdin);
  if (write(sock, content, strlen(content)) < 0) {
    return 1;
  }

  // Step 3: listen to the socket return.
  char buffer[buflen];
  while(read(sock, buffer, buflen) != 0) {
    printf("%s", buffer);
  }

  // Step 4: close the socket and return zero.
  close(sock);
  return 0;
}
