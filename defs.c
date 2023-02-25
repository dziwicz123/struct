#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int read_countries(const char* filename, struct countries_t*** countries){
    if(filename==NULL || countries==NULL){
        return 1;
    }

    FILE *f = fopen(filename,"r");
    if(f==NULL){
        return 2;
    }

    int count = 0;
    int people;
    char country_name[41], city_name[41];
    char *ptr = city_name;
    char *ptr_c = country_name;
    struct countries_t** vec = NULL;

    while(!feof(f)){
        int res = fscanf(f,"%40[^|]|",country_name);
        if(res == EOF){
            break;
        }
        else if(res!=1){
            free_countries(vec);
            fclose(f);
            return 3;
        }

        int x =0;
        while(*(ptr_c+x)!='\0'){
            if(*(ptr_c+x)=='\n'){
                free_countries(vec);
                fclose(f);
                return 3;
            }
            x++;
        }

        if(!fscanf(f,"%d\n",&people)){
            res = fscanf(f,"%40[^|]|%d\n",city_name,&people);
            if(res == EOF){
                free_countries(vec);
                fclose(f);
                return 3;
            }
            else if(res!=2){
                free_countries(vec);
                fclose(f);
                return 3;
            }
        }
        else{
            *ptr = '\0';
        }

        struct countries_t* country = NULL;
        for (int i = 0; i < count; ++i) {
            if(strcmp((*(vec+i))->name,country_name)==0){
                country = *(vec+i);
                break;
            }
        }

        if(country){
            if(*ptr!='\0'){
                if(country->cities==NULL){
                    country->cities = calloc(2, sizeof(char*));
                    if(country->cities==NULL){
                        free_countries(vec);
                        fclose(f);
                        return 4;
                    }

                    int len2 = (int) strlen(ptr);
                    *(country->cities) = calloc(len2+1, sizeof(char));
                    if(*(country->cities)==NULL){
                        free(country->cities);
                        free_countries(vec);
                        fclose(f);
                        return 4;
                    }
                    *(country->cities+1) = NULL;
                    strcpy(*(country->cities),city_name);
                }
                else{
                    int num=0;
                    int id = 0;
                    for (; *(country->cities+num) ; ++num);
                    for (; *(country->cities+id) ; ++id){
                        if(strcmp(*(country->cities+id),city_name)==0){
                            break;
                        }
                    }

                    if(id==num) {
                        char **temp = realloc(country->cities, sizeof(char *) * (num + 2));
                        if (temp == NULL) {
                            free_countries(vec);
                            fclose(f);
                            return 4;
                        }
                        country->cities = temp;
                        *(country->cities+1+num) = NULL;

                        int len = (int) strlen(city_name);
                        *(country->cities+id) = calloc(len+1, sizeof(char));
                        if(*(country->cities+id)==NULL){
                            free_countries(vec);
                            fclose(f);
                            return 4;
                        }
                        strcpy(*(country->cities+id),city_name);
                    }


                }
            }

            country->population += people;
        }
        else{
            struct countries_t** temp = realloc(vec, sizeof(struct countries_t*)*(count+2));
            if(temp==NULL){
                free_countries(vec);
                fclose(f);
                return 4;
            }
            vec = temp;
            *(vec+count+1) = NULL;

            country = calloc(1, sizeof(struct countries_t));
            if(country==NULL){
                free_countries(vec);
                fclose(f);
                return 4;
            }

            int len = (int) strlen(country_name);
            country->name = calloc(len+1, sizeof(char));
            if(country->name==NULL){
                free(country);
                free_countries(vec);
                fclose(f);
                return 4;
            }
            strcpy(country->name,country_name);
            country->cities=NULL;

            if(*ptr!='\0'){
                char ** cities = calloc(2, sizeof(char*));
                if(cities==NULL){
                    free(country->name);
                    free(country);
                    free_countries(vec);
                    fclose(f);
                    return 4;
                }

                int len2 = (int) strlen(ptr);
                *cities = calloc(len2+1, sizeof(char));
                if(*cities==NULL){
                    free(country->name);
                    free(country);
                    free(cities);
                    free_countries(vec);
                    fclose(f);
                    return 4;
                }

                *(cities+1) = NULL;
                strcpy(*cities,city_name);
                country->cities = cities;
            }
            country->population = people;
            *(vec+count) = country;
            count++;
        }

    }
    *countries = vec;

    fclose(f);
    return 0;
}
void display_countries(struct countries_t** countries){
    if(countries!=NULL){
        for (int i = 0; *(countries+i); ++i) {
            printf("%s: %u\n",(*(countries+i))->name,(*(countries+i))->population);
            char** cities = (*(countries+i))->cities;
            if(cities!=NULL) {
                for (int j = 0; *(cities + j); ++j) {
                    printf("%d. %s\n", j + 1, *(cities + j));
                }
            }
        }
    }
}
void free_countries(struct countries_t** countries){
    if(countries!=NULL){
        for (int i = 0; *(countries+i); ++i) {
            if((*(countries+i))->cities!=NULL) {
                for (int j = 0; *((*(countries+i))->cities + j); ++j) {
                    free(*((*(countries+i))->cities + j));
                }
                free((*(countries+i))->cities);
            }
            free((*(countries+i))->name);
            free(*(countries+i));
        }
        free(countries);
    }
}
