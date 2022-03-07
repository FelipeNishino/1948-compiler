//
// Created by Nishi on 07/03/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_INSTRUCTIONS 2000
#define MAX_VAR_NAME_SIZE 8
#define SIZE 20
#define ASCII_ZERO 48
#define ASCII_NINE 57
#define ASCII_LOWERA 97
#define ASCII_LOWERZ 122
#define ASCII_UPPERA 65
#define ASCII_UPPERZ 90

typedef struct reg {
    char *name;
    int type;
} Register;

typedef struct registerData {
    Register reg;
    int key;
} RegisterData;

typedef struct reg_node {
    Register reg;
    void *next;
} RegisterNode;

typedef struct instruction {
    int op;
    char **tokens;
} Instruction;

typedef struct range {
    int start;
    int end;
} Range;

enum {
    declaration,
    sum
};

enum {
    literal,
    expression
};

RegisterNode *head = NULL;
RegisterData *hashArray[SIZE];
RegisterData *dummyItem;

int inRange(int nArgs, ...) {
    va_list valist;
//    printf("Received nArgs {%d}", nArgs);
    va_start(valist, nArgs);
    int result = 0, i, n;
    Range range;
//    printf("Reading args from valist\n");
    for (i=0; i < nArgs; i++) {
        if (i == 0) {
            n = va_arg(valist, int);
//            printf("Read param n with value {%d}\n", n);
        }
        else {
            range = va_arg(valist, Range);
//            printf("read param range %d with values %d and %d\n", i, range.start, range.end);
            result = (n-range.start)*(n-range.end) <= 0 ? 1 : result;
//            printf("Checked if is in range, result {%d}\n", result);
        }
    }

    va_end(valist);

    return result;
}

//display the list
//void printList() {
//    RegisterNode *ptr = head;
//    printf("\n[ ");
//
//    //start from the beginning
//    while(ptr != NULL) {
//        printf("(%d,%d) ",ptr->key,ptr->data);
//        ptr = ptr->next;
//    }
//
//    printf(" ]");
//}

//insert link at the first location
void insertFirst(Register data) {
    //create a link
    RegisterNode *link = (RegisterNode *) malloc(sizeof(RegisterNode ));

//    link->key = key;
    link->reg = data;

    //point it to old first node
    link->next = head;

    //point first to new first node
    head = link;
}

//delete first item
RegisterNode *deleteFirst() {

    //save reference to first link
    RegisterNode *tempLink = head;

    //mark next to first link as first
    head = head->next;

    //return the deleted link
    return tempLink;
}

//is list empty
int isEmpty() {
    return head == NULL;
}

int length() {
    int length = 0;
    RegisterNode *current;

    for(current = head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}

//find a link with given key
RegisterNode *find(char *key) {

    //start from the first link
    RegisterNode *current = head;

    //if list is empty
    if(head == NULL) {
        return NULL;
    }

    //navigate through list
    while(strcmp(current->reg.name, key) != 0) {
        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //go to next link
            current = current->next;
        }
    }

    //if data found, return the current Link
    return current;
}

//delete a link with given key
RegisterNode *delete(char key) {

    //start from the first link
    RegisterNode *current = head;
    RegisterNode *previous = NULL;

    //if list is empty
    if(head == NULL) {
        return NULL;
    }

    //navigate through list
    while(strcmp(current->reg.name, key) != 0) {

        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if(current == head) {
        //change first to point to next link
        head = head->next;
    } else {
        //bypass the current link
        previous->next = current->next;
    }

    return current;
}

unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}



int hashCode(int key) {
    return key % SIZE;
}

RegisterData *search(int key) {
    int hashIndex = hashCode(key);

    while(hashArray[hashIndex] != NULL) {
        if(hashArray[hashIndex]->key == key)
            return hashArray[hashIndex];

        hashIndex = (hashIndex + 1) % SIZE;
    }

    return NULL;
}

void insert(int key, Register reg) {
    RegisterData *item = (RegisterData *) malloc(sizeof(RegisterData ));
    item->reg = reg;
    item->key = key;

    int hashIndex = hashCode(key);

    while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        hashIndex = (hashIndex + 1) % SIZE;
    }

    hashArray[hashIndex] = item;
}

//RegisterData * delete(RegisterData * item) {
//    int key = item->key;
//    int hashIndex = hashCode(key);
//
//    while(hashArray[hashIndex] != NULL) {
//        if(hashArray[hashIndex]->key == key) {
//            RegisterData * temp = hashArray[hashIndex];
//
//            hashArray[hashIndex] = dummyItem;
//            return temp;
//        }
//
//        hashIndex = (hashIndex + 1) % SIZE;
//    }
//
//    return NULL;
//}

void display() {
    int i = 0;

    for(i = 0; i<SIZE; i++) {

        if(hashArray[i] != NULL)
            printf(" (%d,%s)",hashArray[i]->key,hashArray[i]->reg.name);
        else
            printf(" ~~ ");
    }

    printf("\n");
}

int hashkey(char *key) {
    return hash(key) % SIZE;
}

