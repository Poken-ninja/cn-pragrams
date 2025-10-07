Week 6: TCP (Connection-Oriented) Chat Program
🔸 Server (TCP)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX 100

int main() {
    int sockfd, connfd;
    char msg[MAX];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { perror("Socket creation failed"); exit(1); }
    printf("Server socket created.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); // ✅ TCP only
    listen(sockfd, 5); // ✅ TCP only
    printf("Waiting for client...\n");

    len = sizeof(cliaddr);
    connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len); // ✅ TCP only
    printf("Client connected.\n");

    while (1) {
        bzero(msg, sizeof(msg));
        read(connfd, msg, sizeof(msg)); // ✅ TCP uses read/write
        printf("Client: %s", msg);

        if (strncmp(msg, "exit", 4) == 0) break;

        printf("Server: ");
        bzero(msg, sizeof(msg));
        fgets(msg, sizeof(msg), stdin);
        write(connfd, msg, sizeof(msg)); // ✅ TCP uses read/write
        if (strncmp(msg, "exit", 4) == 0) break;
    }

    close(connfd);
    close(sockfd);
    printf("Server exited.\n");
    return 0;
}
🔹 Client (TCP)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX 100

int main() {
    int sockfd;
    char msg[MAX];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { perror("Socket creation failed"); exit(1); }
    printf("Client socket created.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); // ✅ TCP only
    printf("Connected to server.\n");

    while (1) {
        printf("Client: ");
        bzero(msg, sizeof(msg));
        fgets(msg, sizeof(msg), stdin);
        write(sockfd, msg, sizeof(msg)); // ✅ TCP uses read/write

        if (strncmp(msg, "exit", 4) == 0) break;

        bzero(msg, sizeof(msg));
        read(sockfd, msg, sizeof(msg)); // ✅ TCP uses read/write
        printf("Server: %s", msg);

        if (strncmp(msg, "exit", 4) == 0) break;
    }

    close(sockfd);
    printf("Client exited.\n");
    return 0;
}

