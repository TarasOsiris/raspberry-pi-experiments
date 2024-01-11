#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <time.h>
#include <bits/time.h>

#define ledCounts 10
int pins[ledCounts] = {17, 18, 27, 22, 23, 24, 25, 2, 3, 8};

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
	signal_received = signal;
}

int main() {
	if (gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initialize the GPIO interface.\n");
		return 1;
	}
	printf("Press CTRL-C to exit.\n");
	signal(SIGINT, sigint_handler);

	for (int i = 0; i < ledCounts; i++) {
		gpioSetMode(i, PI_OUTPUT);
	}

	while (!signal_received) {
		for (int i = 0; i < ledCounts; i++) {
			gpioWrite(pins[i], PI_LOW);
			// printf("hi\n");
			time_sleep(0.1);
			gpioWrite(pins[i], PI_HIGH);
			// printf("lo\n");
		}

		for (int i = ledCounts; i >= 0; i--) {
			gpioWrite(pins[i], PI_LOW);
			// printf("hi\n");
			time_sleep(0.1);
			gpioWrite(pins[i], PI_HIGH);
			// printf("lo\n");
		}
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
