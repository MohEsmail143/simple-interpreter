#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSIZE 256

typedef union
{
    float fData;
    char cData;
} stackItem;

typedef struct
{
    stackItem items[128];
    int top;
} Stack;

typedef struct
{
    float val;
    char key[16];
} data;

typedef struct
{
    struct node *left;
    struct node *right;
    data d;
} node;

node *bst_root = NULL;

Stack *initialize()
{
    Stack *s = malloc(sizeof(Stack));
    s->top = 0;
    return s;
}

void push(Stack *s, stackItem value)
{
    s->items[s->top] = value;
    s->top++;
}

stackItem pop(Stack *s)
{
    stackItem value;
    s->top--;
    value = s->items[s->top];
    return value;
}

stackItem top(Stack *s)
{
    return s->items[s->top - 1];
}

int isemptyStack(Stack *s)
{
    if (s->top == 0)
        return 1;
    else
        return 0;
}

node *newNode(char *key, float val)
{
    node *n = (node *) malloc(sizeof(node));
    n->left = NULL;
    n->right = NULL;
    strcpy(n->d.key, key);
    n->d.val = val;
    return n;
}

node *insertTree(node *root, char *key, float val)
{
    if (root == NULL)
        return newNode(key, val);
    if (strcasecmp(key, root->d.key) < 0)
        root->left = insertTree(root->left, key, val);
    else if (strcasecmp(key, root->d.key) > 0)
        root->right = insertTree(root->right, key, val);
    else if (strcasecmp(key, root->d.key) == 0)
        root->d.val = val;
    return root;
}

int sizeTree(node *root)
{
    if (root == NULL)
        return 0;
    else
        return 1 + sizeTree(root->left) + sizeTree(root->right);
}

void inorder(node *root) //displays variables sorted by key
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%s = %.3f\n", root->d.key, root->d.val);
        inorder(root->right);
    }
}

int i = 0;

void inorder_insert_array(data arr[], node *root) //copies data from tree to an array
{
    if (root != NULL)
    {
        inorder_insert_array(arr, root->left);
        arr[i].val = root->d.val;
        strcpy(arr[i++].key, root->d.key);
        inorder_insert_array(arr, root->right);
    }
}

node *searchRecursive(node *n, char *varkey) //searches tree for variable
{
    if (n == NULL)
        return NULL;
    if (strcasecmp(varkey, n->d.key) == 0)
        return n;
    else if (strcasecmp(varkey, n->d.key) < 0)
        return searchRecursive(n->left, varkey);
    else
        return searchRecursive(n->right, varkey);
}

int isOperator(char c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
        return 1;
    default:
        return 0;
    }
}

float evaluate(char x, float op1, float op2)
{
    switch (x)
    {
    case '+':
        return (op1 + op2);
    case '-':
        return (op1 - op2);
    case '*':
        return (op1 * op2);
    case '/':
        return (op1 / op2);
    case '^':
        return (pow(op1, op2));
    }
}

int priority(char c)
{
    switch (c)
    {
    case '(':
        return 3;
    case '^':
        return 2;
    case '*':
    case '/':
    case '%':
        return 1;
    case '+':
    case '-':
        return 0;
    }
}

void infixToPostfix(char *infix, char *postfix)
{
    int j = 0;
    Stack *s = initialize();
    stackItem s_item;
    for (int i = 0; i < strlen(infix); i++)
    {
        if (isdigit(infix[i]))
        {
            while (isdigit(infix[i]) || infix[i] == '.') //for positive decimal numbers
            {
                postfix[j] = infix[i];
                i++;
                j++;
            }
            i--;
            postfix[j] = ' ';
            j++;
        }
        else if (infix[i] == '-' && isdigit(infix[i + 1]) && isdigit(infix[i - 1]))     // for minus sign
        {
            s_item.cData = infix[i];
            push(s, s_item);
        }
        else if (infix[i] == '-' && isdigit(infix[i + 1]))     // for -ve numbers
        {
            postfix[j] = infix[i];
            i++;
            j++;
            while (isdigit(infix[i]) || infix[i] == '.')
            {
                postfix[j] = infix[i];
                i++;
                j++;
            }
            i--;
            postfix[j] = ' ';
            j++;
        }
        else if (infix[i] == ')')
        {
            while (!isemptyStack(s) && top(s).cData != '(')
            {
                postfix[j] = pop(s).cData;
                j++;
                postfix[j] = ' ';
                j++;
            }
            pop(s);
        }
        else if (isemptyStack(s) && infix[i] != '-')
        {
            s_item.cData = infix[i];
            push(s, s_item);
        }
        else
        {
            while (!isemptyStack(s) && priority(top(s).cData) >= priority(infix[i]) && top(s).cData != '(')
            {
                postfix[j] = pop(s).cData;
                j++;
                postfix[j] = ' ';
                j++;
            }
            s_item.cData = infix[i];
            push(s, s_item);
        }
    }
    while (!isemptyStack(s))
    {
        postfix[j] = pop(s).cData;
        j++;
        postfix[j] = ' ';
        j++;
    }
    postfix[j] = '\0';
    free(s);
}

