
#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"

struct node* create(char val_root)
{
	struct node* new_node = malloc(sizeof(struct node));

	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->value = val_root;

	return new_node;
}

struct node* insert(char value, struct node* input_tree) {

    if (input_tree == NULL) {
        struct node* new_node = (struct node*) malloc (sizeof (struct node));

        if (new_node == NULL) {
            fprintf(stderr, "Failed memory allocation in insert function.\n");
            exit(EXIT_FAILURE);
        }

        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }

    // If input_tree is not empty, recursion on subtree
    if (value < input_tree->value) {
        input_tree->left = insert(value, input_tree->left);
        input_tree->left->parent = input_tree;
    }
    if (value > input_tree->value) {
        input_tree->right = insert(value, input_tree->right);
        input_tree->right->parent = input_tree;
    }

    // In case of duplicate value, no insert.
    return input_tree;
}

void destroy(struct node* input_tree)
{
	if (input_tree->left != NULL)
		destroy (input_tree->left);
	if (input_tree->right != NULL)
		destroy (input_tree->right );

	free (input_tree);
}


int isLeaf(struct node* input_node)
{
	if ( input_node->left == NULL && input_node->right == NULL ) {
	    return 1;
	}
	else {
	    return 0;
    }
}


struct node* minimum(struct node* input_tree)
{
	while (input_tree->left != NULL) {
	    input_tree = input_tree->left;
	}

	return input_tree;
}


struct node* minimumLeaf(struct node* input_tree)
{
	input_tree = minimum(input_tree);

	// If not leaf, go right and find new minimum, iterate if needed.
	while ( ! isLeaf(input_tree) ) {
		input_tree = input_tree->right;
		input_tree = minimum(input_tree);
	}

	return input_tree;
}


// prende in input una foglia e ritorna la foglia successiva a destra o NULL se non esiste
struct node* nextLeaf (struct node* input_leaf)
{

   	if ( !isLeaf(input_leaf) ) {
	    printf("Input error in nextLeaf.\n");
	    exit(EXIT_FAILURE);
	}

	if (input_leaf->parent == NULL) { // If it is root.
	    return NULL;
	}

	struct node* current_node = input_leaf;
	struct node* new_leaf = NULL;


	while ( new_leaf == NULL ) {

		if (current_node->parent == NULL) { // If we have reached the root.
			 break; // Go to return new_leaf which is NULL (while guard).
		}

        // If current_node is the left child.
		if ( current_node == current_node->parent->left ) {

			current_node = current_node->parent; // Move up the tree.

            // If it has a right child, finds the minimum leaf in that subtree.
			if (current_node->right != NULL) {
				new_leaf = minimumLeaf(current_node->right);
			}
			else {
				// If there isn't right child, iterate the while.
			}
		}

		// Current_node is the right child. Move up the tree and re-execute
		// the while loop.
		else {
			current_node = current_node->parent;
		}
	} // End while.

	return new_leaf;
}


struct node* minimumWithDistance(struct node* input_tree, unsigned int* distance)
{
    unsigned int counter = 0;

	while (input_tree->left != NULL) { // var input_tree reused as node.
		input_tree = input_tree->left;
		counter ++;
	}

	*distance = counter;

	return input_tree;
}


struct node* minimumLeafWithDistance(struct node* input_tree, unsigned int* distance)
{
	input_tree = minimumWithDistance (input_tree, distance);
	// var input_tree reused as node.

	// If not leaf go right and search minimum leaf again.
	while ( ! isLeaf(input_tree) ) {
		input_tree = input_tree->right;
		(*distance)++;
		input_tree = minimumWithDistance(input_tree, distance);
	}

	return input_tree;
}


// auxiliary function for nextLeafWithDistance
void decrementConditional(unsigned int* distance, int decrease)
{
	if (decrease == 1) {
	    (*distance)--;
	}
	else{
	    (*distance)++;
	}
}



struct node* nextLeafWithDistance (struct node* leaf, unsigned int* distance, struct node** pointer_node_change_direction)
{

    if ( !isLeaf(leaf) ) {
        printf("Input error in nextLeafWithDistance.\n");
	    exit(EXIT_FAILURE);
    }

	struct node* current_node = leaf;
	struct node* next_leaf = NULL;
	int decrease = 1;

