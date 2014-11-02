#include "linked-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    const unsigned int num_entries = 8;
    const char *strings[] =
        {
            "String 0", "String 1", "String 2", "String 3", "String 4",
            "String 5", "String 6", "String 7"
        };
    Linked_list_entry entries[num_entries];
    Linked_list linked_list = LINKED_LIST_STATIC_INIT;
    Linked_list_entry *entry;

    linked_list_entries_init(num_entries, entries);
    setvbuf(stdout,NULL,_IONBF,0);

    printf("Checking if list is empty; and it is%s.\n",
        linked_list_is_empty(&linked_list) ? "" : "n't");

    for (unsigned int i = 0; i < num_entries; i++)
    {
        entries[i].data = (void *)strings[i];
        linked_list_push(&linked_list, LINKED_LIST_END, &(entries[i]));
        printf("Appended \"%s\" list size is now %zu\n", strings[i],
            linked_list_length(&linked_list));
    }

    printf("Checking if list is empty; and it is%s.\n",
        linked_list_is_empty(&linked_list) ? "" : "n't");

    printf("Now going to pop() entries from the back.\n");
    while (linked_list_pop(&linked_list, LINKED_LIST_END, &entry))
    {
        printf("Popped \"%s\" list size is now %zu\n", (char *)(entry->data),
            linked_list_length(&linked_list));
    }

    printf("Checking if list is empty; and it is%s.\n",
        linked_list_is_empty(&linked_list) ? "" : "n't");

    for (unsigned int i = 0; i < num_entries; i++)
    {
        entries[i].data = (void *)strings[i];
        linked_list_push(&linked_list, LINKED_LIST_BEGINNING, &(entries[i]));
        printf("Inserted \"%s\" list size is now %zu\n", strings[i],
            linked_list_length(&linked_list));
    }

    printf("Checking if list is empty; and it is%s.\n",
        linked_list_is_empty(&linked_list) ? "" : "n't");

    printf("Now wer are going to remove every odd entry.\n");
    for (unsigned int i = 1; i < num_entries; i += 2)
    {
        linked_list_remove(&linked_list, &(entries[i]));
        printf("Removied \"%s\" list size is now %zu\n", strings[i],
            linked_list_length(&linked_list));
    }

    printf("Checking if list is empty; and it is%s.\n",
        linked_list_is_empty(&linked_list) ? "" : "n't");

    printf("Now going to pop() entries from the front.\n");
    while (linked_list_pop(&linked_list, LINKED_LIST_BEGINNING, &entry))
    {
        printf("Popped \"%s\" list size is now %zu\n", (char *)(entry->data),
            linked_list_length(&linked_list));
    }

    printf("Checking if list is empty; and it is%s.\n",
        linked_list_is_empty(&linked_list) ? "" : "n't");

    exit(EXIT_SUCCESS);
}
