/*************************
 * Project 3 "24 part 2"
 * Christian Martin
 * Professor Kidane
 * 04/14/23
**************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

// linked list structure for stacks
typedef struct Stack *StackPtr; // Make:  NodePtr to be a synonym for: struct Node *
	struct Stack {
  		char data; // The data stored at the node
  		StackPtr pNext; // Pointer to the next node
};

// function definiton for options at the end of the game, actually function is written at line 607
void postGameOptions(char difficulty, StackPtr opsHead, StackPtr parensHead); // declare here so can be called anywhere



// checks if the stack is empty 
bool isEmpty(StackPtr pHead) {
    if (pHead == NULL) {
        return true;
    }
    else {
        return false;
    }
}



// checks if the given char is an operator 
bool isOperator(char a) {
    switch(a) {
    case '+':
    case '-':
    case '/':
    case '*': 
    break;
    default: 
        return false;
    }
        return true;
}



// pushes a char onto the top of the stack 
void push_back(StackPtr *pHead, char symbol) {
    StackPtr newNode = malloc(sizeof(struct Stack));
    newNode->data = symbol;
    newNode->pNext = NULL;
    if (*pHead == NULL) {
        *pHead = newNode;
    } else {
        StackPtr pCurrent = *pHead;
        while (pCurrent->pNext != NULL) {
            pCurrent = pCurrent->pNext;
        }
        pCurrent->pNext = newNode;
    }
}




// pops the char at the top of the stack
char pop_back(StackPtr *pHead) { 
    if (*pHead == NULL) {
        printf("Error: stack is empty\n");
        return '\0';
    }
    char data;
    StackPtr pCurr = *pHead;
    StackPtr pPrev = NULL;
    while (pCurr->pNext != NULL) {
        pPrev = pCurr;
        pCurr = pCurr->pNext;
    }
    if (pPrev == NULL) { // only one node in the list
        data = (*pHead)->data;
        free(*pHead);
        *pHead = NULL;
    } else {
        data = pCurr->data;
        free(pCurr);
        pPrev->pNext = NULL;
    }
    return data;
}



// pops the char at the bottom of the stack 
char pop_front(StackPtr *pHead) { 
    if (*pHead == NULL) {
        printf("Error: stack is empty\n");
        return '\0';
    }
    char data = (*pHead)->data;
    StackPtr pTemp = *pHead;
    *pHead = (*pHead)->pNext;
    free(pTemp);
    return data;
}



// returns the char at the top of the stack
char at_back(StackPtr pHead) { 
    if (pHead == NULL) {
        printf("Error: stack is empty\n");
        return '\0';
    }
    while (pHead->pNext != NULL) {
        pHead = pHead->pNext;
    }
    return pHead->data;
}



// frees the nodes in the stack 
void freeStack(StackPtr *pHead) {
    while (*pHead != NULL) {
        StackPtr temp = *pHead;
        *pHead = (*pHead)->pNext;
        free(temp);
    }
}



// checks if the parenthesis are balanced
int CheckBalanced(StackPtr *pHead, char userString[ 81]) {
    int stackSize = 0;
    for (int i = 0; i < strlen(userString); i++) {
        if (userString[i] == '(') {
        push_back(pHead, userString[i]);
        stackSize++;
        }
        if (userString[i] == ')') {
            if (isEmpty(*pHead)) {
                return -1; // too many closing parenthesis
            }
        pop_back(pHead);
        stackSize--;
        }
    }
    if (isEmpty(*pHead)) {
            return 0; // is balanced
    }
    freeStack(pHead);
    return 2; // too many opening parenthesis, this works btw 
}



// checks the precedence of the operators 
int CheckPrecedence(char op)
{
    switch (op) {
        case '+':
        case '-':
            return 1; // second lowest precedence
        case '*':
        case '/':
            return 2; // highest precedece
        case '(':
        case ')':
            return 0; // lowest for postfixing
        default:
            return -1;
    }
}

int CheckAmntNumsUsed(char userString[ 81]) {
    int counter = 0;
    for (int i = 0; i < strlen(userString); i++) {
        if (isdigit(userString[i])) {
            counter++;
        }
    }
    if (counter == 4) { // all numbers used
        return 1;
    }
    else {
        return -1;
    }
}


// checks if all the numbers have been used
int CheckNumsUsed(char a, char b, char c, char d, char userString[ 81]) {
    int counter = 0;
    char tempString[ 81];
    strcpy(tempString, userString);
    // replaces index with rand char so duplicates not counted
    for (int i = 0; i < strlen(tempString); i++) {
        if (tempString[i] == a) {
            tempString[i] = '!'; 
            counter++;
            break;
        }
    }
    for (int i = 0; i < strlen(tempString); i++) {
        if  (tempString[i] == b) {
            tempString[i] = '!';
            counter++;
            break;
        }
    }
    for (int i = 0; i < strlen(tempString); i++) {
        if (tempString[i] == c) {
            tempString[i] = '!';
            counter++;
            break;
        }
    }
    for (int i = 0; i < strlen(tempString); i++) {
        if  (tempString[i] == d) {
            tempString[i] = '!';
            counter++;
            break;
        }
    }
    return counter;  
}



// checks if the char in the string is an operator or parentehses
bool CheckOperators(char userString[ 81]) {
    for (int i = 0; i < strlen(userString); i++) {
        if (isdigit(userString[i])) {
            continue;
        }
        if (isspace(userString[i])) {
            continue;
        }
        switch(userString[i]) {
            case ')':
            case '(':
            case '+':
            case '-':
            case '/':
            case '*':
                break;
            default: 
                return true;
        }
    }
    return false;
}



// evaluates the expression, printing out sub answers along the way 
void evaluation(int *answer, char postfixExpr[ 81], int idiotChecker, int isBalanced, int *error) {
    int tempArray[ 81];
    int index = -1;
    int timesOperated = 0;
    int parens = 0;
    for (int i = 0; postfixExpr[i] != '\0'; i++) {
        char symbol = postfixExpr[i];
        if (isdigit(symbol)) {
            tempArray[++index] = symbol - 48; // ascii char to int 
        } 
        // balance paretheses in parrallel with evaluation 
        else if (postfixExpr[i] == '(') {
            parens++;
        }
        else if (postfixExpr[i] == ')') {
            parens--;
        }
        else if (isOperator(symbol)) {
            int num2 = tempArray[index--];
            int num1 = tempArray[index--];
            int tempAnswer;
            switch (symbol) {
                case '+':
                    if (parens == -1) { // if num closed parethenses falls below zero, unbalanced
                        break;
                    }
                    tempAnswer = num1 + num2;
                    break;
                case '-':
                    if (parens == -1) {
                        break;
                    }
                    tempAnswer = num1 - num2;
                    break;
                case '*':
                    if (parens == -1) {
                        break;
                    }
                    tempAnswer = num1 * num2;
                    break;
                case '/':
                    if (parens == -1) {
                        break;
                    }
                    if (num2 == 0) {
                        printf("cant divide by zero fam lmfaoooo\n");
                        exit(0);
                    }
                    tempAnswer = num1 / num2;
                    break;
            }
            timesOperated++;
            if (parens == -1) {
                break;
            }
            tempArray[++index] = tempAnswer;
            printf("%d %c %d = %d.\n", num1, symbol, num2, tempAnswer);
        }
    }
    if (timesOperated != 3) {
        printf("Error! Too many values in the expression.\n\n");
        *error = -1;
    }
    if ((idiotChecker == -1) && (isBalanced == -1)) {
        printf("Error! Too many closing parentheses in the expression.\n\n");
        *error = -1;
    }
    *answer = tempArray[index--];
}



// converts infix expression to postfix notation for calculation [4+(8*8)-4  -->  4(88*)+4-]
void infixPostfix(StackPtr infixStack, char (*postfixExpr)[ 81], int *idiotChecker) {
    StackPtr tempStack = NULL;
    int index = 0;
    while (infixStack != NULL) {
        char symbol = pop_front(&infixStack);
        if (isdigit(symbol)) {
            (*postfixExpr)[index++] = symbol;
        }
        if (isOperator(symbol)) {
            while ((tempStack != NULL) && (at_back(tempStack) != '(') && (CheckPrecedence(at_back(tempStack)) >= CheckPrecedence(symbol))) {
                char topSymbol = pop_back(&tempStack);  
                (*postfixExpr)[index++] = topSymbol;
            }
            push_back(&tempStack, symbol);
        }
        if (symbol == '(') {
            push_back(&tempStack, symbol);
            // push opening parethesis to postfix expression
            (*postfixExpr)[index++] = symbol;
        }
        if (symbol == ')') {
            while ((tempStack != NULL) && (at_back(tempStack) != '(')) {
                char topSymbol = pop_back(&tempStack);  
                (*postfixExpr)[index++] = topSymbol;
            }
            if ((tempStack != NULL) && (at_back(tempStack) == '(')) {
                pop_back(&tempStack);
            } 
            else {
                *idiotChecker = -1;
            }
            // push closing parenthesis to postfix expression
            (*postfixExpr)[index++] = symbol;
        }
    }
    // push closing parenthesis to postfix expression if there are still opening parentheses on the tempStack
    while (tempStack != NULL) {
        char topSymbol = pop_back(&tempStack); 
        (*postfixExpr)[index++] = topSymbol; 
    }
    (*postfixExpr)[index++] = '\0'; // add null terminator to end of postfix expression
}



// plays the game in easy mode
void easyPlay(char difficulty, StackPtr opsHead, StackPtr parensHead) {
    while ("player is an idiot") {
        opsHead = NULL;
        parensHead = NULL;
        int answer = 0;
        int opsStackSize;
        char filename[] = "easy.txt";
	    FILE *filePtr  = fopen(filename, "r");
        if(filePtr == NULL) {
		    printf("Error: could not open %s for reading\n", filename);
		    exit(-1);
	    }
        int counter = 0;
        char inputString[ 81];
        int randIndex = rand() % 12; 
        for (int i = 0; i < randIndex; i++) {
            fgets(inputString, 81, filePtr); 
        }
        fgets(inputString, 81, filePtr);
        fclose(filePtr);
        int isBalanced = 1;
        bool isInvalidSymbol = true;
        int isAllNums = 0;
        int isAmtNums = 0;
        opsStackSize = 0;
        printf("The numbers to use are: %c, %c, %c, %c.\n", inputString[0], inputString[2], inputString[4], inputString[6]);
        char userString[ 81];
        printf("Enter your solution: ");
        scanf(" %[^\n]", userString);
        // initial validity checks 
        isBalanced = CheckBalanced(&parensHead, userString); freeStack(&parensHead);
        isInvalidSymbol = CheckOperators(userString);
        isAllNums = CheckNumsUsed(inputString[0], inputString[2], inputString[4], inputString[6], userString);
        isAmtNums = CheckAmntNumsUsed(userString);
        if (isInvalidSymbol) {
            printf("Error! Invalid symbol entered. Please try again. \n\n");
            continue;
        }
        // if (isBalanced == 2) : too many opens
        // if (isBalanced == -1) : too many closes
        // if (isBalanced == 0) : all good 
        if ((isAllNums != 4) || (isAmtNums == -1)) {
            printf("Error! You must use all four numbers, and use each one only once. Please try again. \n\n");
            continue;
        }
            opsStackSize = 0;
            for (int i = 0; i < strlen(userString); i++) {
                if (!isspace(userString[i])) {
                    push_back(&opsHead, userString[i]);
                    opsStackSize++;
                }
            }
            char postfixExpr[ 81] = {'\0'};
            int idiotChecker = 0;
            infixPostfix(opsHead, &postfixExpr, &idiotChecker);
            int error = 0;
            evaluation(&answer, postfixExpr, idiotChecker, isBalanced, &error);
        
        if ((answer == 24) && (error != -1)) { 
            printf("Well done! You are a math genius.\n\n");
            break;
        }
        if ((answer != 24) && (error != -1)) {
            printf("Sorry. Your solution did not evaluate to 24.\n\n");
            break;
        }
        break;     
    }
    postGameOptions(difficulty, opsHead, parensHead);
}



// plays the game in medium mode
void mediumPlay(char difficulty, StackPtr opsHead, StackPtr parensHead) {
    while ("player is an idiot") {
        opsHead = NULL;
        parensHead = NULL;
        int answer = 0;
        int opsStackSize;
        char filename[] = "medium.txt";
	    FILE *filePtr  = fopen(filename, "r");
        if(filePtr == NULL) {
		    printf("Error: could not open %s for reading\n", filename);
		    exit(-1);
	    }
        int counter = 0;
        char inputString[ 81];
        int randIndex = rand() % 24; 
        for (int i = 0; i < randIndex; i++) {
            fgets(inputString, 81, filePtr); 
        }
        fgets(inputString, 81, filePtr);
        fclose(filePtr);
        int isBalanced = 1;
        bool isInvalidSymbol = true;
        int isAllNums = 0;
        int isAmtNums = 0;
        opsStackSize = 0;
        printf("The numbers to use are: %c, %c, %c, %c.\n", inputString[0], inputString[2], inputString[4], inputString[6]);
        char userString[ 81];
        printf("Enter your solution: ");
        scanf(" %[^\n]", userString);
        // initial validity checks 
        isBalanced = CheckBalanced(&parensHead, userString); freeStack(&parensHead);
        isInvalidSymbol = CheckOperators(userString);
        isAllNums = CheckNumsUsed(inputString[0], inputString[2], inputString[4], inputString[6], userString);
        isAmtNums = CheckAmntNumsUsed(userString);
        if (isInvalidSymbol) {
            printf("Error! Invalid symbol entered. Please try again. \n\n");
            continue;
        }
        // if (isBalanced == 2) : too many opens
        // if (isBalanced == -1) : too many closes
        // if (isBalanced == 0) : all good 
        if ((isAllNums != 4) || (isAmtNums == -1)) {
            printf("Error! You must use all four numbers, and use each one only once. Please try again. \n\n");
            continue;
        }
        opsStackSize = 0;
        for (int i = 0; i < strlen(userString); i++) {
            if (!isspace(userString[i])) {
                push_back(&opsHead, userString[i]);
                opsStackSize++;
            }
        }
        char postfixExpr[ 81] = {'\0'};
        int idiotChecker = 0;
        infixPostfix(opsHead, &postfixExpr, &idiotChecker);
        int error = 0;
        evaluation(&answer, postfixExpr, idiotChecker, isBalanced, &error);
        
        if ((answer == 24) && (error != -1)) { 
            printf("Well done! You are a math genius.\n\n");
            break;
        }
        if ((answer != 24) && (error != -1)) {
            printf("Sorry. Your solution did not evaluate to 24.\n\n");
            break;
        }
        break;     
    }
    postGameOptions(difficulty, opsHead, parensHead);
}



// plays the game in hard mode
void hardPlay(char difficulty, StackPtr opsHead, StackPtr parensHead) {
    while ("player is an idiot") {
        opsHead = NULL;
        parensHead = NULL;
        int answer = 0;
        int opsStackSize;
        char filename[] = "hard.txt";
	    FILE *filePtr  = fopen(filename, "r");
        if(filePtr == NULL) {
		    printf("Error: could not open %s for reading\n", filename);
		    exit(-1);
	    }
        int counter = 0;
        char inputString[ 81];
        int randIndex = rand() % 11; 
        for (int i = 0; i < randIndex; i++) {
            fgets(inputString, 81, filePtr); 
        }
        fgets(inputString, 81, filePtr);
        fclose(filePtr);
        int isBalanced = 1;
        bool isInvalidSymbol = true;
        int isAllNums = 0;
        int isAmtNums = 0;
        opsStackSize = 0;
        printf("The numbers to use are: %c, %c, %c, %c.\n", inputString[0], inputString[2], inputString[4], inputString[6]);
        char userString[ 81];
        printf("Enter your solution: ");
        scanf(" %[^\n]", userString);
        // initial validity checks 
        isBalanced = CheckBalanced(&parensHead, userString); freeStack(&parensHead);
        isInvalidSymbol = CheckOperators(userString);
        isAllNums = CheckNumsUsed(inputString[0], inputString[2], inputString[4], inputString[6], userString);
        isAmtNums = CheckAmntNumsUsed(userString);
        if (isInvalidSymbol) {
            printf("Error! Invalid symbol entered. Please try again. \n\n");
            continue;
        }
        // if (isBalanced == 2) : too many opens
        // if (isBalanced == -1) : too many closes
        // if (isBalanced == 0) : all good 
        if ((isAllNums != 4) || (isAmtNums == -1)) {
            printf("Error! You must use all four numbers, and use each one only once. Please try again. \n\n");
            continue;
        }
        opsStackSize = 0;
        for (int i = 0; i < strlen(userString); i++) {
            if (!isspace(userString[i])) {
                push_back(&opsHead, userString[i]);
                opsStackSize++;
            }
        }
        char postfixExpr[ 81] = {'\0'};
        int idiotChecker = 0;
        infixPostfix(opsHead, &postfixExpr, &idiotChecker);
        int error = 0;
        evaluation(&answer, postfixExpr, idiotChecker, isBalanced, &error);
        
        if ((answer == 24) && (error != -1)) { 
            printf("Well done! You are a math genius.\n\n");
            break;
        }
        if ((answer != 24) && (error != -1)) {
            printf("Sorry. Your solution did not evaluate to 24.\n\n");
            break;
        }
        break;     
    }
    postGameOptions(difficulty, opsHead, parensHead);
}



// displays options for player after the game
void postGameOptions(char difficulty, StackPtr opsHead, StackPtr parensHead) {
    printf("Enter: \t1 to play again,\n");
	printf("\t2 to change the difficulty level and then play again, or\n");
	printf("\t3 to exit the program.\n");
    int menuOption = 0;
    scanf (" %d", &menuOption);
    if (menuOption == 1) {
        if (difficulty == 'E') {
            printf("Your choice --> ");
            easyPlay('E', opsHead, parensHead);
        }
        if (difficulty == 'M') {
            printf("Your choice --> ");
            mediumPlay('M', opsHead, parensHead);
        }
        if (difficulty == 'H') {
            printf("Your choice --> ");
            hardPlay('H', opsHead, parensHead);
        }
    }
    if (menuOption == 2) {
        printf("Your choice --> ");
        printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
        char newDifficulty = '\0';
        scanf(" %c", &newDifficulty);
        if (newDifficulty == 'E') {
            printf("Your choice --> ");
            easyPlay('E', opsHead, parensHead);
        }
        else if (newDifficulty == 'M') {
            printf("Your choice --> ");
            mediumPlay('M', opsHead, parensHead);
        }
        else if (newDifficulty == 'H') {
            printf("Your choice --> ");
            hardPlay('H', opsHead, parensHead);
        }
        else {
            printf("Your choice --> ");
            easyPlay('E', opsHead, parensHead);
        }
    }
    if (menuOption == 3) {
        printf("Your choice --> ");
        printf("\nThanks for playing!\n");
	    printf("Exiting...\n");
        exit(0);
    }
}



// displays start of game and its inital options
int main() {
    srand(1);
    StackPtr opsHead = NULL;
    StackPtr parensHead = NULL;
	printf("Welcome to the game of TwentyFour Part Two!\n");
	printf("Use each of the four numbers shown exactly once, \n");
	printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
	printf("to yield the value twenty-four.\n");
    printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
    char difficulty = '\0';
    scanf(" %c", &difficulty);
    if (difficulty == 'E') {
        printf("Your choice --> ");
        easyPlay('E', opsHead, parensHead);
    }
    else if (difficulty == 'M') {
        printf("Your choice --> ");
        mediumPlay('M', opsHead, parensHead);
    }
    else if (difficulty == 'H') {
        printf("Your choice --> ");
        hardPlay('H', opsHead, parensHead);
    }
    else {
        printf("Your choice --> ");
        easyPlay('E', opsHead, parensHead);
    }
    postGameOptions(difficulty, opsHead, parensHead);

return 0;
}