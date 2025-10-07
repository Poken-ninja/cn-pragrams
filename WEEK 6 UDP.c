⚡ UDP — Connectionless
🖥️ Server (UDP)
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
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // ⚠️ UDP uses SOCK_DGRAM
    if (sockfd < 0) { perror("Socket creation failed"); exit(1); }
    printf("UDP Server socket created.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); // ✅ Still needed for UDP
    printf("Waiting for message from client...\n");

    while (1) {
        bzero(msg, sizeof(msg));
        recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&cliaddr, &len); // ⚠️ UDP uses recvfrom()
        printf("Client: %s", msg);

        if (strncmp(msg, "exit", 4) == 0) break;

        printf("Server: ");
        bzero(msg, sizeof(msg));
        fgets(msg, sizeof(msg), stdin);
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&cliaddr, len); // ⚠️ UDP uses sendto()
        if (strncmp(msg, "exit", 4) == 0) break;
    }

    close(sockfd);
    printf("UDP Server exited.\n");
    return 0;
}

💻 Client (UDP)
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
    socklen_t len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // ⚠️ UDP uses SOCK_DGRAM
    if (sockfd < 0) { perror("Socket creation failed"); exit(1); }
    printf("UDP Client socket created.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    while (1) {
        printf("Client: ");
        bzero(msg, sizeof(msg));
        fgets(msg, sizeof(msg), stdin);
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, len); // ⚠️ UDP uses sendto()

        if (strncmp(msg, "exit", 4) == 0) break;

        bzero(msg, sizeof(msg));
        recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, &len); // ⚠️ UDP uses recvfrom()
        printf("Server: %s", msg);

        if (strncmp(msg, "exit", 4) == 0) break;
    }

    close(sockfd);
    printf("UDP Client exited.\n");
    return 0;
}

