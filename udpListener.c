// udpListener.c
#include "audioMixer.h"
#include "audioPlay.h"
#include "udpListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <stdbool.h>

#define UDP_PORT 12345
#define MAX_RECEIVE_MESSAGE_LENGTH 1024
#define REPLY_BUFFER_SIZE (10*1024)

#define COMMAND_BEAT   "beat"
#define COMMAND_GET    "get"
#define COMMAND_TEMPO  "tempo"
#define COMMAND_VOLUME "volume"
#define COMMAND_DRUM   "drum"

static pthread_t s_threadId;
static char replyBuffer[REPLY_BUFFER_SIZE];
static _Bool stopping = false;

// Header
static void *udpListeningThread(void *args);
static void processCommand(char* command);
static int secondWordToInt(char *string);

void UdpListener_startListening(void)
{
    pthread_create(&s_threadId, NULL, &udpListeningThread, NULL);
}

void UdpListener_cleanup(void)
{
    stopping = true;
    pthread_join(s_threadId, NULL);
}

static void *udpListeningThread(void *args)
{
    // Buffer to hold packet data:
    char message[MAX_RECEIVE_MESSAGE_LENGTH];

    // Address
    struct sockaddr_in sin;
    unsigned int sin_len;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);       // Host to Network long
    sin.sin_port = htons(UDP_PORT);                // Host to Network short

    // Create the socket for UDP
    int socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // Bind the socket to the port that we specify
    bind(socket_descriptor, (struct sockaddr*) &sin, sizeof(sin));

    while (!stopping) {
        // Get the data (blocking)
        // Will change sin (the address) to be the address of the client.
        sin_len = sizeof(sin);
        int bytesRx = recvfrom(socket_descriptor, message, MAX_RECEIVE_MESSAGE_LENGTH, 0,
                (struct sockaddr *) &sin, &sin_len);

        // Make it null terminated (so string functions work):
        message[bytesRx] = 0;
        //printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, message);

        processCommand(message);

        // Transmit a reply:
        sin_len = sizeof(sin);
        sendto( socket_descriptor,
                replyBuffer, strnlen(replyBuffer, REPLY_BUFFER_SIZE), 0,
                (struct sockaddr *) &sin, sin_len);
    }

    // Close (signals to main that we are ending)
    close(socket_descriptor);
    return NULL;
}

static void processCommand(char* command)
{
    replyBuffer[0] = 0;

    if (strncmp(command, COMMAND_BEAT, strlen(COMMAND_BEAT)) == 0) {
        int numToReturn = secondWordToInt(command);
        if ( numToReturn == 1 ) {
            changeMode(NONE);
        }
        else if ( numToReturn == 2 ) {
            changeMode(ROCK);
        }
        else if (numToReturn == 3) {
            changeMode(JANET);
        }
        else {
            //Error msg
        }
    }
    else if (strncmp(command, COMMAND_VOLUME, strlen(COMMAND_VOLUME)) == 0) {
        int numToReturn = secondWordToInt(command);
        int currentVolume = AudioMixer_getVolume();
        if ( numToReturn == 1) {
            currentVolume++;
            AudioMixer_setVolume(currentVolume);
        }
        else {
            currentVolume--;
            AudioMixer_setVolume(currentVolume);
        }
    }
    else if (strncmp(command, COMMAND_TEMPO, strlen(COMMAND_TEMPO)) == 0) {
        int numToReturn = secondWordToInt(command);
        int currentTempo = getBpm();
        if ( numToReturn == 1) {
            currentTempo++;
            setBpm(currentTempo);
        }
        else {
            currentTempo--;
            setBpm(currentTempo);
        }
    }
    else if (strncmp(command, COMMAND_DRUM, strlen(COMMAND_DRUM)) == 0) {
        int numToReturn = secondWordToInt(command);
        if ( numToReturn == 1) {
            addSoundToQueue(HIHAT);
        }
        else if ( numToReturn == 2 ) {
            addSoundToQueue(SNARE);
        }
        else if (numToReturn == 3) {
            addSoundToQueue(BASS);
        }
        else {
            //Error msg
        }
    }
    else if (strncmp(command, COMMAND_GET, strlen(COMMAND_GET)) == 0) {
        int numToReturn = secondWordToInt(command);
        if ( numToReturn == 1) {
            beatMode_t daBeat = getMode();
            if ( daBeat == NONE) {
                sprintf(replyBuffer, "beat NONE");
            }
            else if ( daBeat == ROCK){
                sprintf(replyBuffer, "beat ROCK");
            }
            else if (daBeat == JANET) {
                sprintf(replyBuffer, "beat JANET");
            }
        }
        else if ( numToReturn == 2 ) {
            int tempoIs = getBpm();
            sprintf(replyBuffer, "tempo %d", tempoIs);
        }
        else if ( numToReturn == 3 ) {
            int volumeIs = AudioMixer_getVolume();
            sprintf(replyBuffer, "volume %d", volumeIs);
        }
    }
}

static int secondWordToInt(char *string)
{
    // Default to a 1 if nothing.
    int value = 1;
    sscanf(string, "%*s%d", &value);

    return value;
}
