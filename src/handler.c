#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "handler.h"
#include "route.h"
#include "validator.h"

void handle_post(int client_fd, char *body) {
    if (!is_valid_url(body)) {
        write(client_fd,
              "HTTP/1.1 400 Bad Request\r\n"
              "Content-Length: 0\r\n"
              "\r\n", 38);
        return;
    }
    char *code = add_route(body);

    char response[512];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s", strlen(code), code);

    write(client_fd, response, strlen(response));
}

void handle_get(int client_fd, char *path) {
    char *url = find_url(path + 1);

    char redirect[512];
    const char *response;

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
}

void handle_delete(int client_fd, char *path) {
    int deleted = delete_route(path + 1);  // +1 ตัด / ออก

    const char *response;
    if (deleted) {
        response = "HTTP/1.1 200 OK\r\n"
                   "Content-Length: 0\r\n"
                   "\r\n";
    } else {
        response = "HTTP/1.1 404 Not Found\r\n"
                   "Content-Length: 0\r\n"
                   "\r\n";
    }

    write(client_fd, response, strlen(response));
}

void handle_list(int client_fd) {
    char body[BUFFER_SIZE];
    get_all_routes(body, sizeof(body));

    char response[BUFFER_SIZE + 256];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s", strlen(body), body);

    write(client_fd, response, strlen(response));
}

void handle_stats(int client_fd, char *code) {
    // เช็คก่อนว่า code มีอยู่จริงไหม
    char *url = find_url_no_log(code);  
    if (url == NULL) {
        write(client_fd, 
              "HTTP/1.1 404 Not Found\r\n"
              "Content-Length: 0\r\n"
              "\r\n", 38);
        return;
    }
    int count = get_stats(code);

    char body[64];
    snprintf(body, sizeof(body), "%s clicked %d times\n", code, count);

    char response[256];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s", strlen(body), body);

    write(client_fd, response, strlen(response));
}