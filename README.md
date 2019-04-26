# Sockets

Socket experiments made for Computer Networks classes at Fatec.

## Usage

Compiling:

`gcc -o sock_client sock_client.c`

Using:

`echo "GET /~fdc/sample.html" | ./sock_client www.columbia.edu 80 > index.html`
