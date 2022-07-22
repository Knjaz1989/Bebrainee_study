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

typedef void (*dump)(User *user);

typedef struct Yaml {
    dump dump;
} Yaml;

typedef struct Json {
    dump dump;
} Json;

typedef struct Xml {
    dump dump;
} Xml;

typedef struct Type {
    dump dump;
} Type;

User *user_cons (char *name, char *surname, int age, char *address);
User **list_of_users ();

Yaml *yaml_constructor ();
Json *json_constructor ();
Xml *xml_constructor ();
Type **type_list_constructor ();

static char *generate_name(User *user, char *type);
static void write_to_yaml (User *user);
static void write_to_json (User *user);
static void write_to_xml (User *user);

#endif //OOP_HEADER_H