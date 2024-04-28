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