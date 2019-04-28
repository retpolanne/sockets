#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "sock.c"

#define WEBROOT "./webroot"

int handle_http(int sock) {
  char *BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\n\r\n";
  char *OK = "HTTP/1.1 200 OK\r\n\r\n";
  char *NOT_FOUND = "HTTP/1.1 404 Not found\r\n\r\n"; 
  int buflen = 255;
  char request[buflen], *ptr;
  read(sock, request, buflen);
  printf("Request is: %s\n", request);
  // strstr will trim the HTTP/something
  // GET /foobar HTTP/1.0
  ptr = strstr(request, " HTTP/");
  if (ptr == NULL) {
    write(sock, BAD_REQUEST, strlen(BAD_REQUEST));
    return -1;
  } else {
    // At this point, ptr is pointing to before the " HTTP/1.0". 
    // Terminating the string here will trim this part
    *ptr = 0;
    ptr = NULL;
    // This will get the file URI from the request
    if (strstr(request, "GET"))
      // GET /
      ptr = request + 4;
    if (ptr == NULL) {
      write(sock, BAD_REQUEST, strlen(BAD_REQUEST));
      return -1;
    }
    char content_uri[buflen]; 
    strcpy(content_uri, WEBROOT);
    strcat(content_uri, ptr);
    if (ptr[strlen(ptr) - 1] == '/')
      strcat(content_uri, "index.html");
    printf("Content uri is: %s\n", content_uri);
    int fd = open(content_uri, O_RDONLY, 0);
    if (fd == -1) {
      write(sock, NOT_FOUND, strlen(NOT_FOUND));
      return -1;
    }
    char file_content[buflen];
    read(fd, file_content, buflen);
    write(sock, OK, strlen(OK));
    write(sock, file_content, strlen(file_content));
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int buflen = 255;
  const int port = atoi(argv[1]);
  
  // Step 1: create a socket.
  int sock = make_serv_socket(port);
  if (sock < 0) {
    printf("Couldnt create socket\n");
    return 1;
  }

  // Step 2: accept connections by creating an accept socket
  int accept_sock;
  //int fd = open("index.html", O_RDONLY, 0);
  while(1) {
    accept_sock = make_server_socket_loop(sock);
    if (handle_http(accept_sock) < 0) {
      printf("Error handling http\n");
      close(accept_sock);
    }
    close(accept_sock);
  }

  // Step 4: close the socket and return zero.
  close(sock);
  return 0;
}

