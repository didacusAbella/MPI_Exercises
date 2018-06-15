#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MapReduce.h"

int contains_word(word_t *list, int size, char *text)
{
  for(int i = 0; i < size; i++)
  {
    if(strcmp(list[i].text, text) == 0)
    {
      return i;
    }
  }
  return -1;
}

void count_lines(task_t *task, char *path)
{
  char buffer[MAX_LINE_BUFFER];
  unsigned long start = 0;
  unsigned long offset = 0;
  FILE *fh;
  fh = fopen(path, "r");
  const char nline = '\n';
  const char eos = '\0';
  while (fgets(buffer, MAX_LINE_BUFFER, fh) != NULL)
  {
    char *nl = strchr(buffer, nline);
    int index;
    if(nl)
    {
      index = nl - buffer;
      offset += (index + 1);
    }
    else // last line not newline
    {
      char *es = strchr(buffer, eos);
      index = es - buffer;
      offset += index;
    }
    line_t *line;
    line_ctor(&line, path, start, offset);
    add_line(task, line);
    start = offset; // start after new line
  }
  fclose(fh);
}

void line_splitter(const char *dir_path, task_t **master_task)
{
  DIR *directory_handler;
  struct dirent *directory_struct;
  directory_handler = opendir(dir_path);
  if (directory_handler != NULL)
  {
    while ((directory_struct = readdir(directory_handler)))
    {
      if (directory_struct->d_name[0] != '.')
      {
        char *full_path = (char *)malloc((strlen(directory_struct->d_name) + strlen(dir_path) + 2) * sizeof(char));
        full_path[0] = '\0';
        strcat(full_path, dir_path);
        strcat(full_path, "/");
        strcat(full_path, directory_struct->d_name);
        count_lines(*master_task, full_path);
      }
    }
    closedir(directory_handler);
  }
  else
  {
    perror("Errore apertura della directory\n");
  }
}

void map(task_t **slave_task)
{
  FILE *fh = NULL;
  fh = fopen((*slave_task)->task_lines[0].file, "r");
  for (int i = 0; i < (*slave_task)->lines_used; i++)
  {
    if(i != 0 && strcmp((*slave_task)->task_lines[i].file, (*slave_task)->task_lines[i-1].file) != 0)
    {
      fclose(fh);
      fh = fopen((*slave_task)->task_lines[i].file, "r");
    }
    unsigned long s = (*slave_task)->task_lines[i].start, e = (*slave_task)->task_lines[i].end;
    fseek(fh, s, SEEK_SET);
    unsigned long delta = e - s;
    char bf[delta];
    bf[delta] = '\0';
    fread(bf, 1, delta, fh);
    char *token = strtok(bf, TOKENIZER);
    while (token != NULL)
    {
      word_t *wd;
      word_ctor(&wd, token);
      add_word(*slave_task, wd);
      token = strtok(NULL, TOKENIZER);
    }
  }
  fclose(fh);
}

void reduce(task_t **slave_task)
{
  int tmp_size = 0;
  word_t *compacted_array = (word_t *) calloc((*slave_task)->words_used, sizeof(word_t)); //worst_case
  for(int i = 0; i < (*slave_task)->words_used; i++)
  {
    int index = contains_word(compacted_array, (*slave_task)->words_used, (*slave_task)->task_words[i].text);
    if(index >= 0)
    {
      compacted_array[index].frequency += (*slave_task)->task_words[i].frequency;
    }
    else
    {
      word_t *compacted_word;
      word_ctor(&compacted_word,(*slave_task)->task_words[i].text);
      *(compacted_array + tmp_size) = *compacted_word;
      tmp_size++;
    }

  }
  if(tmp_size != (*slave_task)->words_used)
  {
    compacted_array = (word_t *) realloc(compacted_array, tmp_size * sizeof(word_t));
  }
  free((*slave_task)->task_words);
  (*slave_task)->words_used = tmp_size;
  (*slave_task)->word_size = tmp_size;
  (*slave_task)->task_words = compacted_array;
}