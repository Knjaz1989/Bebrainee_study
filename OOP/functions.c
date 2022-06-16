#include "header.h"


static void xml(User *this) {
    FILE *file = fopen("data.xml", "w");
    fprintf(file, "<?xml-stylesheet type=\"text/xsl\" href=XSLFilePath?>\n"
                  "<person>\n"
                  "    <name>%s</name>\n"
                  "    <surname>%s</surname>\n"
                  "    <age>%d</age>\n"
                  "    <address>%s</address>\n"
                  "</person>\n", this->name, this->surname, this->age, this->address);
    fclose(file);
};


static void json(User *this) {
    json_object *new_json = json_object_new_object(); //create object
    json_object_object_add(new_json, "name", json_object_new_string(this->name));
    json_object_object_add(new_json, "surname", json_object_new_string(this->surname));
    json_object_object_add(new_json, "age", json_object_new_int(this->age));
    json_object_object_add(new_json, "address", json_object_new_string(this->address));
    json_object_to_file("data.json", new_json);
};


static void yaml(User *this) {
        FILE *file = fopen("data.yaml", "w");
        fprintf(file, "- name: %s\n", this->name);
        fprintf(file, "  surname: %s\n", this->surname);
        fprintf(file, "  age: %d\n", this->age);
        fprintf(file, "  address: %s\n", this->address);
        fclose(file);
};


static Dump *dump_constructor() {
    Dump *new_dump = (Dump *)malloc(sizeof(Dump));
    new_dump->xml = &xml;
    new_dump->json = &json;
    new_dump->yaml = &yaml;
    return new_dump;
}


User *user_constructor(char *name, char *surname, int age, char *address) {
    User *new_user = (User *)malloc(sizeof(User));
    new_user->name = name;
    new_user->surname = surname;
    new_user->age = age;
    new_user->address = address;
    new_user->dump = dump_constructor();
    return new_user;
}
