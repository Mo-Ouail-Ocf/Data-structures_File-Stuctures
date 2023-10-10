#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Queue struct node
struct queueLeafNode {
    char character;
};

// Queue_Tree struct node
struct queueTreeNode {
    void*leftSon;  
    void*rightSon;  
};

// Union attribute
union unionAttr {
    struct queueLeafNode queueLeafNode;
    struct queueTreeNode queueTreeNode;
}; 

//queue node
struct queueNode {
    bool isLeafNode;
    int ocurence;
    void*nextQueueNode;
    union unionAttr unionAttr;
};

// queueLeafNode ptr
typedef struct queueLeafNode *queueLeafNodePtr;
// queueTreeNode ptr
typedef struct queueTreeNode *queueTreeNodePtr;
//queue node ptr
typedef struct queueNode *queueNodePtr;
// Huffmann tree
typedef queueLeafNodePtr HuffmannTree;

int main(){

}
