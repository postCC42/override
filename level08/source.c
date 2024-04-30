void log_wrapper(FILE *fs, char *string, char *filename)
{
  char    *content;
  size_t  idx;

  strcpy(content, string);
  snprintf(content, strlen(filename), filename);
  idx = strcspn(content, "\n");
  content[idx] = '\0';
  fprintf(fs, "LOG: %s\n", content);
}

int main(int argc, char **argv)
{
  FILE  *logFS;
  FILE  *fileFS;
  char  *file = "./backups/";
  int   fd;
  char  c;

  if (argc != 2)
    printf("Usage: %s filename\n",*argv,*argv);

  logFS = fopen("./backups/.log","w");
  if (logFS == NULL)
  {
    printf("ERROR: Failed to open %s\n", "./backups/.log");
    exit(1);
  }

  log_wrapper(logFS, "Starting back up: ", argv[1]);

  fileFS = fopen(argv[1],"r");
  if (fileFS == NULL)
  {
    printf("ERROR: Failed to open %s\n", argv[1]);
    exit(1);
  }

  strncat(file, argv[1], strlen(argv[1]));
  fd = open(file, 0xc1, 0x1b0);
  if (fd < 0) {
    printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
    exit(1);
  }
  while(c = fgetc(fileFS))
    write(fd, &c, 1);

  log_wrapper(logFS, "Finished back up ", argv[1]);

  fclose(fileFS);
  close(fd);
  return 0;
}