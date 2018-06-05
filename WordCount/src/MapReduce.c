#include "MapReduce.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void count_lines(vector_line *array, char *path)
{
  char buffer[MAX_LINE];
  unsigned long start = 0;
  unsigned long offset = 0;
  FILE *fh;
  fh = fopen(path, "r");
  const char nline = '\n';
  while (fgets(buffer, MAX_LINE, fh) != NULL)
  {
    char *nl = strchr(buffer, nline);
    if (nl)
    {
      int index = nl - buffer;
      offset += (index + 1);
      line_t *line = new_line(path, start, offset);
      add_line(array, *line);
      start = offset; //start after new line
    }
  }
  fclose(fh);
}

vector_line *line_splitter(const char *dir_path)
{
  DIR *directory_handler;
  struct dirent *directory_struct;
  directory_handler = opendir(dir_path);
  if (directory_handler != NULL)
  {
    vector_line *array = new_line_vector(10);
    while ((directory_struct = readdir(directory_handler)))
    {
      if (directory_struct->d_name[0] != '.')
      {
        char *full_path = (char *)malloc((strlen(directory_struct->d_name) + strlen(dir_path) + 2) * sizeof(char));
        full_path[0] = '\0';
        strcat(full_path, dir_path);
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