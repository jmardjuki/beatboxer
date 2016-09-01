// Routines for reading/writing to a file by name (simulate cat and echo)

#ifndef _FILEIO_DRV_H_
#define _FILEIO_DRV_H_

// Return number bytes written.
int FileIODrv_echo_to_file(char* file, char* buff);

// Returns number of bytes read into buff (null-terminated string)
int FileIODrv_read_str(char* file_name, char* buff, int max_length);


#endif
