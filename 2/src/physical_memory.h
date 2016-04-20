#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include "conf.h" 
#include "page.h"

enum fflags{
	USED = 0x1,
	DIRTY= 0x2
};


struct frame{
  uint8_t flags;
};

struct physical_memory
{
  FILE* backing_store;
  FILE* log;
  char memory[PHYSICAL_MEMORY_SIZE]; 
  struct frame frame_table[NUM_FRAMES];
  uint16_t nextFrame;
};



/* Initialise la mémoire physique */
void pm_init(struct physical_memory*, FILE*, FILE*);
uint16_t pm_find_free_frame(struct physical_memory*, struct page page_table[NUM_PAGES]);
uint16_t pm_demand_page(struct physical_memory*, uint16_t, struct page page_table[NUM_PAGES]);
void pm_backup_frame(struct physical_memory*, uint16_t, uint16_t);
void pm_clean(struct physical_memory*, struct page page_table[NUM_PAGES]);

#endif
