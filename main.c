/* main.c
 * Author: Spyrou Michalis 
 * University of Thessaly 2012 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fat_header.h"

/* Initializes and starts combine function */
int main(int argc, char **argv) {

  if(argc<3) {
    printf("Usage: %s <output> <elf_header> <arg1> ... <argN>",argv[0]);
    return EXIT_FAILURE;
  }  
    
  int i;

  /* Initialize bins struct */
  for(i=0;i<256;i++)
    strcpy(bins[i].offset,"0x00000000");

  /* Combine the ELF binaries */
  combine(argv[1],&argv[2],argc-2);

  return EXIT_SUCCESS;
}
