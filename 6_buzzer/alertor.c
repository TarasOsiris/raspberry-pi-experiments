#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>
#include <bits/time.h>
#include <unistd.h>
#include <math.h>

#define buzzerPin 17
#define buttonPin 18

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
	signal_received = signal;
}

void alertor(int pin) {
    int x;
    double sinVal, toneVal;

	 gpioPWM(buzzerPin, 128);
	
     for (x = 0; x < 720; x++) {
	     if (signal_received) {
		     break;
	     }
         sinVal = sin(x * (M_PI / 180));
         toneVal = 2000 + sinVal * 500; 
	 printf("tone: %f.\n", toneVal);
         gpioSetPWMfrequency(buzzerPin, toneVal);
         time_sleep(0.01);
     }
}

void stopAlertor(int pin) {
	gpioPWM(buzzerPin, 0);
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
	gpioSetPullUpDown(buttonPin, PI_PUD_UP);

	while (!signal_received) {
		if (gpioRead(buttonPin) == PI_LOW) {
//			printf("on.\n");
			alertor(buzzerPin);
		} else {
//			printf("off.\n");
			stopAlertor(buzzerPin);
		}
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
