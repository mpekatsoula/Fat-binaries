/* fat_header.h
 * Author: Spyrou Michalis 
 * University of Thessaly 2012 */

#ifndef _FAT_HEADER_H
#define _FAT_HEADER_H

/* Define labels */
/*  TODO: labels */
#define FATBIN_HEADER "FAT HEADER"
#define NVIDIA_x86 "NVIDIA x86"
#define ATI_x86    "ATI_x86ELF"

#define MAX_BINS 256

void combine(char *output,char **binaries, int bin_num);

typedef struct {

  char offset[11];  /* Offest of the binary, -char for better writting to file */
  char name[11];    /* Name is 10 bytes +1 \0 */

} binary;

binary bins[MAX_BINS];

#endif
