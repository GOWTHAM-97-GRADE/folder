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
  int darr[m + k], d[n];

  // Convert string data to integer array
  for (int i = 0; i < m; i++) {
    darr[i] = data[i] - '0';
  }
  for (int i = 0; i < k; i++) {
    darr[m + i] = 0; // Append zeros for CRC
  }

  for (int i = 0; i < n; i++) {
    d[i] = div[i] - '0';
  }

  // Perform CRC
  crc(darr, d, m + k, n);

  // Prepare transmitted data (original + CRC)
  int tarr[m + k]; // transmitted array
  for (int i = 0; i < m; i++) {
    tarr[i] = data[i] - '0';
  }
  for (int i = 0; i < k; i++) {
    tarr[m + i] = darr[m + i];
  }

  // Setup client
  char *ip = "127.0.0.1";
  int port = 5555;

  int sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket error");
    exit(1);
  }
  printf("TCP client socket created successfully!\n");
  sleep(1);
  struct sockaddr_in addr;
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("connect error");
    exit(1);
  }
  printf("Connected to server\n");

  // Send data to server
  send(sock, tarr, (m + k) * sizeof(int), 0); // Use correct size

  close(sock);
  printf("Disconnected successfully\n");
  return 0;
}
