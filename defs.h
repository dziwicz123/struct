#ifndef STRUCT_DEFS_H
#define STRUCT_DEFS_H
struct countries_t
{
    char* name;
    unsigned int population;
    char** cities;
};

int read_countries(const char* filename, struct countries_t*** countries);
void display_countries(struct countries_t** countries);
void free_countries(struct countries_t** countries);
#endif
