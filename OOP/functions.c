#include "header.h"
#include <sys/stat.h>


User *user_constructor (char *name, char *surname, int age, char *address) {
    User *new_user = (User *)malloc(sizeof(User));
    new_user->name = name;
    new_user->surname = surname;
    new_user->age = age;
    new_user->address = address;
    return new_user;
}


User **list_of_users () {
    char *names[] = {
            "Vasya", "Petya", "Dmitriy", "Evgeniy", "Oleg", "Egor", "Igor", "Pavel",
            "Alex", "Vladimir"
    };
    char *surnames[] = {
            "Sidorov", "Pokemonov", "Ivanov", "Minaev", "Sobchak", "Markov", "Nichaev",
            "Pavlov", "Ratnikov", "Zimin"
    };
    int ages[] = {
            35, 25, 30, 42, 18, 28, 39, 20, 31, 21
    };
    char *addresses[] = {
            "Voronezh", "Moscow", "Orel", "Volgograd", "Samara", "Ekaterinburg",
            "Krasnodar", "Kaliningrad", "Belgorod", "Kursk"
    };
    User **list = malloc(10 * sizeof(User));
    for (int i=0; i<10; i++) {
        User *user = user_constructor(names[i], surnames[i], ages[i], addresses[i]);
        list[i] = user;
    }
    return list;
}


static char *generate_name(User *user, char *type) {
    char *file_name = malloc(sizeof(char ) * 50);
    strcpy(file_name, user->name);
    strcat(strcat(strcat(file_name, "_"), user->surname), type);
    return file_name;
}


static void write_to_yaml (User *user) {
    char *filename = generate_name(user, ".yaml");
    FILE *file = fopen(filename, "w");
    fprintf(file, "- name: %s\n", user->name);
    fprintf(file, "  surname: %s\n", user->surname);
    fprintf(file, "  age: %d\n", user->age);
    fprintf(file, "  address: %s\n", user->address);
    fclose(file);
}


static void write_to_json (User *user) {
    char *filename = generate_name(user, ".json");
    json_object *new_json = json_object_new_object(); //create object
    json_object_object_add(new_json, "name", json_object_new_string(user->name));
    json_object_object_add(new_json, "surname", json_object_new_string(user->surname));
    json_object_object_add(new_json, "age", json_object_new_int(user->age));
    json_object_object_add(new_json, "address", json_object_new_string(user->address));
    json_object_to_file(filename, new_json);
}

static void write_to_xml (User *user) {
    char *filename = generate_name(user, ".xml");
    FILE *file = fopen(filename, "w");
    fprintf(file, "<?xml-stylesheet type=\"text/xsl\" href=XSLFilePath?>\n"
                  "<person>\n"
                  "    <name>%s</name>\n"
                  "    <surname>%s</surname>\n"
                  "    <age>%d</age>\n"
                  "    <address>%s</address>\n"
                  "</person>\n", user->name, user->surname, user->age, user->address);
    fclose(file);
}

Yaml *yaml_constructor () {
    Yaml *type = (Yaml *) malloc(sizeof(Yaml));
    type->dump = write_to_yaml;
    return type;
}

Json *json_constructor ()  {
    Json *type = (Json *) malloc(sizeof(Json));
    type->dump = write_to_json;
    return type;
}

Xml *xml_constructor () {
    Xml *type = (Xml *) malloc(sizeof(Xml));
    type->dump = write_to_xml;
    return type;
}

Type **type_list_constructor () {
    Type **list = (Type**) malloc(sizeof(Type*));
    list[0] = malloc(sizeof(Yaml ));
    list[0]->dump = write_to_yaml;
    list[1] = malloc(sizeof(Json ));
    list[1]->dump = write_to_json;
    list[2] = malloc(sizeof(Xml ));
    list[2]->dump = write_to_xml;
    return list;
}


