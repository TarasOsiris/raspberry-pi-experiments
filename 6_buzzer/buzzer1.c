#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>
#include <bits/time.h>

#define buzzerPin 17
#define buttonPin 18

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

	gpioSetMode(buzzerPin, PI_OUTPUT);
	gpioSetMode(buttonPin, PI_INPUT);

	while (!signal_received) {
		if (gpioRead(buttonPin) == PI_LOW) {
			gpioWrite(buzzerPin, PI_HIGH);
			printf("on.\n");
		} else {
			gpioWrite(buzzerPin, PI_LOW);
			printf("off.\n");
		}
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
