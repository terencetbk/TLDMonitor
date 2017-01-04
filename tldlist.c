// AP3 Accessed Exercise 1
// Done by: Terence Tan Boon Kiat
// Matriculation ID: 15AC083B
// This is my own work except that the following were used references:
// http://allprogramsinc.blogspot.sg/2013/04/program-for-avl-tree.html
// http://anoopsmohan.blogspot.sg/2011/11/avl-tree-implementation-in-c.html
// https://gist.github.com/Knifa/1318375

// tldlist.c

#include "date.h"
#include "tldlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// function prototypes
static TLDNode *tldnode_create(char *tldName); // creates/initiates an empty node for storing tlds
static int tldnode_add(TLDList *tld, char *tldname, TLDNode *node); // add nodes with tlds and counts them
static TLDNode *findLastNode(TLDNode *node); // for finding last node of the tree/tldlist
static int tldnode_findHeight(TLDNode *node); // for finding maximum height of a node, whether left or right
static int tldnode_compareHeight(TLDNode *node); // for comparing height of left and right branches of a node
static void rebalance(TLDNode *node); // assists in rebalancing a node
static TLDNode *rebalance_RR(TLDNode *node);
static TLDNode *rebalance_RL(TLDNode *node);
static TLDNode *rebalance_LL(TLDNode *node);
static TLDNode *rebalance_LR(TLDNode *node);

struct tldlist {
	//number of successful addition of nodes
	long countSuccess;
	//tldlist always has a begin and end date
	Date *beginDate;
	Date *endDate;
	//tldlist contain a rootNode node
	TLDNode *rootNode;
};


struct tldnode {
	//number of this specific node (e.g. count of com tld is 50)
	long count;
	//store top level domain in the string char
	char *tldString;
	//tldnode can have a leftNode and rightNode child of same struct and have a parentNode excluding rootNode
	TLDNode *parentNode;
	TLDNode *leftNode;
	TLDNode *rightNode;
};


struct tlditerator {
	TLDNode *nodes;
};

/*
* tldlist_create generates a list structure for storing counts against
* top level domains (TLDs)
*
* creates a TLDList that is constrained to the `begin' and `end' Date's
* returns a pointer to the list if successful, NULL if not
*/
TLDList *tldlist_create(Date *begin, Date *end) {
	TLDList *newList = malloc(sizeof(TLDList));
	if (newList != NULL) {
		newList->countSuccess = 0;
		newList->beginDate = begin;
		newList->endDate = end;
		newList->rootNode = NULL;
	}
	else
	{
		printf("list allocation failure");
		free(newList);
		return NULL;
	}
	return newList;
}

/*
* tldlist_destroy destroys the list structure in `tld'
*
* all heap allocated storage associated with the list is returned to the heap
*/
void tldlist_destroy(TLDList *tld) {
	if (tld != NULL) {
		TLDIterator *iter = tldlist_iter_create(tld);
		TLDNode *temp;

		while ((temp = tldlist_iter_next(iter)) != NULL) {
			free(temp->tldString);
			free(temp);
		}
		free(tld);
		tldlist_iter_destroy(iter);
	}
	else
		printf("tld is null");
}

/*
* tldlist_add adds the TLD contained in `hostname' to the tldlist if
* `d' falls in the begin and end dates associated with the list;
* returns 1 if the entry was counted, 0 if not
*/
int tldlist_add(TLDList *tld, char *hostname, Date *d) {
	// compare dates to check if within range
	if (date_compare(tld->beginDate, d) > 0 || date_compare(tld->endDate, d) < 0)
		return 0;
	// temp TLD from hostname without the dot
	char *tldName = strrchr(hostname, '.') + 1;
	int i;
	//change all to lowercase
	for (i = 0; (tldName[i] != '\0'); i++) {
		tldName[i] = tolower(tldName[i]);
	}
	char *tldNameCopy = malloc(sizeof(tldName));
	strcpy(tldNameCopy, tldName);

	if (!tldnode_add(tld, tldNameCopy, tld->rootNode)) {
		tld->countSuccess++;
	}
	else {
		free(tldNameCopy);
	}

	return 1;
}

// function for empty node creation
static TLDNode *tldnode_create(char *tldName) {
	TLDNode *newNode = malloc(sizeof(TLDNode));

	if (newNode != NULL) {
		newNode->parentNode = NULL;
		newNode->leftNode = NULL;
		newNode->rightNode = NULL;
		newNode->count = 1;
		newNode->tldString = tldName;
	}
	else {
		printf("node allocation failure");
		free(newNode);
		return NULL;
	}
	return newNode;
}

// function for adding nodes as per tld string
static int tldnode_add(TLDList *tld, char *tldname, TLDNode *node) {

	if (node != NULL) {
		// compare ASCII size of input tld and node's tld
		int compare = strcmp(tldname, node->tldString);

		// go leftNode if smaller
		if (compare < 0) {
			// recursive function because current leftNode position is already taken
			if (node->leftNode != NULL)
				return tldnode_add(tld, tldname, node->leftNode);
			else {
				TLDNode *temp = tldnode_create(tldname); // creates an empty node to the left of current node with input tld
				temp->parentNode = node; // set temp node's parent as current node
				node->leftNode = temp; // set temp node as the leftNode of current node

				rebalance(node->leftNode); // AVL rebalancing
				return 0;
			}
		}
		// go rightNode if larger
		else if (compare > 0) {
			// recursive function because current rightNode position is already taken
			if (node->rightNode != NULL)
				return tldnode_add(tld, tldname, node->rightNode);
			else {
				TLDNode *temp = tldnode_create(tldname); // creates an empty node with input tld
				temp->parentNode = node; // set temp node's parent as current node
				node->rightNode = temp; // set temp node as the rightNode of current node

				rebalance(node->rightNode); // AVL rebalancing
				return 0;
			}
		}
		else
		{
			// there is no need to create a new node with the same tld as it already exists
			free(tldname); // free it because not necessary
			node->count++; // just add to this tld's count
			return 0;
		}
	}
	else {
		// node not created yet, then create it
		TLDNode *tempNode = tldnode_create(tldname);
		tempNode->parentNode = NULL;
		tld->rootNode = tempNode;
		return 0;
	}
	return 1;
}

