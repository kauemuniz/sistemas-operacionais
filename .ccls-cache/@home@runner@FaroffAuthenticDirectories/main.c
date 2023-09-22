#include <stdio.h>
#include <stddef.h>

#define MAX_SIZE 1000

static char memory_pool[MAX_SIZE];
static size_t used_size = 0;

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

int main(void) {
    int *p1 = (int *)aloca(sizeof(int));
    if (p1 != NULL) {
        *p1 = 100;
        printf("Valor alocado: %d\n", *p1);
    } else {
        printf("Não foi possível alocar memória!\n");
    }

    int *p2 = (int *)aloca(2 * sizeof(int));
    if (p2 != NULL) {
        *p2 = 200;
        printf("Valor alocado: %d\n", *p2);
    } else {
        printf("Não foi possível alocar memória!\n");
    }

    libera(p1);
    libera(p2);

    return 0;
}

