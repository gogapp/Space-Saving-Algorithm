/*
 *
 *  Written by : Prashant Kumar Prajapati
 *  Time complexity : O(n*log(n))
 *  Space complexity : O(n)
 *
 */

 /*
  *
  *  This script implements a prototype of the space saving algorithm.
  *  Internally the algorithm uses a max-heap and a hash map for finding the number with the maximum count.
  *  The hash map has been implemented as an AVL Tree in this script.
  *  Input is taken from input.txt and output is written to output1.txt.
  *  This script writes a number to output1.txt file if it is present in more than 20% of the dataset
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/************ AVL TREE ***********/

/* Node structure of AVL Tree */
struct node
{
    unsigned int key;
    int val;
    int height;
    struct node* left;
    struct node* right;
};

/* Calculates and returns the maximum of two values */
int max (int a, int b)
{
    return (a > b) ? a : b;
}

/* Returns the height of node in AVL Tree */
int height (struct node* root)
{
    if ( !root )
        return 0;

    return root->height;
}

/* Creates a new node */
struct node* newNode (unsigned int key, int val)
{
    struct node* root = (struct node*) malloc (sizeof( struct node));
    root->key = key;
    root->val = val;
    root->left = NULL;
    root->right = NULL;
    root->height = 1;
    return root;
}

/* Performs a left rotation of the edge connected with the root node */
struct node* left_rotate (struct node* root)
{
    struct node* temp1 = root->right;
    struct node* temp2 = temp1->left;

    temp1->left = root;
    root->right = temp2;

    root->height = max ( height(root->left), height(root->right) ) + 1;
    temp1->height = max( height(temp1->left), height(temp1->right) ) + 1;

    return temp1;
}

/* Performs a right rotation of the edge connected with the root node */
struct node* right_rotate (struct node* root)
{
    struct node* temp1 = root->left;
    struct node* temp2 = temp1->right;

    temp1->right = root;
    root->left = temp2;

    root->height = max ( height(root->left), height(root->right) ) + 1;
    temp1->height = max ( height(temp1->left), height(temp1->right) ) + 1;

    return temp1;
}

/* Rearranges a node in case if it is left heavy or right heavy */
struct node* rearrange (struct node* root)
{
    int lh = height(root->left);
    int rh = height(root->right);
    root->height = max(lh,rh) + 1;
    int br = lh-rh;
    if (br > 1)
    {
        if (lh >= rh)
            return right_rotate(root);
        else
        {
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }
    }
    else if (br < -1)
    {
        if (rh >= lh)
            root = left_rotate(root);
        else
        {
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }
    }
    return root;
}

/* Used for inserting the key in the tree */
struct node* tree_insert ( struct node* root, unsigned int key, int val )
{
    if (root == NULL)
        return newNode(key,val);

    else if (key < root->key)
        root->left = tree_insert(root->left,key,val);

    else if (key > root->key)
        root->right = tree_insert(root->right,key,val);

    return rearrange(root);
}

/* Update the value at key to val */
void tree_data_update ( struct node* root, unsigned int key, int val )
{
    if(key < root->key)
    {
        tree_data_update(root->left, key, val);
        return;
    }
    else if(key > root->key)
    {
        tree_data_update(root->right, key, val);
        return;
    }
    root->val = val;
}

/* Returns the value of key */
int tree_search ( struct node* root, unsigned int key )
{
    if(root==NULL)
        return -100000000;

    if(key<root->key)
        return tree_search (root->left, key);

    if(key>root->key)
        return tree_search (root->right, key);

    return root->val;
}

/* Returns the inorder traversal successor of a node */
struct node* inorder_successor (struct node* root)
{
    struct node* curr = root;

    while (curr->left != NULL)
        curr=curr->left;

    return curr;
}

/* Deletes a key from the tree */
struct node* tree_delete ( struct node* root, unsigned int key )
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = tree_delete(root->left, key);

    else if (key > root->key)
        root->right = tree_delete( root->right, key);

    else
    {
        if (root->left == NULL)
        {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node* temp = root->left;
            free(root);
            return temp;
        }

        struct node* successor = inorder_successor(root->right);
        root->key = successor->key;
        root->val = successor->val;
        root->right = tree_delete(root->right,successor->key);

    }

    return rearrange(root);
}

