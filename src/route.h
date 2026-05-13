#ifndef ROUTE_H
#define ROUTE_H

#define MAX_ROUTES 100
#define MAX_CODE 50
#define MAX_URL 500

struct Route {
    char code[MAX_CODE];
    char url[MAX_URL];
};

void load_routes(char *filename);
char *find_url(char *code);
void generate_code(int n,char *out);        // สร้าง code
void add_route(char *code, char *url);  // เพิ่ม mapping

#endif