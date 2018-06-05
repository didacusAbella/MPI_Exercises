#ifndef WORD_H
#define WORD_H
typedef struct word_t
{
  char *word;
  int frequency;
}word_t;

word_t * new_word(char *word);
void destroy_word(word_t *word);
#endif /* WORD_H */