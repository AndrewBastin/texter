#include <sys/stat.h>

#include "unistd.h"

short isFileWritable(char *filename) {
    struct stat s;
    short code = stat(filename, &s);
    
    return (code == 0) && (access(filename, W_OK) == 0); 
}

short file_exists(char *filename) {
    return access(filename, F_OK) != -1;
}
