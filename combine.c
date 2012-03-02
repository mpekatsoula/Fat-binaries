/* combine.c
 * Author: Spyrou Michalis 
 * University of Thessaly 2012 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fat_header.h"

/* This function combines the ELF binaries and writes  
 * the header in the end of the file                  */
void combine(char *output,char **binaries, int bin_num){

  int i,j;
  FILE *fd2,*fd;
  unsigned char *buf;
  char temp[8];
  size_t size,total_off=0;

  /* Open the output file for writing */
  if((fd = fopen(output,"a+"))<0) {
    printf("Error. Opening file failed\n");
    exit(1);
  }

  /* Get information */
  for (i=0;i<bin_num;i++) {

    /* Open next binary for reading */
    if((fd2 = fopen(binaries[i],"rb"))<0) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }
    
    /* Find size of the file */
    fseek(fd2, 0, SEEK_END);
    size = ftell(fd2);
    rewind(fd2); 

    /* Load file to memory for copying */
    buf= (unsigned char *)malloc(size);
    fread(buf,1,size,fd2);

    /* Write binary to output */
    fwrite(buf,1,size,fd);

    /* Compute the total offset */
    total_off += size;

    /* TODO: Labels (?) */
    /* Name Label (8 bytes) */
    if (i==0)
      strcpy(bins[i].name,FATBIN_HEADER);
    else 
      strcpy(bins[i].name,NVIDIA_x86);

    /* Write offset */
    j=0;

    /* Offset in hex */
    sprintf(temp,"%x",total_off);

    int t_len = strlen(temp);

    /* We want offset to the end.. */
    while(j<t_len){
      bins[i].offset[9-j] = temp[t_len-j-1];
      j++;
    }

    free (buf);              // free file buffer
    fclose(fd2);             // close file
}

  /*****************************************/
  /********** Write fatbin Header **********/
  /*****************************************/

  /* Starting fatbin string */
  fprintf(fd, "FATBIN_ST.");

  /* Write name and offsets in the end of file */
  for (i=0;i<bin_num;i++) {
    fprintf(fd, "%s", bins[i].name);
    fprintf(fd, "%s", bins[i].offset);
  }

  /* Ending fatbin string */
  fprintf(fd, "FATBIN_END");

  /* Close file */
  fclose(fd);

  /* Make it executable - non POSIX */
  chmod(output,S_IRWXU);

}
