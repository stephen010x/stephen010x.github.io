#include <stdbool.h>
#include <emscripten.h>

#include "files.h"
#include "../main.h"




#define DB_NAME "wasm-c-frame-filesystem"




const char* db_name = DB_NAME;


bool file_exists(char* path) {
    int error;
    int exists;
    emscripten_idb_exists(db_name, path, &exists, &error);
    ASSERT(!error, false, "ERROR checking if file \"%s\" exists\n", path);
    return !!exists;
}




int load_file(char* path, void** buffer) {
    int error;
    int bytes;
    emscripten_idb_load(db_name, path, buffer, &bytes, &error);
    ASSERT(!error, -1, "ERROR loading file \"%s\"\n", path);
    return bytes;
}




void unload_file(void* buffer) {
    free(buffer);
}




int write_file(char* path, void* data, int bytes) {
    //if (file_exists(path))
    //    delete_file(path);
    int error;
    emscripten_idb_store(db_name, path, data, bytes, &error);
    ASSERT(!error, -1, "ERROR writing file \"%s\"\n", path);
    return 0;
}




void delete_file(char* path) {
    int error;
    emscripten_idb_delete(db_name, path, &error);
    ASSERTVOID(!error, "ERROR deleting file \"%s\"\n", path);
}
