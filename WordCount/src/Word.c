#include <stdio.h>
#include <stdlib.h>
#include "Word.h"

word_t * new_word(char *word)
{
  word_t *wrd = malloc(sizeof(word_t));
  wrd->word = word;
  wrd->frequency = 0;
  return wrd;
}

void destroy_word(word_t *word)
{
  free(word->word);
  free(word);
}