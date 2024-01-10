#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int ledPin = 17;
const int buttonPin = 18;
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

	gpioSetMode(ledPin, PI_OUTPUT);
	gpioSetMode(buttonPin, PI_INPUT);
	gpioSetPullUpDown(buttonPin, PI_PUD_UP);


	signal(SIGINT, sigint_handler);
	
	while (!signal_received) {
		if (gpioRead(buttonPin) == PI_LOW) {
			gpioWrite(ledPin, PI_HIGH);
			printf("Button is pressed, led turned on >>>\n");
		} else {
			gpioWrite(ledPin, PI_LOW);
			printf("Button is released, led turned off <<<\n");
		}
	}
	
	gpioSetMode(ledPin, PI_INPUT);
	gpioTerminate();
	printf("\n");
	return 0;
}
