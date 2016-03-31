#ifndef VMM_H
#define VMM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "conf.h"
#include "physical_memory.h"
#include "tlb.h"

enum page_flags
{
  verification = 0x1,
  dirty        = 0x2
};

struct page
{
  uint8_t flags;
  int32_t frame_number;
};

struct virtual_memory_manager
{
  unsigned int page_fault_count;
  unsigned int page_found_count;
  unsigned int tlb_hit_count;
  unsigned int tlb_miss_count;

  struct page page_table[NUM_PAGES];

  struct tlb tlb;
  struct physical_memory pm;

  FILE* log;
};

void vmm_init(struct virtual_memory_manager*, FILE*, FILE*, FILE*, FILE*);
void vmm_read(struct virtual_memory_manager*, uint16_t);
void vmm_write(struct virtual_memory_manager*, uint16_t, char);
void vmm_clean(struct virtual_memory_manager*);

#endif