/*********** HASH TABLE ***********/

/* Generates the hash value of a key */
unsigned int hash (unsigned int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

/* Stores the root of AVL tree */
struct node* groot;

/* Insert a new key in hash table */
void map_insert (int key, int val)
{
    int hash_key = hash(key);
    groot = tree_insert(groot, hash_key, val);

}

/* Deletes a key from the hash table */
void map_delete (int key)
{
    int hash_key = hash(key);
    groot = tree_delete (groot, hash_key);

}

/* Updates the value at key to val */
void map_data_update (int key, int val)
{
    int hash_key = hash(key);
    tree_data_update(groot, hash_key, val);

}

/* Returns the value of key */
int map_search (int key)
{
    int hash_key = hash(key);
    return tree_search(groot, hash_key);

}

/* Updates the hash value of a value from key1 to key2 */
void map_hash_update (int key1, int key2)
{
    unsigned int hash1 = hash(key1);
    unsigned int hash2 = hash(key2);

    int val = tree_search (groot, hash1);

    groot = tree_delete (groot, hash1);
    groot = tree_insert (groot, hash2, val);

}

/*************** MAX-HEAP ***************/

/* Node structure of max heap */
struct heap_node
{
    int val;
    int cnt;
};

struct heap_node heap[500000];
int size=0;

/* Swaps the elements at position a and b */
void swap (int a,int b)
{
    struct heap_node temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

/* Maintains the heap invariant */
void heapify (int position)
{
    int l = 2*position;
    int r = 2*position+1;
    int ans = position;
    if ( ( l <= size ) && ( heap[l].cnt > heap[ans].cnt ) )
        ans=l;

    if ( ( r <= size ) && ( heap[r].cnt > heap[ans].cnt ) )
        ans=r;

    if (ans != position)
    {
        map_data_update (heap[position].val, ans);
        map_data_update (heap[ans].val, position);
        swap (ans, position);
        heapify(ans);
    }
}

/* Moves a node upwards in the heap maintaining the heap invariant */
void move (int position)
{
    while ( ( heap[position].cnt > heap[position/2].cnt) && ( position > 1 ) )
    {
        map_data_update ( heap[position/2].val, position );
        map_data_update ( heap[position].val , position/2 );
        swap ( position, position/2 );
        position = position / 2;
    }
}

/* Inserts a new val in the heap */
void heap_insert (int val)
{
    struct heap_node to_add;
    to_add.val = val;
    to_add.cnt = 1;
    heap[ ++size ] = to_add;
    map_insert (val, size);
    move (size);
}

/* Updates the value to val */
void heap_update (int val)
{
    int pos = map_search (val);
    heap[ pos ].val = val;
    heap[ pos ].cnt++;
    move (pos);
}

/* Deletes a item from heap */
void heap_delete (int val)
{
    int position = map_search (val);
    map_data_update (heap[size].val, position);
    map_delete (heap[position].val);
    swap (position, size);
    heap[ size ].val = 0;
    heap[ size ].cnt = 0;
    size--;
    heapify (position);
}

/* Returns the maximum element in the heap */
struct heap_node max_element()
{
    return heap[1];
}

/******* SPACE SAVING ALGORITHM *******/

void space_saving(int key)
{
    if ( map_search(key) != -100000000 )
        heap_update(key);

    else
    {
        if (size < 499999)
            heap_insert(key);
        else
        {
            map_hash_update (heap[size].val, key);
            heap_update (key);
        }
    }
}

/************* MAIN FUNCTION **********/

int main()
{
    int i;
    FILE *f1,*f2;
    f1 = fopen ( "input.txt" , "r" );
    f2 = fopen ( "output2.txt", "w" );
    if (f1)
    {
        int n, val, cnt = 0;
        fscanf (f1, "%d", &n);
        for (i = 0; i < n; i++ )
        {
            fscanf (f1, "%d", &val);
            space_saving (val);
            if (max_element().cnt > n/5)
            {
                fprintf (f2, "The number to delete is %d\n", max_element().val);
                heap_delete (max_element().val);
                cnt++;
            }
        }
        fprintf (f2, "No. of deleted numbers = %d\n", cnt);
    }
    return 0;
}
