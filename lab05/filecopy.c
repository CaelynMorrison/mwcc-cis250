#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
 
#define BUFFER_SIZE 4096
 
int main(int argc, char *argv[])
{
    int source_fd;
    int destination_fd;
 
    ssize_t bytes_read;
    ssize_t bytes_written;
 
    char buffer[BUFFER_SIZE];

    if (argc != 3) {
        printf("Invalid command line arguments.\n");
        printf("Usage: %s source destination\n", argv[0]);
        return 1;
    } 
 
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("open");
        return 1;
    }
 
    destination_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd == -1) {
        perror("open");
        close(source_fd);
        return 1;
    }
 
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        if (bytes_read == -1) {
            perror("read");
            close(source_fd);
            close(destination_fd);
            return 1;
        }
        bytes_written = write(destination_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("write");
            close(source_fd);
            close(destination_fd);
            return 1;
        }
    } 
   
    if (close(source_fd) != -1 && close(destination_fd) != -1){
        printf("%s successfully copied to %s\n", argv[1], argv[2]);
        return 0;
    } else {
        perror("close");
        return 1;
    }
}