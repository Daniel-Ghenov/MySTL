#ifndef MYSTD_STRING_HELPER_H
#define MYSTD_STRING_HELPER_H
#include <iostream>

namespace mystd {

int strcmp(const char* first, const char* second);
void strcpy_s(char* Destination,size_t size, const char* Source);
void strcpy(char* Destination, const char* Source);
void strcat(char* Destination, const char* Source);
unsigned strlen(const char* str);
size_t linear_find(const char* toFind, const char* findIn);

} // namespace mystd

#endif // MYSTD_STRING_HELPER_H
