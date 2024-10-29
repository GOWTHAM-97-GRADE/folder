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
    char *dns_names[] = {"example.com", "google.com", "yahoo.com"};
    char *dns_ips[] = {"93.184.216.34", "142.250.190.14", "98.137.11.163"};
    struct sockaddr_in server_addr, client_addr;
    int n;
    socklen_t len;  // Correct type for len

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Fill server information
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(client_addr); // Length of client address

    // Receive domain name from client
    n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&client_addr, &len);
    buffer[n] = '\0';
    printf("Client requested domain: %s\n", buffer);

    // Search for the IP address corresponding to the domain
    char *ip_address = NULL;
    for (int i = 0; i < 3; i++) {
        if (strcmp(buffer, dns_names[i]) == 0) {
            ip_address = dns_ips[i];
            break;
        }
    }

    // If domain name not found, set response to "Domain not found"
    if (ip_address == NULL) {
        ip_address = "Domain not found";
    }

    // Send the IP address back to the client
    sendto(sockfd, ip_address, strlen(ip_address), 0, (const struct sockaddr *)&client_addr, len);
    printf("Sent IP address: %s\n", ip_address);

    // Close the socket
    close(sockfd);

    return 0;
}
