typedef unsigned short u16;

typedef unsigned long sz;

const u16 ALIGN = 8U;

const sz ARENA = 64 * 1024;

// pack the struct in 24 bytes (8 byte alignment = 3 rows in the memory for 64
// bit arch)
typedef struct __attribute__((packed)) _head {

  u16 bfree, bsize, free, size;

  struct _head *next, *previous;

} head_t;
