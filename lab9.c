#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10 // Adjust the size of the hash table as needed

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for linked list in hash table
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Hash table structure with an array of pointers to nodes
struct HashType
{
    struct Node* head;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; // Simple modulus hashing for demonstration
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;
    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            printf("(%d, %c, %d) -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * HASH_SIZE);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
    {
        pHashArray[i].head = NULL; // Initialize each bucket head to NULL
    }

    // Hash and insert records into hash table
    for (i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        if (newNode == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        newNode->data = pRecords[i];
        newNode->next = pHashArray[index].head;
        pHashArray[index].head = newNode;
    }

    // Display records in the hash table
    displayRecordsInHash(pHashArray, HASH_SIZE);

    // Free memory
    for (i = 0; i < HASH_SIZE; ++i)
    {
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
    free(pRecords);

    return 0;
}
