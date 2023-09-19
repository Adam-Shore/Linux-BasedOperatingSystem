#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "i8259.h"
#include "filesystem.h"
#include "keyboard.h"


#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

/* IDT Test - Divide by Zero
 * 
 * Tests divide by zero
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Divide by zero error
 * Files: x86_desc.h/S
 */
int div_zero_test(){
	TEST_HEADER;

	int a = 5;
	int b = 0;
	return a/b;
}

/* page_fault_test
 * Whether you can access outside the 8mb mem area
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

 //assertion_failure();
			// result = FAIL;
int page_fault_test(){
	TEST_HEADER;

	uint32_t * pointer = (uint32_t *) 0xFA1201; //Outside the area in memory
	unsigned int test = *pointer;
	test = 0;
	return * pointer;
}


/* page_fault_test
 * Whether you can access inside the 8mb mem area
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

int page_fault_test_in_range(){
	TEST_HEADER;

	uint32_t * pointer = (uint32_t *)0x4A1201; //inside the area in memory
	unsigned int test = *pointer;
	test = 0;
	return PASS;
}

/* Checkpoint 2 tests */

/* RTC read/write/change_rate Test
 * 
 * Asserts that the RTC driver is working (open, read, write)
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: PIC and RTC
 * Files: RTC.C/H
 */
int rtc_test(){
	//TEST_HEADER;
	// int i;
	rtc_open(0);
	uint32_t set_freq = 2;
	rtc_write(0, &set_freq, 0);
	//rtc_change_rate(15);
	int i;
	for(i = 0; i <10; i++){
		printf("Slow Frequency");
		rtc_read(0, 0, 0);
	}
	printf("\n");

	set_freq = 32;
	rtc_write(0, &set_freq, 0);
	//rtc_change_rate(8);
	for(i = 0; i <70; i++){
		printf(" Medium Frequency");
		rtc_read(0, 0, 0);
	}
	printf("\n");

	set_freq = 1024;
	rtc_write(0, &set_freq, 0);
	//rtc_change_rate(3);
	for(i = 0; i <999999999; i++){
		printf("    Fast Frequency");
		rtc_read(0, 0, 0);
	}

	int result = PASS;
	return result;
 }

/* read_dentry_by_name Test
 * Tests if you can get a dentry by name
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

int read_dentry_by_name_test(){
	TEST_HEADER;
	uint8_t* fname = (uint8_t*)boot_block_ptr->direntries; // Gives you the first file name for the first directory
	directory_entry temp;
	directory_entry * temp_ptr = &temp;

	printf("Started Test Case ");

	read_dentry_by_name(fname,temp_ptr);

	if(strncmp((int8_t*)fname, (int8_t*)temp.filename, 32) == 0){
		printf("PASSED THIS SHIT");
		return PASS;
	}

	return FAIL;
}

/* read_dentry_by_index_test
 * Tests if you can get a dentry by index
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

int read_dentry_by_index_test(){
	TEST_HEADER;
	// uint8_t* fname = directory_entry_ptr;
	uint32_t index = 1;
	uint8_t* fname = (uint8_t*)&boot_block_ptr->direntries[index];
	directory_entry temp;
	directory_entry * temp_ptr = &temp;

	printf("Started Test Case ");

	read_dentry_by_index(index,temp_ptr);
	printf("\n");
	printf("First fname:");
	printf("%d", *fname);
	printf("\n");
	printf("Second fname:");
	printf("%d", *temp.filename);
	printf("\n");
	if(strncmp((int8_t*)fname, (int8_t*)temp.filename, 32) == 0){
		printf("PASSED THIS SHIT");
		return PASS;
	}

	return FAIL;
}

/* read_data
 * Reads small file
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

int read_data_test_small(){
	TEST_HEADER;

	directory_entry tmp;
	int i;
	uint8_t buf[4096];
	//read_dentry_by_index(10, &tmp);
	read_dentry_by_name((uint8_t *)"frame0.txt", &tmp);
	// printf("Length: %d",inode_entry_ptr[tmp.inode_num].length);
	// printf("%d", tmp.inode_num); //inode 38
	int n = read_data(tmp.inode_num,0,buf,187); //187
	printf("n:%d \n", n); 
	for (i = 0; i < 187; i++) {
		printf("%c", buf[i]);
	}

	if (n == 0){
		printf("PASSED THIS SHIT");
		return PASS;
	}

	return FAIL;
}

/* read_data
 * Reads large file
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

int read_data_test_large(){
	TEST_HEADER;

	// inode_count = boot_block_ptr->inode_count;
    // printf("inode_count: %d", inode_count);
    // data_count = boot_block_ptr->data_count;
    // printf("data_count: %d", data_count);
    // dir_count = boot_block_ptr->dir_count;
    // printf("dir_count: %d", dir_count);

    // printf("boot_block_ptr: %d", boot_block_ptr);
    // printf("directory_entry_ptr: %d", directory_entry_ptr);
    // printf("inode_entry_ptr: %d", inode_entry_ptr);


	directory_entry tmp;
	// int i;
	uint8_t buf[6000];
	//read_dentry_by_index(10, &tmp);
	read_dentry_by_name((uint8_t *)"verylargetextwithverylongname.txt", &tmp);
	printf("Length: %d",inode_entry_ptr[tmp.inode_num].length);
	// printf("%d", tmp.inode_num); //inode 38
	int n = read_data(tmp.inode_num,1000,buf,4277);
	printf("hello world");
	// printf("n:%d \n", n); 
	// for (i = 0; i < 5277; i++) {
	// 	printf("%c", buf[i]);
	// }

	printf("Second to last character (should be '?') :%c \n", buf[4275]);

	if (n == 0){ //whole file
		printf("PASSED THIS SHIT");
		return PASS;
	}

	return FAIL;
}

/* read_data_test_executable
 * Reads exec file
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */

