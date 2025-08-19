// UDP Server Program: Receives rows of a matrix and sends back full matrix
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 9704
#define MAX 100

int main() {
    int sd;
    struct sockaddr_in sadd, cadd;
    char buf[256];
    int matrix[MAX][MAX];
    int rows, cols;

    // Create a UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("socket");
        exit(1);
    }

    // Construct the server address
    sadd.sin_family = AF_INET;
    sadd.sin_addr.s_addr = inet_addr("172.16.57.181"); // Replace with your IP
    sadd.sin_port = htons(PORT);

    if(bind(sd, (struct sockaddr *)&sadd, sizeof(sadd)) < 0) {
        perror("bind");
        exit(1);
    }

    socklen_t len = sizeof(cadd);

    // First receive rows and cols
    recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&cadd, &len);
    sscanf(buf, "%d %d", &rows, &cols);
    printf("Server: expecting a %dx%d matrix\n", rows, cols);

    // Receive matrix rows
    for(int i = 0; i < rows; i++) {
        recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&cadd, &len);
        sscanf(buf, "%d %d %d", &matrix[i][0], &matrix[i][1], &matrix[i][2]); // adjust if cols > 3
    }

    printf("Server received full matrix:\n");
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Send back full matrix to client
    for(int i = 0; i < rows; i++) {
        memset(buf, 0, sizeof(buf));
        for(int j = 0; j < cols; j++) {
            char temp[10];
            sprintf(temp, "%d ", matrix[i][j]);
            strcat(buf, temp);
        }
        sendto(sd, buf, sizeof(buf), 0, (struct sockaddr *)&cadd, len);
    }

    close(sd);
    return 0;
}
