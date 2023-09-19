#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "types.h"
#include "systemcall.h"

#define MAX_DATA 4096
#define MAX_INDEX 63
#define STR_LEN 32

typedef struct directory_entry{
    uint8_t filename[32]; //IS this right?
    uint32_t filetype;
    uint32_t inode_num;
    uint8_t reserved[24];
} __attribute__ ((packed)) directory_entry;

typedef struct boot_block {
    uint32_t dir_count;
    uint32_t inode_count;
    uint32_t data_count;
    uint8_t reserved[52];
    directory_entry direntries[63];
} __attribute__ ((packed)) boot_block;

typedef struct data_block{
    uint8_t data[4096];
} __attribute__ ((packed)) data_block;

typedef struct inode_entry{
    uint32_t length;
    uint32_t data_block[1023];

} __attribute__ ((packed)) inode_entry;


boot_block * boot_block_ptr;
directory_entry * directory_entry_ptr;
inode_entry * inode_entry_ptr;
data_block * data_block_ptr;

uint32_t inode_count;
uint32_t data_count;
uint32_t dir_count;
uint32_t base_address;
uint32_t file_counter;

void initFileSystem();
int32_t read_dentry_by_name (const uint8_t * fname, directory_entry * dentry);
int32_t read_dentry_by_index (uint32_t index, directory_entry * dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);
int filesystem_open(const uint8_t* filename);
int filesystem_close(int32_t fd);
int32_t filesystem_read(int32_t fd, void* buf, int32_t nbytes);
int filesystem_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_read(int32_t fd, void * buf, int32_t nbytes);
int32_t dir_write(int32_t fd, void * buf, int32_t nbytes);
int32_t dir_open(uint8_t* dirname);
int32_t dir_close(int32_t fd);

#endif
