## To compile this programm execute the code below
```c
gcc GIT.c -o GIT.bin -ljson-c
```

#### Get status
```c
./GIT.bin status
```

#### Add to stage
```c
./GIT.bin add <file_name>
```

#### Get log
```c
./GIT.bin log
```

#### Create commit
```c
./GIT.bin commit --message=<commit_name
```