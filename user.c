#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MAX_PROCESSES 5
#define PROC_FILE "/proc/mem_map"
#define BUFFER_SIZE 2048

void read_mem_map(void);

int main(void) {
    pid_t pids[MAX_PROCESSES];
    printf("Parent PID: %d\n", getpid());
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        }
        
        if (pid == 0) { // Child
            printf("Child Process PID: %d, Parent PID: %d\n", getpid(), getppid());
            size_t size = (1 << (i + 7)); // 128, 256, 512, 1024, 2048 KB
            char *mem = malloc(size * 1024);
            if (!mem) {
                perror("Memory allocation failed");
                exit(1);
            }
            memset(mem, 0, size * 1024);
            sleep(10); // Keep alive for reading
            free(mem);
            exit(0);
        }
        
        pids[i] = pid;
    }
    
    sleep(2); // Let children allocate memory
    read_mem_map();
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}

void read_mem_map(void) {
    char buffer[BUFFER_SIZE];
    int fd = open(PROC_FILE, O_RDONLY);
    
    if (fd < 0) {
        perror("Failed to open proc file");
        return;
    }
    
    ssize_t bytes;
    while ((bytes = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
    
    if (bytes < 0) {
        perror("Failed to read proc file");
    }
    
    if (close(fd) < 0) {
        perror("Failed to close proc file");
    }
}
