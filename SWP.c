#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024       // Receiver's buffer size in bytes
#define DATA_PROCESS_RATE 50       // Bytes per second processed from the buffer
#define MAX_DATA_SEND_RATE 100     // Maximum bytes per second the sender can send
#define TOTAL_DATA_TO_SEND 500     // Total data to be sent by the sender

int calculate_advertised_window(int buffer_size, int bytes_in_buffer) {
    int advertised_window = buffer_size - bytes_in_buffer;
    return (advertised_window > 0) ? advertised_window : 0; // Ensure window size is non-negative
}

int main() {
    int buffer_size = MAX_BUFFER_SIZE;
    int bytes_in_buffer = 0;
    int total_bytes_sent = 0;

    while (total_bytes_sent < TOTAL_DATA_TO_SEND) {
        // Calculate the advertised window
        int advertised_window = calculate_advertised_window(buffer_size, bytes_in_buffer);
        printf("Advertised Window Size: %d bytes\n", advertised_window);

        // Sender sends data only up to the advertised window size
        int data_to_send = (advertised_window < MAX_DATA_SEND_RATE) ? advertised_window : MAX_DATA_SEND_RATE;
        if (total_bytes_sent + data_to_send > TOTAL_DATA_TO_SEND) {
            data_to_send = TOTAL_DATA_TO_SEND - total_bytes_sent; // Adjust to avoid sending more than needed
        }
        
        // Update buffer and total bytes sent
        bytes_in_buffer += data_to_send;
        total_bytes_sent += data_to_send;
        
        // Prevent overflow if buffer size is reached
        if (bytes_in_buffer > buffer_size) {
            bytes_in_buffer = buffer_size;
        }
        
        // Simulate receiver processing
        bytes_in_buffer -= DATA_PROCESS_RATE;
        if (bytes_in_buffer < 0) {
            bytes_in_buffer = 0;
        }

        // Display buffer status
        printf("Bytes in Buffer after sending and processing: %d bytes\n", bytes_in_buffer);
        printf("Total Data Sent: %d bytes\n", total_bytes_sent);

        // Wait for 1 second to simulate real-time
        sleep(1);
    }

    printf("Data transfer complete. Total bytes sent: %d\n", total_bytes_sent);
    return 0;
}
