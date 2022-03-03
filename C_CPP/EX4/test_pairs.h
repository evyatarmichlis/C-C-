
#ifndef EX4_TEST_PAIRS_H
#define EX4_TEST_PAIRS_H


/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  if (value==NULL)
    return NULL;
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}
void *int_key_cpy (const_keyT key)
{
  if (key==NULL)
    return NULL;
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) key);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

int int_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(int *) key_1 == *(int *) key_2;
}
/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT* key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

void int_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}
/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem)
{
  char c = *((char *) elem);
  return (c > 47 && c < 58);
}
/**
 * @param elem  pointer to int
 * @return  1 if the int is even
 */
int is_even (const_keyT elem){
  int i = *((int *) elem);
  return ( i % 2 == 0);
}


/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}
#endif //EX4_TEST_PAIRS_H