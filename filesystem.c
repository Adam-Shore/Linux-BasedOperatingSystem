// Read only FS
    // File Read Directory Read
        // Directory Read -> Read the File Name
        // File Read -> read the Datablocks
// Open Close returns 0
// Write returns -1

#include "filesystem.h"
#include "lib.h"

pcb_t processes[6];
int pid;

/* void initFileSystem
 * Inputs: none
 * Return Value: none
 * Function: set up file system pointers */
void initFileSystem(){
    /*initialize the values for each following variable*/
    boot_block_ptr = (boot_block *) base_address;
    // printf("boot_block_ptr: %d" boot_block_ptr);
    directory_entry_ptr = (directory_entry *) boot_block_ptr->direntries;
    // printf("directory_entry_ptr: %d" directory_entry_ptr);
    inode_entry_ptr = (inode_entry *)  (base_address + MAX_DATA);
    // printf("inode_entry_ptr: %d" inode_entry_ptr);

    inode_count = boot_block_ptr->inode_count;
    // printf("inode_count: %d", inode_count);
    data_count = boot_block_ptr->data_count;
    // printf("data_count: %d", data_count);
    dir_count = boot_block_ptr->dir_count;
    // printf("dir_count: %d", dir_count);

    data_block_ptr = (data_block *) inode_entry_ptr + inode_count; // Pointer to the first data block index at 0

    file_counter = 0;
    
}

/* int32_t read_dentry_by_name
 * Inputs: const uint8_t * fname, directory_entry * dentry
 * Return Value: none
 * Function: locate dentry from fname, pass it back through dentry */
int32_t read_dentry_by_name(const uint8_t * fname, directory_entry * dentry){
    uint32_t count = 0;

    // We might exceed dir_count
    for (count = 0; count < dir_count; count++){ //use loop to find index
        if(strncmp((int8_t*)fname, (int8_t*)boot_block_ptr->direntries[count].filename, STR_LEN) == 0){ //check if equal names to see if found
            //copy contents
            strcpy((int8_t*)dentry->filename, (int8_t*)boot_block_ptr->direntries[count].filename);
            dentry->filetype =  boot_block_ptr->direntries[count].filetype;
            dentry->inode_num =  boot_block_ptr->direntries[count].inode_num;
            return 0;
        }
    }

    return -1;
}

/* int32_t read_dentry_by_index
 * Inputs: uint32_t index, directory_entry * dentry
 * Return Value: none
 * Function: locate dentry from index, pass it back through dentry */
int32_t read_dentry_by_index (uint32_t index, directory_entry * dentry){
    if (index >= 0 && index <= MAX_INDEX){ //check if index is in range
        //copy components
        strcpy((int8_t*)dentry->filename, (int8_t*)boot_block_ptr->direntries[index].filename);
        dentry->filetype =  boot_block_ptr->direntries[index].filetype;
        dentry->inode_num =  boot_block_ptr->direntries[index].inode_num;
        // *dentry = boot_block_ptr->direntries[index];
        return 0;
    }

    return -1;
}

/* int32_t read_data
 * Inputs: uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length
 * Return Value: none
 * Function: read data from file indicated by inode, pass back into buf */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length){

    if(inode < 0 || inode >= inode_count){ // inode count might be zero index
        return -1;
    }
    
    inode_entry* current_inode_entry_ptr = &inode_entry_ptr[inode];
    //printf("%d", current_inode_entry_ptr);
    // printf("Checkpoint 1 \n");
    uint32_t data_length = current_inode_entry_ptr->length; //Byte Length of the whole file
    // printf("Checkpoint 1 \n");
    uint32_t data_max_index = data_length / MAX_DATA; //Total Indexes of the whole file, including 0
    // printf("Checkpoint 1 \n");
    uint32_t curr_data_block_index = offset / MAX_DATA; // Current data block index
    // printf("Checkpoint 1 \n");
    uint32_t curr_block_byte = offset % MAX_DATA; // offset in the current block
    // printf("Checkpoint 1 \n");
    uint8_t* src_pointer = &((data_block_ptr + (current_inode_entry_ptr->data_block[curr_data_block_index]))->data[curr_block_byte]);
    // printf("Checkpoint 1 \n");
    uint8_t* dest_pointer = buf;
    // printf("Checkpoint 1 \n");
    uint32_t mem_till_end_of_block = ((curr_data_block_index + 1) * MAX_DATA) - offset;
    // printf("Checkpoint 1 \n");
    uint32_t total_mem_copied = length;
    // printf("Checkpoint 1 \n");


    if(curr_data_block_index > data_max_index){ // if the current data block exceeds the total data blocks you can have
        return -1;
    }

    // if(offset > data_length){
    //     return -1;
    // }


    // if(offset + length > data_length){
    //     memcpy((int8_t*)dest_pointer, (int8_t*)src_pointer, data_length);
    //     return data_length;
    // }

// 2000 + 4000 // 5000 remind TODO

    // printf("Offset: %d", offset);
    // printf("Length: %d", length);
    // printf("mem_till_end_of_block: %d", mem_till_end_of_block);
    
    while (length > 0){ //How does the returns work? when do we return 0?
        
        // printf("Before If-else statement \n");
        if (mem_till_end_of_block <= length){
            // Copy over this to memory and move to the next block
            // printf("Before string Copy in true statement\n");
            memcpy((int8_t*)dest_pointer, (int8_t*)src_pointer, mem_till_end_of_block);

            //Move to next block
            // printf("After STR copy\n");
            src_pointer = (uint8_t*) (data_block_ptr + (current_inode_entry_ptr->data_block[++curr_data_block_index]));
            
            dest_pointer = dest_pointer + mem_till_end_of_block;
            length = length - mem_till_end_of_block;
            mem_till_end_of_block = MAX_DATA;// copy over the whole thing

        }else {
            // Copy over the rest of length
            // printf("In else statement \n");
            memcpy((int8_t*)dest_pointer, (int8_t*)src_pointer, length);
            length = 0;
        }
    }

    if(offset + total_mem_copied == data_length){ //reached end of file
        return total_mem_copied;
    }

    return total_mem_copied;
}

