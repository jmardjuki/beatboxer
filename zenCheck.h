/**
 * File Name: zenCheck.h
 * Description: Header for zenCheck
 */
#ifndef ZEN_CHECK_H
#define ZEN_CHECK_H

/**
 * Start a zen thread that keeps track of the accelerometer and the joystick
 */
void zen_launchThread(void);


/**
 * Join the zen thread
 */
void zen_joinThread(void);


#endif
