[README](../README.md)
# Level06

```
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: toto
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: toto
```

By checking the source code, no buffer overflow vulnerability is present.
The `auth` function generate a hash number based on the provided login.
We need to determine the correct serial to match the login.

The login needs to be at least 5 chars long.

## Exploit
We can write a simple python script to compute the hash for any provided login.

```python
import sys

if (len(sys.argv) > 1):
    username = sys.argv[1]
    print(username)
    hash = (ord(username[3]) ^ 4919) + 6221293

    for char in username:
        if ord(char) <= 31:
            print("All char should be > 31 ASCII")
            exit(1)
        hash += (hash ^ ord(char)) % 0x539
    
    print(f'Username {username} - hash {hash}')
```

```
➜  override git:(PCC-22-level-6) ✗ python3 level06/get_hash.py override
override
Username override - hash 6234511
```

```
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: override
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6234511
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```