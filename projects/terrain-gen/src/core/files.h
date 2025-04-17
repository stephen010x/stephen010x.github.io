#ifndef CORE_FILE_H
#define CORE_FILE_H

#include <stdbool.h>


bool file_exists(char* path);
// returns number of bytes loaded
// success if return is greater than 0
int load_file(char* path, void** buffer);
void unload_file(void* buffer);
// returns error code
int write_file(char* path, void* data, int bytes);
void delete_file(char* path);


#endif
