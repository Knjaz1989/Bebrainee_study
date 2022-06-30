//
// Created by igor on 26.06.22.
//

#ifndef USER_AUTHORIZATION_PROTOTYPES_H
#define USER_AUTHORIZATION_PROTOTYPES_H

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include <sys/stat.h> // Для работы с папками
#include "json-c/json.h"
#include "getopt.h"


typedef struct User {
    char *login;
    char *password;
    char *name;
    char *surname;
    int age;
} User;

User *user_constructor (char *login, char *password, char *name, char *surname, int age);



#endif //USER_AUTHORIZATION_PROTOTYPES_H
