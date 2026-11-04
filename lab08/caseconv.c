#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

void to_upper(struct stat source_stat, char *memfile) {
    for (int i = 0; i < source_stat.st_size; i++) {
        printf("%c", toupper(memfile[i]));
    }
    printf("\n");
}

void to_lower(struct stat source_stat, char *memfile) {
    for (int i = 0; i < source_stat.st_size; i++) {
        printf("%c", tolower(memfile[i]));
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int source_fd;

    struct stat source_stat;

	if (argc != 3) {
        printf("Invalid command line arguments.\n");
        printf("Usage: %s mode filename\n", argv[0]);
        return 1;
    }

    source_fd = open(argv[2], O_RDWR);
    if (source_fd == -1) {
        perror("open");
        return 1;
    }

    if (fstat(source_fd, &source_stat) == -1) {
        perror("Could not retrieve file size.");
    }

    printf("File size is %li.\n", source_stat.st_size);

    char *memfile = mmap(NULL, source_stat.st_size, PROT_READ, MAP_PRIVATE, source_fd, 0);

    if (strcmp(argv[1], "upper") == 0) {
        to_upper(source_stat, memfile);
    } else if (strcmp(argv[1], "lower") == 0) {
        to_lower(source_stat, memfile);
    } else {
        fprintf(stderr, "Invalid usage mode. Please use either lower or upper.\n");
        //printf("Invalid usage mode. Please use either lower or upper.\n");
    }
    

    munmap(memfile, source_stat.st_size);

    if (close(source_fd) != -1){
        return 0;
    } else {
        perror("close");
        return 1;
    }
}