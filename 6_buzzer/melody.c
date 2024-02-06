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

// Define a structure for musical notes
typedef struct {
    int frequency; // Frequency of the note in Hz
    int duration;  // Duration of the note in milliseconds
} Note;

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
	    // Define a simple melody (C major scale upwards)
	    Note melody[] = {
		{262, 500}, // C4
		{294, 500}, // D4
		{330, 500}, // E4
		{349, 500}, // F4
		{392, 500}, // G4
		{440, 500}, // A4
		{494, 500}, // B4
		{523, 500}  // C5
	    };
	    int notes = sizeof(melody) / sizeof(Note);

	    
	}

	gpioTerminate();
	printf("\n");
	return 0;
}
