/**
 * File Name: zenCheck.c
 * Description: 
 *
 */

#include "zenCheck.h"
#include "audioPlay.h"
#include "audioMixer.h"
#include "joystickInteract.h"
#include "i2c_drv.h"
#include "general.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/ioctl.h> 
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

/**** Global Variables ****/
static _Bool zenRunning;
static pthread_t zen_tid;
static joystick_t debounce;
static int debounceCounter = 0;

/*** FUNCTIONS ***/
static _Bool joystcikValidCheck(joystick_t toRead)
{
    if ( debounce != toRead ) {
        debounce = toRead;
        debounceCounter = 0;
        return readJoystickAndReturnResult(toRead);
    }
    return false;
} 

/* zenChecker
 * Keep reading the zen input, from joystick and accelerometer
 * If the reading is positive, do action appropriately:
 *  Joystick: Volume, BPM, Mode
 *  Accelerometer: Air drum
 */
static void *zenChecker(void *args)
{
 
    zenRunning = true;

    activateAllJoyStick();
    
    //i2c_device_t daAccelBus  = {I2C_HW_BUS_1, 0x1C, -1};
	//I2cDrv_init(&daAccelBus);
    // Accelerometer 

    while (zenRunning) {
        // Do accelerometer

        // Do joystick
        if ( joystcikValidCheck(UP) ){
            int volume = AudioMixer_getVolume();
            volume = volume + 5;
            AudioMixer_setVolume(volume);
        }
        if ( joystcikValidCheck(DOWN) ){
            int volume = AudioMixer_getVolume();
            volume = volume - 5;
            AudioMixer_setVolume(volume);
        }
        if ( joystcikValidCheck(RIGHT) ){
            int bpm = getBpm();
            bpm = bpm + 5;
            setBpm(bpm);
        }
        if ( joystcikValidCheck(LEFT) ){
            int bpm = getBpm();
            bpm = bpm - 5;
            setBpm(bpm);
        }
        if ( joystcikValidCheck(CENTER) ){
            beatMode_t mode = getMode();
            if ( mode == NONE ){
                mode = ROCK;
            }
            else if ( mode == ROCK ){
               mode = JANET;
            }
            else {
               mode = NONE;
            }
            changeMode(mode);

        }
        // Debounce
        if ( debounce != JOYSIZE ) {
            debounceCounter++;
        }
        if (debounceCounter > 10 ) {
            debounce = JOYSIZE;
            debounceCounter = 0;
        }
 
        sleep_usec(10*1000);
    }
    return NULL;

}

void zen_launchThread(void)
{
    int err = pthread_create(&zen_tid, NULL, &zenChecker, NULL);
    if (err != 0){
        printf("ERROR: Failed to create zen checker thread\n");
        exit(-1);
    }
    return;
}

void zen_joinThread(void)
{
    zenRunning = false;
    pthread_join(zen_tid, NULL);
    return;
}
