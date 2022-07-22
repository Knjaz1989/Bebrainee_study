## To compile execute the code below

```c
gcc client.c -o client.bin -ljson-c
gcc server.c -o server.bin -ljson-c
```
To run 

```c
./server.bin start --host=<...> --port=<...>
./client.bin --host=<...> --port=<...> --login=<...>
```