    // if we are at the first call of the function, node_change_direction needs to be set.
    // node_change_direction is used as flagged node to understang if we have
    // to move up or down the tree.
	if ( *pointer_node_change_direction == NULL ) {

	    // First time we search for the next leaf, which could not exists.
	    // node_change_direction will be set inside the while.
		while (next_leaf == NULL) {

				// Check if current_node is root:
				// true) tree has the root as leaf, no next leaf.
				// false) keeps searching.

				if ( current_node->parent == NULL ) {
				    return NULL;
				}
                // else
                // If current_node is left child.
				if ( current_node == current_node->parent->left ) {
					current_node = current_node->parent;
					(*distance)++;

					if ( current_node->right != NULL ) { // If it has a right child.

					    // It flags current_node as the node where direction change.
						*pointer_node_change_direction = current_node;
// printf("\nValue of node_change_direction %c\n", current_node->value);
						current_node = current_node->right;
						(*distance)++;

						// Finds next right leaf.
						current_node = minimumLeafWithDistance(current_node, distance);
						next_leaf = current_node;
					}
				}
				// If current_node is right child, move up and repeat the while.
				else {
					current_node = current_node->parent;
					(*distance)++;
				}
		} // End of while loop.
	} // End of the branch, where we are at the first call of the function.

    // Not the first call, pointer_node_change_direction is already set.
	else {
		while (next_leaf == NULL) {

			// Explanations as above.
			if ( current_node->parent == NULL ){
			    return NULL;
			}

// printf("current_node %c-", current_node->value);

			// If current_node is left child.
			if ( current_node == current_node->parent->left ) {

				current_node = current_node->parent;
				decrementConditional (distance, decrease);
				// decrease is 1 so distance is decremented by one.

				// If we already passed the node_change_direction, we "push up" the flag.
				if(decrease == 0) {
					*pointer_node_change_direction = current_node;
				}

                // If we have reached the flagged node.
				if (current_node == *pointer_node_change_direction) {
					decrease = 0;
				}

				if ( current_node->right != NULL ) {// Exists right subtree.
					(*distance)++;
					current_node = current_node->right;
					next_leaf = minimumLeafWithDistance (current_node, distance);
//printf("nuova foglia %c distance %d\n", next_leaf->value, *distance);
				}
			}

			// If current_node is right child.
			else {
				current_node = current_node->parent;
				decrementConditional(distance, decrease);

				if(decrease == 0) { // If we already passed node_change_direction
					*pointer_node_change_direction = current_node;
				}

				if (current_node == *pointer_node_change_direction) {
				    decrease=0;
				}

			}
		} // End while loop.
	}// End if else.

//printf("next_leaf %c\n", next_leaf->value);
	return next_leaf;
}


void printer(struct node* root)
{
	if (root->left != NULL) {
	    printer(root->left);
	}

	printf("value %c\n", root->value);
	printf("address structure %p\n", (void*)root);
	printf("left %p\n", (void*)root->left);
	printf("right %p\n", (void*)root->right);
	printf("parent %p\n\n", (void*)root->parent);

	if (root->right != NULL) {
	    printer(root->right);
	}

}


// main for testing
int main(void)
{
	struct node* root;
/*
	    f
       / \
      e   g
     /     \
    d       h
   /         \
  b           q
 / \         / \
a   c       p   r
                 \
                  t
                 / \
                s   u
*/
	root = create('f');

    root = insert('f', root);
    root = insert('e', root);
    root = insert('d', root);
    root = insert('b', root);
    root = insert('a', root);
    root = insert('c', root);
    root = insert('g', root);
    root = insert('h', root);
    root = insert('q', root);
    root = insert('p', root);
    root = insert('r', root);
    root = insert('t', root);
    root = insert('s', root);
    root = insert('u', root);

	// printer(root); // Print entire tree.

    printf("\nNext leaf of a is c:\n");
    printer( nextLeaf (root->left->left->left->left));
    printf("\nNext leaf of c is p:\n");
	printer( nextLeaf (root->left->left->left->right));
	printf("\nNext leaf of p is s:\n");
	printer( nextLeaf (root->right->right->right->left));
	printf("\nNext leaf of s is u:\n");
	printer( nextLeaf (root->right->right->right->right->right->left));

	printf("\nNext leaf of u doesn't exists:\n");
	if (nextLeaf (root->right->right->right->right->right->right ) == NULL)
		printf("yes\n");

	return 0;
}

