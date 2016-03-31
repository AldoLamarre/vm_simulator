#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "conf.h"
#include "common.h"
#include "vmm.h"
#include "tlb.h"
#include "physical_memory.h"

void
vmm_init (struct virtual_memory_manager *vmm,
	  FILE * backing_store, FILE * vmm_log, FILE * tlb_log, FILE * pm_log)
{

  // Initialise la mémoire physique.
  pm_init (&vmm->pm, backing_store, pm_log);
  tlb_init (&vmm->tlb, tlb_log);

  // Initialise les compteurs.
  vmm->page_fault_count = 0;
  vmm->page_found_count = 0;
  vmm->tlb_hit_count = 0;
  vmm->tlb_miss_count = 0;

  // Initialise le fichier de journal.
  vmm->log = vmm_log;

  // Initialise la table de page.
  for (unsigned int i; i < NUM_PAGES; i++)
    {
      vmm->page_table[i].flags = 0x0;
      vmm->page_table[i].frame_number = -1;
    }

}


// NE PAS MODIFIER CETTE FONCTION
void
vmm_log_command (FILE * out, const char *command, uint16_t laddress,	/* logical address */
		 uint16_t page, uint16_t frame, uint16_t offset, uint16_t paddress,	/* physical address */
		 char c)	/* char lu ou écrit */
{
  if (out)
    {
      fprintf (out, "%s[%c]@%d: p=%d, t=%d, o=%d pa=%d\n", command, c, laddress,
	       page, frame, offset, paddress);
    }
}

/* Effectue une lecture à l'adresse logique `laddress` */
void
vmm_read (struct virtual_memory_manager *vmm, uint16_t laddress)
{

  /* Complétez */

  // Vous devez fournir les arguments manquants. Basez-vous sur la signature de
  // la fonction.
  vmm_log_command (stdout, "READING", laddress, 0, 0, 0, 0, '0');
}

/* Effectue une écriture à l'adresse logique `laddress` */
void
vmm_write (struct virtual_memory_manager *vmm, uint16_t laddress, char c)
{

  /* Complétez */

  // Vous devez fournir les arguments manquants. Basez-vous sur la signature de
  // la fonction.
  vmm_log_command (stdout, "WRITING", laddress, 0, 0, 0, 0, c);
}


// NE PAS MODIFIER CETTE FONCTION
void
vmm_clean (struct virtual_memory_manager *vmm)
{
  fprintf (stdout, "\n\n");
  fprintf (stdout, "tlb hits:   %d\n", vmm->tlb_hit_count);
  fprintf (stdout, "tlb miss:   %d\n", vmm->tlb_miss_count);
  fprintf (stdout, "tlb hit ratio:   %f\n",
           vmm->tlb_hit_count / (float)(vmm->tlb_miss_count + vmm->tlb_miss_count));
  fprintf (stdout, "page found: %d\n", vmm->page_found_count);
  fprintf (stdout, "page fault: %d\n", vmm->page_fault_count);
  fprintf (stdout, "page fault ratio:   %f\n",
           vmm->page_fault_count / (float)(vmm->page_found_count + vmm->page_fault_count));

  if (vmm->log)
    {
      for (unsigned int i = 0; i < NUM_PAGES; i++)
	{
	  fprintf (vmm->log,
		   "%d -> {%d, %d%d%d%d%d%d%d%d}\n",
		   i,
		   vmm->page_table[i].frame_number,
                   vmm->page_table[i].flags & 1,
                   vmm->page_table[i].flags & (1 << 1),
                   vmm->page_table[i].flags & (1 << 2),
                   vmm->page_table[i].flags & (1 << 3),
                   vmm->page_table[i].flags & (1 << 4),
                   vmm->page_table[i].flags & (1 << 5),
                   vmm->page_table[i].flags & (1 << 6),
                   vmm->page_table[i].flags & (1 << 7));
	}
    }
  tlb_clean (&vmm->tlb);
  pm_clean (&vmm->pm);
}
