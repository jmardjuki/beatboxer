// udpListener.h
// Listen to a UDP port and accept commands.
//
// Commands:
//   beat 1 (none)
//   beat 2 (rock)
//   beat 3 (janet)
//
//   volume 30
//   bpm 60
//   play 1 (hihat)
//   play 2 (snare)
//   play 3 (bass)
//
// Usage:
//   netcat -u 192.168.0.121 12345
//   Then just type commands and press enter.

#ifndef _UDP_LISTENER_H_
#define _UDP_LISTENER_H_

// Start listening to the UDP port on a separate thread.
void UdpListener_startListening(void);

void UdpListener_cleanup(void);

#endif
