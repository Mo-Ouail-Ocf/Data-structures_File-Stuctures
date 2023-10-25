#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


///Structures -----------------------------------------------------------------------
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

//------------ Abstract machine  //------------

// Leaf nodes :
queueLeafNodePtr createLeafNode(char character){
    queueLeafNodePtr ptr = malloc(sizeof(struct queueLeafNode));
    (*ptr).character = character;
    return ptr;
}

//function to create a leaf node of type QueueNode //!for testing
struct queueNode *ExampleCreateLeafNode(char character, int ocurence) {
    struct queueNode *leafNode = malloc(sizeof(struct queueNode));
    leafNode->isLeafNode = true;
    leafNode->ocurence = ocurence;
    
    leafNode->unionAttr.queueLeafNode.character = character;
    // Initialize other leaf node attributes if needed

    return leafNode;
}

// queue tree node
queueTreeNodePtr createTreeNode(void*leftSon,void*rightSon){
    queueTreeNodePtr ptr = malloc(sizeof(struct queueTreeNode));
    ptr->leftSon=leftSon;
    ptr->rightSon=rightSon;
    return ptr;
}

//function to create a nonleaf node of type QueueNode //!for testing
struct queueNode *ExampleCreateTreeNode(struct queueNode *leftSon, struct queueNode *rightSon) {
    struct queueNode *treeNode = malloc(sizeof(struct queueNode));
    treeNode->isLeafNode = false;
    treeNode->ocurence = leftSon->ocurence + rightSon->ocurence;

    // Use generic pointers to store left and right sons
    treeNode->unionAttr.queueTreeNode.leftSon = (void *)leftSon;
    treeNode->unionAttr.queueTreeNode.rightSon = (void *)rightSon;

    // Initialize other non-leaf node attributes if needed

    return treeNode;
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


//Travers tree modules
//example function to traverse and print Huffman codes
/* void printHuffmanCodes(struct queueNode *treeRoot, char *code, int depth) {
    if (treeRoot->isLeafNode) {
        //leaf node: Print character and Huffman code
        printf("Character: %c, Code: %s\n", treeRoot->unionAttr.queueLeafNode.character, code);
    } else {
        //non-leaf node: Recursively traverse left and right sons
        code[depth] = '0';
        code[depth + 1] = '\0';
        printHuffmanCodes((struct queueNode*)treeRoot->unionAttr.queueTreeNode.leftSon, code, depth + 1);

        code[depth] = '1';
        code[depth + 1] = '\0';
        printHuffmanCodes((struct queueNode*)treeRoot->unionAttr.queueTreeNode.rightSon, code, depth + 1);
    }
} */
//END

///END STRUCTURES-----------------------------------------------------------------------



///ADJAB REDA -----------------------------------------------------------------------
//0- GLOBAL FILES AND VARIABLES AND CONSTANTS
    //Test FILES
    //!MUST CREATE TEHM AND OPEN THEM WITH r OR OPEN THEM ACCORDING TO THEIR UTILIZATIONS IN THE FUNCTIONS
    FILE* originalFile;
    FILE* encodedFile;
    FILE* encodedFileBin;
    FILE* decodedFileBin;
    FILE* encodedChars; //store the huffman code of each used char in the text (Evident: Unique to each file and must be of format: "Char : Code\n")
    FILE* decodedFile;

    //Constants
    #define MAX_CHAR_NUMBER 128 //define a size for a table to store the frequency of each character
    #define MAX_CODE_LENGTH 128 //define the lenght of the huffman code of each char
    
    //Storing Variables
    char charCodeTable[MAX_CHAR_NUMBER][MAX_CODE_LENGTH]; //we are gonna use a table of strings, where the index represents the char and the string the huffman code+EOF
    long originalFileSize = 0; //store the size of the original file in bytes

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
            charFreqTable[character]++;
        }
    }
//1- END

