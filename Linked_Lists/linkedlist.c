#include <stdlib.h>
#include <stdio.h>

struct Node
{
        int data;
        struct Node *next;
};

struct Node *createDigit(int inputNum);

struct Node *createLinkedList(struct Node *start);

void printNode (struct Node *start); 

struct Node *appendForCreate(struct Node *prev_node, int inputNum);

void append(struct Node *end, struct Node *new_node);

struct Node *insert(struct Node *start, struct Node *new_node);

void delete_position(struct Node ** head, int position);

int main()
{
        struct Node *head, *new_node, *end;
        int choice, inputNum;
        while (1)
        {

                printf("\n               MENU                            \n");
                printf("1.Create     \n");
                printf("2.Display    \n");
                printf("3.Insert at the beginning    \n");
                printf("4.Insert at the end  \n");
                printf("5.Delete from specified position     \n");
                printf("9.Exit       \n");
                printf("--------------------------------------\n");
                printf("Enter your choice:\t");
                scanf("%d", &choice);
                switch (choice)
                {
                case 1:
                        head = createLinkedList(head);
                        break;
                case 2:
                        printNode(head);
                        break;
                case 3:
                        printf("Enter a number to insert: ");
                        scanf("%d", &inputNum);
                        new_node = createDigit(inputNum);
                        head = insert(head, new_node);
                        break;
                case 4:
                        printf("Enter a number to append: ");
                        scanf("%d", &inputNum);
                        new_node = createDigit(inputNum);
                        append(head, new_node);
                        break;
                case 5:
                        printf("Enter index for deletion: ");
                        scanf("%d", &inputNum);
                        delete_position(&head, inputNum + 1);
                        break;

                case 9:
                        exit(0);
                        break;

                default:
                        printf("\n Wrong Choice:\n");
                        break;
                }
        }
        return 0;
}

struct Node *createDigit(int inputNum)
{
    struct Node *ptr = (struct Node*)malloc(sizeof(struct Node));
    ptr->data = inputNum;
    ptr->next = NULL;
    return ptr;
}

struct Node *createLinkedList(struct Node *start)
{
        int numOfElements;
        struct Node *end = (struct Node*)malloc(sizeof(struct Node));

        printf("Enter the number of elements: ");
        scanf("%d", &numOfElements);

        for (int i = 0; i < numOfElements; i++)
        {
                int element;

                if (i == 0)
                {
                        printf("Enter digit: ");
                        scanf("%d", &element);
                        start = createDigit(element);
                }

                else if (i == 1)
                {
                        printf("Enter digit: ");
                        scanf("%d", &element);
                        end = appendForCreate(start, element);
                }

                else if (i > 1)
                {
                        printf("Enter digit: ");
                        scanf("%d", &element);
                        end = appendForCreate(end, element);
                }
        }
        return start;
}

void printNode (struct Node *start)
{
        int i = 1;
        while (start != NULL)
        {
                printf("\nNode%d->num: %d", i, start->data);
                start = start ->next;
                i += 1;
        }
        printf("\n");
}

struct Node* appendForCreate(struct Node *prev_node, int inputNum)
{
        struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
        prev_node->next = new_node;
        new_node->data = inputNum;
        new_node->next = NULL;
        return new_node; 
}
 
void append(struct Node *prev_node, struct Node *new_node)
{
        while(prev_node->next != NULL)
        {
                prev_node = prev_node->next;
        }
        prev_node->next = new_node;
}

struct Node *insert(struct Node *start, struct Node *new_node)
{
        while(1)
        {
                if (new_node->next == NULL)
                {
                        new_node->next = start;
                        return new_node;
                }
                new_node = new_node->next;
        }
}


void delete_position(struct Node **head, int position)
{

    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    struct Node *prev = (struct Node *)malloc(sizeof(struct Node));
    temp = *head;
    prev = *head;

    for (int i = 0; i < position; i++) 
    {
        if (i == 0 && position == 1) 
        {
            *head = (*head)->next;
            free(temp);
        }
        else {
            if (i == position - 1 && temp) {
                prev->next = temp->next;
                free(temp);
            }
            else {
                prev = temp;

                // Position was greater than
                // number of nodes in the list
                if (prev == NULL)
                    break;
                temp = temp->next;
            }
        }
    }
}