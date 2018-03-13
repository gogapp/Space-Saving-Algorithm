/*
 *
 *  Written by : Prashant Kumar Prajapati
 *  Time complexity : O(n)
 *  Space complexity : O(n)
 *
 */

 /*
  *
  *  This script implements a prototype of the space saving algorithm.
  *  Internally the algorithm uses a max-heap and a hash map for finding the number with the maximum count.
  *  The hash map has been implemented as a hash-table in this script.
  *  Input is taken from input.txt and output is written to output.txt.
  *  This script writes a number to output.txt file if it is present in more than 20% of the dataset
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node
{
    int key;
    int val;
    struct node* next;
};

struct heap_node
{
    int val;
    int cnt;
};

int size=0;
struct node database [ 500001 ];
struct heap_node heap [ 500001 ];

/*********LINKED LIST*******/

struct node* newNode (int key, int val)
{
    struct node* temp = (struct node*) malloc ( sizeof (struct node) );
    temp->key = key;
    temp->val = val;
    temp->next = NULL;
    return temp;
}

void list_insert (int key, int val)
{
    if (database[ key ].val == -1)
    {
        database[ key ].val = 0;
        struct node* temp = newNode (key, val);
        database[ key ].next = temp;
    }
    else
    {
        struct node* temp = database[ key ].next;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode (key, val);
    }
}

struct node* list_search (int key)
{
    if (database[ key ].val == -1)
        return newNode(key,-100000000);

    return database[ key ].next;
}

/*********HASH TABLE********/

const int md = 50000;

unsigned int hash (unsigned int x)
{
    unsigned int a = 636586, b = 546234;

    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    x = (a + (x * b)) % md;
    if (x <= 0)
        x = x + md;

    return x;
}

void map_insert (int key, int val)
{
    int hash_key = hash (key);
    list_insert (hash_key, val);
}

int map_search (int key)
{
    int hash_key = hash (key);
    struct node* temp = list_search (hash_key);

    if (temp->val == -100000000)
        return -1;

    if (heap [ temp->val ].val == key)
        return temp->val;

    while (temp->next != NULL)
    {
        if(heap[temp->val].val==key)
            return temp->val;

        temp = temp->next;
    }
    return -1;
}

void map_data_update (int key, int val)
{
    int hash_key = hash(key);
    struct node* temp = list_search (hash_key);
    if (temp->val == -100000000)
        return;

    if( heap[temp->val].val ==key )
    {
        temp->val = val;
        return;
    }

    while (temp->next != NULL)
    {
        if(heap[temp->val].val == key)
        {
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
}

void map_hash_update (int key1, int key2)
{
    unsigned int hash1 = hash(key1);
    unsigned int hash2 = hash(key2);

    struct node* temp = list_search (hash1);
    struct node* temp2 = newNode(0,0);
    temp2->next = temp;

    if ( heap[temp->val].val == key1 )
    {
        if ( temp->next == NULL )
            database[hash1] = *(newNode(0,-1));
        else
            database[hash1].next = temp->next;

        free(temp);
        list_insert(hash2,size);

        return;
    }
    while (temp->next != NULL)
    {
        if (heap[temp->val].val == key1)
        {
            temp2->next = temp->next;
            free(temp);
            break;
        }
        temp2 = temp;
        temp = temp->next;
    }
    list_insert(hash2,size);
}

void map_delete (int key)
{
    int hash_key = hash(key);
    struct node* temp = list_search(hash_key);
    struct node* temp2 = newNode(0,0);
    temp2->next = temp;

    if ( heap[temp->val].val == key )
    {
        if ( temp->next == NULL )
            database[hash_key] = *(newNode(0,-1));
        else
            database[hash_key].next = temp->next;

        free(temp);
        return;
    }

    while (temp->next != NULL)
    {
        if (heap[temp->val].val == key)
        {
            temp2->next = temp->next;
            free(temp);
            break;
        }
        temp2 = temp;
        temp = temp->next;
    }
}

/********** MAX - HEAP *********/

void swap (int a, int b)
{
    struct heap_node temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

void heapify (int position)
{
    int l = 2 * position;
    int r = 2 * position + 1;
    int ans = position;
    if ( ( l <= size ) && ( heap[l].cnt > heap[ans].cnt ) )
        ans = l;

    if ( ( r <= size ) && ( heap[r].cnt > heap[ans].cnt ) )
        ans = r;

    if (ans != position)
    {
        map_data_update ( heap[position].val, ans);
        map_data_update ( heap[ans].val, position);
        swap (ans, position);
        heapify (ans);
    }
}

void move (int position)
{
    while ( ( heap[position].cnt > heap[position/2].cnt ) && ( position > 1 ) )
    {
        map_data_update ( heap[position/2].val, position);
        map_data_update ( heap[position].val, position/2);
        swap ( position, position/2);
        position = position / 2;
    }
}

void heap_insert (int val)
{
    struct heap_node to_add;
    to_add.val = val;
    to_add.cnt = 1;
    heap[++size] = to_add;
    map_insert (val, size);
    move (size);
}

void heap_update (int val)
{
    int pos = map_search (val);
    heap[pos].val = val;
    heap[pos].cnt++;
    move(pos);
}

void heap_delete (int val)
{
    int position = map_search (val);
    map_data_update ( heap[size].val, position);
    map_delete (heap[position].val);
    swap (position, size);
    heap[size].val = 0;
    heap[size].cnt = 0;
    size--;
    heapify (position);
}

struct heap_node max_element(){
    return heap[1];
}

/******* SPACE SAVING ALGORITHM *******/

void space_saving (int key)
{
    if ( map_search (key) != -1)
        heap_update (key);
    else
    {
        if (size < 49999)
            heap_insert(key);

        else
        {
            map_hash_update (heap[size].val, key);
            heap[size].val = key;
            heap[size].cnt++;
            move(size);
        }
    }
}

/********** MAIN FUNCTION ***********/

int main()
{
    int i;
    for ( i = 0; i <= 50000; i++)
        database[i] = *(newNode(0,-1));

    FILE *f1,*f2;
    f1 = fopen( "input.txt", "r");
    f2 = fopen( "output.txt", "w");

    if(f1)
    {
        int n, val, cnt = 0;
        fscanf ( f1, "%d", &n);
        for( i = 0; i < n; i++)
        {
            fscanf ( f1, "%d", &val);
            space_saving (val);
            if (max_element().cnt > n/5)
            {
                fprintf ( f2, "The number to delete is %d\n", max_element().val);
                heap_delete ( max_element().val);
                cnt++;
            }
        }
        fprintf ( f2, "No. of deleted numbers = %d\n", cnt);
    }
    return 0;
}