//2- After getting the whole tree: 
    //1- We need to get the code of each character by traversing the tree,
    //we store them into the array (charCode, where the index represents the ascii code, where there is -1 means that the character does not exist in the file.
    //2- Based on the charCode array, we create the encodedFile.txt and we create encodedChars.txt for decompression.

    //1-Travese huffmann tree, and get the code of each character into charCodeTable[] array.
  
    //initialize table at initializer
    #define codeInitializer "\0"
    void InitializeCodeTable() {
        for(int i=0; i<MAX_CHAR_NUMBER; i++) {
            snprintf(charCodeTable[i], MAX_CODE_LENGTH*sizeof(char), codeInitializer);
        }
    }

    //traverse Huffman tree and store in charCodeTable[]
    void extractHuffmanCodes(struct queueNode *treeRoot, char *charCode, int depth) {
        if (treeRoot->isLeafNode) {
            //store the huffman code in the coresponding space
            strcpy(charCodeTable[treeRoot->unionAttr.queueLeafNode.character], charCode);
            charCodeTable[treeRoot->unionAttr.queueLeafNode.character][MAX_CODE_LENGTH - 1] = '\0';  //ensure null-termination
        } else {
            //non-leaf node: Recursively traverse left and right sons
            charCode[depth] = '0';
            charCode[depth + 1] = '\0';
            extractHuffmanCodes((struct queueNode*)treeRoot->unionAttr.queueTreeNode.leftSon, charCode, depth + 1);

            charCode[depth] = '1';
            charCode[depth + 1] = '\0';
            extractHuffmanCodes((struct queueNode*)treeRoot->unionAttr.queueTreeNode.rightSon, charCode, depth + 1);
        }
    }

    //2-Creating the encodedFile.txt and encodedChars.txt
    //? we are gonna use a table of strings, where the index represents the char and the string the huffman code+EOF

    //use the table of huffman codes to replace each char with its code and put it as a series of 1s and 0s
    void CompressFile() {
        //start of file
        fseek(originalFile, 0, SEEK_SET);

        char character; //store orirginal file character
        while ((character = fgetc(originalFile)) != EOF) {
            fputs(charCodeTable[character], encodedFile);
        }
    }

    //store the table of huffman codes in a file
    void StoreCharTable() {
        for(int i=0; i<MAX_CHAR_NUMBER; i++) {
            if (strcmp(charCodeTable[i], codeInitializer) != 0) {
                fprintf(encodedChars, "%c:%s\n", i, charCodeTable[i]);
            }
        }
    }
//2- END

