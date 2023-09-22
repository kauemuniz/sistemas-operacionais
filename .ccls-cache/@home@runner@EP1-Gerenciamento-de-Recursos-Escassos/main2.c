#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 1000

static char memory_pool[MAX_SIZE];
static size_t used_size = 0;

typedef struct {
    int integer;
    char *string;
} Tuple;

void *aloca(size_t size) {
    if (used_size + size <= MAX_SIZE) {
        void *ptr = &memory_pool[used_size];
        used_size += size;
        return ptr;
    } else {
        return NULL; // Não há memória suficiente disponível
    }
}

void libera(void *ptr) {
    // A função libera() aqui não precisa fazer nada,
    // já que estamos apenas marcando espaço no vetor como liberado.
    // Isso é uma simplificação e não faz uma desalocação real.
}

// Estrutura do nó da lista
typedef struct Node {
    Tuple data;
    struct Node *prev;
    struct Node *next;
} Node;

// Lista duplamente encadeada
typedef struct {
    Node *head;
    Node *tail;
} LinkedList;

LinkedList* initialize_list() {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void insert_element(LinkedList *list, int integer, const char *string) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data.integer = integer;
    newNode->data.string = strdup(string);

    newNode->prev = list->tail;
    newNode->next = NULL;

    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

Node* find_node(LinkedList *list, int integer) {
    Node *current = list->head;
    while (current != NULL) {
        if (current->data.integer == integer) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void update_element(LinkedList *list, int integer, const char *newString) {
    Node *nodeToUpdate = find_node(list, integer);
    if (nodeToUpdate != NULL) {
        free(nodeToUpdate->data.string);
        nodeToUpdate->data.string = strdup(newString);
    }
}

void delete_element(LinkedList *list, int integer) {
    Node *nodeToDelete = find_node(list, integer);
    if (nodeToDelete != NULL) {
        if (nodeToDelete->prev != NULL) {
            nodeToDelete->prev->next = nodeToDelete->next;
        } else {
            list->head = nodeToDelete->next;
        }

        if (nodeToDelete->next != NULL) {
            nodeToDelete->next->prev = nodeToDelete->prev;
        } else {
            list->tail = nodeToDelete->prev;
        }

        free(nodeToDelete->data.string);
        free(nodeToDelete);
    }
}

void print_list(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        printf("Integer: %d, String: %s\n", current->data.integer, current->data.string);
        current = current->next;
    }
}

void free_list(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data.string);
        free(current);
        current = next;
    }
    free(list);
}

int main() {
    LinkedList *list = initialize_list();
    int choice;
    
    while (true) {
        printf("\nMenu:\n");
        printf("1. Incluir elemento\n");
        printf("2. Alterar elemento\n");
        printf("3. Excluir elemento\n");
        printf("4. Listar elementos\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        
        scanf("%d", &choice);

        int integer;
        char string[100];

        switch (choice) {
            case 1:
                printf("Digite um número inteiro: ");
                scanf("%d", &integer);
                printf("Digite uma string: ");
                scanf("%s", string);
                insert_element(list, integer, string);
                break;

            case 2:
                printf("Digite o número inteiro do elemento a ser alterado: ");
                scanf("%d", &integer);
                printf("Digite a nova string: ");
                scanf("%s", string);
                update_element(list, integer, string);
                break;

            case 3:
                printf("Digite o número inteiro do elemento a ser excluído: ");
                scanf("%d", &integer);
                delete_element(list, integer);
                break;

            case 4:
                printf("Conteúdo da lista:\n");
                print_list(list);
                break;

            case 5:
                free_list(list);
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