/*
* tldlist_count returns the number of successful tldlist_add() calls since
* the creation of the TLDList
*/
long tldlist_count(TLDList *tld) {
	return tld->countSuccess;
}

/*
* tldlist_iter_create creates an iterator over the TLDList; returns a pointer
* to the iterator if successful, NULL if not
*/
TLDIterator *tldlist_iter_create(TLDList *tld) {
	TLDIterator *newiter = malloc(sizeof(TLDIterator));

	if (newiter != NULL) {
		newiter->nodes = findLastNode(tld->rootNode); // save last node into iterator
		return newiter;
	}
	else {
		tldlist_iter_destroy(newiter);
		return NULL;
	}
}

// function for looking to the last node (inorder traversal)
static TLDNode *findLastNode(TLDNode *node) {
	if (node != NULL) {
		TLDNode *tempLeft = findLastNode(node->leftNode);
		TLDNode *tempRight = findLastNode(node->rightNode);
		// if input node's left node is not null, iterate the function
		if (tempLeft != NULL)
			return tempLeft;
		// if input node's right node is not null, iterate the function
		else if(tempRight != NULL)
			return tempRight;
		// if left and right are null, return node as it is the last node
		else
			return node;
	}
	else
		return node;
}

/*
* tldlist_iter_next returns the next element in the list; returns a pointer
* to the TLDNode if successful, NULL if no more elements to return
*/
TLDNode *tldlist_iter_next(TLDIterator *iter) {
	TLDNode *temp = iter->nodes;

	if (temp != NULL)
	{
		// check if node in iterator has a parent, if not then it is the root node
		if (iter->nodes->parentNode != NULL) {
			TLDNode *tempRight = iter->nodes->parentNode->rightNode;
			if (tempRight != iter->nodes && tempRight != NULL)
				iter->nodes = findLastNode(iter->nodes->parentNode->rightNode);
			else
				iter->nodes = iter->nodes->parentNode;
		}
		else {
			iter->nodes = NULL; // set the iterator's node storage to null
			return temp; // returns the node if it is the root node of the tree
		}

	}
	else
		return NULL;

	return temp;
}

/*
* tldlist_iter_destroy destroys the iterator specified by `iter'
*/
void tldlist_iter_destroy(TLDIterator *iter) {
	// free all iterator elements and itself
	free(iter->nodes);
	free(iter);
}

/*
* tldnode_tldname returns the tld associated with the TLDNode
*/
char *tldnode_tldname(TLDNode *node) {
	return node->tldString;
}

/*
* tldnode_count returns the number of times that a log entry for the
* corresponding tld was added to the list
*/
long tldnode_count(TLDNode *node) {
	return node->count;
}

// returns height of the current tldnode
static int tldnode_findHeight(TLDNode *node)
{
	if (node != NULL)
	{
		int left = tldnode_findHeight(node->leftNode);
		int right = tldnode_findHeight(node->rightNode);
		if (left > right)
		{
			return 1 + left;
		}
		else
		{
			return 1 + right;
		}
	}
	else
		return 0;
}

// compares height of the leftNode and rightNode nodes for current node
static int tldnode_compareHeight(TLDNode *node)
{
	int leftHeight;
	int rightHeight;
	int difference;

	if (node != NULL) {
		leftHeight = tldnode_findHeight(node->leftNode);
		rightHeight = tldnode_findHeight(node->rightNode);
		difference = leftHeight - rightHeight;
	}
	else
		difference = 0;

	return difference;
}

// rebalancing function for height comparison and various movements to balance AVL
static void rebalance(TLDNode *node)
{
	int heightDiff = tldnode_compareHeight(node);
	int compare;
	
	if (node != NULL) {
		if (heightDiff > 1)
		{
			compare = tldnode_compareHeight(node->leftNode);

			if (compare > 0)
				node = rebalance_LL(node);
			else
				node = rebalance_LR(node);
		}
		else if (heightDiff < -1)
		{
			compare = tldnode_compareHeight(node->rightNode);

			if (compare > 0)
				node = rebalance_RL(node);
			else
				node = rebalance_RR(node);
		}
	}
}


// perform a left-left movement
static TLDNode *rebalance_LL(TLDNode *node)
{
	TLDNode *newNode = node->leftNode;
	node->leftNode = newNode->rightNode;
	newNode->rightNode = node;

	return newNode;
}

// perform a left-right movement
static TLDNode *rebalance_LR(TLDNode *node)
{
	TLDNode *newNode = node->leftNode;
	node->leftNode = rebalance_RR(newNode);

	return rebalance_LL(node);
}

// perform a right-right movement
static TLDNode *rebalance_RR(TLDNode *node)
{
	TLDNode *newNode = node->rightNode;
	node->rightNode = newNode->leftNode;
	newNode->leftNode = node;

	return newNode;
}

// perform a right-left movement
static TLDNode *rebalance_RL(TLDNode *node)
{
	TLDNode *newNode = node->rightNode;
	node->rightNode = rebalance_LL(newNode);

	return rebalance_RR(node);
}
