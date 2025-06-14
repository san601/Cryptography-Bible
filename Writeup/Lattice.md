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
