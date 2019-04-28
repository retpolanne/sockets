#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/* send_to_sock will send bytes to a socket.
   Example taken from Hacking: the art of exploitation, by Jon Erickson, 2nd ed, page 209.
*/
int send_to_sock(int sock, char *buffer) {
  int bytes_to_send = strlen(buffer), sent_bytes;
  while(bytes_to_send > 0) {
    sent_bytes = send(sock, buffer, bytes_to_send, 0);
    if (bytes_to_send == -1)
      return 1;
    bytes_to_send -= sent_bytes;
    // This will set the buffer pointer to start its offset + sent_bytes
    buffer += sent_bytes;
  }
  return 0;
}

int rec_from_sock(int sock, char *buffer) {
  char *ptr = buffer;
  
  // Read socket byte per byte
  while(recv(sock, ptr, 1, 0) == 1) {
    if (*ptr ==)  
  }
}
int accept_loop(int sock, void *backend);
