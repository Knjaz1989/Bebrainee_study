#include <stdio.h>   //Для printf
#include <string.h>  //Для strcmp
#include <unistd.h>  //Для sleep

int main(void) {

    int count = 1;

    while (count < 6)
    {
        if (count > 3)
        {
            printf("Ввод данных заморожен на три секунды. Осталось %d раз/раза ввести число!\n", 6 - count);
            sleep(3);
        }

        char login[20];
        char password[30];

        printf("Please enter login: ");
        scanf("%s", login);
        printf("Please enter password: ");
        scanf("%s", password);

        if (strcmp(login, "test0") == 0 && strcmp(password, "test0") == 0)
        {
            printf("Welcom ADMIN!\n");
            break;
        }
        else if (count == 5)
        {
            printf("Programm was stopped\n");
            break;
        }
        else
        {
            printf("User with such credentials was not found\n");
            count += 1;
        }
    }
}
