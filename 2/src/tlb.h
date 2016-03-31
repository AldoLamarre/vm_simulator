#ifndef TLB_H
#define TLB_H

#include "conf.h"

struct tlb_entry
{
  int32_t page_number;
  int32_t frame_number;
};

struct tlb
{
  FILE* log;
  struct tlb_entry entries[TLB_NUM_ENTRIES]; 
  unsigned int next_entry_available;
};

void tlb_init(struct tlb*, FILE*);
void tlb_clean(struct tlb*);
int32_t tlb_lookup(struct tlb*, uint16_t);
void tlb_add_entry(struct tlb*, uint16_t, uint16_t);

#endif
