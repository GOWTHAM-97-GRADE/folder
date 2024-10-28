#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_SIZE 4   // Define the window size
#define TOTAL_FRAMES 10 // Total frames to be sent

void send_frames(int frames[], int frame_count, int *next_frame, int *ack);

int main() {
  int frames[TOTAL_FRAMES];
  int next_frame = 0; // Next frame to send
  int ack = 0;        // Last acknowledgment received

  // Initialize frames with frame numbers
  for (int i = 0; i < TOTAL_FRAMES; i++) {
    frames[i] = i;
  }

  srand(time(0)); // Seed random number generator

  // Start sending frames
  while (ack < TOTAL_FRAMES) {
    send_frames(frames, TOTAL_FRAMES, &next_frame, &ack);
  }

  printf("All frames sent and acknowledged successfully.\n");
  return 0;
}

void send_frames(int frames[], int frame_count, int *next_frame, int *ack) {
  int end_frame = *next_frame + WINDOW_SIZE;

  // Transmit frames within the window
  for (int i = *next_frame; i < end_frame && i < frame_count; i++) {
    printf("Sending frame %d\n", frames[i]);
  }

  // Simulate receiving acknowledgments
  for (int i = *next_frame; i < end_frame && i < frame_count; i++) {
    int lost = rand() % 2; // Randomly decide if ack is lost (50% chance)

    if (!lost) {
      printf("Acknowledgment received for frame %d\n", frames[i]);
      (*ack)++;
      (*next_frame)++;
    } else {
      printf("Acknowledgment lost for frame %d. Retransmitting from frame %d\n",
             frames[i], frames[*next_frame]);
      break;
    }
  }
}