# Beatboxer

## Introduction
This is an application able to make drum beats' sound just like a beatboxer. This application consist of two parts, a server build on Node.js and the executable C program. Two piece of hardware are required to run this application, BeagleBone and Zen cape. Zen cape is a hardware that could be inserted to BeagleBone's GPIO directly like a cape. The hardware is custom made.

## Features
Here are the features that was build
- Loop beats sound from the Cape's headphone jack
- Receive input from JoyStick and change the tempo and the volume; up-increase volume, down-decrease-volume, right-increase tempo by 5 BPM, left-decrease by 5 BPM, center-cycle thru pre-determined beats
- Play additional beats based on accelerometer x,y,z input, each direction would play different sound
- User would be able to interact with the application thru the web interface, it would be able to support all the features mentioned above, except that it would be able to switch around the pre-determined beats without cycling thru everything

<p align="center">
<img src="https://raw.githubusercontent.com/jmardjuki/beatboxer/master/images/web_interface.png">
Figure 1: Web Interface
</p>

<p align="center">
<img src="https://raw.githubusercontent.com/jmardjuki/beatboxer/master/images/web_bg.png">
</p><p align="center">
Figure 2: Debug messages from the node server
</p>
## Building and Running
To build and run this application, mounting the beaglebone using NFS is very recommended. There are a lot of ways to deploy the application (such as using SCP), but this seems to be a easiest way. The following command will build all the application, and transfer all the required executable and sound files from the host.

```
$ make    #on top directory
```
The Makefile will
1. Cross compile the application **beatbox** for the BeagleBone
2. Send the node server files, beatbox executable, and wave files
3. Install the required Node Modules in the BeagleBone

To run the whole application, both the beatbox executable and the Node server have to be run on BeagleBone. SSH to the BeagleBone would be the easiest way to access the device. Interacting using UART is also an option, but that would require more hardware.

```
$ <path_to_beatbox>/beatbox & #run in background
$ cd <path_to_node>/beatbox-server-copy
$ node server.js
```
