/*
 * Name: Jared Campbell
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "String.h"

#define SIGNATURE (~0xdeadbeef)

/*
 * Helper macro to find the signature of a UTString
 * Treat this like a function that takes a UTString*
 * Accesses a uint32_t from where the signature should be located
 */
#define CHECK(s) (*(uint32_t*) ((s)->string + (s)->length + 1))

uint32_t string_size(const char *src);
/*
 * Checks if a UTString is valid or not.
 */
bool isOurs(UTString* s) {
    if (CHECK(s) == SIGNATURE) { return true; }
    else { return false; }
}

/* 
 * Allocate a utstring on the heap. 
 * Initialize the string correctly by copying src.
 * Return a pointer to the UTString.
 */
UTString* utstrdup(const char* src) {
	uint32_t str_size = string_size(src);

    UTString* s = (UTString*)malloc(sizeof(UTString));


    s->length = str_size;
    s->capacity = str_size;
    s->string = (char*)calloc(str_size+1+sizeof(SIGNATURE), sizeof(char));

    for(int i=0; src[i] != '\0'; i++) {
        s->string[i] = src[i];
    }
    s->string[s->length] = '\0';
    CHECK(s) = SIGNATURE;
    return s;
}

/* 
 * Returns the length of s.
 *  s must be a valid UTString.
 */
uint32_t utstrlen(const UTString* s) {
    assert(CHECK(s) == SIGNATURE);
	return s->length;
}

/*
 * Append the string suffix to the UTString s.
 *  s must be a valid UTString.
 * Do not allocate any additional storage: only append as many characters
 *  as will actually fit in s. 
 * Update the length of s.
 * Return s with the above changes. */
UTString* utstrcat(UTString* s, const char* suffix) {
    assert(isOurs(s));
    int more = s->capacity - s->length;
    int slen = string_size(suffix);

    int j = 0;
    while(suffix[j] != '\0' && s->length < s->capacity) {
        char x = suffix[j];
        int l = s->length;
        s->string[l] = x;
        s->string[l+1] = '\0';
        s->length++;
        j++;
    }
    //s->length = string_size(s->string);
    //s->string[s->length+1] = '\0';
    CHECK(s) = SIGNATURE;
    //s->string[s->length+2] = SIGNATURE;

    return s;
}

/* 
 * Copy src into dst.
 *  dst must be a valid UTString. 
 *  src is an ordinary string.
 * Do not allocate any additional storage: only copy as many characters
 *  as will actually fit in dst.
 * Update the length of dst. 
 * Return dst with the above changes.
 */
UTString* utstrcpy(UTString* dst, const char* src) {
    assert(isOurs(dst));
    for(uint32_t i = 0; i < dst->capacity && src[i] != '\0'; i++) {
        dst->string[i] = src[i];
        dst->length++;
    }
    dst->string[dst->length] = '\0';
    CHECK(dst) = SIGNATURE;
	return dst;
}

/*
 *
 * Free all memory associated with the given UTString. 
 */
void utstrfree(UTString* self) {
    assert(isOurs(self));
    if(self->string != NULL) free(self->string);
    if(self != NULL) free(self);
}

/* 
 * Make s have at least as much capacity as new_capacity.
 *  s must be a valid UTString.
 * If s has enough capacity already, do nothing.
 * If s does not have enough capacity, then allocate enough capacity, 
 *  copy the old string and the null terminator, free the old string,
 *  and update all relevant metadata. 
 * Return s with the above changes.
 */
UTString* utstrrealloc(UTString* s, uint32_t new_capacity) {
    assert(isOurs(s));
    if(s->capacity >= new_capacity) return s;
    else {
        UTString *temp = (UTString *) malloc(sizeof(UTString));
        temp->capacity = new_capacity;
        temp->length = 0;
        temp->string = (char *)calloc(new_capacity+1+sizeof(SIGNATURE), sizeof(char));
        CHECK(temp) = SIGNATURE;
        utstrcpy(temp, s->string);
        utstrfree(s);
        return temp;
    }
}

uint32_t string_size(const char *src) {
    uint32_t str_size = 0;
    for(int i = 0; src[i] != '\0'; i++) {
        str_size++;
    }
    return str_size;
}