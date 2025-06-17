# Cryptohack
## Man-in-the-middle attack
### Parameter Injection
We can now intercept message from Alice and sent Bob anything we want. Alice's message contains `p, g, A = g^a`. When you are the man in the middle, you can generate a pair of `(m, M = g^m)` and send Bob `M`, after that, send `M` back to Alice. You now have 2 shared key with Alice and Bob without them knowing.

The diagram below shows how the process is done:

```
Alice --(p, g, A)--> You --(p, g, M)--> Bob
Alice <--(M)-- You <--(B)-- Bob
```

```
nc socket.cryptohack.org 13371
Intercepted from Alice: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0x576d3f2678ae6e6978844e5b6e896975ac202d5a428815915375a6b11a5d7f8979185bd8bbd5bae25d0352b676ed0ce029da036213b3db025a95eb9a30eaf85f2a4d947930305e117cda33e2faee04b9379c9538c2db208f96efafac6991eb271423dd17c1226d177b60fe5ed0948480054bafde55ffe81f3429b28c51b0652486da508ff758bab52146a3f33dcbdb4e9e7f3b6630b6ee442e29ee6738595138c66cb463357e13b76ce66a2b76aaf61969f179b2f759c398943b8841fe714ea"}
Send to Bob: {"p": "0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", "g": "0x02", "A": "0x83e907190b6484aa982847f873111a28a3f1a0617a0973b24f8ed036d01d01009f050fa636cfe030cdd26f1309465cdea4ebc97d421fa5ebeedda63d948c8b00e81c8e8e63e720ad74bf867139ac2112883928d0441290f9f40e67a44e4447b7f8841f6f573b8b6a85d679bb611d7f026a4c2c904dd4a97a2d0048531f43b78e7c539d9e59149229ed32630506d11f13b42609bb4b8c4644e0f3ede537022ac7de96288c1794746f3f57b25a2668363a4314879c3834a9961ba3800f7de4798d"}
Intercepted from Bob: {"B": "0x8399f47cb5820dd2c8d08b46619d705faa1df4834d8f96dccf7e75d5c9f5ac1b4bd72421a06a0bd25c4201d78df5b718183f3a1e15c0a22ba600256fbd700b7fa7138faec59930922706d5fe3910e9a018d602d45ed771b604c5b38232bc766ef9ab5a0c7eec5991813625453ed429602daf1857d5272faabdafd7e2c7d0a8390b71f3c7f44f2640125e353f9ea0019027c08db2d4cca924faf31a52eb7b7193c1864662ee3653237d8586c7b019af8b711aa609abe093f89f481cbe18d66495"}
Send to Alice: {"B": "0x83e907190b6484aa982847f873111a28a3f1a0617a0973b24f8ed036d01d01009f050fa636cfe030cdd26f1309465cdea4ebc97d421fa5ebeedda63d948c8b00e81c8e8e63e720ad74bf867139ac2112883928d0441290f9f40e67a44e4447b7f8841f6f573b8b6a85d679bb611d7f026a4c2c904dd4a97a2d0048531f43b78e7c539d9e59149229ed32630506d11f13b42609bb4b8c4644e0f3ede537022ac7de96288c1794746f3f57b25a2668363a4314879c3834a9961ba3800f7de4798d"}
Intercepted from Alice: {"iv": "c69f9d582562f4b3302fae9e9505accd", "encrypted_flag": "2139da2dd36db56053b16647b32be7a91a352cf8b9631f69c0ebd6f72f6f12b7"}
```

Now you can decrypt the flag using the shared key between you and Alice.

