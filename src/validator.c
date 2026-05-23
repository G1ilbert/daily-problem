#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include "validator.h"

int is_valid_url(char *url) {
    if (strlen(url) == 0) return 0;

    regex_t reg;
    int result;

    regcomp(&reg, "^https?://[a-zA-Z0-9._-]+\\.[a-zA-Z]{2,}[a-zA-Z0-9/_\\-\\.\\?=&#%]*$", REG_EXTENDED);
    result = regexec(&reg, url, 0, NULL, 0);
    regfree(&reg);

    return result == 0;  // 0 = match
}

void normalize_url(char *url) {
    // ตัด trailing slash
    int len = strlen(url);
    if (url[len - 1] == '/') {
        url[len - 1] = '\0';
    }

    // แปลง domain เป็น lowercase
    // หา domain หลัง ://
    char *domain = strstr(url, "://");
    if (domain == NULL) return;
    domain += 3;

    // lowercase จนเจอ / หรือจบ string
    for (int i = 0; domain[i] != '\0' && domain[i] != '/'; i++) {
        domain[i] = tolower(domain[i]);
    }
}