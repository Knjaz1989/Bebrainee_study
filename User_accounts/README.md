# To compile main.c file you need execute this command below

```c
gcc main.c -o main.bin -l json-c

```

## Пример использования:

### Выведет `Пользователь pavel создан`
```c
./main.bin create-user --login=superstar --password=Pwdpwd11 --name=pavel --lastname=noname --age=20
```

### Выведет `Пользователь maks создан`
```c
./accounts.bin create-user --login=moonlight --password=Strong11 --name=maks --lastname=surname --age=22
```

### Выведет `name=pavel, lastname=noname, age=20`
```c
./accounts.bin read-user --login=superstar --password=Pwdpwd11
```

### Выведет `Сообщение пользователю moonlight отправлено`
```c
./accounts.bin send-message --login=superstar --password=Pwdpwd11 --destination=moonlight --message="Ну, здаствуйте!"
```

### Выведет `Входящих сообщений: 1; id=1, from=superstar`
```c
./accounts.bin read-mailbox --login=moonlight --password=Strong11
```

### Выведет `Ну, здаствуйте!`
```c
./accounts.bin read-mail --login=superstar --password=pwd --message-id=1
```

