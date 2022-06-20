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
} User;

typedef struct Yaml {
    void (*dump)(User *user);
} Yaml;

typedef struct Json {
    void (*dump)(User *user);
} Json;

typedef struct Xml {
    void (*dump)(User *user)
} Xml;

User *user_cons (char *name, char *surname, int age, char *address);
User **list_of_users ();

Yaml *yaml_constructor ();
Json *json_constructor ();
Xml *xml_constructor ();

static char *generate_name(User *user, char *type);
static void write_to_yaml (User *user);
static void write_to_json (User *user);
static void write_to_xml (User *user);

#endif //OOP_HEADER_H