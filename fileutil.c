#include <sys/stat.h>

#include "unistd.h"

short isFileWritable(char *filename) {
    struct stat s;
    short code = stat(filename, &s);

    return (code == 0) && (s.st_mode & W_OK);
}

short file_exists(char *filename) {
    return access(filename, F_OK) != -1;
}
