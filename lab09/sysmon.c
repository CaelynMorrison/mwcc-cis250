#include <stdio.h>      // fclose(), fgets(), fopen(), perror(), printf()
#include <stdlib.h>     // exit()
#include <string.h>     // strlen()
#include <sys/wait.h>   // waitpid()
#include <unistd.h>     // close(), fork(), pipe(), read(), sleep(), write()

#define BUFFER_SIZE 1024
#define INTERVAL 5

int main(void)
{
  // File Descriptors
  int cpu_pipe[2];
  int mem_pipe[2];

  char cpu_buffer[BUFFER_SIZE];
  char mem_buffer[BUFFER_SIZE * 2];

  // CPU Pipe
  if (pipe(cpu_pipe) == -1)
  {
    perror("pipe");
    return -1;
  }

  if (pipe(mem_pipe) == -1)
  {
    perror("pipe");
    return -1;
  }

  // Create Child Processes.
  pid_t cpu_pid = fork();

  if (cpu_pid == 0)
  {
 
    // CPU MONITOR PROCESS

    // Close Read-End of CPU Pipe.
    close(cpu_pipe[0]);

    // Buffer for Reading CPU Info.
    char buffer[BUFFER_SIZE];

    while(1)
    {
      FILE *file = fopen("/proc/loadavg", "r");

      if (file != NULL)
      {
        if (fgets(buffer, sizeof(buffer), file) != NULL)
        {
          // Send the CPU Info Through the Pipe.
          write(cpu_pipe[1], buffer, strlen(buffer) + 1);

          // Close the CPU file.
          fclose(file);
        }
      }

      sleep(INTERVAL);
    }

    // Close the Write-End of the CPU Pipe.
    close(cpu_pipe[1]);

    exit(0);
  }
  else
  {
    pid_t mem_pid = fork();
    if (mem_pid == 0)
    {
      close(mem_pipe[0]);
      char buffer[BUFFER_SIZE * 2];
      char mem_total[BUFFER_SIZE];
      char mem_available[BUFFER_SIZE];
      

      while(1)
      {
        buffer[0] = '\0'; 
        FILE *file = fopen("/proc/meminfo", "r");
        if (file != NULL)
        {
          fgets(mem_total, sizeof(mem_total), file);
          fgets(mem_available, sizeof(mem_available), file);
          fclose(file);
          strcat(buffer, mem_total);
          strcat(buffer, mem_available);
          
          write(mem_pipe[1], buffer, strlen(buffer) + 1);

        }
        
        sleep(INTERVAL);
      }
      close(mem_pipe[1]);
      exit(0);
    }
    else
    {
    
    // PARENT PROCESS

    // Close the Write-End of the Pipes.
    close(cpu_pipe[1]);
    close(mem_pipe[1]);

    while(1)
    {
      // Read from the Pipes.
      read(cpu_pipe[0], cpu_buffer, sizeof(cpu_buffer));
      read(mem_pipe[0], mem_buffer, sizeof(mem_buffer));

      // Clear the Terminal and Move Cursor to the Top.
      printf("\033[2J\033[H");

      // Display Info.
      printf("CPU Load Average: %s", cpu_buffer);
      printf("%s", mem_buffer);
    }

    // Close the Read-End of the Pipes.
    close(cpu_pipe[0]);
    close(mem_pipe[0]);

    // Wait for Child Process.
    waitpid(cpu_pid, NULL, 0);
    waitpid(mem_pid, NULL, 0);

    }
}

  return 0;
}