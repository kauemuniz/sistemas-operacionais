#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(1) // Define o alinhamento de estrutura para 1 byte

struct PartitionEntry {
    uint8_t status;
    uint8_t start_head;
    uint16_t start_sector : 6;
    uint16_t start_cylinder : 10;
    uint8_t type;
    uint8_t end_head;
    uint16_t end_sector : 6;
    uint16_t end_cylinder : 10;
    uint32_t start_lba;
    uint32_t size;
};

struct MasterBootRecord {
    uint8_t bootstrap_code[446];
    struct PartitionEntry partitions[4];
    uint16_t signature;
};

int main() {
    FILE *file = fopen("mbr.bin", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    struct MasterBootRecord mbr;
    fread(&mbr, sizeof(struct MasterBootRecord), 1, file);
    fclose(file);

    printf("Disk /dev/sda: %u bytes\n", mbr.partitions[0].size * 512);
    printf("Disk identifier: 0x%02X%02X%02X%02X\n", mbr.signature, mbr.signature >> 8, mbr.signature >> 16, mbr.signature >> 24);

    for (int i = 0; i < 4; ++i) {
        struct PartitionEntry partition = mbr.partitions[i];
        printf("Device     Boot    Start       End   Sectors   Size Id Type\n");
        printf("/dev/sda%d  %c     %-10u%-10u%-10u%-10u %02X %s\n", 
            i + 1,
            (partition.status & 0x80) ? '*' : ' ',
            partition.start_lba,
            partition.start_lba + partition.size - 1,
            partition.size,
            partition.size * 512,
            partition.type,
            (partition.type == 0x83) ? "Linux" : "Unknown"
        );
    }

    return EXIT_SUCCESS;
}
