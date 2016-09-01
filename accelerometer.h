// Handles the accelerometer
#ifndef ACCEL_H
#define ACCEL_H

// init() must be called before any other functions,
// cleanup() must be called last to stop playback threads and free memory.
void Accelerometer_init(void);
void Accelerometer_cleanup(void);

#endif
