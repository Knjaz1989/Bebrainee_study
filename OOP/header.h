#ifndef OOP_HEADER_H
#define OOP_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json-c/json.h"


typedef struct User {
    char *name;
    char *surname;
    int age;
    char *address;
    struct Dump *dump;
} User;


typedef struct Dump {
    void (*xml)(struct User *this);
    void (*json)(struct User *this);
    void (*yaml)(struct User *this);
} Dump;


User *user_cons(char *name, char *surname, int age, char *address);
static Dump *dump_constructor();
static void xml(User *this);
static void json(User *this);
static void yaml(User *this);

#endif //OOP_HEADER_H
