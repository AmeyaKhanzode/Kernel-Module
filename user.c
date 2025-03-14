#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PROCESSES 5
#define PROC_FILE "/proc/mem_map_module" // Fixed: matches PROC_NAME in kernel module

void read_mem_map();

int main() {
  printf("Parent PID: %d\n", getpid());
  
  for (int i = 0; i < MAX_PROCESSES; i++) {
    int pid = fork();
    if (pid == 0) {
      printf("Child Process PID: %d and Parent PID: %d\n", getpid(), getppid());
      int* mem = malloc(sizeof(int) * 1024);
      if (!mem) {
          printf("Allocation Failed.\n");
      }
      sleep(3);
      exit(0);
    }
    else if (pid < 0) {
      printf("Could not fork.\n");
      return 0;
    }
    else {
      waitpid(pid, NULL, 0);
    }
  }
  
  sleep(2);
  read_mem_map();
  
  return 0; // Fixed: added return statement
}

void read_mem_map() {
  char buffer[2048];
  int fd = open(PROC_FILE, O_RDONLY);
  if (fd < 0) {
    printf("Failed to open proc file.\n");
    return;
  }
  
  int bytes = read(fd, buffer, sizeof(buffer)-1);
  if (bytes > 0) {
    buffer[bytes] = '\0';
    printf("Memory map from kernel module:\n%s\n", buffer);
  }
  else {
    printf("Failed to read proc file.\n");
  }
  
  close(fd);
}
