#include "general.h"
#include "fileio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int FileIODrv_echo_to_file(char* file, char* buff)
{
	// Open direction file
	int file_desc = open(file, O_WRONLY);
	if (file_desc < 0) {
		printf("FILEIODRV ERROR: Unable to open file for write: %s\n", file);
		perror("Error is:");
		exit(-1);
	}

	int bytes_written = write(file_desc, buff, strlen(buff));
	die_on_failed("Unable to write to file for echo.");

	// Close
	close(file_desc);

	return bytes_written;
}

// Returns number of bytes read into buff (null-terminated string)
int FileIODrv_read_str(char* file_name, char* buff, int max_length)
{
	int file_desc = open(file_name, 0);
	if (file_desc < 0) {
		printf("FILEIODRV ERROR: Unable to open file for read: %s\n", file_name);
		exit(-1);
	}

	// Read max-1 bytes so room for a null.
	int bytes_read = read(file_desc, buff, max_length-1);

	// Close
	close(file_desc);

	// Null terminate "string" being returned
	buff[bytes_read] = 0;

	return bytes_read;
}
