#include "paging.h"


/* void init_paging
 * Inputs: none
 * Return Value: none
 * Function: Inits the paging by setting the proper structs and aread in memory
    by following intel instructions
  */
void init_paging() { 
    int i;
    for (i = 2; i < 1024; i++) {
        page_dir[i].mb.p = 0;
        page_dir[i].mb.rw = 1;
        page_dir[i].mb.us = 0;
        page_dir[i].mb.pwt = 0;
        page_dir[i].mb.pcd = 0;
        page_dir[i].mb.a = 0;
        page_dir[i].mb.d = 0;
        page_dir[i].mb.ps = 1;
        page_dir[i].mb.g = 0;
        page_dir[i].mb.avail = 0;
        page_dir[i].mb.pat = 0;
        page_dir[i].mb.res = 0;
        page_dir[i].mb.p_base = i;

    }

    page_dir[1].mb.p = 1;
    page_dir[1].mb.rw = 1;
    page_dir[1].mb.us = 0;
    page_dir[1].mb.pwt = 0;
    page_dir[1].mb.pcd = 0;
    page_dir[1].mb.a = 0;
    page_dir[1].mb.d = 0;
    page_dir[1].mb.ps = 1;
    page_dir[1].mb.g = 1;
    page_dir[1].mb.avail = 0;
    page_dir[1].mb.pat = 0;
    page_dir[1].mb.res = 0;
    page_dir[1].mb.p_base = 1;

    for (i = 0; i < 1024; i++) { 
        first_page_table[i].rw = 1;
        first_page_table[i].us = 0; 
        first_page_table[i].pwt = 0;
        first_page_table[i].pcd = 0;
        first_page_table[i].a = 0;
        first_page_table[i].d = 0;
        first_page_table[i].pat = 0;
        first_page_table[i].g = 0;
        first_page_table[i].avail = 0;
        first_page_table[i].p_base = i;

        video_mapping_page[i].rw = 1;
        video_mapping_page[i].us = 0; 
        video_mapping_page[i].pwt = 0;
        video_mapping_page[i].pcd = 0;
        video_mapping_page[i].a = 0;
        video_mapping_page[i].d = 0;
        video_mapping_page[i].pat = 0;
        video_mapping_page[i].g = 0;
        video_mapping_page[i].avail = 0;
        video_mapping_page[i].p_base = i;

        if (i == 0xB8000 >> 12) {
            first_page_table[i].p = 1;
            video_mapping_page[i].p = 1;
        }else{
            first_page_table[i].p = 0;
            video_mapping_page[i].p = 0;
        }

        if (i == 0xB9000 >> 12) {
            first_page_table[i].p = 1;
            //video_mapping_page[i].p = 1;
        }

        if (i == 0xBA000 >> 12) {
            first_page_table[i].p = 1;
            //video_mapping_page[i].p = 1;
        }

        if (i == 0xBB000 >> 12) {
            first_page_table[i].p = 1;
            //video_mapping_page[i].p = 1;
        }

    }

    uint32_t addr = (uint32_t) first_page_table;
    page_dir[0].kb.p = 1;
    page_dir[0].kb.rw = 1;
    page_dir[0].kb.us = 0;
    page_dir[0].kb.pwt = 0;
    page_dir[0].kb.pcd = 0;
    page_dir[0].kb.a = 0;
    page_dir[0].kb.zero = 0;
    page_dir[0].kb.ps = 0;
    page_dir[0].kb.g = 0;
    page_dir[0].kb.avail = 0;
    page_dir[0].kb.p_base = addr >> 12;


    uint32_t vid_map_addr = (uint32_t) video_mapping_page;
    page_dir[33].kb.p = 1;
    page_dir[33].kb.rw = 1;
    page_dir[33].kb.us = 1;
    page_dir[33].kb.pwt = 0;
    page_dir[33].kb.pcd = 0;
    page_dir[33].kb.a = 0;
    page_dir[33].kb.zero = 0;
    page_dir[33].kb.ps = 0;
    page_dir[33].kb.g = 0;
    page_dir[33].kb.avail = 0;
    page_dir[33].kb.p_base = vid_map_addr >> 12;
    
}
