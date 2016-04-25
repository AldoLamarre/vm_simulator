#include <stdio.h>


#include "conf.h"
#include "physical_memory.h"

// Initialise la mémoire physique
void
pm_init (struct physical_memory *pm, FILE * backing_store, FILE * pm_log)
{
  pm->backing_store = backing_store;
  pm->log = pm_log;
  pm->nextFrame=0;
	for(unsigned int i=0;i<NUM_FRAMES;i++){
		pm->frame_table[i].flags=(uint8_t)0;
	}
  
  for (unsigned int i = 0; i < PHYSICAL_MEMORY_SIZE; i++)
    {
      pm->memory[i] = ' ';	  
    }
}

// Retourne le numéro d'une trame (frame) libre
uint16_t
pm_find_free_frame (struct physical_memory *pm, struct page page_table[NUM_PAGES] )
{
  for (uint16_t i = 0; i < NUM_FRAMES; i++)
    {
      if (!(pm->frame_table[i].flags & USED)){//mettre la bonne condition
		pm->frame_table[i].flags |= USED;
		return i;
	  }
    }
	
  pm->nextFrame = (pm->nextFrame + 1) % NUM_FRAMES ; 
  
   for (unsigned int i = 0; i < NUM_PAGES; i++)
    {
      if (page_table[i].frame_number == pm->nextFrame ){//mettre la bonne condition
		if(page_table[i].flags & verification) {///"
			pm->nextFrame = (pm->nextFrame + 1) % NUM_FRAMES ; 
		}else{			
			if(page_table[i].flags & dirty){
				pm_backup_frame(pm,pm->nextFrame,i);
			}
			pm->frame_table[page_table[i].frame_number].flags |= USED;
			page_table[i].frame_number = -1;
			break;
		}
	  }
    }
  
  return  pm->nextFrame;
	
	
}

// Charge la page demandée du backing store
uint16_t
pm_demand_page (struct physical_memory *pm, uint16_t page_number, 
	struct page page_table[NUM_PAGES])
{	
	int r = 0;
	//if(page_number > NUM_PAGES){
	// return -1;
	//}
	uint16_t free_frame = pm_find_free_frame(pm,page_table);
	r = fseek( pm->backing_store, page_number*PAGE_FRAME_SIZE, SEEK_SET );
	//if (r)
	//	return -1;	
	//printf("%d\n",ftell(pm->backing_store));	
	r = fread( pm->memory+(free_frame*PAGE_FRAME_SIZE), sizeof(char),
		PAGE_FRAME_SIZE, pm->backing_store);
	//printf("%d\n",ftell(pm->backing_store));

	//if (r == PAGE_FRAME_SIZE )
		return free_frame;
	//return -1;		
}

// Sauvegarde la page spécifiée
void
pm_backup_frame (struct physical_memory *pm, uint16_t frame_number,
		 uint16_t page_number)
{	
	
	if(page_number < NUM_PAGES  && frame_number < NUM_FRAMES){		
		fseek( pm->backing_store, page_number*PAGE_FRAME_SIZE, SEEK_SET );			
		fwrite( pm->memory+(frame_number*PAGE_FRAME_SIZE), PAGE_FRAME_SIZE,
			 sizeof(char) , pm->backing_store);
			//fwrite( pm->memory+(frame_number*PAGE_FRAME_SIZE) , sizeof(char), PAGE_FRAME_SIZE,stdout);
	}	
}

void
pm_clean (struct physical_memory *pm, struct page page_table[NUM_PAGES])
{
  
	
	for(int i=0;i<NUM_PAGES;i++){
		if(page_table[i].flags & dirty){
			pm_backup_frame(pm,page_table[i].frame_number,i);
		}		
	}
	
  // Enregistre l'état de la mémoire physique.
  if (pm->log)
    {
      for (unsigned int i = 0; i < PHYSICAL_MEMORY_SIZE; i++)
	{
	  if (i % 80 == 0)
	    fprintf (pm->log, "%c\n", pm->memory[i]);
	  else
	    fprintf (pm->log, "%c", pm->memory[i]);
	}
    }
}
