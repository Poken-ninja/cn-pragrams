🧠 Multi-Server (TCP + UDP) — Simplified Version
🖥️ Server (TCP + UDP)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#define PORT 5000
#define MAX 1024

int main() {
    int tcpfd, udpfd, connfd, nready;
    char buffer[MAX];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    fd_set rfds;
    int maxfd;

    // ✅ Create TCP socket
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);

    // ✅ Create UDP socket
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // ✅ Bind TCP & UDP sockets
    bind(tcpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    listen(tcpfd, 10); // ✅ TCP only

    maxfd = (tcpfd > udpfd ? tcpfd : udpfd) + 1;

    while(1) {
        FD_ZERO(&rfds);
        FD_SET(tcpfd, &rfds);
        FD_SET(udpfd, &rfds);

        nready = select(maxfd, &rfds, NULL, NULL, NULL);

        // ✅ TCP client ready
        if(FD_ISSET(tcpfd, &rfds)) {
            len = sizeof(cliaddr);
            connfd = accept(tcpfd, (struct sockaddr*)&cliaddr, &len);
            bzero(buffer, MAX);
            read(connfd, buffer, sizeof(buffer));
            printf("TCP client: %s\n", buffer);
            write(connfd, "Hello TCP Client\n", 17);
            close(connfd);
        }

        // ✅ UDP client ready
        if(FD_ISSET(udpfd, &rfds)) {
            len = sizeof(cliaddr);
            bzero(buffer, MAX);
            int n = recvfrom(udpfd, buffer, MAX, 0, (struct sockaddr*)&cliaddr, &len);
            buffer[n] = '\0';
            printf("UDP client: %s\n", buffer);
            sendto(udpfd, "Hello UDP Client\n", 17, 0, (struct sockaddr*)&cliaddr, len);
        }
    }

    close(tcpfd);
    close(udpfd);
    return 0;
}

💻 TCP Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 5000
#define MAX 1024

int main() {
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    strcpy(buffer, "Hello TCP Server");
    write(sockfd, buffer, strlen(buffer));

    bzero(buffer, MAX);
    read(sockfd, buffer, sizeof(buffer));
    printf("Server: %s", buffer);

    close(sockfd);
    return 0;
}

💻 UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 5000
#define MAX 1024

int main() {
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    strcpy(buffer, "Hello UDP Server");
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servaddr, len);

    bzero(buffer, MAX);
    int n = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr*)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server: %s", buffer);

    close(sockfd);
    return 0;
}
