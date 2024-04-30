#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_payload {
  char message[140];
  char username[40];
  int message_length;
} t_payload;

void secret_backdoor(void) {
  char cmd[128];

  fgets(cmd, 128, stdin);
  system(cmd);

  return;
}

void handle_msg(void) {
  t_payload payload;

  memset(payload.username, 0, 40);
  payload.message_length = 140;
  set_username(&payload);
  set_msg(&payload);
  puts(">: Msg sent!");

  return ;
}

void set_msg(t_payload *payload) {
  char buffer[1024];

  memset(buffer, 0, 1024);

  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(buffer, 1024, stdin);
  strncpy(payload->message, buffer, payload->message_length);

  return;
}

void set_username(t_payload *payload) {
  char buffer[128];

  memset(buffer, 0, 128);
  puts(">: Enter your username");
  printf(">>: ");
  fgets(buffer, 128, stdin);

  for(int i = 0; i <= 40
   && buffer[i]; i++)
    payload->username[i] = buffer[i];

  printf(">: Welcome, %s", payload->username);

  return;
}

int main(void)
{
  puts(
    "--------------------------------------------\n"\
    "|   ~Welcome to l33t-m$n ~    v1337        |\n"\
    "--------------------------------------------");
  handle_msg();

  return 0;
}