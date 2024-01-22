#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <time.h>
#include <bits/time.h>

#define pin 18

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

	gpioSetPWMrange(pin, 100);
	int result = gpioPWM(pin, 0);

	while (!signal_received) {
		for (int i = 0; i <= 100; i++) {
			gpioPWM(pin, i);
			gpioSleep(PI_TIME_RELATIVE, 0, 10000);
		}
		for (int i = 100; i >= 0; i--) {
			gpioPWM(pin, i);
			gpioSleep(PI_TIME_RELATIVE, 0, 10000);
		}
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