Instruction extractInstructionData(char *codeline) {
    int i = 0, step = 0;
    char *token;
    Instruction inst;
    inst.tokens = malloc(sizeof(char*));

    token = strtok(codeline, " ");

//    printf("Extracting tokens...\n");
    while( token != NULL ) {
//        printf("current token {%s}\n", token);
        switch (step++) {
            case 0:
                inst.tokens[0] = malloc(strlen(token) + 1);
                strcpy(inst.tokens[0], token);
                break;
            case 1:
                if (strcmp(token, ":=") != 0){
                    token = NULL;
                    continue;
                }
                break;
            case 2:
                inst.tokens[1] = calloc(strlen(token) + 1, sizeof(char));
//                inst.tokens[1] = malloc(11 * sizeof(char));
                strcpy(inst.tokens[1], token);
                break;
            case 3:
                if (strcmp(token, "+") != 0){
                    token = NULL;
                    continue;
                }
                break;
            case 4:
                inst.tokens[2] = calloc(11, sizeof(char));
                strcpy(inst.tokens[2], token);
                break;
            default:
                break;
        }
        token = strtok(NULL, " ");
    }
    if (step == 2 || step == 4)
        return (Instruction) {.tokens = NULL, .op = -1};

    inst.op = (step == 5) ? sum : declaration;
    return inst;
}

int setRegister(Instruction inst) {
    Register reg;
    int i, nameSize = strlen(inst.tokens[0]);
//    printf("checking if varname is within size limit...");
    if (nameSize > 8)
        return 0;
//    printf(" done\n setting name to register var\n");
    reg.name = malloc(nameSize * sizeof(char));
    strcpy(reg.name, inst.tokens[0]);

    switch (inst.op) {
        case declaration:
//            printf("Instruction is a declaration, setting type to literal\n");
            reg.type = literal;
            break;
        case sum:
//            printf("Instruction is a sum, checking proper type to set\n");
            reg.type = literal;
            for (i = 1; i <= 2; i++) {
                if (inRange(3, inst.tokens[i][0], (Range) {.start = ASCII_LOWERA, .end = ASCII_LOWERZ}, (Range) {.start = ASCII_UPPERA, .end = ASCII_UPPERZ}))
                    reg.type = expression;
            }
            break;
    }
//    printf("identified type {%d}, checking if var is already declared\n", reg.type);
//    if (search(hashkey(reg.name))) {
//        printf("Detected redeclaration of key {%d}, returning error\n", hashkey(reg.name));
//        return 0;
//    }
//    printf("var wasn't used before, inserting into hash... ");
//    insert(hashkey(reg.name), reg);
//    printf("done, inserted with key {%d}\n returning success\n", hashkey(reg.name));

//    printf("identified type {%d}, checking if var is already declared\n", reg.type);
    if (find(reg.name)) {
//        printf("Detected redeclaration of key {%d}, returning error\n", hashkey(reg.name));
        return 0;
    }
//    printf("var wasn't used before, inserting into hash... ");
    insertFirst(reg);
//    printf("done, inserted with key {%d}\n returning success\n", hashkey(reg.name));
    return (find(inst.tokens[1]));
}

int main() {
    int i, j, nInstructions;
    char currentChar;
    Instruction instruction;
    char **instructions = malloc(MAX_INSTRUCTIONS * sizeof(char*));
    for (i = 0; i < MAX_INSTRUCTIONS; i++) {
        instructions[i] = malloc((MAX_VAR_NAME_SIZE * 3 + 8) * sizeof(char));
        for (j = 0; j < MAX_VAR_NAME_SIZE * 3 + 8; j++)
            instructions[i][j] = 0;
    }
//    instructions = calloc(MAX_INSTRUCTIONS, (MAX_VAR_NAME_SIZE * 3 + 8) * sizeof(char));

    i = 0;
    j = 0;

    while (1) {
        currentChar = getchar();
        if (currentChar == 10) {
            if (j == 0)
                break;
            else {
                j = 0;
                i++;
                continue;
            }
        }
        instructions[i][j++] = currentChar;
    }

    nInstructions = i;

//    printf("Input recebido:\n");
//    for (j = 0; j < i; j++)
//        printf("%s\n", instructions[j]);

    for (i = 0; i < nInstructions; i++) {
//        printf("Instruction %d\n", i);
        instruction = extractInstructionData(instructions[i]);
//        printf("Extracted data: op{%d}, t0{%s}, t1{%s}", instruction.op, instruction.tokens[0], instruction.tokens[1]);
//        if (instruction.op == sum)
//            printf(", t2{%s}", instruction.tokens[2]);
//        printf("\nChecking if var has valid name... ");
        for (j = 0; j < strlen(instruction.tokens[0]); j++) {
//            printf("Verifying char {%c}, at {%d}", instruction.tokens[0][j], j);
            if (!inRange(3, instruction.tokens[0][j], (Range) {.start = ASCII_LOWERA, .end = ASCII_LOWERZ}, (Range) {.start = ASCII_UPPERA, .end = ASCII_UPPERZ})) {
                printf("Compilation error\n");
                return 0;
            }
        }
//        printf("done\n");
//        printf("Trying to set Register\n");
        if (!setRegister(instruction)) {
            printf("Compilation error\n");
            return 0;
        }
    }

    printf("OK\n");

    return 0;
}

//    while (codeline[i] != "\0") {
//        if (codeline[i] ==  " ") {
//            switch (step++) {
//                case 0:
//                    name = (Bounds) {.start = 0, .end = i};
//                    break;
//                case 1:
//                    val1 = (Bounds) {.start = name.end+1, .end = i};
//                    break;
//                case 2:
//                    val2 = (Bounds) {.start = val1.end+1, .end = i};
//                    break;
//            }
//        }
//        i++;
//
//
//    }