int read_data_test_executable(){
	TEST_HEADER;

	// inode_count = boot_block_ptr->inode_count;
    // printf("inode_count: %d", inode_count);
    // data_count = boot_block_ptr->data_count;
    // printf("data_count: %d", data_count);
    // dir_count = boot_block_ptr->dir_count;
    // printf("dir_count: %d", dir_count);

    // printf("boot_block_ptr: %d", boot_block_ptr);
    // printf("directory_entry_ptr: %d", directory_entry_ptr);
    // printf("inode_entry_ptr: %d", inode_entry_ptr);


	directory_entry tmp;
	uint8_t buf[36164];
	//read_dentry_by_index(10, &tmp);
	read_dentry_by_name((uint8_t *)"fish", &tmp);
	printf("Length: %d",inode_entry_ptr[tmp.inode_num].length);
	// printf("%d", tmp.inode_num); //inode 38
	int n = read_data(tmp.inode_num,0,buf,36164);
	printf("hello world");
	printf("n:%d \n", n); 
	//int i; 
	//
	// for (i = 0; i < 36164; i++) {
	// 	printf("%c", buf[i]);
	// }

	if (n == 0){ //Reads the whole file
		printf("PASSED THIS SHIT");
		return PASS;
	}

	return FAIL;
}


/* list_files_test
 * Lists File Directory (tests the dir_read function)
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: mentioned above
 * Files: x86_desc.h/S
 */
int list_files_test(){
	TEST_HEADER;
	
	directory_entry tmp;
	uint32_t i = 0;
	uint32_t j;
	for (i =0; i < dir_count; i++){
		
		uint8_t fname[STR_LEN];
		dir_read(0,fname,STR_LEN);
		uint8_t * ftype = (uint8_t *)directory_entry_ptr[file_counter].filetype;
		read_dentry_by_name(fname, &tmp);
		printf("file name: ");
		for (j = 0; j < STR_LEN; j++) {
			putc(fname[j]);
		}
		
		printf(", file type: %d, file size: %d \n", ftype, inode_entry_ptr[tmp.inode_num].length);
	}

	return PASS;
}



int terminal_test(){
	//TEST_HEADER;

	unsigned char buf[128];
	int w;
	//terminal_write(1, (unsigned char*)"Testing\n", w);
	while (1) {
		w = terminal_read(1, buf, 128);
		terminal_write(1, buf, w);
	}
	// int w = 0;
	// while (!w) {
	// 	w = terminal_read(1, buf, 5);
	// }
	// int j;
	// terminal_write(1, buf, w);
	return PASS;
}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("idt_test_div", div_zero_test());
	// TEST_OUTPUT("page_fault_test", page_fault_test());
	// TEST_OUTPUT("page_fault_test_in_range", page_fault_test_in_range());


	// TEST_OUTPUT("RTC_test", rtc_test());
	// TEST_OUTPUT("read_dentry_by_name_test", read_dentry_by_name_test());
	// TEST_OUTPUT("read_dentry_by_index_test", read_dentry_by_index_test());
	// TEST_OUTPUT("read_data_test_small", read_data_test_small());
	// TEST_OUTPUT("read_data_test_executable", read_data_test_executable());
	// TEST_OUTPUT("read_data_test_large", read_data_test_large());
	TEST_OUTPUT("list_files_test", list_files_test());
	// TEST_OUTPUT("Terminal_test", terminal_test());
	// launch your tests here
}
