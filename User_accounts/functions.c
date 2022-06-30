//
// Created by igor on 26.06.22.
//

#include "prototypes.h"



int check_create_argv(char **argv) {

    return 0;
}

void create_file () {
    json_object *list = json_object_new_array();
    json_object_to_file("accounts.json", list);
}

static int check_file () {
    if (fopen("accounts.json", "r")) { // Допустимо ли такое открытие файла?
        return 0;
    }
    return 1;
}

void add_user_to_file(char **argv) {
    if (check_file()) {
        create_file();
    }
    json_object *list = json_object_from_file("accounts.json");
    json_object *dict = json_object_new_object(); //create dict
    json_object_object_add(dict, "name", json_object_new_string("1"));
    json_object_object_add(dict, "surname", json_object_new_string("1"));
    json_object_object_add(dict, "age", json_object_new_int(1));
    json_object_object_add(dict, "login", json_object_new_string("1"));
    json_object_object_add(dict, "password", json_object_new_string("1"));
    json_object_array_add(list, dict);
    json_object_to_file("accounts.json", list);
};

void read_user_from_file(char *user_login, char *user_password) {
    if (check_file()) {
        puts("There is no such user");
    }
    json_object *account_list = json_object_from_file("accounts.json"); // Читаем файл
    unsigned int item_count = json_object_array_length(account_list);
    for (int i = 0; i < item_count; i++) {
        json_object *item = json_object_array_get_idx(account_list, i); // Получаем объект по индексу
        json_object *login = json_object_object_get(item, "login"); // Получаем пару ключ:значение
        json_object *password = json_object_object_get(item, "password");
        const char *login_str = json_object_get_string(login); // Достаем значение из ключа
        const char *password_str = json_object_get_string(password);
        if (strcmp(user_login, login_str) == 0 && strcmp(user_password, password_str) == 0) {
            json_object *name = json_object_object_get(item, "name");
            json_object *surname = json_object_object_get(item, "surname");
            json_object *age = json_object_object_get(item, "age");
            const char *name_str = json_object_get_string(name);
            const char *surname_str = json_object_get_string(surname);
            int age_int = json_object_get_int(age);
            printf("name=%s, lastname=%s, age=%d", name_str, surname_str, age_int);
        }
    }
}

//User *user_constructor (char *login, char *password, char *name, char *surname, int age) {
//    User *user = (User *)malloc(sizeof(User));
//    user->login = login;
//    user->password = password;
//    user->name = name;
//    user->surname = surname;
//    user->age = age;
//    return user;
//}


//FILE *file = fopen("Accounts/1.txt", "w");
////        fputs("Hello, World!", file);
//fclose(file);