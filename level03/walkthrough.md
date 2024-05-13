[README](../README.md)
# Level03
previous flag: `Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`

## disas main
- `0x080488c1 <+103>:	call   0x8048530 <__isoc99_scanf@plt>`: scanf is called for taking user input. It epects a format specifier, that is prepared at instruction <+87>: (%d) => expext an int
- at line `<+112>`, the program moves the immediate value 0x1337d00d into memory location [esp + 0x4], which suggests that this value is being used for comparison
- 0x1337d00d converted into decimal is 322424845
- call test function passing user input as 1st argument (stored at [esp + 0x1c]) and 0x1337d00d as second

## disas test
- in test the user input (that in <+6> has been moved to %eax)is substracted from 0x1337d00d (that in <+9> has been moved to %edx)
- result is stored in -0xc(%ebp) and compared with 0X15 (21 in decimal) using cmpl. If 21 is lower than the result we ja to instruction +259
- if our result is lower than 21:
   - there are some instructions related to the preparation of indexes to access an array
      - it is shifted left of 2 bytes (shl) => multiplied by 4 (each left shift by 1 position doubles the value). This is likely done to prepare the index for accessing an array or table.
      - the result (already multiplied by 4) is now added to the constant 0x80489f0 (134520240 in decimal). This constant likely represents the base address of an array or table in memory.
      - mov    (%eax),%eax = moves the value stored at the memory address pointed to by %eax into %eax itself.
      - jmp *%eax: This instruction jumps to the address contained in %eax, effectively transferring control to the function whose address is stored in %eax. (equivalennt to a function call)
- decrypt is called with the result as argument

## disas decrypt
- <+146>(gdb) x/s 0x80489c3
0x80489c3:	 "Congratulations!"
- <+181> : (gdb) x/s 0x80489d4
0x80489d4:	 "/bin/sh"

## solution
- since we know that to call /bin/sh in decrypt the result of the suctraction need to be lower than 21, it means that the password - 322424845 <= 21 
- we have 21 possibilities and test all of them.
```
for x in range {322424824..322424845}; do (python -c  "print $x"; cat -) | ./level03 ; done
```
- The right one is 322424827.
```
level03@OverRide:~$ ./level03
***********************************
*		level03		**
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```