//3- Ater compressing the file, now using EncodedChars.txt file we must decompress the EncodedFile.txt to restore the OriginalFile.txt
    
    //define the lenght of line buffer of encodedChars
    #define MAX_LINE_SIZE (MAX_CHAR_NUMBER+4) //bcz of the phrase "(char) : (code[MAX_CHAR_NUMBER])""

    //restore the huffman char code from the encodedFile.txt to charCodeTable since we wont need it in the contxt of decompressing
    void ReStoreCharTable() {
        //start of file
        fseek(encodedChars, 0, SEEK_SET); 

        char fileLine[MAX_LINE_SIZE];
        char character; //store character
        char charCode[MAX_CODE_LENGTH]; //store code
        while (fgets(fileLine, MAX_LINE_SIZE, encodedChars) > 0) {
            if (sscanf(fileLine, "%c:%s", &character, charCode) == 2) {
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

        long originalSize = originalFileSize;
        //restore original file size
        /* fread(&originalSize, sizeof(long), 1, decodedFileBin); */

        char charCode; //store the char of the encoded file (0 or 1)
        char codeTable[MAX_CODE_LENGTH]; //store the char code
        int codeIndex = 0; //index the codeTable to store the code depending on its size (its equal to code size-1)
        int charNumber; //index the charCodeTable (equal to the char code askii)
        long charsStored = 0; //keep track of how many chars were stored

        while (((charCode = fgetc(decodedFileBin)) != EOF) && (charsStored<originalSize)) {
            codeTable[codeIndex++] = charCode; //store code
            codeTable[codeIndex] = '\0'; //index where code stops
            for(charNumber = 0; charNumber<MAX_CHAR_NUMBER; charNumber++) {
                if (strcmp(charCodeTable[charNumber], codeInitializer) != 0) { //char must exist
                    if ((strcmp(charCodeTable[charNumber], codeTable)) == 0) { //code must match   
                        fputc((char)charNumber, decodedFile); //store the charNumber as the character since it index the charCodeTable 
                        codeIndex = 0;
                        charsStored++;
                        break; //stop testing
                    }
                }
            }
        }
    }
//3- END

//-4 After finishing the creation of all files, it is evident that the logic is correct, so we transform the encodedFile.txt into a binary file
//? encodedFile is a text file so a series of char each one holds a byt, while encodedFileBin is a bin file so a series of bits
    //use the encodedFile.txt to read each byte of 0 or 1 and store it as a bit series of 1s and 0s
    void CompressFileBin() {
        //start of file
        fseek(encodedFile, 0, SEEK_SET);

        //store original file size in bin file
        fwrite(&originalFileSize, sizeof(long), 1, encodedFileBin);        
        
        int bit; //store the 0 or 1 character
        int bitBuffer = 0; //store the series of bits to store
        int bitCount = 0; //store the number of bits stored
        while ((bit = fgetc(encodedFile)) != EOF) {    
            if (bit == '0' || bit == '1') {    
                //insert the new bit to the bitBuffer by shifting it to the left and adding 0 or 1 
                //that is by doing an OR operation of the bitbuffer byte and the byte (bit - '0')=(00000000) or (bit - '0')=(00000001)
                bitBuffer = (bitBuffer << 1) | (bit - '0');
                bitCount++;

                //if we have a full byte, write it to the binary file
                if (bitCount == 8) {
                    fputc(bitBuffer, encodedFileBin);
                    bitBuffer = 0;  //reset the buffer
                    bitCount = 0;   //reset the bit count
                }
            }    
        }
        //write remaining bits to the file
        if (bitCount > 0) {
            //insert the rest of bits to the bitBuffer by shifting it by (8 - bitCount)=(remaining unused bits) to the left
            bitBuffer = bitBuffer << (8 - bitCount);
            fputc(bitBuffer, encodedFileBin);
        }            
    }

    //use the encodedFileBin.bin to read each bit of 0 or 1 and store it as a series of 1s and 0s each one as a character in encodedFileBin.txt
    void DecompressFileBin() {
        //start of file
        fseek(encodedFileBin, 0, SEEK_SET);

        long originalSize;
        //restore original file size
        fread(&originalSize, sizeof(long), 1, encodedFileBin);

        int bit; //store the 0 or 1 bit
        int bitBuffer = 0; //store the series of bits to store
        int bitCount = 0; //store the number of bits stored

        while ((bit = fgetc(encodedFileBin)) != EOF) {
            //extract each bit from the byte and write it to the text file
            for (int i = 7; i >= 0; i--) {
                int currentBit = (bit >> i) & 1;
                char bitChar = (currentBit == 0) ? '0' : '1';
                fputc(bitChar, decodedFileBin);
            }
        }
    }
//-4 END


///END -----------------------------------------------------------------------

//test
//sui
int main(){
    //0 START FILE -----------------------------------------------------
    printf("\n\nPART 0: START FILE\n");
    originalFile = fopen("OriginalFile.txt", "r");
    if (originalFile == NULL) {
        perror("ERROOR READING <OriginalFile.txt> FILE");
        return 1;
    }

    fseek(originalFile, 0, SEEK_END);
    originalFileSize = ftell(originalFile);
    printf("%ld\n",originalFileSize);
    rewind(originalFile);

    //1 INITIALIZE -----------------------------------------------------
    printf("\n\nPART 1: INITIALIZE\n");
    int charFreqTable[MAX_CHAR_NUMBER];
    InitializeFreqTable(charFreqTable);
    CalculateFreqTable(charFreqTable);
    for (int i=0; i<MAX_CHAR_NUMBER; i++) {
        if (charFreqTable[i] != 0) {
            printf("char(%c) frequency is %d \n", i,charFreqTable[i]);
        }
    }

    //1.2 INITIALIZE TREE -----------------------------------------------------
    printf("\n\nPART 1.2: INITIALIZE TREE\n");

    //create the Huffman tree's leaf nodes manually
    struct queueNode *leaf_ = ExampleCreateLeafNode('_', 9);
    struct queueNode *leaf_a = ExampleCreateLeafNode('a', 2);
    struct queueNode *leaf_c = ExampleCreateLeafNode('c', 4);
    struct queueNode *leaf_d = ExampleCreateLeafNode('d', 2);
    struct queueNode *leaf_e = ExampleCreateLeafNode('e', 9);
    struct queueNode *leaf_f = ExampleCreateLeafNode('f', 1);
    struct queueNode *leaf_g = ExampleCreateLeafNode('g', 2);
    struct queueNode *leaf_i = ExampleCreateLeafNode('i', 2);
    struct queueNode *leaf_l = ExampleCreateLeafNode('l', 1);
    struct queueNode *leaf_m = ExampleCreateLeafNode('m', 3);
    struct queueNode *leaf_n = ExampleCreateLeafNode('n', 6);
    struct queueNode *leaf_o = ExampleCreateLeafNode('o', 5);
    struct queueNode *leaf_p = ExampleCreateLeafNode('p', 1);
    struct queueNode *leaf_r = ExampleCreateLeafNode('r', 3);
    struct queueNode *leaf_s = ExampleCreateLeafNode('s', 3);
    struct queueNode *leaf_t = ExampleCreateLeafNode('t', 4);
    struct queueNode *leaf_u = ExampleCreateLeafNode('u', 3);


    //create the initial Huffman tree with leaf nodes
    struct queueNode *node13 = ExampleCreateTreeNode(leaf_e, leaf_);
    struct queueNode *node9 = ExampleCreateTreeNode(leaf_o, leaf_t);
    struct queueNode *node3 = ExampleCreateTreeNode(leaf_a, leaf_d);
    struct queueNode *node8 = ExampleCreateTreeNode(leaf_c, node3);
    struct queueNode *node12 = ExampleCreateTreeNode(node9, node8);
    struct queueNode *node4 = ExampleCreateTreeNode(leaf_i, leaf_g);
    struct queueNode *node7 = ExampleCreateTreeNode(node4, leaf_u);
    struct queueNode *node11 = ExampleCreateTreeNode(node7, leaf_n);
    struct queueNode *node1 = ExampleCreateTreeNode(leaf_f, leaf_l);
    struct queueNode *node2 = ExampleCreateTreeNode(node1, leaf_p);
    struct queueNode *node5 = ExampleCreateTreeNode(leaf_m, node2);
    struct queueNode *node6 = ExampleCreateTreeNode(leaf_r, leaf_s);
    struct queueNode *node10 = ExampleCreateTreeNode(node5, node6);
    struct queueNode *node14 = ExampleCreateTreeNode(node11, node10);
    struct queueNode *node15 = ExampleCreateTreeNode(node13, node12);
    struct queueNode *root = ExampleCreateTreeNode(node15, node14);


    //extract Huffman codes
    InitializeCodeTable();
    char code[MAX_CODE_LENGTH];
    code[0] = '\0';   //initialize code
    extractHuffmanCodes(root, code, 0);

    //2 COMPRESS -----------------------------------------------------
    printf("\n\nPART 2: COMPRESS\n");
    encodedFile = fopen("EncodedFile.txt","w");
    if (encodedFile == NULL) {
        perror("ERROOR READING <EncodedFile.txt> FILE");
        return 1;
    }
    encodedFileBin = fopen("EncodedFileBin.bin","wb");
    if (encodedFileBin == NULL) {
        perror("ERROOR READING <EncodedFileBin.bin> FILE");
        return 1;
    }

    //initialize the code table with teh slides example
/*     strcpy(charCodeTable['a'], "01110");
    strcpy(charCodeTable['c'], "0110");
    strcpy(charCodeTable['d'], "01111");
    strcpy(charCodeTable['e'], "000");
    strcpy(charCodeTable['f'], "110100");
    strcpy(charCodeTable['g'], "10001");
    strcpy(charCodeTable['i'], "10000");
    strcpy(charCodeTable['l'], "110101");
    strcpy(charCodeTable['m'], "1100");
    strcpy(charCodeTable['n'], "101");
    strcpy(charCodeTable['o'], "0100");
    strcpy(charCodeTable['p'], "11011");
    strcpy(charCodeTable['r'], "1110");
    strcpy(charCodeTable['s'], "1111");
    strcpy(charCodeTable['t'], "0101");
    strcpy(charCodeTable['u'], "1001");
    strcpy(charCodeTable['_'], "001"); */

    CompressFile();

    fclose(encodedFile);
    fclose(encodedFileBin);

    //3 STORE -----------------------------------------------------
    printf("\n\nPART 3: STORE\n");
    encodedChars = fopen("EncodedChars.txt","w");
    if (encodedChars == NULL) {
        perror("ERROOR READING <EncodedChars.txt> FILE");
        return 1;
    }

    StoreCharTable();
    InitializeCodeTable();

    fclose(encodedChars);

    //4 RESTORE -----------------------------------------------------
    printf("\n\nPART 4: RESTORE\n");
    encodedChars = fopen("EncodedChars.txt","r");
    if (encodedChars == NULL) {
        perror("ERROOR READING <EncodedChars.txt> FILE");
        return 1;
    }
    ReStoreCharTable();
    for (int i = 0; i < MAX_CHAR_NUMBER; i++) {
        if (strcmp(charCodeTable[i], codeInitializer) != 0) {
            printf("Character: %c, Code: %s\n", i, charCodeTable[i]);
        }
    }

    //5 DECOMPRESS -----------------------------------------------------
    printf("\n\nPART 5: DECOMPRESS\n");
    decodedFile = fopen("DecodedFile.txt","w");
    if (decodedFile == NULL) {
        perror("ERROOR READING <DecodedFile.txt> FILE");
        return 1;
    }
    encodedFile = fopen("EncodedFile.txt","r");
    if (encodedFile == NULL) {
        perror("ERROOR READING <EncodedFile.txt> FILE");
        return 1;
    }
    decodedFileBin = fopen("DecodedFileBin.txt","r");
    if (decodedFileBin == NULL) {
        perror("ERROOR READING <DecodedFileBin.txt> FILE");
        return 1;
    }

    DecompressFile();

    fclose(decodedFileBin);

    //6 COMPRESS ENCODED TXT FILE TO BIN FILE -----------------------------------------------------
    printf("\n\nPART 6: COMPRESS IN BIN\n");
    encodedFileBin = fopen("EncodedFileBin.bin","wb");
    if (encodedFileBin == NULL) {
        perror("ERROOR READING <EncodedFileBin.bin> FILE");
        return 1;
    }

    CompressFileBin();

    fclose(encodedFileBin);

    //7 DECOMPRESS BIN FILE FILE TO ENCODED TXT -----------------------------------------------------
    printf("\n\nPART 7: DECOMPRESS BIN FILE TO ENCODED TXT\n");
    decodedFileBin = fopen("DecodedFileBin.txt","w");
    if (decodedFileBin == NULL) {
        perror("ERROOR READING <DecodedFileBin.txt> FILE");
        return 1;
    }
    encodedFileBin = fopen("EncodedFileBin.bin","rb");
    if (encodedFileBin == NULL) {
        perror("ERROOR READING <EncodedFileBin.bin> FILE");
        return 1;
    }

    DecompressFileBin();


    fclose(originalFile);
    fclose(encodedFile);
    fclose(encodedFileBin);
    fclose(decodedFileBin);
    fclose(encodedChars);
    fclose(decodedFile);

    return 0;
}
