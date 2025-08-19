// TCP Server: converts client message to uppercase and sends back
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORTNO 10203

int main() {
    int sockfd, newsockfd, clilen, n;
    struct sockaddr_in seraddr, cliaddr;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // bind to IP/port
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.57.181"); // replace with your IP
    seraddr.sin_port = htons(PORTNO);
    bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    // listen for clients
    listen(sockfd, 5);

    while (1) {
        char buf[256];
        printf("server waiting...\n");
        fflush(stdout);

        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen);

        // iterative loop with this client
        while (1) {
            memset(buf, 0, sizeof(buf));
            n = read(newsockfd, buf, sizeof(buf) - 1);
            if (n <= 0) break; // connection closed

            buf[n] = '\0';
            printf("Message from Client: %s\n", buf);

            // check for termination
            if (strcmp(buf, "Stop") == 0) {
                printf("Stopping server.\n");
                close(newsockfd);
                close(sockfd);
                exit(0);
            }

            // convert to uppercase
            for (int i = 0; buf[i]; i++) {
                buf[i] = toupper((unsigned char)buf[i]);
            }

            // send back to client
            write(newsockfd, buf, strlen(buf));
        }

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
