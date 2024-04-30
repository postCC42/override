#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void log_wrapper(FILE *log_file, char *message, char *filename) {
  char buffer[264];

  strcpy(buffer, message);
  snprintf(buffer + strlen(buffer), 255 - strlen(buffer) - 1, filename);
  buffer[strcspn(buffer, "\n")] = '\0';
  fprintf(log_file, "LOG: %s\n", buffer);
}

int main(int argc, char **argv) {
  char byte;
  char character;
  char buffer[100];
  FILE *source_file,
  FILE *log_file;
  int fd = -1;

  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
  }

  log_file = fopen("./backups/.log", "w");

  if (!log_file) {
    printf("ERROR: Failed to open %s\n", "./backups/.log");
    exit(1);
  }

  log_wrapper(log_file, "Starting back up: ", argv[1]);
  source_file = fopen(argv[1], "r");

  if (!source_file) {
    printf("ERROR: Failed to open %s\n", argv[1]);
    exit(1);
  }

  strcpy(buffer, "./backups/");
  strncat(buffer, argv[1], 100 - strlen(buffer) - 1);
  fd = open(buffer, 193);

  if (fd < 0) {
    printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
    exit(1);
  }

  while(true) {
    byte = fgetc(source_file)
    character = (char)byte;

    if (character == -1) {
      break;
    }
    
    write(fd, &character, 1);
  }

  log_wrapper(log_file, "Finished back up ", argv[1]);
  fclose(source_file);
  close(fd);

  return 0;
}