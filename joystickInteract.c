#include "joystickInteract.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*** GLOBAL VARIABLES ***/
#define GPIO_PATH "/sys/class/gpio/"

/*** FUNCTIONS ***/
static void exportAndIn(int gpioNum)
{
    // Export the gpio pins
    FILE *gpioExP = fopen(GPIO_PATH "export", "w");
    if ( gpioExP == NULL ){
        printf("ERROR: Unable to open export file.\n");
        exit(-1);
    }
    fprintf(gpioExP, "%d", gpioNum);
    fclose(gpioExP);

    // Change the direction into in
    char fileNameBuffer[1024];
    sprintf(fileNameBuffer, GPIO_PATH "gpio%d/direction", gpioNum);

    FILE *gpioJoyDirP = fopen(fileNameBuffer, "w");
    fprintf(gpioJoyDirP, "in");
    fclose(gpioJoyDirP);

    return;
}


void activateAllJoyStick()
{
    exportAndIn(26); // UP
    exportAndIn(46); // DOWN
    exportAndIn(65); // LEFT
    exportAndIn(47); // RIGHT
    exportAndIn(27); // CENTER

    return;
}


static _Bool readAndReturnResult(char * fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }

    // Read string (line)
    const int max_length = 1024;
    char buff[max_length];
    fgets(buff, max_length, file);
    
    // Close the file
    fclose(file);

    // Return result
    if ( strstr(buff, "0") != NULL ){
        return true;
    }
    return false;
}

_Bool readJoystickAndReturnResult(joystick_t joystickSelect)
{
    // Variables declaration and initialization
    _Bool joystickIsTrue  = false;

    if ( joystickSelect >= JOYSIZE ){
        printf("ERROR: Joystick selected is not valid\n");
        return false;
    }
    else if ( joystickSelect == UP ) {
        joystickIsTrue = readAndReturnResult(GPIO_PATH "/gpio26/value");
    }
    else if ( joystickSelect == DOWN ) {
        joystickIsTrue = readAndReturnResult(GPIO_PATH "/gpio46/value");
    }
    else if ( joystickSelect == LEFT ) {
        joystickIsTrue = readAndReturnResult(GPIO_PATH "/gpio65/value");
    }
    else if ( joystickSelect == RIGHT ) {
        joystickIsTrue = readAndReturnResult(GPIO_PATH "/gpio47/value");
    }
    else { // joystick == CENTER
        joystickIsTrue = readAndReturnResult(GPIO_PATH "/gpio27/value");
    }
    return joystickIsTrue;
}
