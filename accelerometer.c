#include "accelerometer.h"
#include "i2c_drv.h"
#include "audioPlay.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define I2C_DEVICE_ADDRESS 0x1c

#define REG_STATUS 0x00
#define REG_OUT_X_MSB 0x01
#define REG_OUT_X_LSB 0x02
#define REG_OUT_Y_MSB 0x03
#define REG_OUT_Y_LSB 0x04
#define REG_OUT_Z_MSB 0x05
#define REG_OUT_Z_LSB 0x06
#define CTRL_REG1 0x2a

#define REG_XMSB 1
#define REG_XLSB 2
#define REG_YMSB 3
#define REG_YLSB 4
#define REG_ZMSB 5
#define REG_ZLSB 6

#define DEBOUNCE_TIMER_IN_NS 100000000 // = 100 ms
#define BYTES_TO_READ 7
#define G2 19.6
#define THRESHOLD 100
#define THRESHOLD_Z 270

static i2c_device_t device = {I2C_HW_BUS_1, I2C_DEVICE_ADDRESS, -1};
static _Bool stopping = false;
static pthread_t tid;
static unsigned char buff[BYTES_TO_READ];

//prototypes
static void*accelerThread(void* arg);
static void makeSleep(long seconds, long nanoseconds);
static float convertSampleToGForce(short sample);

void Accelerometer_init() {
	I2cDrv_init(&device);
	buff[0] = 0;
	// change mode to active to enable reading useful data
	I2cDrv_write_register(&device, CTRL_REG1, 0x01);

	printf("starting accelerometer...\n");
	pthread_create(&tid, NULL, accelerThread, NULL);
}

void Accelerometer_cleanup() {
	printf("Stopping accelerometer...\n");

	// Stop the PCM generation thread
	stopping = true;
	pthread_join(tid, NULL);

	// change mode back to standby
	I2cDrv_write_register(&device, CTRL_REG1, 0x00);
	I2cDrv_cleanup(&device);

	printf("Done stopping accelerometer...\n");
	fflush(stdout);
}

static void makeSleep(long seconds, long nanoseconds) {
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

//int16_t x = (buff[REG_XMSB] << 8) | (buff[REG_XLSB]);
static void* accelerThread(void* arg) {
	while(!stopping) {
		I2cDrv_read_registers(&device, REG_STATUS, buff, BYTES_TO_READ);

		int16_t x = (buff[REG_XMSB] << 8) | (buff[REG_XLSB]);
		int16_t y = (buff[REG_YMSB] << 8) | (buff[REG_YLSB]);
		int16_t z = (buff[REG_ZMSB] << 8) | (buff[REG_ZLSB]);

		float x_g = convertSampleToGForce(x);
		float y_g = convertSampleToGForce(y);
		float z_g = convertSampleToGForce(z);

		/*
		printf("\n");
		printf("x_16 is: %d.\n", x);
		printf("y_16 is: %d.\n", y);
		printf("z_16 is: %d.\n", z);

		printf("\n");
		printf("x_g is: %f.\n", x_g);
		printf("y_g is: %f.\n", y_g);
		printf("z_g is: %f.\n", z_g);
		`*/

		if (x_g > THRESHOLD) {
			// change this code
			printf("Make a base drum sound!\n");
            addSoundToQueue(BASS);
		}
		if (y_g > THRESHOLD) {
			// change this code
			printf("Make a snare drum sound!\n");
            addSoundToQueue(SNARE);
		}
		if (z_g > THRESHOLD_Z) {
			// change this code
			printf("Make a hihat sound!\n");
            addSoundToQueue(HIHAT);
		}

		makeSleep(0, DEBOUNCE_TIMER_IN_NS);
	}
	return NULL;
}

static float convertSampleToGForce(short sample) {
	return (sample/pow(2.0, 11) * G2);
}
