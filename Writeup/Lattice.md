# Cryptohack
## Noise Free
Desc: The addition of the noise term is what makes learning into learning with errors. How well can you learn if you don't have to deal with errors?

So in this challenge, you can either get a LWE sample of a character of the flag or encrypt a character of your own. The shape of b and S are (64, 1) so we need to get at least 64 samples in order to solve for S. After that, just calculate `m = b - A * S` for each character of the flag because there is no noise.

```python
from pwn import *
import json
import ast
import numpy as np
from sage.all import *

HOST = "socket.cryptohack.org"
PORT = 13411

r = remote(HOST, PORT)


def json_recv():
    line = r.readline()
    return json.loads(line.decode())

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)


print(r.readline())

A_matrix = []
b_vector = []

for i in range(64):
    request = {
        "option": "encrypt",
        "message": "0"
    }
    json_send(request)
    response = json_recv()
    A = list(map(int, ast.literal_eval(response["A"])))
    b = int(response["b"])
    A_matrix.append(A)
    b_vector.append(b)

q = 0x10001
F = GF(q)
A = Matrix(F, A_matrix)
b = vector(F, b_vector) - int("0")

S = A.solve_right(b)

for i in range(32):
    request = {
        "option": "get_flag",
        "index": f"{i}"
    }
    json_send(request)
    response = json_recv()
    A = list(map(int, ast.literal_eval(response["A"])))
    b = int(response["b"])
    A = vector(F, A)
    m = b - A * S
    print(chr(m), end='')
```

![image](https://github.com/user-attachments/assets/2cb81291-14b1-420f-97c3-6c8fc908b876)

## Bounded Noise

Desc: I find implementing gaussian sampling to be hard and slow, so I prefer binary sampling

From the key generation process and the size of A, we know that there are 625 samples, n = 25 and errors are in [0, 1], which are very convenient for Arora-Ge attack (small n, error bounded by 0 and 1, sufficient amount of samples)

solve.py
```python
import json
import ast
from sage.all import *

def arora_ge_attack(q, A, b, E):
    m = len(A)
    n = len(A[0])
    gf = GF(q)
    pr = gf[tuple(f"x{i}" for i in range(n))]
    gens = pr.gens()

    polys = []
    for i in range(m):
        p = 1
        inner = sum(A[i][j] * gens[j] for j in range(n))
        for e in E:
            p *= (b[i] - inner - e)
        polys.append(p)

    I = pr.ideal(polys)
    G = I.groebner_basis()

    s = []
    for p in G:
        if p.nvariables() == 1 and p.degree() == 1:
            root = -p.constant_coefficient()
            s.append(int(root))

    if len(s) == n:
        return s
    else:
        return None

with open("/home/falcon/Repository/Cryptohack/Lattice/BoundedNoise/output.txt") as f:
    data = json.load(f)

A_str = data["A"]
b_str = data["b"]

A_matrix = ast.literal_eval(A_str)
b_vector = ast.literal_eval(b_str)

A_matrix = [list(row) for row in A_matrix]
b_vector = list(b_vector)

secret_key = arora_ge_attack(0x10001, A_matrix, b_vector, [0, 1])
print("Arora-Ge attack result:", secret_key)

result = 0
q = 0x10001
for i in reversed(range(len(secret_key))):
    result = result * q + secret_key[i]

flag_bytes = result.to_bytes((result.bit_length() + 7) // 8, 'big')
print("Recovered flag:", flag_bytes.decode('utf-8', errors='ignore'))
```

![image](https://github.com/user-attachments/assets/4bb4249d-980e-4a4d-9ac3-157555f32363)

## Noise Cheap

Desc: A core part of making LWE secure is having the noise terms be larger than what lattice reduction algorithms can handle.

This is an example of how primal attack can be used since error is small compared to modulus q. Firstly, we need to construct a lattice, I choose this:

![image](https://github.com/user-attachments/assets/6c1764f1-2f00-4841-9aa9-29c9cd1f9d6a)

which gives us this when apply LLL:

![image](https://github.com/user-attachments/assets/91ff412d-da96-437a-b3e8-0911986b177b)

But actually, the message is hidden in the low bits of the LWE system.

```python
def encrypt(m):
    A = V.random_element()
    e = randint(-1, 1)
    b = A * S + m + p * e
    return A, b
```

So we need to scale down (A, b) by 257 in GF(q). This should scale e down to [-1, 0, 1] and should help LLL to find it.

```python
for x in range(n):
        A[x] = (A[x] * pow(p, -1, q)) % q
    b = (b * pow(p, -1, q)) % q
```

Next, reconstruct the secret key S and use the code in LWE Low Bits to solve for the flag.

Solve.py:
```python
from pwn import *
import json
import ast
import numpy as np
from sage.all import *

HOST = "socket.cryptohack.org"
PORT = 13413

r = remote(HOST, PORT)

def json_recv():
    line = r.readline()
    return json.loads(line.decode())

def json_send(hsh):
    request = json.dumps(hsh).encode()
    r.sendline(request)

print(r.readline())

A_matrix = []
b_vector = []
n = 64
p = 257
q = 1048583
m = 128

for i in range(m):
    request = {
        "option": "encrypt",
        "message": "0"
    }
    json_send(request)
    response = json_recv()
    A = list(map(int, ast.literal_eval(response["A"])))
    b = int(response["b"])

    # Scale down p * e -> e
    for x in range(n):
        A[x] = (A[x] * pow(p, -1, q)) % q
    b = (b * pow(p, -1, q)) % q
    
    A_matrix.append(A)
    b_vector.append(b)

F = GF(q)
A = Matrix(F, A_matrix)
b = vector(F, b_vector)
print(f"A: {A.nrows()}x{A.ncols()}")
print(f"b: {b}")

M = Matrix(ZZ, n + 1 + m, m + 1)

for i in range(m):
    M[0, i] = b[i]
M[0, m] = q
for i in range(n):
    for j in range(m):
        M[i + 1, j] = A[j, i] % q
for i in range(m):
    M[n + 1 + i, i] = q

L = M.LLL()

err = L[-1]
if err[-1] < 0:
    err = [-e for e in err]

e = vector(err[:-1])

print(f"e: {e}")

# Get the secret S
b_vec = vector(F, [(b[i] - e[i]) % q for i in range(m)])
S = A.solve_right(b_vec)
print(f"S: {S}")

for j in range(32):
    request = {
        "option": "get_flag",
        "index": f"{j}"
    }
    json_send(request)
    response = json_recv()
    A = list(map(int, ast.literal_eval(response["A"])))
    b = int(response["b"])
    A = vector(F, A)
    sum = 0
    for i in range(len(S)):
        sum += A[i] * S[i] % q
    x = b - int(sum) % q

    m = x % p
    print(chr(m), end='')
```

Result:

![image](https://github.com/user-attachments/assets/26ee8602-1ba2-41b6-8f09-1f622ce8db20)
