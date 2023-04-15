#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

namespace HY3D
{
#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define KILOBYTES(val) (val * 1024ULL)
#define MEGABYTES(val) (KILOBYTES(val) * 1024ULL)
#define GIGABYTES(val) (MEGABYTES(val) * 1024ULL)
#define TERABYTES(val) (GIGABYTES(val) * 1024ULL)

#define AdvancePointer(ptr, bytes) ptr = (u8 *)ptr + bytes

#define BIT(n)                    ( 1<<(n) )
#define BIT_GET(bitfield, n)      ( bitfield &   (n) )
#define BIT_SET(bitfield, mask)   ( bitfield |=  (mask) )
#define BIT_CLEAR(bitfield, mask) ( bitfield &= ~(mask) )
#define BIT_FLIP(bitfield, mask)  ( bitfield ^=  (mask) )
}

#endif // INCLUDE_UTILS_H