int filesystem_open(const uint8_t* filename){
    return 0;
}

int filesystem_close(int32_t fd){
    return 0;
}


/* int32_t dir_read
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: none
 * Function: read data from given directory, pass back into buf */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes){
    /*
    if(processes[pid].fd_arr[fd].file_pos >= dir_count){ //not totally sure
         return 0;
    }
    
    if (fd < 0 || fd > 7) {
        return -1;
    }
    
    // if(processes[pid].fd_arr[fd].file_pos >= dir_count){
    //     //file_counter = 0;
    //     return 0;
    // }


    

    directory_entry temp;
    uint32_t return_value = read_dentry_by_index(processes[pid].fd_arr[fd].file_pos, &temp);
    if (return_value == -1) {
        return 0;
    }
    uint8_t temp_buf[40];
    strncpy((int8_t*)temp_buf,(int8_t*) ((temp.filename)),STR_LEN);
    strncpy(buf,(int8_t*) ((temp.filename)),STR_LEN);
    //memcpy instead?
    file_counter++;


    
    processes[pid].fd_arr[fd].file_pos++;

    int count = 0;
    while (temp_buf[count] != '\0') {
        count++;
    }


    return count;
    */

    if (fd < 2 || fd > 7) {
        return -1;
    }
    
    // if(processes[pid].fd_arr[fd].file_pos >= dir_count){
    //     //file_counter = 0;
    //     return 0;
    // }


    

    directory_entry temp;
    uint32_t return_value = read_dentry_by_index(processes[pid].fd_arr[fd].file_pos, &temp);
    if (return_value == -1) {
        return 0;
    }
    uint8_t temp_buf[40];
    strncpy((int8_t*)temp_buf,(int8_t*) ((temp.filename)),STR_LEN);
    strncpy((int8_t*)buf,(int8_t*) ((temp.filename)),STR_LEN);
    //memcpy instead?
    // file_counter++;


    
    processes[pid].fd_arr[fd].file_pos++;
    uint32_t readlength = strlen((int8_t*)temp.filename);
    if(readlength > 32){
        return 32; 
    }
    return readlength;

}

/* int32_t dir_open
 * Inputs: uint8_t* dirname
 * Return Value: none
 * Function: open directory driver function */
int32_t dir_open(uint8_t* dirname){
    directory_entry dentry;
    int read_val = read_dentry_by_name(dirname, &dentry);
    if (read_val == -1) {
        return -1;
    }

    //check if directory
    if (dentry.filetype != 1) {
        return -1;
    }

    return 0;
}

int32_t dir_close(int32_t fd){
    return 0;
}

int32_t dir_write(int32_t fd, void* buf, int32_t nbytes){
    return 0;
}

/* int32_t filesystem_read
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: none
 * Function: read file data driver function, call read_data */
int32_t filesystem_read(int32_t fd, void* buf, int32_t nbytes){
    // if(processes[pid].fd_arr[fd].file_pos >= nbytes){ //not totally sure
    //     return 0;
    // }

    //directory_entry temp;
    //uint32_t n_bytes_read = read_dentry_by_name(buf, &temp); //the buf holds the name
    //uint32_t n_bytes_read = read_dentry_by_index(processes[pid].fd_arr[fd].inode_num, &temp);
    if(processes[pid].fd_arr[fd].file_pos >= inode_entry_ptr[processes[pid].fd_arr[fd].inode_num].length){ //not totally sure
         return 0;
    }
    // uint8_t ret_file[8000];
    int32_t return_value = read_data(processes[pid].fd_arr[fd].inode_num, processes[pid].fd_arr[fd].file_pos, buf, nbytes);
    processes[pid].fd_arr[fd].file_pos += return_value;
    // strncpy(buf,ret_file,return_value);

    // if(n_bytes_read < 0){ //Error Checking
    //     return -1;
    // }

    if (return_value < 0){
        return -1;
    }

    return return_value;
}

int filesystem_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}





/* 
Questions to Ask:
current_inode_entry_ptr->data_block[curr_data_block_index] -> calculated the data block # on the array that we need to access using the ptr
data_block[++curr_data_block_index]  -> returns the data block # 
// What are we returnign
*/
