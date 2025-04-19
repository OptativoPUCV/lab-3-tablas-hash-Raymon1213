#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long p = hash(key, map->capacity);
    if (map->buckets[p] == NULL || map->buckets[p]->key == NULL){
        if (map->buckets[p] != NULL && is_equal(map->buckets[p]->key, key)) return;
        Pair* nuevo = malloc(sizeof(Pair));
        nuevo -> key = key;
        nuevo -> value = value;
        map->current = p;
        map->buckets[p] = nuevo;
        map->size += 1;
        return;
    }
    else{
        for(long i = p+1; i < map->capacity; i++){ 
            if(map->buckets[i] == NULL || map->buckets[i]->key == NULL){
                if (map->buckets[p] != NULL && is_equal(map->buckets[p]->key, key)) return;
                Pair* nuevo = malloc(sizeof(Pair));
                nuevo -> key = key;
                nuevo -> value = value;
                map->buckets[i] = nuevo;
                map->current = i;
                map->size += 1;
                return;
            }
        }
        return;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    long old_capacity = map->capacity;
    Pair** old_array = map->buckets;

    map->capacity = 2 * old_capacity;
    map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
    map->size = 0;

    for (long i = 0; i < old_capacity; i++){
      if (old_array[i] != NULL){
        insertMap(map, old_array[i]->key, old_array[i]->value);
        free(old_array[i]);
      }
    }
    free(old_array);
}


HashMap * createMap(long capacity) {
    HashMap* map = malloc(sizeof(HashMap));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;

    map->buckets = malloc(capacity * sizeof(Pair*));

    for (int i = 0; i < capacity; i++){
        map->buckets[i] = NULL;
    }
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    long p = hash(key, map->capacity);
    if (map->buckets[p] == NULL) return;
    else {
        if ((is_equal(map->buckets[p]->key,key)) == 0){
            map->buckets[p]->key = NULL;
            map->size -= 1;
            for(int i = p+1; i < map->capacity; i++){
                if ( map->buckets[i] == NULL) return;
                else if ((is_equal(map->buckets[i]->key,key))){
                    map->buckets[i]->key = NULL;
                    map->size -= 1;
                }
            }
        }
        else{
            return;
        }
    }
}

Pair * searchMap(HashMap * map,  char * key) {
    long p = hash(key, map->capacity);
    if (map->buckets[p] == NULL){
        return NULL;
    }
    else{ 
        if((is_equal(map->buckets[p]->key,key)) == 0){
            for(int i = p+1; i < map->capacity; i++){
                if ( map->buckets[i] == NULL){
                    return NULL;
                }
                else if (is_equal(map->buckets[i]->key,key)){
                    return map->buckets[i];
                }
            }
        }
        else{
            return map->buckets[p];
        }
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    map->current = 0;
    for (int i = 0; i < map->capacity; i++){
        if(map->buckets[i]->key != NULL){
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for(int i = (map->current)+1; i < map->capacity; i++){
        if(map->buckets[i]->key != NULL){
            return map->buckets[i];
        }
    }
    return NULL;
}
