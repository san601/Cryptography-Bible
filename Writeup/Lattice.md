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

This is an example of how primal attack can be used since error is small compared to modulus q. Firstly, we need to construct a lattice, I choose this:

$$
\( M \in \mathbb{Z}^{n + m + 1 \times m + 1} \):

\[
M =
\begin{bmatrix}
b_1 & b_2 & \cdots & b_m & q \\
A_{1,1} & A_{2,1} & \cdots & A_{m,1} & 0 \\
A_{1,2} & A_{2,2} & \cdots & A_{m,2} & 0 \\
\vdots & \vdots & \ddots & \vdots & \vdots \\
A_{1,n} & A_{2,n} & \cdots & A_{m,n} & 0 \\
q & 0 & \cdots & 0 & 0 \\
0 & q & \cdots & 0 & 0 \\
\vdots & \vdots & \ddots & \vdots & \vdots \\
0 & 0 & \cdots & q & 0 \\
\end{bmatrix}
\]

- First row: the `b_i` values and `q` in the last column.
- Rows 2 to \(n+1\): transposed LWE matrix \( A^T \).
- Rows \(n+2\) to \(n+1+m\): scaled identity matrix \( q \cdot I_m \) to stabilize the lattice.
$$
