#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>  // For close()

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char domain_name[MAXLINE];
    struct sockaddr_in server_addr;
    socklen_t len;  // Change len to socklen_t type

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Fill server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Get domain name input from user
    printf("Enter the domain name: ");
    scanf("%s", domain_name);

    // Send domain name to the server
    sendto(sockfd, domain_name, strlen(domain_name), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Domain name sent to server.\n");

    // Receive IP address from the server
    int n;
    len = sizeof(server_addr);  // Use socklen_t for len
    n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&server_addr, &len);
    buffer[n] = '\0';

    // Display the IP address received from the server
    printf("IP Address: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}
