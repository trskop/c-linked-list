/* Copyright (c) 2014, Peter Trško <peter.trsko@gmail.com>
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 * 
 *     * Neither the name of Peter Trško nor the names of other
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LINKED_LIST_H_111834419021372736397274666053784489481
#define LINKED_LIST_H_111834419021372736397274666053784489481

#include <assert.h>
#include <stddef.h>     /* size_t */
#include <stdbool.h>
#include <string.h>     /* memset() */

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
/* Standards C99 and C11 understand "inline" keyword as well as C++. */
#define INLINE static inline
#else
#define INLINE static
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* This is a doubly linked list so we have pointer to next and also previous
 * entries.
 */
typedef struct Linked_list_entry_s
{
    struct Linked_list_entry_s *next;
    struct Linked_list_entry_s *previous;

    /* Data associated with specific entry. This implementation of linked list
     * data structure doesn't do anything with this field.
     */
    void *data;
} Linked_list_entry;

typedef struct
{
    /* First entry of the linked list or NULL if empty.
     */
    Linked_list_entry *first;

    /* Last entry of the linked list or NULL if empty.
     */
    Linked_list_entry *last;

    /* Length of the linked list.
     */
    size_t length;
} Linked_list;

typedef bool(*Linked_list_entry_predicate)(Linked_list_entry *);

enum Linked_list_start
{
    /* Get first entry or start searching from the beginning of the list. See
     * specific function documentation for details.
     */
    LINKED_LIST_BEGINNING = 1,

    /* Get last entry or start searching from the end of the list. See specific
     * function documentation for details.
     */
    LINKED_LIST_END = 2
};

/* Initialize static or stack allocated Linked_list structure.
 *
 * Usage example:
 *
 *   Linked_list linked_list = LINKED_LIST_STATIC_INIT;
 *
 *   // ...
 *   linked_list_append(&linked_list, some_entry);
 *   // ...
 */
#define LINKED_LIST_STATIC_INIT \
    { .first = (Linked_list_entry *)0   \
    , .last = (Linked_list_entry *)0    \
    , .length = 0                       \
    }

/* Initialize static or stack allocated Linked_list_entry structure.
 *
 * Usage example:
 *
 *   Linked_list linked_list = LINKED_LIST_STATIC_INIT;
 *   Linked_list some_entry = LINKED_LIST_ENTRY_STATIC_INIT;
 *
 *   // ...
 *   linked_list_append(&linked_list, &some_entry);
 *   // ...
 */
#define LINKED_LIST_ENTRY_STATIC_INIT       \
    { .next = (Linked_list_entry *)0        \
    , .previous = (Linked_list_entry *)0    \
    , .data = (void *)0                     \
    }

/* Initialize Linked_list structure.
 *
 * Usage example:
 *
 *    // ...
 *    Linked_list *linked_list = malloc(sizeof(Linked_list));
 *
 *    linked_list_init(linked_list);
 *    // ...
 */
INLINE Linked_list *linked_list_init(Linked_list *list)
{
    assert(list != NULL);

    return (Linked_list *)memset(list, 0, sizeof(Linked_list));
}

/* Initialize Linked_list_entry structure(s).
 *
 * Usage example:
 *
 *    // ...
 *    Linked_list_entry entries[SOME_CONSTANT];
 *
 *    linked_list_entries_init(SOME_CONSTANT, entries);
 *    // ...
 *
 * Note that if entries were allocated using calloc() then it wouldn't be
 * necessary to call this (linked_list_entries_init()), since calloc() sets
 * allocated memory to 0.
 */
INLINE Linked_list_entry *linked_list_entries_init(size_t number_of_entries,
    Linked_list_entry *entries)
{
    assert(entries != NULL);

    return (Linked_list_entry *)
        memset(entries, 0, sizeof(Linked_list_entry) * number_of_entries);
}

/* Check if linked list is empty or not. O(1)
 */
INLINE bool linked_list_is_empty(Linked_list *list)
{
    assert(list != NULL);

    return list->length == 0;
}

/* Get length of the list. O(1)
 */
INLINE size_t linked_list_length(Linked_list *list)
{
    assert(list != NULL);

    return list->length;
}

/* If NO_INLINE is defined, then most of the functions won't be inlined and
 * code that includes this header will have to link the unresolved symbols.
 */
#ifdef NO_INLINE

/* This indirection (meaning MAYBE_INLINE) allows us to decide if code should
 * be defined as inline regardles of the fact if currently used language
 * standard understands it or not. See file "linked-list/impl.c" and the #else
 * branch here for how we use this fact to our advantage.
 */
#define MAYBE_INLINE

/* Insert entry at the beginning or the list in O(1).
 */
extern void linked_list_insert(Linked_list *, Linked_list_entry *);

/* Insert entry at the end or the list in O(1).
 */
extern void linked_list_append(Linked_list *, Linked_list_entry *);

/* Insert entry at the beginning or end of the list (depending on
 * start argument) in O(1).
 */
extern void linked_list_push(Linked_list *, enum Linked_list_start,
    Linked_list_entry *);

/* Get first entry of the list in O(1).
 *
 * Same as using:
 *
 *   linked_list_top(list, LINKED_LIST_BEGINNING, result);
 *
 * Function returns false if list is empty, otherwise it returns true.
 */
extern bool linked_list_first(Linked_list *, Linked_list_entry **);

/* Get last entry of the list in O(1).
 *
 * Same as using:
 *
 *   linked_list_top(list, LINKED_LIST_END, result);
 *
 * Function returns false if list is empty, otherwise it returns true.
 */
extern bool linked_list_last(Linked_list *, Linked_list_entry **);

/* Get first or last entry of linked list in O(1).
 *
 * Function returns false if list is empty, otherwise it returns true.
 */
extern bool linked_list_top(Linked_list *, enum Linked_list_start,
    Linked_list_entry **);

/* Remove first or last element and return it in O(1).
 *
 * Function returns false if list is empty, otherwise it returns true.
 */
extern bool linked_list_pop(Linked_list *, enum Linked_list_start,
    Linked_list_entry **);

/* Given address of linked list entry. O(1)
 *
 * This function doesn't check validity of pointers given to it.
 */
extern void linked_list_remove(Linked_list *, Linked_list_entry *);

/* Find/remove element for which predicate returns true. O(n)
 *
 * Function returns false if such entry wasn't found and true otherwise.
 */
extern bool linked_list_find(Linked_list *, enum Linked_list_start,
    Linked_list_entry_predicate, bool, Linked_list_entry **);

#else /* if NO_INLINE is not defined */

#define MAYBE_INLINE    INLINE
#include "linked-list/impl.h"

#endif /* NO_INLINE */

#ifdef __cplusplus
}
#endif

#endif /* LINKED_LIST_H_111834419021372736397274666053784489481 */
