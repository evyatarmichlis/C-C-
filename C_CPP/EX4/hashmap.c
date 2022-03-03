///
// Created by evyat on 15/05/2021.
//
#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1
#include "hashmap.h"
/**
 *  this function is the hash function of this hash map
 * @param hash_func the type we use
 * @param capacity the size of the hashmap
 * @param key_t the key we need to hash
 * @return the location of the key
 */
size_t hashing( hash_func hash_func, size_t capacity , const_keyT key_t){
  size_t val = hash_func(key_t);
  return val&(capacity-1);
}
/**
 * helper function for the hashmap at, if we need to search bt the key
 * @param p1 pair
 * @param p2 paur
 * @return if they are equal or not
 */
int key_pair_cmp (const void* p1, const void* p2)
{
  if (!p1 || !p2)
    {
      return FALSE;
    }
  const pair *pair1 = (const pair *) p1;
  const const_keyT *key_t = (const const_keyT *) p2;
  int key_cmp = pair1->key_cmp (pair1->key, key_t);
  return key_cmp ;
}
void free_bucks(vector *** vec , size_t ind ){
//  if (vec==NULL|| *vec==NULL||ind < 0) return;
  if (vec==NULL|| *vec==NULL) return;
  for (size_t i = 0; i < ind; ++i)
    {
      vector_free(&(*vec)[i]);
    }
  free(*vec);
  (*vec) = NULL;
}
/**
 * create new buckets for the hash map
 * @param size the new size of the buckets we need
 * @return new buckets with new size
 */
vector ** new_buckets(size_t size){
  if (size <0 )return NULL;
  vector** new_b = malloc(sizeof(void*)*size);
  if (new_b == NULL)return NULL;
  for (size_t i = 0; i < size; ++i)
    {
      new_b[i] = vector_alloc(pair_copy,key_pair_cmp,pair_free);
      if ( new_b[i]==NULL){
          free_bucks(&new_b,i);
          free(new_b);
          return NULL;
        }
    }
  return new_b;
}
/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func){
  hashmap * hash = malloc(sizeof (*hash));
  if (hash == NULL) return NULL;
  if (func ==NULL) return NULL;
  hash->buckets = new_buckets(HASH_MAP_INITIAL_CAP);
  if (  hash->buckets == NULL) return NULL;
  hash->capacity = HASH_MAP_INITIAL_CAP;
  hash->size = 0;
  hash->hash_func = func;
  return hash;
}
/**
 * resize the hashmap and rehash all of the buckets to the new hashmap
 * @param hash old hashmap
 * @param flag 1 to increase 0 to decrease
 * @return
 */
int resize_hash( hashmap *hash , int flag ){
  if (hash == NULL)return 0;
  double new_cap;
  if(flag == 1){
      new_cap = (double )hash->capacity*(double )HASH_MAP_GROWTH_FACTOR;
    }
  if (flag == 0){
      new_cap = (double )hash->capacity/(double )HASH_MAP_GROWTH_FACTOR;
    }
  vector** new_buc = new_buckets(new_cap);
  int old_cap = hash->capacity;
  if (new_buc==NULL) new_cap = old_cap;
  for (size_t i = 0; i < hash->capacity; ++i)
    {
      for (size_t j = 0; j < hash->buckets[i]->size; ++j)
        {
          pair * p =(pair*) vector_at(hash->buckets[i],j);
          if (p == NULL) return FALSE;
          size_t location = hashing( hash->hash_func,new_cap, p->key);
          if (vector_push_back(new_buc[location],p) == 0) {
              return TRUE;
            }
        }
    }
  vector** tmp =  hash->buckets;
  hash->capacity = new_cap;
  hash->buckets = new_buc;
  free_bucks(&tmp,old_cap);
  return TRUE;
}
/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map){
  if (p_hash_map == NULL|| *p_hash_map ==NULL) return;
  free_bucks(&((*p_hash_map)->buckets),(*p_hash_map)->capacity);
  free(*p_hash_map);
  (*p_hash_map) = NULL;
}
/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair){
  if (hash_map == NULL|| in_pair == NULL) return FALSE;
  if (hashmap_at(hash_map,in_pair->key)!=NULL) return FALSE;
  double load = hashmap_get_load_factor(hash_map);
  size_t location = hashing( hash_map->hash_func,hash_map->capacity,
                             in_pair->key);
  if (load + (double )1/(double )hash_map->capacity > HASH_MAP_MAX_LOAD_FACTOR)
    {
      if (resize_hash (hash_map, 1) == TRUE)
        location = hashing( hash_map->hash_func,hash_map->capacity,
                            in_pair->key);
      else{
          return FALSE;
      }
    }


  if(vector_push_back(hash_map->buckets[location],in_pair)==FALSE)return FALSE;
  hash_map->size ++;
  return TRUE;
}
/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key){
  if (hash_map == NULL || key == NULL) {
      return NULL;
    }
  size_t vec_ind = hashing(hash_map->hash_func, hash_map->capacity,key );
  vector* vec = hash_map->buckets[vec_ind];
  if (vec == NULL) return NULL;
  int elem_ind = vector_find(vec, key);
  if (elem_ind == NOT_FOUND) {return NULL;}
  pair* p = vector_at(vec, elem_ind);
  if (p == NULL) {
      return NULL;
    }
  return p->value;
}
/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not in map,
 * considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key){
  if (hash_map == NULL || key == NULL) return FALSE;
  valueT value = hashmap_at(hash_map,key);
  if (value == NULL) return FALSE;
  size_t location = hashing( hash_map->hash_func,hash_map->capacity, key);
  double load = hashmap_get_load_factor(hash_map);
  if (load - (double )1/(double )hash_map->capacity <
      HASH_MAP_MIN_LOAD_FACTOR){
      resize_hash(hash_map,0);
    }

  int ind = vector_find(hash_map->buckets[location],key);
  if (ind == -1) return  FALSE;
  vector_erase(hash_map->buckets[location],ind);
  if (hash_map->buckets[location]->size ==FALSE)
    {
      hash_map->size--;
    }
  return TRUE;
}
/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map){
  if (hash_map == NULL) return NOT_FOUND;
  return (double )hash_map->size/(double )hash_map->capacity;
}
/**
 * This function receives a hashmap and 2 functions, the first checks a condition on the keys,
 * and the seconds apply some modification on the values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func){
  if (hash_map == NULL || keyT_func== NULL || valT_func
                                              ==NULL||hash_map->buckets == NULL){
      return NOT_FOUND;
    }
  int counter=0;
  pair* p = NULL;
  for (size_t i = 0; i < hash_map->capacity; ++i)
    for (size_t j = 0; j < hash_map->buckets[i]->size ; ++j)
      {
        p = (pair*) vector_at(hash_map->buckets[i],j);
        if (keyT_func(p->key)){
            valT_func((p->value));
            counter++;
          }
      }
  return counter;
}
