#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <time.h>

#define ledCounts 10
int pins[ledCounts] = {};

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
	signal_received = signal;
}

unsigned int millis () {
  struct timespec t ;
  clock_gettime ( CLOCK_MONOTONIC_RAW , & t ) ; // change CLOCK_MONOTONIC_RAW to CLOCK_MONOTONIC on non linux computers
  return t.tv_sec * 1000 + ( t.tv_nsec + 500000 ) / 1000000 ;
}

int main() {
	if (gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initialize the GPIO interface.\n");
		return 1;
	}
	printf("Press CTRL-C to exit.\n");
	signal(SIGINT, sigint_handler);

	while (!signal_received) {
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
