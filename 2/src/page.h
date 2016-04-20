#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

enum page_flags
{
  verification = 0x1,
  dirty        = 0x2 
};
struct page {
  uint8_t flags;
  int32_t frame_number;

};




#endif

 

