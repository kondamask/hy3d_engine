#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define KILOBYTES(val) (val * 1024ULL)
#define MEGABYTES(val) (KILOBYTES(val) * 1024ULL)
#define GIGABYTES(val) (MEGABYTES(val) * 1024ULL)
#define TERABYTES(val) (GIGABYTES(val) * 1024ULL)

#define AdvancePointer(ptr, bytes) ptr = (u8 *)ptr + bytes

#endif // INCLUDE_UTILS_H