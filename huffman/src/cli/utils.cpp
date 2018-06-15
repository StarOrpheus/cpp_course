#include "include/utils.h"

std::FILE *except_fopen(const char *filename, const char *mode) {
    std::FILE * result = fopen(filename, mode);

    if (result == nullptr) {
        perror("The following error occured: ");
        throw std::runtime_error("Error opening file: " + std::string(filename));
    }

    return result;
}
