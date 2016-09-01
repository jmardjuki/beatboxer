/**
 *  File Name: audioPlay.h
 *  Description: Header file audioPlay
 */
#ifndef AUDIO_PLAY_H
#define AUDIO_PLAY_H

/*** Declaration ***/
typedef enum {
    BASS = 0,
    SNARE,
    HIHAT,
    STYPESIZE
} soundType_t;

typedef enum {
    NONE = 0,
    ROCK,
    JANET,
    BMODESIZE
} beatMode_t;

/*** Functions ***/
// Load all sound into global static variable within this module
void initSounds(void);

// Clean all the sounds from the memory
void cleanUpSounds(void);

// Add the sound chosen to the queue
void addSoundToQueue(soundType_t);

// Get the current beat mode
beatMode_t getMode(void);

// Chnage the mode of the beats
void changeMode(beatMode_t);

// Get the value of current bpm
int getBpm(void);

// Set the Bpm to the value to the given parameter
void setBpm(int bpmNew);

#endif /* AUDIO_PLAY_H */

