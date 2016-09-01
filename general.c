#include "general.h"
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void die_on_failed(char *msg)
{
	if (errno < 0) {
		perror(msg);
		exit(-1);
	}
}

void sleep_usec(long usec)
{
	struct timespec sleep_time;
	sleep_time.tv_sec = (usec / 1000000);
	sleep_time.tv_nsec = (usec % 1000000) * 1000;
	nanosleep(&sleep_time, NULL);
}
