// UDP Client Program: Sends rows of a matrix and receives full matrix back
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

int main() {
    int sd;
    struct sockaddr_in address;
    char buf[256];
    int rows, cols;

    // Create UDP socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("socket");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.57.181"); // Replace with your IP
    address.sin_port = htons(PORT);

    socklen_t len = sizeof(address);

    // Input matrix size
    printf("Enter number of rows and columns: ");
    scanf("%d %d", &rows, &cols);

    int matrix[rows][cols];

    printf("Enter matrix elements row by row:\n");
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    // Send rows and cols first
    sprintf(buf, "%d %d", rows, cols);
    sendto(sd, buf, sizeof(buf), 0, (struct sockaddr *)&address, len);

    // Send matrix rows
    for(int i = 0; i < rows; i++) {
        memset(buf, 0, sizeof(buf));
        for(int j = 0; j < cols; j++) {
            char temp[10];
            sprintf(temp, "%d ", matrix[i][j]);
            strcat(buf, temp);
        }
        sendto(sd, buf, sizeof(buf), 0, (struct sockaddr *)&address, len);
    }

    printf("Client: Sent matrix, waiting for server response...\n");

    // Receive reconstructed matrix
    printf("Client: Received full matrix from server:\n");
    for(int i = 0; i < rows; i++) {
        recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&address, &len);
        printf("%s\n", buf);
    }

    close(sd);
    return 0;
}
