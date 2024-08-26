#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/socketDemo"
#define BUFFER_SIZE 128

int main() {
    // unlink
    unlink(SOCKET_NAME);

    // init variables
    int result;
    int data;
    char buffer[BUFFER_SIZE];
    int socket_;  // because socket or data socket is just file deciptor
    int data_socket;
    int ret;
    struct sockaddr_un name;

    // init socket
    socket_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == socket_) {
        perror("socket init fail");
        exit(EXIT_FAILURE);
    }
    printf("1. Socket initilization success\n");

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path));

    ret = bind(socket_, (const struct sockaddr*)&name, sizeof(name));
    if (-1 == ret) {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }
    printf("2. Bind socket success\n");

    ret = listen(socket_, 20);  // buffer 20 request
    if (-1 == ret) {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }
    printf("3. Open Listen on request success\n");

    while (1) {
        data_socket = accept(socket_, NULL, NULL);

        if (-1 == data_socket) {
            perror("data socket fail");
            exit(EXIT_FAILURE);
        }
        printf(">> Accept -- create data socket\n");

        result = 0;
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);

            ret = read(data_socket, buffer, BUFFER_SIZE);
            if (-1 == ret) {
                perror("read data fail");
                exit(EXIT_FAILURE);
            }

            memcpy(&data, buffer, sizeof(data));
            printf(">> Receive data: %d\n", data);

            if (0 == data) {
                printf("Client request: Close data socket\n");
                break;
            }
            result += data;
        }

        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "Result (sum) = %d", result);
        ret = write(data_socket, buffer, BUFFER_SIZE);
        printf("Return result back to client: %d\n", result);

        close(data_socket);
        printf("Close current data socket\n");
    }
    close(socket_);
    exit(EXIT_SUCCESS);
}