float evaluatePostfix(char *postfix)
{
    Stack *s = initialize();
    double op1, op2, val;
    stackItem s_item;
    for (int i = 0; i < strlen(postfix); i++)
    {
        if (postfix[i] == ' ')
            continue;
        else if (isdigit(postfix[i]) && postfix[i + 1] == ' ')
        {
            s_item.fData = postfix[i] - '0';
            push(s, s_item);
            continue;
        }
        else if (isdigit(postfix[i]) && (postfix[i + 1] != ' ' || postfix[i - 1] != ' '))     //for +ve numbers
        {
            int j = 0;
            char *x = malloc(sizeof(char));
            while (isdigit(postfix[i]) || postfix[i] == '.')
            {
                x[j] = postfix[i];
                j++;
                i++;
            }
            s_item.fData = atof(x);
            push(s, s_item);
            free(x);
        }
        else if (postfix[i] == '-' && isdigit(postfix[i + 1]))     //for -ve numbers
        {
            int j = 0;
            char *x = malloc(sizeof(char));
            x[j] = postfix[i];
            j++;
            i++;
            while (isdigit(postfix[i]) || postfix[i] == '.')
            {
                x[j] = postfix[i];
                j++;
                i++;
            }
            s_item.fData = atof(x);
            push(s, s_item);
            free(x);
        }
        else if (isOperator(postfix[i]))     // for operators
        {
            op2 = pop(s).fData;
            op1 = pop(s).fData;
            val = evaluate(postfix[i], op1, op2);
            s_item.fData = val;
            push(s, s_item);
        }
    }
    val = pop(s).fData;
    if (!isemptyStack(s)) //invalid expression check
    {
        printf("\nError found in this expression\n");
        printf("Error Type: Invalid expression\n");
        printf("\nPlease make the necessary changes then try again\n");
        exit(0);
    }
    free(s);
    return val;
}

void heapify(data arr[], int size, int i)
{
    data temp_max_child;
    int maximum_node = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && arr[left].val > arr[maximum_node].val)
        maximum_node = left;
    if (right < size && arr[right].val > arr[maximum_node].val)
        maximum_node = right;
    if (maximum_node != i)
    {
        temp_max_child = arr[i];
        arr[i] = arr[maximum_node];
        arr[maximum_node] = temp_max_child;
        heapify(arr, size, maximum_node);
    }
}

void heap_sort(data arr[], int size)
{
    data temp_root_leaf;
    int i;
    //First: Build the max heap
    for (i = (size - 1) / 2; i >= 0; i--)
        heapify(arr, size, i);
    //Swap the root with most leaf and then reduce the size by 1
    for (i = size - 1; i >= 0; i--)
    {
        temp_root_leaf = arr[i];
        arr[i] = arr[0];
        arr[0] = temp_root_leaf;
        heapify(arr, i, 0);
    }
}

char *removeWhitespace(char *str) //removes extra spaces from statement
{
    int count = 0;
    for (int j = 0; str[j]; j++)
        if (str[j] != ' ')
            str[count++] = str[j];
    str[count] = '\0';
    return str;
}

