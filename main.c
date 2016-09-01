#include "audioPlay.h"
#include "audioMixer.h"
#include "zenCheck.h"
#include "udpListener.h"
#include "accelerometer.h"
#include "general.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

/*** GLOBAL VARIABLES ***/
_Bool keepBeating = false;

/*** FUNCTIONS ***/
void waitHalfABeat()
{
    double currentBpm = (double)getBpm();
    int timeHalfABeat_ms = (int)(((double)60/currentBpm/(double)2) *(double)1000);
    sleep_usec(timeHalfABeat_ms*1000);

    return;
}

void handle_SIGINT()
{
    keepBeating = false;
    return;
}

int main(void)
{
	printf("Start\n");

    keepBeating = true;

    // Initialise the signals and handler
    struct sigaction handler;
    handler.sa_handler = handle_SIGINT;
    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);
    sigaction(SIGINT, &handler, NULL);

    // Initialize
    AudioMixer_init();
    initSounds();

    // Launch thread
    zen_launchThread();
    UdpListener_startListening();
    Accelerometer_init();  

    int halfBeatCount = 0;

    while (keepBeating) { 
        beatMode_t daMode = getMode();
        if( daMode == ROCK ) {
            if ( halfBeatCount % 1 == 0){
                addSoundToQueue(HIHAT);
            }
            if ( halfBeatCount % 4 == 0 ){
                addSoundToQueue(SNARE);
            }
            if ( halfBeatCount % 2 == 0 ){
                addSoundToQueue(BASS);
            }
        }
        else if ( daMode == JANET ) {
            if ( halfBeatCount % 1 == 0 ){
                addSoundToQueue(BASS);
            }
            if ( halfBeatCount % 2 == 0){
                addSoundToQueue(SNARE);
            }
            if ( halfBeatCount % 6 == 0){
                addSoundToQueue(HIHAT);
            }
           
        }
        else {  // None
            // Do nothing
        }
        halfBeatCount++;
        waitHalfABeat();

    }
    
    // Join all thread
    zen_joinThread();
    Accelerometer_cleanup();
    //UdpListener_cleanup();
    cleanUpSounds();

	printf("Done!\n");
	return 0;
}


