
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


/* init_sockaddr will init a sockaddr_in pointer. It queries the IP from the
   hostname.

   It receives a pointer to a sockaddr_in, a pointer to a char containing the
   hostname and a uint16_t containing the TCP port.
   
   Example taken from
   https://www.gnu.org/software/libc/manual/html_node/Inet-Example.html
*/
void init_sockaddr (
  struct sockaddr_in *name,
  const char *hostname,
  uint16_t port
) {
  struct hostent *hostinfo;
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s\n", hostname);
    exit(EXIT_FAILURE);
  }

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}

/* make_socket creates a socket.
   
   It receives a pointer to a char containing the hostname and a uint16_t
   containing the TCP port. 

   It gets the sockaddr_in from init_sockaddr and creates the socket.  

   Example mostly taken from 
   https://en.wikibooks.org/wiki/C_Programming/Networking_in_UNIX
*/
int make_socket(char *hostname, uint16_t port) {
  // Getting the sockaddr_in for a hostname
  struct sockaddr_in target_addr;
  init_sockaddr(&target_addr, hostname, port);

  // Creating a socket and connecting to the sockaddr
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (connect(sock, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0) {
    printf("Error connecting to target addr\n");
    return -1;
  }
  return sock;
}
