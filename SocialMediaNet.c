#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;
struct node *deleteNode(struct node *, int );
struct node *minValueNode(struct node *node) ;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


  //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}


//search for user with id=key
struct node* search(int key, struct node *users)
{
    //CODE HERE
    if(users == NULL) return NULL;
    else if (users->id == key) return users;
    else if(users->id < key) return search (key ,users->right );
    else return search(key, users->left);
      
}

//see document for explanattion
struct node *refineUser(struct node *user, struct node *users)
{
    // CODE HERE
    int pos;
    int newFriendCount = 0;
    struct node *searchFriends = NULL;
    if(user == NULL) return users;
    else
    {
        user->left = NULL;
        user->right = NULL;
    }

    if (users == NULL)
    {
        user->numfren = 0;
        user->friends[0] = -1;
        return user;
    }

    while (search(user->id, users))
    {
        user->id = user->id + 1;
    }
    
    if (user->numfren < 0)
        return user;
    for (int i = 0; i < user->numfren; i++)
    {
        searchFriends = search(user->friends[i], users);

        if (searchFriends != NULL)
        {
            user->friends[newFriendCount++] = user->friends[i];
            searchFriends->friends[searchFriends->numfren++] = user->id;
        }
    }
    user->numfren = newFriendCount;
    return user;
}

//insert user with id
struct node *insertUser(struct node *root, int id, struct node *user)
{
    // CODE HERE
    if (root == NULL)
    {
        return user;
    }
    else
    {
        if (id < root->id)
        {
            root->left = insertUser(root->left, id, user);
        }
        if (id > root->id)
        {
            root->right = insertUser(root->right, id, user);
        }
    }
    return root;
}

//prints friends list
void friends(int id, struct node *users)
{
    // CODE HERE
    struct node *friendNode = search(id, users);

    if (friendNode != NULL)
    {
        if (friendNode->numfren == 0)
        {
            printf("-1\n");
            return;
        }
        for (int i = 0; i < friendNode->numfren; i++)
        {
            printf("%d\n", friendNode->friends[i]);
        }
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
 struct node *minValueNode(struct node *node) 
{
    struct node* current = node;
    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;
  
    return current;
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{
    //CODE HERE
    if (users == NULL)
        return NULL;
    struct node *delNode = search(key, users);
    struct node *searchFriends = NULL;
    int newFriendCount = 0;
    if (delNode != NULL)
    {

        if (delNode->numfren < 0)
            return NULL;

        for (int i = 0; i < delNode->numfren; i++)
        {
            searchFriends = search(delNode->friends[i], users);
            if (searchFriends != NULL)
            {
                newFriendCount = 0;

                for (int j = 0; j < searchFriends->numfren; j++)
                {
                    if (searchFriends->friends[j] != key)
                    {
                        searchFriends->friends[newFriendCount] = searchFriends->friends[j];
                        newFriendCount++;
                    }
                }

                searchFriends->numfren = newFriendCount;
            }
        }

    }
 
    return users;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key)
{
    // CODE HERE
    if(root == NULL) return root;

    if (key < root->id)
        root->left = deleteNode(root->left, key);
     else if (key > root->id)
        root->right = deleteNode(root->right, key);
    else
    {
        
        if (root->left == NULL)
         {
            struct node *temp = root->right;
            free(root);
            return temp;
        } 
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }
       
        struct node* nodeToDelete = minValueNode(root->right);
        root->id = nodeToDelete->id;
        strcpy (root->name, nodeToDelete->name);
        root->right = deleteNode (root->right, nodeToDelete->id);
    }
   
    return root;

}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if(myusers == NULL)
    return;

    printInOrder (myusers->left);
    printf ("%d %s\n", myusers->id, myusers->name) ;
    printInOrder (myusers->right);
}


int main(int argc, char **argv)
{
    node *users = NULL;
    char str[MAX];
    while (1)
    {

        int opt, id;
        // fflush(stdin);
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            scanf("%s", str);
            struct node *tbins = retUser(str);
            tbins = refineUser(tbins, users);
            users = insertUser(users, tbins->id, tbins);
            break;

        case 2:

            scanf("%d", &id);
            deleteFriends(id, users);
            users = deleteNode(users, id);
            break;

        case 3:

            scanf("%d", &id);
            node *result = search(id, users);
            if (result == NULL)
                printf("USER NOT IN TREE.\n");
            else
            {
                printf("%d\n", result->id);
            }
            break;

        case 4:
            scanf("%d", &id);
            friends(id, users);
            break;

        case 5:
            printInOrder(users);
            break;

        case 6:
            exit(0);
            break;

        default:
            printf("Wrong input! \n");
            break;
        }
    }

    return 0;
}