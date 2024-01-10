#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <time.h>

const int ledPin = 17;
const int buttonPin = 18;
int ledState = PI_LOW;
int buttonState = PI_HIGH;
int lastButtonState = PI_HIGH;
long lastChangeTime;
long captureTime = 50;
int reading;

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

	gpioSetMode(ledPin, PI_OUTPUT);
	gpioSetMode(buttonPin, PI_INPUT);

	gpioSetPullUpDown(buttonPin, PI_PUD_UP);

	while (!signal_received) {
		reading = gpioRead(buttonPin);
		if (reading != lastButtonState) {
			lastChangeTime = millis();
		}

		if (millis() - lastChangeTime > captureTime) {
			if (reading != buttonState) {
				buttonState = reading;

				if (buttonState == PI_LOW) {
					printf("btn press\n");
					ledState = !ledState;
					if (ledState) {
						printf("led on\n");
					} else {
						printf("led off\n");
					}
				} else {
					printf("btn release\n");
				}
			}
		}

		gpioWrite(ledPin, ledState);
		lastButtonState = reading;
	}

	gpioSetMode(ledPin, PI_INPUT);
	gpioTerminate();
	printf("\n");
	return 0;
}