int validateStatement(char *fullExp) //error-checking the full statement
{
    int i;
    int errorFound = 0, equalFoundFlag = 0, oneVarFlag = 0;
    for (i = 0; i < strlen(fullExp); i++)
    {
        if (fullExp[i] == '=')
            equalFoundFlag = 1;
    }
    if (equalFoundFlag == 0) //check if equal sign is found
    {
        printf("\nError found in this expression\n");
        printf("Error Type: Missing = in the expression\n");
        errorFound = 1;
        return errorFound;
    }
    equalFoundFlag = 0;
    for (i = 0; i < strlen(fullExp); i++)
    {
        if (!isOperator(fullExp[i]) && !isdigit(fullExp[i]) && !isalpha(fullExp[i]) //invalid symbol check
                && fullExp[i] != '=' && fullExp[i] != '.' && fullExp[i] != ' ' && fullExp[i] != '\n'
                && fullExp[i] != '(' && fullExp[i] != ')')
        {
            printf("\nError found in this expression\n");
            printf("Error Type: Invalid symbol '%c'\n", fullExp[i]);
            errorFound = 1;
        }
        if (fullExp[i] == '=')
            equalFoundFlag = 1;
        if ((isOperator(fullExp[i]) || isdigit(fullExp[i])) && equalFoundFlag == 0 &&
                oneVarFlag == 0) // one variable in left hand side check
        {
            printf("\nError found in this expression\n");
            printf("Error Type: lvalue required as left operand of assignment\n");
            oneVarFlag = 1;
            errorFound = 1;
        }
    }
    return errorFound;
}

void keyToVal(char *infix) //replaces keys found in statement with their value from BST
{
    int i, j = 0, k;
    char newInfix[MAXSIZE] = "";
    for (i = 0; i < strlen(infix); i++)
    {
        if (isalpha(infix[i]))
        {
            char tempkey[16];
            node *temp = NULL;
            for (k = 0; isalpha(infix[i]); k++)
            {
                tempkey[k] = infix[i];
                i++;
            }
            i--;
            tempkey[k] = '\0'; // key is now stored in tempkey
            temp = searchRecursive(bst_root, tempkey); // the tree is searched for this key
            if (temp == NULL) //variable is not found in tree condition
            {
                printf("\nError found in this expression\n");
                printf("\n%s is not instantiated yet\n", tempkey);
                printf("\nPlease make the necessary changes and try again later\n");
                exit(0);
            }
            else     //if found, take temp then convert from float to string and concatenate to the new infix
            {
                char numToAppend[8];
                gcvt(temp->d.val, 6, numToAppend);
                strcat(numToAppend, "0");
                j = j + strlen(numToAppend) - 1;
                strncat(newInfix, numToAppend, 8);
            }
        }
        else
            newInfix[j] = infix[i]; // copy from original infix to new infix
        j++;
    }
    newInfix[j] = '\0';
    strcpy(infix, newInfix); //copy new infix to original one
}

void main()
{
    printf("------------------------------------------\n");
    printf("WELCOME TO SIMPLE INTERPRETER APPLICATION\n");
    printf("------------------------------------------\n\n");

    FILE *fp = NULL;
    while (fp == NULL)
    {
        char filename[255];
        printf("Please enter valid a file name: ");
        fgets(filename, 255, stdin);
        strtok(filename, "\n");
        fp = fopen(filename, "r");
        if (fp == NULL)
            printf("\nThis file does not exist\n\n");
        else
            printf("File successfully loaded!\n\n");
    }

    int errorFound = 0;
    char infix[MAXSIZE];
    char postfix[MAXSIZE];
    char fullExp[MAXSIZE];
    char tempkey[16];
    float tempval;

    printf("Statements found in file:\n");
    while (fgets(fullExp, MAXSIZE, fp) != NULL)
    {
        printf("%s", fullExp);
        errorFound = validateStatement(fullExp);
        if (errorFound == 1)
        {
            printf("\nPlease make the necessary changes then try again\n");
            exit(0);
        }
        removeWhitespace(fullExp);
        strcpy(infix, strtok(fullExp, "="));
        strcpy(tempkey, infix);

        strcpy(infix, strtok(NULL, ""));
        strtok(infix, "\n");

        keyToVal(infix);
        infixToPostfix(infix, postfix);
        tempval = evaluatePostfix(postfix);
        bst_root = insertTree(bst_root, tempkey, tempval);
    }
    fclose(fp);
    printf("\n");

    printf("\nThe variables sorted by key:\n");
    inorder(bst_root);

    data arr[sizeTree(bst_root)];
    int size = sizeof(arr) / sizeof(arr[0]);
    inorder_insert_array(arr, bst_root);    //Insert all values in an array to sort it
    heap_sort(arr, size);

    printf("\nThe variables sorted by value:\n");
    for (int k = 0; k < size; k++)
        printf("%s = %.3f\n", arr[k].key, arr[k].val);
}
