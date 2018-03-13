/*
 *
 *  Written by : Prashant Kumar Prajapati
 *  Time complexity : O(n*log(n))
 *  Space complexity : O(n)
 *
 */

 /*
  *
  *  This script generates a 100% accurate output for finding the count of numbers in the file
  *  The script takes input from input.txt file and writes it to correct.txt
  *  It writes an output only when a number is present in more than 20% of the dataset
  *
  *
  */

#include <stdio.h>

int n,cnt;

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
struct node* tree_insert(struct node* root, unsigned int key)
{
    if (root == NULL)
        return newNode(key,1);

    else if (key < root->key)
        root->left = tree_insert (root->left, key);

    else if (key > root->key)
        root->right = tree_insert (root->right, key);

    return rearrange (root);
}

/* Increment the value at key by 1 */
void tree_data_update (struct node* root, unsigned int key)
{
    if (key < root->key)
    {
        tree_data_update (root->left, key);
        return;
    }
    else if (key > root->key)
    {
        tree_data_update (root->right, key);
        return;
    }
    root->val = root->val + 1;
}

/* Returns the value of key */
int tree_search (struct node* root, unsigned int key)
{
    if (root == NULL) return -100000000;

    if (key < root->key)
        return tree_search (root->left, key);

    if (key > root->key)
        return tree_search (root->right, key);

    return root->val;
}

/*********** HASH TABLE ***********/

/* Generates the hash value of a key */
unsigned int hash(unsigned int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

/* Generates the key from the hash value */
unsigned int unhash(unsigned int x)
{
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = (x >> 16) ^ x;
    return x;
}

/* Stores the root of AVL tree */
struct node* groot;

/* Insert a new key in hash table */
void map_insert (int key)
{
    int hash_key = hash(key);
    groot = tree_insert (groot, hash_key);
}

/* Increments the value at key by 1 */
void map_data_update (int key)
{
    int hash_key = hash(key);
    tree_data_update (groot, hash_key);
}

/* Returns the value of key */
int map_search (int key)
{
    int hash_key = hash(key);
    return tree_search (groot, hash_key);
}

/******* INORDER TRAVERSAL *******/

int result[1000000];

void inorder(struct node* root)
{
    if (root==NULL)
        return;

    inorder (root->left);
    if (root->val > n/5)
    {
        result[cnt] = unhash (root->key);
        cnt++;
    }
    inorder (root->right);
}

/********* MAIN FUNCTION ********/

int main()
{
    FILE *f1,*f2;
    f1 = fopen ( "input.txt", "r");
    f2 = fopen ( "correct.txt", "w");
    fscanf (f1, "%d", &n);

    int i;
    for( i = 0; i < n; i++)
    {
        int x;
        fscanf (f1, "%d", &x);

        if (map_search(x) == -100000000)
            map_insert(x);
        else
            map_data_update(x);
    }

    inorder(groot);
    for( i = 0; i < cnt; i++)
        fprintf (f2, "The number to delete is %d\n", result[i]);

    fprintf (f2, "No. of deleted numbers = %d", cnt);
    return 0;
}
