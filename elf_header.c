/* elf_header.c
 * Author: Spyrou Michalis
 * University of Thessaly 2012 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fat_header.h"

void extract_elf(long int li1,long int li2, char *name);
void run_elf(unsigned int num);

/* File descriptor */
FILE *fd;

/* Takes as argument the ELF binary 
 * number, extractes it and executes it */
void run_elf(unsigned int num) {

  long int li1,li2;
  char *name = "demo";

  /* Get the start and the end of the binary */
  li1 = strtol(bins[num].offset,NULL,0);
  li2 = strtol(bins[num-1].offset,NULL,0);

  /* Call extract_elf to extract the binary */
  extract_elf(li1,li2,name);

  /* Make it executable - non POSIX */
  chmod(name,S_IRWXU);

  /* Execute the binary */
  execv(name,NULL);
  perror("execve");
}

/* Extracts the selected binary and returns the 
 * name of the extracted file                  */
void extract_elf(long int li1,long int li2, char *name) {

  FILE *temp;
  unsigned char *buf;

  /* Size of the ELF binary */
  size_t size = li2-li1;

  /* Create ELF file */
  if((temp = fopen(name,"a"))<0){
    perror("fseek");
    exit(EXIT_FAILURE);
  }

  buf= (unsigned char *)malloc(size);

  /* Seek to the start and read the size */
  fseek(fd, li1, SEEK_SET);
  fread(buf,1,size,fd);
  fwrite(buf,1,size,temp);

  /* Free and close */
  free(buf);
  fclose(temp);
  
}

/* ELF header main function. Loads all the 
 * info from the header and then calls run_elf 
 * function to execute the selected ELF       */
int main (int argc, char **argv) {

  size_t size,res;
  int ELF_NUM;
  char *buff = malloc(10);
  
  /* Open self */
  fd = fopen(argv[0],"rb");

  /* Search for string FATBIN_END. */
  if ((fseek(fd,-10,SEEK_END))<0) {
    perror("fseek");
    exit(EXIT_FAILURE);
  }
  
  /* Read */
  
  (void) fread (buff,1,10,fd);

  /* Check if we have a valid fatbin file */
  if (strcmp(buff,"FATBIN_END")!=0)  {
    printf("Error! Not valid fatbin file.\n Exiting..\n");
    fclose(fd);
    exit(EXIT_FAILURE);
  }

  /* Valid file! */
  printf("Fatbin file detected! Now trying to access info..\n\n");
  
  /* Access and load info */
  int i=0;
  while(1) {
    free(buff);
    buff = (char *)malloc(10); 

    /* Got to the correct location */
    if ((fseek(fd,(-10-10*(i+1)),SEEK_END))<0) {
     perror("fseek");
     exit(EXIT_FAILURE);
    }

    /* Read data */
    (void)fread(buff,1,10,fd);

    /* Check if we are at the start */
    if(strcmp(buff,"FATBIN_ST.")==0)
      break;

    /* Get offset and file name */
    if (i%2){    
      strcpy(bins[i/2].name,buff); 
      printf("Type  : \t%s\n\n",bins[i/2].name); 
    }
    else{
      printf("Binary No%d\n",i/2+1); 
      strcpy(bins[i/2].offset,buff); 
      printf("Offset: \t%s\n",bins[i/2].offset);  
    }

    i++;
  } 

  printf("Select the file you want to execute: ");
  scanf("%d", &ELF_NUM);

  /* Check the input: You cannot run the fatbin header */
  while(ELF_NUM<1 || ELF_NUM>(i/2 - 1)){
    printf("Not valid binary\n");
    scanf("%d", &ELF_NUM);
  }

  /* TODO: fork() ? */
  /* Run the selected ELF */
  run_elf(ELF_NUM);

  /* Close file */
  fclose(fd);

}
