/**
 *  File Name: audioPlay.c
 *  Description: Audio helper module to initialize sound, store mode, bpm
 *               value, and add sound to soundbites queue
 */
#include "audioPlay.h"
#include "audioMixer.h"
#include <stdio.h>
#include <stdlib.h>

/*** Global variables definition***/
/* AUDIO FILES SOURCE */
#define BASS_FILE "beatbox-wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT_FILE "beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_FILE "beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"

#define MIN_BPM 40
#define MAX_BPM 300

/* WAVE DATAS */
static wavedata_t bassSound;
static wavedata_t snareSound;
static wavedata_t hiHatSound;

static beatMode_t currentBeat = ROCK;
static int bpmValue = 120;

/*** Functions ***/
void initSounds(void)
{
    AudioMixer_readWaveFileIntoMemory(BASS_FILE, &bassSound);
    AudioMixer_readWaveFileIntoMemory(SNARE_FILE, &snareSound);
    AudioMixer_readWaveFileIntoMemory(HIHAT_FILE, &hiHatSound);
    return;
}

void cleanUpSounds(void)
{
    AudioMixer_freeWaveFileData(&bassSound);
    AudioMixer_freeWaveFileData(&snareSound);
    AudioMixer_freeWaveFileData(&hiHatSound);
    return;
}

void addSoundToQueue(soundType_t soundAdd)
{
    if ( soundAdd > STYPESIZE ) {
        printf("ERROR: Sound to be added is invalid\n");
        return;
    }
    else if ( soundAdd == BASS ) {
        AudioMixer_queueSound(&bassSound);
    }
    else if ( soundAdd == SNARE ) {
        AudioMixer_queueSound(&snareSound);
    }
    else {
        AudioMixer_queueSound(&hiHatSound);
    }
    return;
}

beatMode_t getMode(void)
{
    return currentBeat;
}

void changeMode(beatMode_t beatNew)
{
    if ( beatNew > BMODESIZE ) {
        printf("ERROR: Beat selected is invalid\n");
        return;
    }
    currentBeat = beatNew;
    return;
}

int getBpm()
{
    return bpmValue;
}

void setBpm(int bpmNew)
{
    if (bpmNew > MAX_BPM || bpmNew < MIN_BPM) {
        printf("ERROR: BPM selected is too low or too high\n");
        return;
    }
    bpmValue = bpmNew;
}


