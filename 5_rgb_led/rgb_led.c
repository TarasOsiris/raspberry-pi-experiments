#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>
#include <bits/time.h>

#define ledPinRed 17
#define ledPinGreen 18
#define ledPinBlue 27

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
	signal_received = signal;
}

void setupLedPin(void) {
	gpioSetPWMrange(ledPinRed, 100);
	gpioSetPWMrange(ledPinGreen, 100);
	gpioSetPWMrange(ledPinBlue, 100);

	gpioPWM(ledPinRed, 0);
	gpioPWM(ledPinGreen, 0);
	gpioPWM(ledPinBlue, 0);
}

void setLedColor(int r, int g, int b) {
	gpioPWM(ledPinRed, r);
	gpioPWM(ledPinGreen, g);
	gpioPWM(ledPinBlue, b);
}

int main() {
	if (gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initialize the GPIO interface.\n");
		return 1;
	}
	printf("Press CTRL-C to exit.\n");
	signal(SIGINT, sigint_handler);

	int r,g,b;

	setupLedPin();

	while (!signal_received) {
		r = random() % 100;
		g = random() % 100;
		b = random() % 100;
		setLedColor(r,g,b);
		printf("r=%d, r=%d, b=%d \n", r, g, b);
		gpioSleep(PI_TIME_RELATIVE, 1, 0);
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
