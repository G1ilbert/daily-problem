#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "route.h"

struct Route routes[MAX_ROUTES];
int route_count = 0;
char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void load_routes(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        // ใช้ perror แทน printf
        perror("Error loading mapping file");
        exit(1);  // หยุดเลย
    }
    
    while (fscanf(f, "%s %s", 
           routes[route_count].code,
           routes[route_count].url) == 2) {
        route_count++;
    }
    
    fclose(f);
}

char *find_url(char *code) {
    for (int i = 0; i < route_count; i++) {
        if (strcmp(routes[i].code, code) == 0) {
            return routes[i].url;
        }
    }
    return NULL;
}

void generate_code(int n, char *out) {
    int base = 62;
    int len = 6;
    
    for (int i = len - 1; i >= 0; i--) {
        out[i] = charset[n % base];
        n /= base;
    }
    out[len] = '\0';
}

void add_route(char *code, char *url) {
    strcpy(routes[route_count].code, code);
    strcpy(routes[route_count].url, url);
    route_count++;
}