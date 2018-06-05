#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MapReduce.h"
#include "Line.h"
#define LOG_ON 1
#include "Logger.h"
#define CHUNK_SIZE 1024


void count_lines(line_array *array, char *path){
  char buffer[CHUNK_SIZE];
  unsigned long start = 0;
  unsigned long offset = 0;
  FILE *fh;
  fh = fopen(path, "r");
  const char nline = '\n';
  while(fgets(buffer, CHUNK_SIZE, fh)  != NULL)
  {
    char *nl = strchr(buffer, nline);
    if(nl)
    {
      int index = nl - buffer;
      offset += (index + 1);
      line_t *line = new_line(start, offset, path);
      push_line(array, *line);
      start = offset; //start after new line
    }
  }
  fclose(fh);
}

line_array * line_splitter(const char *directory_path, int slots)
{
  DIR *directory_handler;
  struct dirent *directory_struct;
  directory_handler = opendir(directory_path);
  if(directory_handler != NULL)
  {
    line_array *array = new_line_array(10);
    while((directory_struct = readdir(directory_handler)))
    {
      if(directory_struct->d_name[0] != '.')
      {
        char *full_path = (char *) malloc((strlen(directory_struct->d_name) + strlen(directory_path) + 2) * sizeof(char));
        full_path[0] = '\0';
        strcat(full_path, directory_path);
        strcat(full_path, "/");
        strcat(full_path, directory_struct->d_name);
        count_lines(array, full_path);
      }
    }
    closedir(directory_handler);
    return array;
  }
  else
  {
    return NULL;
  }
}