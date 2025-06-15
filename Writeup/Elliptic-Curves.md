# Cryptohack
## Efficient Exchange
Use discrete_log in sage to calculate `n` from `A = n_a * G`

```python
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


p = 310717010502520989590157367261876774703
F = FiniteField(p)
E = EllipticCurve(F, [2, 3])

G = E(179210853392303317793440285562762725654, 105268671499942631758568591033409611165)
A = E(280810182131414898730378982766101210916, 291506490768054478159835604632710368904)
n = A.log(G)
B = E(272640099140026426377756188075937988094, 51062462309521034358726608268084433317)
S = n * B
print(f'n: {n}')
print(f'S: {S}')

shared_secret = S[0]
iv = '07e2628b590095a5e332d397b8a59aa7'
ciphertext = '8220b7c47b36777a737f5ef9caa2814cf20c1c1ef496ec21a9b4833da24a008d0870d3ac3a6ad80065c138a2ed6136af'

print(decrypt_flag(shared_secret, iv, ciphertext))
```

![image](https://github.com/user-attachments/assets/1bf0d2a0-a9bc-46e7-9542-47c09bff9f30)

## Smooth Criminal

Desc: Spent my morning reading up on ECC and now I'm ready to start encrypting my messages. Sent a flag to Bob today, but you'll never read it.

So the parameters for the curve are poorly chosen, we can find n from `A = n * G` and calculate the shared secret between them. Sage have a built-in function to handle it.

```python
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


p = 310717010502520989590157367261876774703
F = FiniteField(p)
E = EllipticCurve(F, [2, 3])

G = E(179210853392303317793440285562762725654, 105268671499942631758568591033409611165)
A = E(280810182131414898730378982766101210916, 291506490768054478159835604632710368904)
n = A.log(G)
B = E(272640099140026426377756188075937988094, 51062462309521034358726608268084433317)
S = n * B
print(f'n: {n}')
print(f'S: {S}')

shared_secret = S[0]
iv = '07e2628b590095a5e332d397b8a59aa7'
ciphertext = '8220b7c47b36777a737f5ef9caa2814cf20c1c1ef496ec21a9b4833da24a008d0870d3ac3a6ad80065c138a2ed6136af'

print(decrypt_flag(shared_secret, iv, ciphertext))
```

Result:

![image](https://github.com/user-attachments/assets/5a889a3c-2a2f-4821-8027-4fc82e4cb494)