![image](https://github.com/user-attachments/assets/95221d4c-8594-4a1c-885f-3478625b1202)


### Export Grade
This is a concrete example of how a downgrade attack works. Downgrade attack can also happen if you misconfigure your system.

```
nc socket.cryptohack.org 13379
Intercepted from Alice: {"supported": ["DH1536", "DH1024", "DH512", "DH256", "DH128", "DH64"]}
Send to Bob: {"supported": ["DH64"]}                                                   
Intercepted from Bob: {"chosen": "DH64"}
Send to Alice: {"chosen": "DH64"}
Intercepted from Alice: {"p": "0xde26ab651b92a129", "g": "0x2", "A": "0xc858d28023bcf37d"}
Intercepted from Bob: {"B": "0xb675204c299c9fde"}
Intercepted from Alice: {"iv": "385b665bade59ba17bb2c1ba1afff492", "encrypted_flag": "56b549bc4308b65594d8088a17bf27e40e29e12060ef6a4ab2bab71355f4c7e2"}
```

With the algorithm being downgraded to DH64, it's really easy to compute `a` from `A` and `g`

```python
sage: p = 0xde26ab651b92a129
sage: F = Zmod(p)
sage: g = F(2)
sage: a = discrete_log(0xc858d28023bcf37d, g)
sage: a
5870161511817033205
```

You now have the shared key between Alice and Bob. Next, process to decrypt the flag using AES mode CBC like normal.

![image](https://github.com/user-attachments/assets/43c9409c-8f6e-45ee-9393-f31eb0b7f1ce)

### Static Client
We can give Bob p, g, A and Bob will use those parameters to calculate his g ^ b

I tried to give Bob `g = 0x01` and `A = 0x01` so that Bob returned me this:

```
Bob says to you: {"B": "0x1"}
Bob says to you: {"iv": "cc586fe27ea4e14d2d9985d5197459c3", "encrypted": "5bcbf643840d9225db9ab76aa136edb43df0bb3e384ef04817f52d7701efe4d563a14a166c87781f5f0a2cd87bc1b4752efbbd084acd1f7232717e238f66634a5d4a9f38c1f93f3cbcc314b51517b277"}
```

When B = 0x1, shared key = 0x1 too. Decrypting the flag got me ```Hey, what's up. I got bored generating random numbers did you see?``` so I know that Alice was the one who sent the flag.

We need to find `shared_key = g ^ (a * b) =  A ^ b = B ^ a`. On the other hand, we can send B a fake g value so that:

```
fake_g = A -> Bob return B = fake_g ^ b = A ^ b = shared_key
```

Solve.py:
```python
from pwn import * # pip install pwntools
import json

HOST = "socket.cryptohack.org"
PORT = 13373

r = remote(HOST, PORT, level='debug')

def json_recv():
    line = r.readline()
    return json.loads(line.decode())

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)

r.recvuntil(b'Intercepted from Alice: ')
alice = json_recv()
p, g, A = alice['p'], alice['g'], alice['A']
print(f"p: {p}\ng: {g}\nA: {A}")

r.recvuntil(b'Intercepted from Alice: ')
secret_flag = json_recv()
iv, encrypted = secret_flag['iv'], secret_flag['encrypted']

r.recvuntil(b"Bob connects to you, send him some parameters:")

request = {
    "p": p,
    "g": A,
    "A": "0x0"
}
json_send(request)

r.recvuntil(b'Bob says to you: ')
bob = json_recv()
B = bob['B']
# B is now shared key

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import hashlib


def is_pkcs7_padded(message):
    padding = message[-message[-1]:]
    return all(padding[i] == len(padding) for i in range(0, len(padding)))


def decrypt_flag(shared_secret: int, iv: str, ciphertext: str):
    # Derive AES key from shared secret
    sha1 = hashlib.sha1()
    sha1.update(str(shared_secret).encode('ascii'))
    key = sha1.digest()[:16]
    # Decrypt flag
    ciphertext = bytes.fromhex(ciphertext)
    iv = bytes.fromhex(iv)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = cipher.decrypt(ciphertext)

    if is_pkcs7_padded(plaintext):
        return unpad(plaintext, 16).decode('ascii')
    else:
        return plaintext.decode('ascii')

shared_secret = int(B, 16)
print(hex(shared_secret))
ciphertext = encrypted

print(decrypt_flag(shared_secret, iv, ciphertext))
```

![image](https://github.com/user-attachments/assets/47da88f0-574c-4e44-9397-6ed3728d8138)

### Additive
Desc: Alice and Bob decided to do their DHKE in an additive group rather than a multiplicative group. What could go wrong?

So instead of 

```
A = g^a (mod p)
B = g^b (mod p)
key = B^a = A^b = g^(a*b) (mod p)
```

It is now 
```
A = g*a (mod p)
B = g*b (mod p)
key = A*b = B*a = g*a*b (mod p)
```

To find shared secret, we need to have `a` or `b`. Because this is in (G, +), we just have to calculate mod inverse of g instead of using discrete_log()

```python
ginv = pow(g, -1, p)
a = (A * ginv) % p
shared_secret = B * a % p
```

solve.py
```python
from pwn import * # pip install pwntools
import json
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import hashlib


HOST = "socket.cryptohack.org"
PORT = 13380

r = remote(HOST, PORT, level='debug')

def json_recv():
    line = r.readline()
    return json.loads(line.decode())

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)

def is_pkcs7_padded(message):
    padding = message[-message[-1]:]
    return all(padding[i] == len(padding) for i in range(0, len(padding)))


def decrypt_flag(shared_secret: int, iv: str, ciphertext: str):
    # Derive AES key from shared secret
    sha1 = hashlib.sha1()
    sha1.update(str(shared_secret).encode('ascii'))
    key = sha1.digest()[:16]
    # Decrypt flag
    ciphertext = bytes.fromhex(ciphertext)
    iv = bytes.fromhex(iv)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = cipher.decrypt(ciphertext)

    if is_pkcs7_padded(plaintext):
        return unpad(plaintext, 16).decode('ascii')
    else:
        return plaintext.decode('ascii')

r.recvuntil(b'Intercepted from Alice: ')
alice = json_recv()
p, g, A = int(alice['p'], 16), int(alice['g'], 16), int(alice['A'], 16)

r.recvuntil(b'Intercepted from Bob: ')
bob = json_recv()
B = int(bob['B'], 16)

ginv = pow(g, -1, p)
a = (A * ginv) % p
shared_secret = B * a % p

r.recvuntil(b'Intercepted from Alice: ')
secret_flag = json_recv()
iv, encrypted = secret_flag['iv'], secret_flag['encrypted']
print(decrypt_flag(shared_secret, iv, encrypted))
```

Result:
![image](https://github.com/user-attachments/assets/1801ce4a-016b-4220-a44f-bf414016046b)

### Script Kiddie
Everything is correct except for:

```python
def generate_public_int(g, a, p):
    return g ^ a % p

def generate_shared_secret(A, b, p):
    return A ^ b % p
```

Maybe a mistake since `^` in sage is equivalent to pow. So we just need to calculate `a` with `A ^ g = a` (don't care about p because g ^ a is not bigger than p)

solve.py

```python
from Crypto.Cipher import AES
import hashlib
import secrets


def is_pkcs7_padded(message):
    padding = message[-message[-1]:]
    return all(padding[i] == len(padding) for i in range(0, len(padding)))


def pkcs7_unpad(message, block_size=16):
    if len(message) == 0:
        raise Exception("The input data must contain at least one byte")
    if not is_pkcs7_padded(message):
        return message
    padding_len = message[-1]
    return message[:-padding_len]


def decrypt_flag(shared_secret: int, iv: str, ciphertext: str):
    # Derive AES key from shared secret
    sha1 = hashlib.sha1()
    sha1.update(str(shared_secret).encode('ascii'))
    key = sha1.digest()[:16]
    # Decrypt flag
    ciphertext = bytes.fromhex(ciphertext)
    iv = bytes.fromhex(iv)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = cipher.decrypt(ciphertext)
    return pkcs7_unpad(plaintext).decode('ascii')

p = 2410312426921032588552076022197566074856950548502459942654116941958108831682612228890093858261341614673227141477904012196503648957050582631942730706805009223062734745341073406696246014589361659774041027169249453200378729434170325843778659198143763193776859869524088940195577346119843545301547043747207749969763750084308926339295559968882457872412993810129130294592999947926365264059284647209730384947211681434464714438488520940127459844288859336526896320919633919
g = 2
A = 539556019868756019035615487062583764545019803793635712947528463889304486869497162061335997527971977050049337464152478479265992127749780103259420400564906895897077512359628760656227084039215210033374611483959802841868892445902197049235745933150328311259162433075155095844532813412268773066318780724878693701177217733659861396010057464019948199892231790191103752209797118863201066964703008895947360077614198735382678809731252084194135812256359294228383696551949882
B = 652888676809466256406904653886313023288609075262748718135045355786028783611182379919130347165201199876762400523413029908630805888567578414109983228590188758171259420566830374793540891937904402387134765200478072915215871011267065310188328883039327167068295517693269989835771255162641401501080811953709743259493453369152994501213224841052509818015422338794357540968552645357127943400146625902468838113443484208599332251406190345653880206706388377388164982846343351
iv = 'c044059ae57b61821a9090fbdefc63c5'
encrypted_flag = 'f60522a95bde87a9ff00dc2c3d99177019f625f3364188c1058183004506bf96541cf241dad1c0e92535564e537322d7'

# g ^ a = A 
# -> A ^ g = a 

a = A ^ g % p
shared_secret = B ^ a % p

flag = decrypt_flag(shared_secret, iv, encrypted_flag)
print(f'[+] Flag recovered: {flag}')
```

![image](https://github.com/user-attachments/assets/d229127e-a502-4d02-ba88-e79666544d68)
