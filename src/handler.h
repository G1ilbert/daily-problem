#ifndef HANDLER_H
#define HANDLER_H

#define BUFFER_SIZE 4096

void handle_post(int client_fd, char *body);
void handle_get(int client_fd, char *path);
void handle_delete(int client_fd, char *path);
void handle_list(int client_fd);

#endif