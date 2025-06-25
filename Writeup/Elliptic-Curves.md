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

So the parameters for the curve are poorly chosen because the order of the curve can be factored into many smaller prime (smooth).

![image](https://github.com/user-attachments/assets/62e6228c-8694-44ae-a024-2e9ad0033bcf)

We can find n from `A = n * G` and calculate the shared secret between them. Sage have a built-in function to find n and even speed it up if the curve is smooth. From what I know, it uses a combination of Pohlig-Hellman and baby-step giant-step.

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

## Digestive

Desc: Should ECDSA be avoided for better digestive health?

From the source code of the server, we can see that it takes our username (which is, in my case, `falcon`), combine it with `{"admin": false}` and sign them altogether `sign({"admin": false, "username": "falcon"})`. But ECDSA only takes the left most n bit from the hash with n is the order of the generator (NIST FIPS 186-4 at the end of section 6.4). But this only work when you hash the message before signing it, which is not in this challenge:

```python
class HashFunc:
    def __init__(self, data):
        self.data = data

    def digest(self):
        # return hashlib.sha256(data).digest()
        return self.data
```

So basically when we use the signed value from `sign({"admin": false, "username": "falcon"})` and verify it with a bigger message (bigger than n bit, in this challenge n is 192), the algorithm only take the first 192 bit of our message to sign. One more thing to consider is it is common to keep the last occurrence like this:

```python
>>> json.loads('{"admin": false, "admin": true}')
{'admin': True}
```

So all we need to do is to add a `"admin": true` at the end of a long json string

```python
>>> len(json.dumps({"admin": False, "username": "falcon"}))
38
```

38 bytes is greater than 192 bits the curve takes so we are good to go.

```python
import requests

url = 'https://web.cryptohack.org/digestive/'

response = requests.get(f'{url}/sign/falcon').json()
print(response)
msg = '{"admin": false, "username": "falcon", "admin": true}'
print(requests.get(f'{url}/verify/{msg}/{response["signature"]}/').json()['flag'])
```

![image](https://github.com/user-attachments/assets/a9f29377-0046-4970-b7bf-107b94ba5262)
