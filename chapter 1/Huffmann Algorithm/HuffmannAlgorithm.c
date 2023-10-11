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
///ADJAB REDA -----------------------------------------------------------------------
//0- GLOBAL FILES AND VARIABLES AND CONSTANTS
    //Test FILES
    //!MUST CREATE TEHM AND OPEN THEM WITH r OR OPEN THEM ACCORDING TO THEIR UTILIZATIONS IN THE FUNCTIONS
    FILE* originalFile;
    FILE* encodedFile;
    FILE* encodedChars; //store the huffman code of each used char in the text (Evident: Unique to each file and must be of format: "Char : Code\n")
    FILE* decodedFile;

    //Constants
    #define MAX_CHAR_NUMBER 128 //define a size for a table to store the frequency of each character
    #define MAX_CODE_LENGTH 128 //define the lenght of the huffman code of each char
    
    //Storing Variables
    char charCodeTable[MAX_CHAR_NUMBER][MAX_CODE_LENGTH]; //we are gonna use a table of strings, where the index represents the char and the string the huffman code+EOF

//1- Calculate the frequency of each car in the text :
    //-> Use array[128] ,where the index id the ascii of the character.  

    //initialize table at initializer
    #define freqInitializer 0
    void InitializeFreqTable(int* charFreqTable) {
        for(int i=0; i<MAX_CHAR_NUMBER; i++) {
            charFreqTable[i] = freqInitializer;
        }
    }

    //calculate the frequency of a character and store it in the coresspondent index of the table (index=ASCII(char))
    void CalculateFreqTable(int* charFreqTable) {
        char character;
        while((character = fgetc(originalFile)) != EOF) {
            charFreqTable[fgetc(originalFile)]++;
        }
    }
//1- END

//2- After getting the whole tree: 
    //1- We need to get the code of each character by traversing the tree,
    //we store them into the array (charCode, where the index represents the ascii code, where there is -1 means that the character does not exist in the file.
    //2- Based on the charCode array, we create the encodedFile.txt and we create encodedChars.txt for decompression.

    //2-Creating the encodedFile.txt and encodedChars.txt
    //? we are gonna use a table of strings, where the index represents the char and the string the huffman code+EOF

    //initialize table at initializer
    #define codeInitializer "\0"
    void InitializeCodeTable() {
        for(int i=0; i<MAX_CHAR_NUMBER; i++) {
            snprintf(charCodeTable[i], MAX_CODE_LENGTH*sizeof(char), codeInitializer);
        }
    }

    //use the table of huffman codes to replace each char with its code and put it in the encoded file
    void CompressFile() {
        fseek(originalFile, 0, SEEK_SET);
        char character;
        while ((character = fgetc(originalFile)) != EOF) {
            fputs(charCodeTable[character], encodedFile);
        }
    }

    //store the table of huffman codes in a file
    void StoreCharTable() {
        for(int i=0; i<MAX_CHAR_NUMBER; i++) {
            if (strcmp(charCodeTable[i], codeInitializer) != 0) {
                fprintf(encodedChars, "%c : %s\n", i, charCodeTable[i]);
            }
        }
    }
//2- END

//3- Ater compressing the file, now using EncodedChars.txt file we must decompress the EncodedFile.txt to restore the OriginalFile.txt
    
    //define the lenght of line buffer of encodedChars
    #define MAX_LINE_SIZE (MAX_CHAR_NUMBER+4) //bcz of the phrase "(char) : (code[MAX_CHAR_NUMBER])""

    //restore the huffman char code from the file to a new table instead since we know the number of characters
    void ReStoreCharTable() {
        char fileLine[MAX_LINE_SIZE];
        char character; //store character
        char charCode[MAX_CODE_LENGTH]; //store code
        while (fgets(fileLine, MAX_LINE_SIZE, encodedChars) > 0) {
            if (sscanf(fileLine, "%c : %s", &character, charCode) == 2) {
                // Store the code in the charCodeTable
                strcpy(charCodeTable[character], charCode);
                charCodeTable[character][MAX_CODE_LENGTH - 1] = '\0';  // Ensure null-termination
            }
        }
    }

    // Decompress the encodedFile.txt using the new table
    void DecompressFile() {
        //set to start of file
        fseek(originalFile, 0, SEEK_SET);

        char charCode; //store the char of the encoded file (0 or 1)
        char codeTable[MAX_CODE_LENGTH]; //store the char code
        int codeIndex = 0; //index the codeTable to store the code depending on its size (its equal to code size-1)
        int charNumber; //index the charCodeTable (equal to the char code askii)

        while ((charCode = fgetc(encodedFile)) != EOF) {
            codeTable[codeIndex++] = charCode; //store code
            codeTable[codeIndex] = '\0'; //index where code stops
            for(charNumber = 0; charNumber<MAX_CHAR_NUMBER; charNumber++) {
                if (strcmp(charCodeTable[charNumber], codeInitializer) != 0) { //char must exist
                    if ((strcmp(charCodeTable[charNumber], codeTable)) == 0) { //code must match   
                        fputc((char)charNumber, decodedFile); //store the charNumber as the character since it index the charCodeTable 
                        codeIndex = 0;
                        break; //stop testing
                    }
                }
            }
        }
    }
//3- END
 
///END -----------------------------------------------------------------------

///Ouail Mohammed Oucherif -----------------------------------------

// Initialize the queue by : traversing charFreqTable[] , creating queue nodes , enqueueing the nodes ,


// create huffmann tree by : traversing the queue , combining and spliting nodes , deleting queue nodes ... //? new functions


// travese huffmann tree , and get the code of each character into charCodeTable[] array .

 
///END -----------------------------------------------------------------------

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
