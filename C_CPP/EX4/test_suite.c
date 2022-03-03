
#include "test_suite.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "hash_funcs.h"
#include "test_pairs.h"
#define  SUCCESS 1
#define  FAIL  0
#define  ERROR -1


/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert(void){

  int threshold = HASH_MAP_INITIAL_CAP * HASH_MAP_MAX_LOAD_FACTOR;


  hashmap *hash = hashmap_alloc(hash_char);
  if (hash == NULL)exit(EXIT_FAILURE);
  hashmap *int_hash = hashmap_alloc(hash_int);
  if (int_hash == NULL)exit(EXIT_FAILURE);
  assert(hash->size == 0);//check if the alloc is ok
  int i = 0;
  char c = 'a';
  int  n = 50;
  int  m = 100;
  pair *p;
  pair *int_int;
  assert(hashmap_insert(hash,NULL)== FAIL);
  for (; i <= threshold; i++,c++,n++,m++)
    {
       p = pair_alloc(&c,& i, char_key_cpy,
                           int_value_cpy,
                           char_key_cmp,
                           int_value_cmp, char_key_free, int_value_free);
       int_int = pair_alloc(&n,&m,int_key_cpy,int_value_cpy,int_value_cmp,
                            int_key_cmp,
                            int_key_free,int_value_free);

       assert(hashmap_insert(int_hash,int_int)==SUCCESS);
       pair_free((void**) &int_int );
       assert(hashmap_insert(hash,p)== SUCCESS);
       pair_free((void**) &p );
    }//every element enter to different vector so the hash h
    c--;
    p = pair_alloc(&c,& i, char_key_cpy,
                   int_value_cpy,
                   char_key_cmp,
                   int_value_cmp, char_key_free, int_value_free);
  assert(hashmap_insert(hash,p)== FAIL); // try to enter a pair we have in
  // the hash
  pair_free((void**)&p);
    assert(hash->size ==(size_t) threshold+1);
   assert(int_hash->size == (size_t)threshold+1);
  int x = 200;
  char y = '$';
  pair* p1= pair_alloc(&y,& x, char_key_cpy,
                       int_value_cpy,
                       char_key_cmp,
                       int_value_cmp, char_key_free, int_value_free);

    assert(hashmap_insert(hash,p1)== SUCCESS);// after this insert the the
//     hash need to resize
    pair_free((void **) &p1);
    assert( hash->capacity ==HASH_MAP_INITIAL_CAP*HASH_MAP_GROWTH_FACTOR);
    hashmap_free(&hash);
    hashmap_free(&int_hash);

}
/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void){

  assert(hashmap_at(NULL,NULL)==FAIL);// the hash map dont exist
  hashmap *hash = hashmap_alloc(hash_char);
  hashmap *int_hash = hashmap_alloc(hash_int);// check hashmap with ints
  if (int_hash == NULL|| hash == NULL)exit(EXIT_FAILURE);
  int i = 0;
  char c = 'a';
  pair *p  ;
  //create another pairs for the ints
  int  n = 50;
  int  m = 100;
  pair *int_int;
  int threshold = HASH_MAP_INITIAL_CAP * HASH_MAP_MAX_LOAD_FACTOR;
  assert(hashmap_at(hash,NULL) == NULL);//the key is NULL

  for (; i < threshold; i++,c++,n++,m++)//fill the hash map with pairs
    {
      p = pair_alloc (&c, &i, char_key_cpy,
                      int_value_cpy,
                      char_key_cmp,
                      int_value_cmp, char_key_free, int_value_free);
      int_int = pair_alloc(&n,&m,int_key_cpy,int_value_cpy,int_value_cmp,
                           int_key_cmp,
                           int_key_free,int_value_free);
      hashmap_insert (hash, p);
      hashmap_insert(int_hash,int_int);
      pair_free((void**)&p);
      pair_free((void**)&int_int);

    }
    c-- , i--,n = 50,m = 100;
  pair* p_check = pair_alloc(&c, &i, char_key_cpy,
      int_value_cpy,
      char_key_cmp,
      int_value_cmp, char_key_free, int_value_free);
  pair* p_check2 = pair_alloc(&n,&m,int_key_cpy,int_value_cpy,int_value_cmp,
                              int_key_cmp,
                              int_key_free,int_value_free);
  //check the if the hashmaps insert the the key and can get the values
    assert(*(int*)hashmap_at(hash,p_check->key) == *(int*) p_check->value);
  assert(*(int*)hashmap_at(int_hash,p_check2->key) == *(int*) p_check2->value);
  pair_free((void**)&p_check);
  pair_free((void**)&p_check2);
  char x = 'z';
  int k = 17;
  p = pair_alloc (&x, &k, char_key_cpy,
                  int_value_cpy,
                  char_key_cmp,
                  int_value_cmp, char_key_free, int_value_free);
  assert(hashmap_at(hash,p->key)== NULL);  // the pair dont in the hash map
  assert(hashmap_at(int_hash,p->key)==NULL);
  hashmap_insert(hash,p);
  assert(*(int*) hashmap_at(hash,p->key)==*(int*)  p->value);//check if the
  // pair is in the hash after resize
  pair_free((void**)&p);
  hashmap_free(&hash);
  hashmap_free(&int_hash);

}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void){
  int i = 0;
  char c = 'a';
  int threshold = HASH_MAP_INITIAL_CAP * HASH_MAP_MIN_LOAD_FACTOR +1;
  hashmap *hash = hashmap_alloc(hash_char);
  pair* p;
  assert(hashmap_erase(NULL,NULL)== FAIL); // check fail if the input is wrong
  assert(hashmap_erase(hash,NULL)== FAIL);
  for (; i < threshold; i++,c++)
    {
      p = pair_alloc(&c,& i, char_key_cpy,
                     int_value_cpy,
                     char_key_cmp,
                     int_value_cmp, char_key_free, int_value_free);

      hashmap_insert(hash,p);
      pair_free((void**)&p);
    }
  char key = 'a';
  assert(hashmap_erase(hash,&key)== SUCCESS);// check if the erase works
  // on this key he is in the hashmap
  assert(hash->size == (size_t)(threshold -1));
  assert(hashmap_erase(hash,&key)== FAIL);
  assert( hash->capacity ==HASH_MAP_INITIAL_CAP);/// TODO change back
  char key2 = 'b';
  assert(hashmap_erase(hash,&key2)== SUCCESS);//
  assert( hash->capacity ==HASH_MAP_INITIAL_CAP/HASH_MAP_GROWTH_FACTOR);//
  // after resize.
  hashmap_free(&hash);

}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor(void)
{
  assert(hashmap_get_load_factor (NULL) == -1); // check if the load factor
  // get null hash map
  hashmap *hash = hashmap_alloc (hash_char);
  if (hash == NULL )return;
  assert(hashmap_get_load_factor (hash) == FAIL);// the hash map is empty
  pair *p;
  int threshold = HASH_MAP_INITIAL_CAP * HASH_MAP_MAX_LOAD_FACTOR;
  int i = 0;
  char c = 'a';
  for (; i < threshold; i++, c++)//fill the hash map with pairs
    {
      p = pair_alloc (&c, &i, char_key_cpy,
                      int_value_cpy,
                      char_key_cmp,
                      int_value_cmp, char_key_free, int_value_free);
      hashmap_insert (hash, p);//enter first pair
      assert(hashmap_get_load_factor (hash)
             == ((double) (i+1) / (double) HASH_MAP_INITIAL_CAP));
      pair_free((void**)&p);
    }
    hashmap_free(&hash);


}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if()
{
  assert(hashmap_apply_if (NULL, is_digit, double_value) == -1);// Null hashmap
  hashmap *hash = hashmap_alloc (hash_char);
  hashmap *int_hash = hashmap_alloc(hash_int);// check hashmap with ints
  if (hash== NULL || int_hash == NULL ) exit(EXIT_FAILURE);
  assert(hashmap_apply_if (hash, NULL, double_value) == ERROR);//NUll functions
  assert(hashmap_apply_if (hash, is_digit, NULL) == ERROR);
  assert(hashmap_apply_if (hash, is_digit, double_value) == FAIL);// the hash map
  // is empty.

  int i = 1;
  char c = '1';
  int  n = 50;
  int  m = 100;
  pair *int_int;
  pair *p;
  for (; i < 7; i++, c++,m++,n++)//fill the hash map with pairs
    {
      p = pair_alloc (&c, &i, char_key_cpy,
                      int_value_cpy,
                      char_key_cmp,
                      int_value_cmp, char_key_free, int_value_free);
      int_int = pair_alloc(&n,&m,int_key_cpy,int_value_cpy,int_value_cmp,
                           int_key_cmp,
                           int_key_free,int_value_free);
      hashmap_insert(int_hash,int_int);
      hashmap_insert (hash, p);
      pair_free((void**)&p);
      pair_free((void**)&int_int);
    }
  assert(hashmap_apply_if (hash, is_digit, double_value) == 6); // do the
  assert(hashmap_apply_if(int_hash,is_even,double_value) == 3 );//check
  // another function and another hashtable
  // action on all of the elem.
  int j = 7 , h = 13;
  char x = '&', v = 100;
  pair *p2 = pair_alloc (&j, &x, char_key_cpy,
                         int_value_cpy,
                         char_key_cmp,
                         int_value_cmp, char_key_free, int_value_free);
  int_int = pair_alloc(&h,&v,int_key_cpy,int_value_cpy,int_value_cmp,
                       int_key_cmp,
                       int_key_free,int_value_free);
  hashmap_insert(int_hash,int_int);

  hashmap_insert (hash, p2);//enter first pair

  pair_free((void**)&int_int);
  pair_free((void**)&p2);

  assert(hashmap_apply_if (hash, is_digit, double_value) == 6); // enter a non
  assert(hashmap_apply_if(int_hash,is_even,double_value) == 3);
  // digit char to the hashmap doesnt change
  hashmap_free(&hash);
  hashmap_free(&int_hash);
}

