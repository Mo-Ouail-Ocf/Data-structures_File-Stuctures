#include <stdio.h>
#include <stdlib.h>
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

//queue node ptr
typedef struct queueNode *queueNodePtr;
queueNodePtr globalPriorityQueue;
//queue node
struct queueNode {
    bool isLeafNode;
    int ocurence;
    queueNodePtr nextQueueNode;
    union unionAttr unionAttr;
};



// queueLeafNode ptr
typedef struct queueLeafNode *queueLeafNodePtr;
// queueTreeNode ptr
typedef struct queueTreeNode *queueTreeNodePtr;

// Huffmann tree
typedef queueLeafNodePtr HuffmannTree;

//------------ Astract machine  //------------

// Leaf nodes :
queueLeafNodePtr createLeafNode(char character){
    queueLeafNodePtr ptr = malloc(sizeof(struct queueLeafNode));
    (*ptr).character = character;
    return ptr;
}

// queue tree node
queueTreeNodePtr createTreeNode(void*leftSon,void*rightSon){
    queueTreeNodePtr ptr = malloc(sizeof(struct queueTreeNode));
    ptr->leftSon=leftSon;
    ptr->rightSon=rightSon;
    return ptr;
}

// --------- queueNode
queueNodePtr createQueueNode( bool isLeafNode,int ocurence,queueNodePtr nextQueueNode,union unionAttr unionAttr){
    queueNodePtr ptr = malloc(sizeof(struct queueNode));
    ptr->isLeafNode=isLeafNode;
    ptr->ocurence=ocurence;
    ptr->nextQueueNode=nextQueueNode;
    ptr->unionAttr = unionAttr;
    return ptr;
}

queueNodePtr dequeue(){
    queueNodePtr ptr = globalPriorityQueue;
    globalPriorityQueue= globalPriorityQueue->nextQueueNode;
    return ptr;
}

bool enqueue(struct queueNode qNode){
    queueNodePtr ptr = createQueueNode(qNode.isLeafNode,qNode.ocurence,qNode.nextQueueNode,qNode.unionAttr);
    //insert the node into the right position regarding the order of the queue
    bool found = false; //found the right position

    queueNodePtr currNode = globalPriorityQueue;    
    queueNodePtr previousNode =NULL;

    if(globalPriorityQueue==NULL){
        globalPriorityQueue=ptr;
        return true;
    }
    else {
        while (!found)
        {
            if(currNode ==NULL){ // we reached the end of queue
                previousNode->nextQueueNode = ptr;
                found = true;
            }
            else{
                if ((currNode->ocurence)>=(ptr->ocurence)){ //insert in currNode
                    found = true;
                    if(previousNode==NULL){  //inserting in the head of the queue
                        ptr->nextQueueNode= globalPriorityQueue;
                        globalPriorityQueue = ptr;
                    }
                    else {
                        previousNode->nextQueueNode = ptr;
                        ptr->nextQueueNode = currNode;
                    }
                }
                else {
                    previousNode=currNode;
                    currNode= currNode->nextQueueNode;
                }
            }
        }
        return found;
    }
}


// A
int main(){

union unionAttr unionAttr= {{
    'a'
}};

struct queueNode qNode={
    true,2,NULL,unionAttr
};

enqueue(qNode);

qNode.ocurence=0;

enqueue(qNode);

qNode.ocurence=3;

enqueue(qNode);
qNode.ocurence=1;

enqueue(qNode);

printf("%d",globalPriorityQueue->ocurence);
}
