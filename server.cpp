#include<sys/socket.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/ip.h>

static void message_function(const char *message) {
    fprintf(stderr, "%s\n", message);
}


static void die(const char *message){
    int error_number = errno;
    fprintf(stderr, "[%d] %s \n", error_number, message);
    abort();
}


static void listen_to_client(int client_fd){
    char rbuf[64] = {};
    ssize_t n = read(client_fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        message_function("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "message from ak";
    write(client_fd, wbuf, sizeof(wbuf)-1);
}


int main(){
    // initialize socket 
    int file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (file_descriptor<0) {
        die("socket()");
    }
    int val = 1;
    setsockopt(file_descriptor, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);

    int rv = bind(file_descriptor, (const sockaddr *) &addr, sizeof(addr));
    if (rv){
        die("bind()");  }

    rv = listen(file_descriptor, SOMAXCONN);
    while(true){
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int client_fd = accept(file_descriptor, (struct sockaddr *)&client_addr, &socklen);
        if (client_fd < 0) {
            continue;
        }
        listen_to_client(client_fd);
        close(client_fd);

    }
    return 0;
}