# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET= beatbox

SOURCES= \
	main.c \
	audioMixer.c \
	audioPlay.c \
	zenCheck.c \
	joystickInteract.c \
	udpListener.c \
	fileio_drv.c \
	i2c_drv.c \
	accelerometer.c \
	general.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabi-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


# Asound process:
# get armel lib on target:
# 	# apt-get install libasound2:armel
# Copy target's /usr/lib/arm-linux-gnueabi/libasound.so.2.0.0 
#      to host  ~/public/asound_lib_BBB/libasound.so
# Copy to just base library:

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB
# -pg for supporting gprof profiling.
#CFLAGS += -pg



all: wav node
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lpthread -lasound

# Copy wave files to the shared folder
wav:
	mkdir -p $(PUBDIR)/beatbox-wave-files/
	cp -R beatbox-wave-files/* $(PUBDIR)/beatbox-wave-files/ 
node:
	mkdir -p $(PUBDIR)/beatbox-server-copy
	cp -R beatbox-server-copy/* $(PUBDIR)/beatbox-server-copy/

clean:
	rm -f $(OUTDIR)/$(TARGET)
	rm -rf $(OUTDIR)/beatbox-wave-files/
	rm -rf $(OUTDIR)/beatbox-server-copy/
