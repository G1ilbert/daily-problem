#ifndef HANDLER_H
#define HANDLER_H

void handle_post(int client_fd, char *body);
void handle_get(int client_fd, char *path);
void handle_delete(int client_fd, char *path);

#endif