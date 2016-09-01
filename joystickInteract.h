/**
 * File Name: joystickInteract.h
 * Description: Header file for joystick interaction
 */
#ifndef JOYSTICK_INTERACT_H
#define JOYSTICK_INTERACT_H

/*** GLOBAL VARIABLES ***/
typedef enum{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    CENTER,
    JOYSIZE,
} joystick_t;


/*** FUNCTIONS ***/
/*
 * activateAllJoystick
 * Export all joystic on zencape and change the direction to in
 */
void activateAllJoyStick();

/* 
 * readJoystickAndReturnResult
 * Read string from joystick file and return true if string contains 
 * 0 (positive)
 * params:
 *  joystick_t: enum to define which joystick to read
 * return:
 *  _Bool: variable to tell that the file read is positive
 */
_Bool readJoystickAndReturnResult(joystick_t);

#endif /* JOYSTICK_INTERACT_H */
