#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include "route.h"
#include "handler.h"

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(address);

    load_routes("mapping.txt");
    srand(time(NULL));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) { perror("socket failed"); exit(1); }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed"); exit(1);
    }

    listen(server_fd, 10);
    printf("Server running on http://localhost:%d\n", PORT);

    while(1) {
        client_fd = accept(server_fd, (struct sockaddr*)&address,
                          (socklen_t*)&addrlen);

        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        if (strstr(buffer, "GET /stats/") != NULL) {
            char *path = strstr(buffer, "GET /stats/");
            path += 11;  // ข้าม "GET /stats/"
            char *end = strstr(path, " ");
            *end = '\0';
            handle_stats(client_fd, path);
        } else if (strstr(buffer, "GET /list") != NULL) {
            handle_list(client_fd);
        } else if (strstr(buffer, "POST /shorten") != NULL) {
            char *body = strstr(buffer, "\r\n\r\n");
            body += 4;
            handle_post(client_fd, body);
        } else if (strstr(buffer, "DELETE") != NULL) {
            char *path = strstr(buffer, "DELETE ");
            path += 7;
            char *end = strstr(path, " ");
            *end = '\0';
            handle_delete(client_fd, path);
        } else {
            char *path = strstr(buffer, "GET ");
            path += 4;
            char *end = strstr(path, " ");
            *end = '\0';
            handle_get(client_fd, path);
        } 

        close(client_fd);
    }

    close(server_fd);
    return 0;
}