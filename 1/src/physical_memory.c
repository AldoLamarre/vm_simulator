#include <stdio.h>


#include "conf.h"
#include "physical_memory.h"

// Initialise la mémoire physique
void
pm_init (struct physical_memory *pm, FILE * backing_store, FILE * pm_log)
{
  pm->backing_store = backing_store;
  pm->log = pm_log;

  for (unsigned int i = 0; i < PHYSICAL_MEMORY_SIZE; i++)
    {
      pm->memory[i] = ' ';
    }
}

// Retourne le numéro d'une trame (frame) libre
uint16_t
pm_find_free_frame (struct physical_memory *pm)
{
  for (unsigned int i = 0; i < PHYSICAL_MEMORY_SIZE; i+=PAGE_FRAME_SIZE)
    {
      if (pm->memory[i] == ' '){//mettre la bonne condition
		return i%PAGE_FRAME_SIZE ;
	  }
    }
	return -1;
}

// Charge la page demandée du backing store
uint16_t
pm_demand_page (struct physical_memory *pm, uint16_t page_number)
{	
	int r = 0;
	//if(page_number > NUM_PAGES){
	// return -1;
	//}
	uint16_t free_frame = pm_find_free_frame(pm);
	r = fseek( pm->backing_store, page_number*PAGE_FRAME_SIZE, SEEK_SET );
	//if (r)
	//	return -1;	
	r = fread( pm->memory+free_frame, sizeof(char),
		PAGE_FRAME_SIZE, pm->backing_store);
	//if (r == PAGE_FRAME_SIZE )
		return free_frame;
	//return -1;		
}

// Sauvegarde la page spécifiée
void
pm_backup_frame (struct physical_memory *pm, uint16_t frame_number,
		 uint16_t page_number)
{	
	if(page_number < NUM_PAGES  || frame_number < NUM_FRAMES){	
		fwrite( pm->memory+frame_number, sizeof(char),
			PAGE_FRAME_SIZE, pm->backing_store);
	}	
}

void
pm_clean (struct physical_memory *pm)
{
  // Assurez vous d'enregistrer les modifications apportées au backing store!
	for( int i=0;i < NUM_FRAMES; i++){
	
	
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
