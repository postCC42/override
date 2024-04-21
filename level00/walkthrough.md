[README](../README.md)
# level00

```
level00@OverRide:~$ ./level00 
***********************************
* 	          -Level00 -		  *
***********************************
Password:hello

Invalid Password!
```

## Disassembly
**Main:**
```
0x08048494 <+0>:	push   %ebp
0x08048495 <+1>:	mov    %esp,%ebp
0x08048497 <+3>:	and    $0xfffffff0,%esp
0x0804849a <+6>:	sub    $0x20,%esp           // Reserves 32 bytes
0x0804849d <+9>:	movl   $0x80485f0,(%esp)    // '*' <repeats 35 times>
0x080484a4 <+16>:	call   0x8048390 <puts@plt>
0x080484a9 <+21>:	movl   $0x8048614,(%esp)    //  "* \t     -Level00 -\t\t  *"
0x080484b0 <+28>:	call   0x8048390 <puts@plt>
0x080484b5 <+33>:	movl   $0x80485f0,(%esp)    // '*' <repeats 35 times>
0x080484bc <+40>:	call   0x8048390 <puts@plt>
0x080484c1 <+45>:	mov    $0x804862c,%eax      // "Password:"
0x080484c6 <+50>:	mov    %eax,(%esp)
0x080484c9 <+53>:	call   0x8048380 <printf@plt>
0x080484ce <+58>:	mov    $0x8048636,%eax      // "%d" moved to eax
0x080484d3 <+63>:	lea    0x1c(%esp),%edx      // compute the location to store the input
0x080484d7 <+67>:	mov    %edx,0x4(%esp)       
0x080484db <+71>:	mov    %eax,(%esp)
0x080484de <+74>:	call   0x80483d0 <__isoc99_scanf@plt>   // Ask for user input
0x080484e3 <+79>:	mov    0x1c(%esp),%eax
0x080484e7 <+83>:	cmp    $0x149c,%eax         // Compare 0x149c (5275 decimal) to eax
0x080484ec <+88>:	jne    0x804850d <main+121>
0x080484ee <+90>:	movl   $0x8048639,(%esp)    // "\nAuthenticated!"
0x080484f5 <+97>:	call   0x8048390 <puts@plt>
0x080484fa <+102>:	movl   $0x8048649,(%esp)    // "/bin/sh"
0x08048501 <+109>:	call   0x80483a0 <system@plt>
0x08048506 <+114>:	mov    $0x0,%eax            // Return value of 0
0x0804850b <+119>:	jmp    0x804851e <main+138>
0x0804850d <+121>:	movl   $0x8048651,(%esp)    // "\nInvalid Password!"
0x08048514 <+128>:	call   0x8048390 <puts@plt>
0x08048519 <+133>:	mov    $0x1,%eax            // Return value of 1
0x0804851e <+138>:	leave  
0x0804851f <+139>:	ret    
```

1. The program prints the banner, and "Password:"
2. Asks for user input using `scanf` and `"%d` format
3. Compare the result to the 5276 in decimal
4. If the comparison is equal print `"\nAuthenticated"` and execute `system("/bin/sh")`
5. Otherwise print `"\nInvalid Password!"` and return 1

## Exploit
```
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ whoami
level01
$ cat ../level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```