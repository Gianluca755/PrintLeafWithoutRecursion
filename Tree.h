// header for tree functions

#ifndef TREE_H
#define TREE_H

struct node
{
	char value;
	struct node* parent;
	struct node* left;
	struct node* right;
};


// Create tree and initialize root.
struct node* create(char val_root);    // ϴ(1)

// Create node with value "value" and insert it in the input_tree.
struct node* insert(char value, struct node* input_tree); // ϴ(h) tree height, O(n)

// Delete the tree.
void destroy(struct node* input_tree); // ϴ(n)

// Verify that input_node is a leaf (both child are NULL).
int isLeaf(struct node* input_node);   // ϴ(1)


// Find node with minimum value in the input_tree.
struct node* minimum(struct node* input_tree);     // ϴ(h) tree height, O(n)

// Find leaf with minimum value in the input_tree.
struct node* minimumLeaf(struct node* input_tree); // ϴ(h) tree height, O(n)

// Takes in input a leaf and returns the pointer to the next leaf in
// the tree (in-order) or NULL if there isn't a next leaf
struct node* nextLeaf(struct node* input_leaf);     // ϴ(h) tree height, O(n)

/* Given input_tree, it finds the node with minimum value in that sub-tree,
   the pointer of the node found is returned and the number of nodes
   between is counted and salved in distance.
   Complexity as the plain version above. */
struct node* minimumWithDistance (struct node* input_tree, unsigned int* distance);

// Similar to the previous function but the node found must be a leaf.
// Complexity as the plain version above.
struct node* minimumLeafWithDistance (struct node* input_tree, unsigned int* distance);

/* Given input_leaf, it finds the next leaf and the distance between the two.
   The function requires as input pointer_node_change_direction, which points
   to the node that indicates where the search needs to change direction.
   If it is the first execution it needs point to a location with value NULL.
   The function returns NULL if there is no next leaf.
   Complexity as the plain version above. */
struct node* nextLeafWithDistance( struct node* input_leaf, unsigned int* distance,
                                   struct node** pointer_node_change_direction
                                 );


// Print on teminal all the details of input_tree. Recursive in-place traversal.
void printer(struct node* input_tree); // ϴ(n)

#endif

