#ifndef fs_h
#define fs_h

#include <stdint.h>
#include <stddef.h>

#define FAT_FREE 0x0000
#define FAT_EOF 0xFFFF

typedef struct{
    uint32_t magic;          
    uint32_t total_blocks;  
    uint32_t total_clusters;
    uint32_t cluster_size;
} superblock_t;

typedef struct{
  char name[32];

  uint32_t size;

  uint16_t first_cluster;
} dir_entry_t;

int fs_init(void);
int fs_create(const char *name);
int fs_open(const char *name);
int fs_close(int fd);
int fs_read(int fd, void *buffer, uint32_t size);
int fs_write(int fd, const void *buffer, uint32_t size);
int fs_delete(const char *name);
