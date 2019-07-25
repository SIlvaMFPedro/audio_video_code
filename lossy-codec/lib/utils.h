#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <assert.h>
#include <sys/stat.h>

std::string expand_user(std::string);
long get_file_size(std::string filename);

#endif