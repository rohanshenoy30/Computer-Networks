// TCP Client: sends strings until "Stop", prints uppercase reply
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int len, result, sockfd, n;
    struct sockaddr_in address;
    char ch[256], buf[256];

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.57.181"); // replace with your IP
    address.sin_port = htons(10203);
    len = sizeof(address);

    // connect to server
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("\nCLIENT ERROR");
        exit(1);
    }

    while (1) {
        printf("\nENTER STRING: ");
        fflush(stdout);

        // read input
        if (fgets(ch, sizeof(ch), stdin) == NULL) break;
        ch[strcspn(ch, "\n")] = '\0';  // remove newline

        // send to server
        write(sockfd, ch, strlen(ch));

        // if Stop → break loop and quit
        if (strcmp(ch, "Stop") == 0) {
            printf("Client exiting.\n");
            break;
        }

        // read reply from server
        memset(buf, 0, sizeof(buf));
        n = read(sockfd, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("STRING SENT BACK FROM SERVER IS: %s\n", buf);
        }
    }

    close(sockfd);
    return 0;
}
