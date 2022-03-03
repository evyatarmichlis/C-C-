#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define USAGE_ERROR "Usage: the progrem need to get this arguments : <Seed>"\
"< Number of sentences to generate> <Path to file>"\
"Optional - <Number of words to read>"
#define OPEN_FILE_ERROR "Error: Cannot open file: %s\n"
#define ALLOCATE_ERROR "Allocation failure: cannot allocate memory"

#define MIN_ARGS  4
#define MAX_ARGS 5
#define TEN 10
#define TRUE 1
#define FALSE 0
#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_SENTENCE_LENGTH 1000


typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;
    int words_in_prob;
    int counter;// count the number the word appear
    int have_dot;//  flag of true or false


} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    int number_after_word;// counter of the word inside the prob list
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }

  link_list->size++;
  return 0;
}

/*************************************/
/**
 * search the word in the link list
 * @param link_list
 * @param word_struct
 * @return  true or false
 */
WordStruct *add_to_dict (LinkList *link_list, WordStruct *word)
{
  Node *current;
  current = link_list->first;
  while (current != NULL)
    { // search for the word in the dict
      if (strcmp (current->data->word, word->word) == 0)
        {
          free (word->word);
          free (word);
          current->data->counter++;
          return current->data;
        }
      current = current->next;
    }
  if (add (link_list, word))
    {
      printf (ALLOCATE_ERROR);
      exit (EXIT_FAILURE);
    } // if new word add to the end of the dict
  link_list->last->data->counter++;
  return link_list->last->data;

}

/**
 * check if the word end with dot
 * @param word
 * @return  true or false;
 */
int end_with_dot (WordStruct *word)
{
  char c = word->word[0];
  for (int i = 0; i < (int)strlen (word->word); ++i)
    {
      c = word->word[i];
    }
  if (c == '.')
    {
      return TRUE;
    }
  return FALSE;
}

/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number)
{

  return rand () % max_number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  int good_word = FALSE;
  Node *curr;
  while (good_word == FALSE) // random first word  only if the word doest
    // end with dot
    {
      int random = get_random_number (dictionary->size);
      curr = dictionary->first;
      int i = 0;
      while (i < random)
        {
          curr = curr->next;
          i++;
        }
      if (curr->data->have_dot == FALSE) // end the loop if have dot
        {
          good_word = TRUE;
        }
    }
  return curr->data;

}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  int random = get_random_number (word_struct_ptr->counter);
  int k = 0;
  int j = 0;
  while (j < word_struct_ptr->counter)
    { //iterate on the prob list and stop`
      if (k <= random
          && random < k + word_struct_ptr->prob_list[j].number_after_word)
        {
          return word_struct_ptr->prob_list[j].word_struct_ptr;
        }
      k = k + word_struct_ptr->prob_list[j].number_after_word;
      j++;
    }
  return word_struct_ptr->prob_list[word_struct_ptr->words_in_prob - 1]
      .word_struct_ptr;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  WordStruct *w = get_first_random_word (dictionary);
  int number_of_words = 0;

  while (number_of_words < MAX_WORDS_IN_SENTENCE_GENERATION)
    {
      printf ("%s ", w->word);

      w = get_next_random_word (w);
      number_of_words++;
      if (w->have_dot == 1)
        {// if the word end with dot stop the sentence
          printf ("%s", w->word);
          break;
        }

    }
  return number_of_words;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word,
                                  WordStruct *second_word)
{
  if (first_word->words_in_prob == 0)
    { // if the prob list is empty
      first_word->prob_list = malloc (sizeof (WordProbability));
      if (first_word->prob_list == NULL) exit(EXIT_FAILURE);
      first_word->words_in_prob = 1;
      first_word->prob_list[0].word_struct_ptr = second_word;
      first_word->prob_list[0].number_after_word = 1;
      return 1;
    }
  int i = 0;
  while (i < first_word->words_in_prob) // search for the word in the problist
    {
      if (strcmp (first_word->prob_list[i].word_struct_ptr->word,
                  second_word->word) == 0)
        {
          first_word->prob_list[i].number_after_word++;
          return 0;
        }
      i++;
    }
  first_word->words_in_prob++;
  WordProbability *temp;
  temp = realloc (first_word->prob_list,
                  (first_word->words_in_prob) * sizeof
                      (WordProbability));
  if (temp == NULL)
    {
      printf(ALLOCATE_ERROR);
      exit (EXIT_FAILURE);
    }
  first_word->prob_list = temp;
  first_word->prob_list[first_word->words_in_prob - 1].word_struct_ptr
      = second_word;
  first_word->prob_list[first_word->words_in_prob - 1].number_after_word = 1;
  return 1;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  char buf[MAX_SENTENCE_LENGTH];
  char *c;
  int i = 0;
  WordStruct *curr_word = NULL;
  WordStruct *prev_word = NULL;
  while (fgets (buf, MAX_SENTENCE_LENGTH, fp))//read the text to the buffer
    {
      c = strtok (buf, " \n\r");
      while (c != NULL && i != words_to_read)// read word from the buffer
        {
          curr_word = calloc (1, sizeof (WordStruct));
          curr_word->word = malloc (sizeof (char) * (strlen (c) + 1));
          if (curr_word == NULL || curr_word->word == NULL)
            {
              printf (ALLOCATE_ERROR);
              exit (EXIT_FAILURE);
            }
          strcpy (curr_word->word, c);
          if (end_with_dot (curr_word) == TRUE)
            { // mark all the words if they
              // have dots
              curr_word->have_dot = TRUE;
            }
          curr_word = add_to_dict (dictionary, curr_word);
          if (prev_word != NULL && prev_word->have_dot == FALSE)
            {
              if (add_word_to_probability_list (prev_word, curr_word) == -1)
                {
                  printf (ALLOCATE_ERROR);
                  exit (EXIT_FAILURE);
                }
            }
          i++;
          prev_word = curr_word;
          c = strtok (NULL, " \n\r");

        }
    }

}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *current, *prev;
  current = dictionary->first;
  while (current != NULL)
    {
      free (current->data->prob_list);
      current->data->prob_list = NULL;
      free (current->data->word);
      current->data->word = NULL;
      free (current->data);
      current->data = NULL;
      prev = current;
      current = current->next;
      free (prev);
      prev = NULL;
    }
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main (int argc, char *argv[])
{
  if (argc < MIN_ARGS || argc > MAX_ARGS)
    {
      printf (USAGE_ERROR);
      return EXIT_FAILURE;
    }
  int seed = (int) strtol (argv[1], NULL, TEN);
  int num_of_sentences = (int) strtol (argv[2], NULL, TEN);
  int num_of_words = -1;
  if (argc == MAX_ARGS)
    { num_of_words = (int) strtol (argv[4], NULL, TEN); }
  FILE *file = fopen (argv[3], "r");
  if (file == NULL)
    {
      printf (OPEN_FILE_ERROR, argv[3]);
      return EXIT_FAILURE;
    }
  LinkList dict = {NULL, NULL, 0};

  srand (seed);
  fill_dictionary (file, num_of_words, &dict);
  int i = 1;
  while (i <= num_of_sentences)
    {
      printf("%s%d%s","Tweet ",i ,": ");
      generate_sentence (&dict);
      printf ("\n");
      i++;
    }
  free_dictionary (&dict);
  fclose(file);
  return EXIT_SUCCESS;
}

