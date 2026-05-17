#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "route.h"

struct Route routes[MAX_ROUTES];
int route_count = 0;

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
char *find_url_no_log(char *code) {
    for (int i = 0; i < route_count; i++) {
        if (strcmp(routes[i].code, code) == 0) {
            return routes[i].url;
        }
    }
    return NULL;
}
char *find_url(char *code) {
    for (int i = 0; i < route_count; i++) {
        if (strcmp(routes[i].code, code) == 0) {
            write_log(code);  // เพิ่มตรงนี้
            return routes[i].url;
        }
    }
    return NULL;
}

void generate_code(char *out) {
    char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 6; i++) {
        out[i] = charset[rand() % 62];
    }
    out[6] = '\0';
}

// add_route คืน code กลับ
char *add_route(char *url) {
    // เช็คก่อนว่า url นี้มีอยู่แล้วไหม
    for (int i = 0; i < route_count; i++) {
        if (strcmp(routes[i].url, url) == 0) {
            return routes[i].code;  // คืน code เดิม
        }
    }
    char code[7];
    
    do {
        generate_code(code);
    } while (find_url(code) != NULL);

    strcpy(routes[route_count].code, code);
    strcpy(routes[route_count].url, url);
    route_count++;

    FILE *f = fopen("mapping.txt", "a");
    fprintf(f, "%s %s\n", code, url);
    fclose(f);

    return routes[route_count - 1].code;
}

int delete_route(char *code) {
    for (int i = 0; i < route_count; i++) {
        if (strcmp(routes[i].code, code) == 0) {
            // เลื่อน element ถัดไปมาทับ
            for (int j = i; j < route_count - 1; j++) {
                routes[j] = routes[j + 1];
            }
            route_count--;

            // เขียน mapping.txt ใหม่ทั้งหมด
            FILE *f = fopen("mapping.txt", "w");
            for (int k = 0; k < route_count; k++) {
                fprintf(f, "%s %s\n", routes[k].code, routes[k].url);
            }
            fclose(f);

            return 1;  // ลบสำเร็จ
        }
    }
    return 0;  // ไม่เจอ
}

void get_all_routes(char *out, int size) {
    out[0] = '\0';
    for (int i = 0; i < route_count; i++) {
        char line[600];
        snprintf(line, sizeof(line), "%s %s\n", 
                 routes[i].code, routes[i].url);
        strncat(out, line, size - strlen(out) - 1);
    }
}

// เพิ่มใน find_url ตรงที่เจอ url
void write_log(char *code) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char datetime[20];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", t);

    FILE *log = fopen("log.txt", "a");
    if (log == NULL) { perror("Cannot open log.txt"); return; }
    fprintf(log, "%s %s\n", datetime, code);
    fclose(log);
}

int get_stats(char *code) {
    FILE *log = fopen("log.txt", "r");
    if (log == NULL) return 0;

    int count = 0;
    char date[12], time_str[10], logged_code[50];

    while (fscanf(log, "%s %s %s", date, time_str, logged_code) == 3) {
    if (strcmp(logged_code, code) == 0) count++;
}

    fclose(log);
    return count;
}