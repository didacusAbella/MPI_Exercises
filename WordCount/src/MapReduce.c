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

void line_splitter(const char *dir_path, vector_line *all_lines)
{
  DIR *directory_handler;
  struct dirent *directory_struct;
  directory_handler = opendir(dir_path);
  if (directory_handler != NULL)
  {
    new_line_vector(10, all_lines);
    while ((directory_struct = readdir(directory_handler)))
    {
      if (directory_struct->d_name[0] != '.')
      {
        char *full_path = (char *)malloc((strlen(directory_struct->d_name) + strlen(dir_path) + 2) * sizeof(char));
        full_path[0] = '\0';
        strcat(full_path, dir_path);
        strcat(full_path, "/");
        strcat(full_path, directory_struct->d_name);
        count_lines(all_lines, full_path);
      }
    }
    closedir(directory_handler);
  }
  else
  {
    perror("Errore apertura della directory\n");
  }
}
void map(vector_line *vector, vector_word *mvector)
{
  FILE *fh;
  char path[1024];
  char *token;
  new_word_vector(20, mvector);
  strcpy(path, vector->lines[0].file);
  fh = fopen(path, "r");
  for (int i = 0; i < vector->used; i++)
  {

    if (strcmp(path, vector->lines[i].file) != 0)
    {
      fclose(fh);
      memset(path,0,strlen(path));
      strcpy(path, vector->lines[i].file);
      fh = fopen(path, "r");
    }
    unsigned long s = (vector->lines + i)->start, e = (vector->lines + i)->end;
    fseek(fh, s, SEEK_SET);
    unsigned long delta = e - s;
    char *bf = (char *)malloc(delta * sizeof(char));
    *(bf + delta) = '\0';
    fread(bf, 1, delta, fh);
    token = strtok(bf, TOKENIZER);
    while(token != NULL)
    {
      if(strcmp(token, "\n") == 0)
      {
        continue;
      }
      else
      {
      word_t *wd = new_word(token);
      add_word(mvector, *wd);
      }
      token = strtok(NULL, TOKENIZER);
    }
  }
}

void reduce(vector_word *vector, vector_word *cvector)
{
  new_word_vector(10, cvector);
  for(int i = 0; i < vector->used; i++)
  {
    int index = contains_word(cvector, vector->words[i].word);
    if(index >= 0)
    {
      cvector->words[index].frequency += vector->words[i].frequency;
    }
    else
    {
      add_word(cvector, *new_word(vector->words[i].word));
    }
  }
}