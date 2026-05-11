#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "route.h"

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(address);

    // โหลด mapping จาก file
    load_routes("mapping1.txt");

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

        // ดึง path
        char *path = strstr(buffer, "GET ");
        path += 4;
        char *end = strstr(path, " ");
        *end = '\0';

        // หา url จาก mapping
        char *url = find_url(path + 1);  // +1 ตัด / ออก

        const char *response;
        char redirect[512];

        if (url != NULL) {
            snprintf(redirect, sizeof(redirect),
                "HTTP/1.1 302 Found\r\n"
                "Location: %s\r\n"
                "Content-Length: 0\r\n"
                "\r\n", url);
            response = redirect;
        } else {
            response = "HTTP/1.1 404 Not Found\r\n"
                       "Content-Length: 0\r\n"
                       "\r\n";
        }

        write(client_fd, response, strlen(response));
        close(client_fd);
    }

    close(server_fd);
    return 0;
}