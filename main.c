#include <stdio.h>
#include "defs.h"

int main() {
    struct countries_t** countries;
    int res = read_countries("countries.txt",&countries);
    if(res==2){
        printf("Couldn't open file");
        return 4;
    }
    else if(res==3){
        printf("File corrupted");
        return 6;
    }
    else if(res==4){
        printf("Failed to allocate memory");
        return 8;
    }

    display_countries(countries);
    free_countries(countries);
    return 0;
}

