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

MAYBE_INLINE void linked_list_append(Linked_list *list,
    Linked_list_entry *entry)
{
    assert(list != NULL);
    assert(entry != NULL);

    entry->next = NULL;
    entry->previous = list->last;
    if (list->last != NULL)
    {
        list->last->next = entry;
    }
    else
    {
        /* When list->last = NULL then linked list is empty and therefore
         * list->first = NULL also and so it needs to be set too.
         */
        list->first = entry;
    }
    list->last = entry;
    list->length++;

    /* Linked list must by non-empty after appending element.
     */
    assert(list->first != NULL);
    assert(list->last == entry);
    assert(list->length > 0);

    /* If linked list has only one element, then it is also its first element.
     */
    assert(list->length != 1 || (list->length == 1 && list->first == entry));
}

MAYBE_INLINE void linked_list_insert(Linked_list *list,
    Linked_list_entry *entry)
{
    assert(list != NULL);
    assert(entry != NULL);


    entry->next = list->first;
    entry->previous = NULL;
    if (list->first != NULL)
    {
        list->first->previous = entry;
    }
    else
    {
        /* When list->first = NULL then linked list is empty and therefore
         * list->last = NULL also and so it needs to be set too.
         */
        list->last = entry;
    }
    list->first = entry;
    list->length++;

    /* Linked list must by non-empty after inserting element.
     */
    assert(list->first == entry);
    assert(list->last != NULL);
    assert(list->length > 0);

    /* If linked list has only one element, then it is also its last element.
     */
    assert(list->length != 1 || (list->length == 1 && list->last == entry));
}

MAYBE_INLINE void linked_list_push(Linked_list *list,
    enum Linked_list_start start, Linked_list_entry *entry)
{
    /* Assertations are used in linked_list_insert() and linked_list_append(),
     * we aren't going to duplicate them.
     */
    switch (start)
    {
        case LINKED_LIST_BEGINNING:
            return linked_list_insert(list, entry);
        case LINKED_LIST_END:
            return linked_list_append(list, entry);
    }
}

MAYBE_INLINE bool linked_list_first(Linked_list *list,
    Linked_list_entry **result)
{
    assert(list != NULL);
    assert(result != NULL);

    if (list->first == NULL)
    {
        return false;
    }

    *result = list->first;
    return true;
}

MAYBE_INLINE bool linked_list_last(Linked_list *list,
    Linked_list_entry **result)
{
    assert(list != NULL);
    assert(result != NULL);

    if (list->last == NULL)
    {
        return false;
    }

    *result = list->last;
    return true;
}

MAYBE_INLINE bool linked_list_top(Linked_list *list,
    enum Linked_list_start start, Linked_list_entry **result)
{
    /* Assertations are used in linked_list_first() and linked_list_last(), we
     * aren't going to duplicate them.
     */
    switch (start)
    {
        case LINKED_LIST_BEGINNING:
            return linked_list_first(list, result);
        case LINKED_LIST_END:
            return linked_list_last(list, result);
    }
    return false;
}

MAYBE_INLINE void linked_list_remove(Linked_list *list,
    Linked_list_entry *entry)
{
    Linked_list_entry *previous;
    Linked_list_entry *next;

    assert(list != NULL);
    assert(entry != NULL);

    /* We have 3 special cases where entry is:
     *
     * 1. First in the list (detected by predicate entry->previous == NULL).
     * 2. Last in the list (detected by predicate entry->next == NULL).
     * 3. First and last in the list.
     *
     * Third case can be handled by properly handling first two independently.
     *
     * Note that if next or previous is NULL, then entry is at the end or
     * beginning of the list, respectively. Don't forget that it can be both at
     * the same time. This facts were used to simplify this algorithm.
     */

    /* previous <-- entry --> next
     */
    previous = entry->previous;
    next = entry->next;

    if (next != NULL)
    {
        next->previous = previous;
    }
    else
    {
        /* Our entry was last entry in linked list.
         */
        list->last = previous;
    }

    if (previous != NULL)
    {
        previous->next = next;
    }
    else
    {
        /* Our entry was first entry in linked list.
         */
        list->first = next;
    }

    list->length--;
    entry->next = NULL;
    entry->previous = NULL;

    /* After removing one entry we either get empty list or non-empty list.
     * This invariant checks for internal consistency.
     */
    assert((list->length == 0 && list->first == NULL && list->first == NULL)
        || (list->length > 0 && list->first != NULL && list->first != NULL));
}

MAYBE_INLINE bool linked_list_pop(Linked_list *list,
    enum Linked_list_start start, Linked_list_entry **result)
{
    /* We have to create this for storing temporary result since
     * linked_list_top() may fail when linked list is empty.
     */
    Linked_list_entry *bookmark;

    /* Some assertations are used in linked_list_top(), we aren't going to
     * duplicate them.
     */
    assert(result != NULL);

    /* Bookmark will contain first or last entry, depending on the "start"
     * argument, but if list is empty then we terminate with false.
     */
    if (!linked_list_top(list, start, &bookmark))
    {
        return false;
    }
    linked_list_remove(list, bookmark);
    *result = bookmark;

    return true;
}

MAYBE_INLINE bool linked_list_find(Linked_list *list,
    enum Linked_list_start start,
    Linked_list_entry_predicate predicate,
    bool remove_when_found,
    Linked_list_entry **result)
{
    Linked_list_entry *bookmark;

    /* Some assertations are used in linked_list_top(), we aren't going to
     * duplicate them.
     */
    assert(predicate != NULL);
    assert(result != NULL);

    if (!linked_list_top(list, start, &bookmark))
    {
        return false;
    }

    while (bookmark != NULL)
    {
        if (predicate(bookmark))
        {
            if (remove_when_found)
            {
                linked_list_remove(list, bookmark);
            }

            break;
        }

        switch (start)
        {
            case LINKED_LIST_BEGINNING:
                bookmark = bookmark->next;
                break;
            case LINKED_LIST_END:
                bookmark = bookmark->previous;
                break;
        }
    }

    *result = bookmark;
    return true;
}
