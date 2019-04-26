
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


/* get_hostname_inaddr queries and gets a in_addr struct to a hostname.

   It receives a pointer to a char containing a hostname.

   If you are using a bind socket, use INADDR_ANY instead.
*/
struct in_addr * get_hostname_inaddr (char *hostname) {
  struct hostent *hostinfo; 
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s\n", hostname);
    exit(EXIT_FAILURE);
  }
  return (struct in_addr *) hostinfo->h_addr;
}

/* init_sockaddr will init a sockaddr_in pointer.

   It receives a pointer to a sockaddr_in, a pointer to a in_add
   and a uint16_t containing the TCP port.
   
   Example mostly taken from
   https://www.gnu.org/software/libc/manual/html_node/Inet-Example.html
*/
void init_sockaddr (
  struct sockaddr_in *name,
  struct in_addr *inaddr,
  uint16_t port
) {
  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  name->sin_addr = *inaddr;
}


/* make_conn_socket creates a socket and connects it to a remote peer.
   
   It receives a pointer to a char containing the hostname and a uint16_t
   containing the TCP port. 

   It gets the sockaddr_in from init_sockaddr and creates the socket.  

   Example mostly taken from 
   https://en.wikibooks.org/wiki/C_Programming/Networking_in_UNIX
*/
int make_conn_socket(char *hostname, uint16_t port) {
  struct sockaddr_in target_sockaddr;
  init_sockaddr(&target_sockaddr, get_hostname_inaddr(hostname), port);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (connect(sock,
      (struct sockaddr *)&target_sockaddr,
      sizeof(target_sockaddr)) < 0) {
    printf("Error connecting to target addr\n");
    return -1;
  }
  return sock;
}

/* make_serv_socket creates a socket and binds it to all interfaces.
   
   It receives a uint16_t containing the port.

   It accepts connections and returns the client socket.
*/
int make_serv_socket(uint16_t port) {
  struct sockaddr_in target_sockaddr;
  init_sockaddr(&target_sockaddr, INADDR_ANY, port);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (bind(sock,
      (struct sockaddr *)&target_sockaddr,
      sizeof(target_sockaddr)) < 0) {
    printf("Error binding socket. Is the port %d in use?\n", port);
    return -1;
  }
  if (listen(sock, 1) < 0) {
    printf("Error listening on socket fd %d\n", sock);
  }
  printf("Server is listening on port %d\n", port);
  return accept(sock, (struct sockaddr *)NULL, NULL);
}
