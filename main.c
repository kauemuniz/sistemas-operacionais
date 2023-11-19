#define _GNU_SOURCE
#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>

int main() {
    struct utsname buffer;

    // Chama o sistema uname para obter informações do sistema
    if (uname(&buffer) != 0) {
        perror("uname");
        return 1;
    }

    // Imprime as informações obtidas
    printf("Nome do Sistema: %s\n", buffer.sysname);
    printf("Nome do Nó (Hostname): %s\n", buffer.nodename);
    printf("Release do Sistema: %s\n", buffer.release);
    printf("Versão do Sistema: %s\n", buffer.version);
    printf("Tipo de Máquina: %s\n", buffer.machine);

    return 0;
}
