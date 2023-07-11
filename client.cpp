#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


static void die(const char *msg){
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

int main(){
    int file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (file_descriptor < 0){
        die("socket()");
    }
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

    int rv = connect(file_descriptor, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        fprintf(stderr, "Error in code \n");
        die("connect");
    }
    char msg[] = "hello";
    write(file_descriptor, msg, strlen(msg));
    char rbuf[64] = {};
    ssize_t n = read(file_descriptor, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        die("read");
    }
    printf("server says: %s\n", rbuf);
    close(file_descriptor);
    return 0;
}