#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void crc(int *data, int *d, int datalen, int dlen) {
  for (int i = 0; i < datalen - dlen; i++) {
    if (data[i] == 1) {
      for (int j = 0; j < dlen; j++) {
        data[i + j] = data[i + j] ^ d[j];
      }
    }
  }
}

int main() {
  char data[100] = "10111001";
  char div[100] = "100000111";

  int m = strlen(data);
  int n = strlen(div);
  int k = n - 1;
  int d[n];

  for (int i = 0; i < n; i++) {
    d[i] = div[i] - '0';
  }

  char *ip = "127.0.0.1";
  int port = 5555;

  int sock, client_sock;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket error");
    exit(1);
  }
  printf("TCP server socket created successfully!\n");

  struct sockaddr_in addr, client_addr;
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  int ny = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if (ny < 0) {
    perror("Bind error");
    exit(1);
  }

  listen(sock, 5);
  printf("Listening for incoming connections...\n");

  sleep(1); // Optional: Short pause for testing purposes

  socklen_t addr_size = sizeof(client_addr);

  client_sock = accept(sock, (struct sockaddr *)&client_addr, &addr_size);
  if (client_sock < 0) {
    perror("Accept error");
    exit(1);
  }

  int tarr[m + k];
  int recvb = recv(client_sock, tarr, (m + k) * sizeof(int), 0);
  if (recvb < 0) {
    perror("Receive error!");
    exit(1);
  }

  crc(tarr, d, m + k, n);

  int r = 0;
  for (int i = 0; i < k; i++) {
    if (tarr[m + i] == 1) {
      printf("\nRejected\n");
      r = 1;
      break;
    }
  }

  if (!r) {
    printf("\nAccepted\n");
  }

  close(client_sock);
  close(sock);
  printf("Server disconnected successfully\n");

  return 0;
}
