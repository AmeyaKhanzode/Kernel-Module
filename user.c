#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MAX_PROCESSES 5
#define PROC_FILE "/proc/mem_map_module_ameya"
#define BUFFER_SIZE 2048

void read_mem_map(void);

int main(void) {
    printf("Parent PID: %d\n", getpid());
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        }
        
        if (pid == 0) {
            printf("Child Process PID: %d, Parent PID: %d\n", 
                   getpid(), getppid());
                   
            int *mem = malloc(sizeof(int) * 1024);
            if (!mem) {
                perror("Memory allocation failed");
                exit(1);
            }
            
            memset(mem, 0, sizeof(int) * 1024);
            sleep(3);
            free(mem);
            exit(0);
        }
        
        waitpid(pid, NULL, 0);
    }
    
    sleep(2);
    read_mem_map();
    
    return 0;
}

void read_mem_map(void) {
    char buffer[BUFFER_SIZE];
    int fd = open(PROC_FILE, O_RDONLY);
    
    if (fd < 0) {
        perror("Failed to open proc file");
        return;
    }
    
    printf("Memory map from kernel module:\n");
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
