#include "unistd.h"


short file_exists(char *filename) {
    return access(filename, F_OK) != -1;
}
