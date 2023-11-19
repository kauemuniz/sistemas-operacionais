#define FUSE_USE_VERSION 30
#define _FILE_OFFSET_BITS 64

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


#define BMP_HEADER_SIZE 54
#define LSB_SIZE 4

static const char *bmp_path = "/Documentos/Imagem/imagem_saida.bmp";
static const char *mount_path = "/Documentos/Montagem";

static int bmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    FILE *file = fopen(bmp_path, "rb");
    if (file == NULL) {
        perror("Erro ao abrir arquivo BMP");
        return -errno;
    }

    fseek(file, BMP_HEADER_SIZE + offset, SEEK_SET);
    fread(buf, 1, size, file);

    fclose(file);

    return size;
}

static int bmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    FILE *file = fopen(bmp_path, "r+b");
    if (file == NULL) {
        perror("Erro ao abrir arquivo BMP");
        return -errno;
    }

    fseek(file, BMP_HEADER_SIZE + offset, SEEK_SET);
    fwrite(buf, 1, size, file);

    fclose(file);

    return size;
}

static int bmp_getattr(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));
    stbuf->st_uid = getuid();
    stbuf->st_gid = getgid();
    stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = time(NULL);

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        stbuf->st_mode = S_IFREG | 0644;
        stbuf->st_nlink = 1;
        stbuf->st_size = 1024;  // Tamanho do arquivo (ajustar conforme necessário)
    }

    return 0;
}

static struct fuse_operations bmp_operations = {
    .getattr = bmp_getattr,
    .read = bmp_read,
    .write = bmp_write,
};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &bmp_operations, NULL);
}
