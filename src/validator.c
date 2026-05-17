#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "validator.h"

int is_valid_url(char *url) {
    if (strlen(url) == 0) return 0;

    regex_t reg;
    int result;

    regcomp(&reg, "^https?://[a-zA-Z0-9._-]+\\.[a-zA-Z]{2,}", REG_EXTENDED);
    result = regexec(&reg, url, 0, NULL, 0);
    regfree(&reg);

    return result == 0;  // 0 = match
}