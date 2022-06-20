#include "functions.c"


int main() {
    User **users = list_of_users();

    Yaml *yaml = yaml_constructor();
    Json *json = json_constructor();
    Xml  *xml = xml_constructor();


    int i = 0;
    while (users[i]) {
        json->dump(users[i]);
        xml->dump(users[i]);
        yaml->dump(users[i]);
        i++;
    }
    return 0;
}
