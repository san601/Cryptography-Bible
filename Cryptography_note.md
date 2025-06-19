# Cryptography
This is my note for NT219.P21.ANTN - Cryptography
## Ciphers
### Symmetric (DES, AES)

A symmetric encryption scheme has five ingredients:
- Plaintext: This is the original intelligible message or data that is fed into the algorithm as input.
- Encryption algorithm: The encryption algorithm performs various substitutions and transformations on the plaintext.
- Secret key: he secret key is also input to the encryption algorithm. The key is a value independent of the plaintext and of the algorithm. The algorithm will  produce a different output depending on the specific key being used at the time. The exact substitutions and transformations performed by the  algorithm depend on the key.
- Ciphertext: This is the scrambled message produced as output. It depends on the plaintext and the key. For a given message, two different keys will produce two different ciphertexts.
- Decryption algorithm: This is essentially the encryption algorithm run in reverse. it accepts the ciphertext and the matching key and produces the original plaintext.

#### DES
- Data are encrypted in 64-bit blocks using a 56-bit key
- The algorithm transforms 64-bit input in a series of steps into a 64-bit output
- The same steps, with the same key, are used to reverse the encryption

![image](https://github.com/user-attachments/assets/48a0e324-0324-4fc0-b307-4daf0dc44ed1)

#### AES
Overview of AES:
1. KeyExpansion or Key Schedule:
From the 128 bit key, 11 separate 128 bit "round keys" are derived: one to be used in each AddRoundKey step.

2. Initial key addition: AddRoundKey - the bytes of the first round key are XOR'd with the bytes of the state.

3. Round - this phase is looped 10 times, for 9 main rounds plus one "final round"
- SubBytes - each byte of the state is substituted for a different byte according to a lookup table ("S-box").

`Note: SubBytes was designed to have a low correlation between input bits and output bits and the property that the output is not a linear mathematical function of the input`

- ShiftRows - the last three rows of the state matrix are transposed—shifted over a column or two or three.

![image](https://github.com/user-attachments/assets/d590a4db-6506-481c-890e-3cd43ad7f0ea)

- MixColumns - matrix multiplication is performed on the columns of the state, combining the four bytes in each column. This is skipped in the final round.

![image](https://github.com/user-attachments/assets/e977c1fb-a7e4-4757-9b07-749a0d08f6e2)

`Note: ShiftRows, along with MixColumns, create a transformation where the 4 bytes of one column are spread out to four different columns. Those 2 ensure that after a few rounds all output bits depend on all input bits.`

- AddRoundKey - the bytes of the current round key are XOR'd with the bytes of the state.

 ![image](https://github.com/user-attachments/assets/ca76bc38-d7ab-48d5-a2a0-bda46d1a23a1)

`Note: The ShiftRows and MixColumns work together to ensure every byte affects every other byte in the state within just two rounds. This creates non-linearity and stop cryptanalysts to attack each byte position in the state matrix separately.`

There are modes of operation:
- ECB: simple but can keep the structure of the original data, ciphertexts are the same if encrypted with the same key.

![image](https://github.com/user-attachments/assets/164b1e3a-dc36-4f4f-9b81-ecb34373768c)

- CBC: XOR the plaintext in the first block with an initialization vector (IV), then XOR the ciphertext of the previous block with the plaintext in the new block for the rest (chaining).

![image](https://github.com/user-attachments/assets/71a227d9-6394-4747-974f-8aa6fa2a546e)

- CFB

![image](https://github.com/user-attachments/assets/deb09902-3f97-4d02-9060-0b02e262b764)
 
- OFB

![image](https://github.com/user-attachments/assets/73d550a4-9f97-4a83-a54d-766df5f6eaa0)

`Note: bit errors in transmission do not propagate (can be used over noisy channel like satellite communication.`

- CTR:

![image](https://github.com/user-attachments/assets/050b28b3-71fa-4851-a808-eda3185938d3)

`Note: can leverage parallel computing -> fast`

- GCM: based on CTR, add a tag to provide both data confidentiality and authenticity.

![image](https://github.com/user-attachments/assets/0b218a76-6acf-41a4-b49c-b8ac92f2403e)

### Asymmetric (RSA, ElGamar, ECC)
Asymmetric cryptography
- Key distribution: How to have secure communications in general without having to trust a KDC with your key
- Digital signatures: How to verify that a message comes intact from the claimed sender

Asymmetric encryption is not more secure than Symmetric encryption, in fact, quantum algorithms can attack RSA and ECC but not AES. It is used to distribute the key of symmetric algorithms and verify the identity of the sender (public key certificate).

But this come with a cost: the runtime of asymmetric encryptions are generally much longer than symmmetric one and the key size are often huge. For example, the security available with a 1024-bit key using asymmetric RSA is considered approximately equal in security to an 80-bit key in a symmetric algorithm.

A public-key encryption scheme has six ingredients:
- Plaintext: This is the readable message or data that is fed into the algorithm as input.
- Encryption algorithm: The encryption algorithm performs various transformations on the plaintext.
- Private and public keys: This is a pair of keys that have been selected so that if one is used for encryption, the other is used for decryption. The exact transformations performed by the algorithm depend on the public or private key that is provided as input.
- Ciphertext: This is the encrypted message produced as output. It depends on the plaintext and the key. For a given message, two different keys will produce two different ciphertexts.
- Decryption algorithm: This algorithm accepts the ciphertext and the matching key and produces the original plaintext.

#### RSA
The RSA is based on Integer factorization Problem, that is, it relies on the difficulty of the factorisation of the modulus N. If the prime factors can be deduced, then we can calculate the Euler totient of N and thus decrypt the ciphertext.

The public key is an exponent `e` and a modulus `N = p * q` => (N, e). We encrypt a message by calculating `c = m^e mod N`. To decrypt it, we calculate `p = c^d mod N` with `d = e^-1 mod phi(N)` and `phi(N) = (p - 1) * (q - 1)`

With a very small e, RSA is vulnerable. Check this [link](https://ir0nstone.gitbook.io/crypto/rsa/public-exponent-attacks/small-e) out.

Now what if we use one big prime number instead of two to form N? In that case, `phi(N) = N - 1`.

How to sign with RSA? This [website](https://cryptohack.org/courses/public-key/rsa_starter_6/) explain:

Imagine you write a message `m` . You encrypt this message with your friend's public key: `c = m^e0 mod N0`. To sign this message, you calculate the hash of the message: `H(m)` and encrypt this with your private key `S = H(m)^d1 mod N1`

Your friend can decrypt the message using their private key `m = c^d0 mod N0`. Using your public key they calculate `s = S^e1 mod N1`.

Now he just need to compute `H(m)` and compare it to `s` to ensure that the message you sent them, is the message that they received.

#### ElGamar

![image](https://github.com/user-attachments/assets/8e1cdd23-aa83-437d-b882-f3e4fefd1ca8)

This is based on the discrete logarithm problem

![image](https://github.com/user-attachments/assets/194d15a7-9dad-4eac-8a9e-350a1eb1abc5)

#### ECC

![image](https://github.com/user-attachments/assets/021b3df0-eab8-4c57-be5a-392b53135de0)

P is the modulus of the finite field Fp over which the curve is defined. In this picture, p = 7. P-256 is equivalent to AES-128, P-512 is equivalent to AES-256.

Let's define addition

![image](https://github.com/user-attachments/assets/c890d0b0-f10f-416c-91b5-19da4d8699d8)

![image](https://github.com/user-attachments/assets/ed5ad33e-a509-4a0f-abec-863f78700719)

If non-standardized curve is used, the order of the curve can be factored and become 🍌. [For example](https://github.com/san601/Cryptography-Bible/blob/main/Writeup/Elliptic-Curves.md#smooth-criminal)

ECC Cipher

![image](https://github.com/user-attachments/assets/f5ea0592-0015-466f-878a-81d9a9662b04)

ECC Diffie-Hellman (ECDHE)

![image](https://github.com/user-attachments/assets/469dab9e-ba76-4385-a513-a84ad32a2c84)

`Note: secret key is a number, public key is a point on the curve`

This scheme is vulnerable to MITM attack in the next section.

#### Diffie-Hellman exchange protocol

![image](https://github.com/user-attachments/assets/196ec6cb-dfac-44da-a100-a8a562cfd070)

This is based on Diffie-Hellman Problem (DHP):

![image](https://github.com/user-attachments/assets/487afb8a-a34b-4e30-9f3b-c0835595c3cf)

The "hard problem" in Diffie-Hellman key exchange refers to the difficulty of computing the shared secret key (g^(ab)) given the public values (g^a) and (g^b), and the generator (g)

Without authentication, Diffie-Hellman is vulnerable to MITM attack. [A Cryptohack example](https://github.com/san601/Cryptography-Bible/blob/main/Writeup/Diffie-Hellman.md#man-in-the-middle-attack)

![image](https://github.com/user-attachments/assets/2e0a9fe5-72a2-48ab-9d1b-98082df52a94)

So for example we can intercept message from Alice and sent Bob anything we want. Alice's message contains `p, g, A = g^a`. When you are the man in the middle, you can generate a pair of `(m, M = g^m)` and send Bob `M`, after that, send `M` back to Alice. You now have 2 shared key with Alice and Bob without them knowing.

The diagram below shows how the process is done:

```
Alice --(p, g, A)--> You --(p, g, M)--> Bob
Alice <--(M)-- You <--(B)-- Bob
```

We need to verify who send us the message, which will be covered in the next section.
## Hash functions and MACs
A hash function maps a message of an arbitrary length to a l-bit output. MD5 and SHA-1 are phased out so we focus on SHA-2 and SHA-3

### SHA-2 (SHA-224, SHA-256, SHA-384, SHA-512)
SHA-2 processes input blocks sequentially. It divides the message into smaller blocks and process them sequentially, embedding one into the next. (Merkle-Damgard construction)

![image](https://github.com/user-attachments/assets/e26fe051-c5bd-4e35-aae3-d31afff8b63e)

For example, this is the preprocessing step.

![image](https://github.com/user-attachments/assets/8f861f9c-74bb-414a-a26a-c0e5b922e2c1)

After this, a bunch of operations happen. At the end of the loop, the hash values will be updated with results from the current block and start again until there are no blocks left.

```python
H = [(old + new) & 0xffffffff for old, new in zip(H, [a, b, c, d, e, f, g, h_var])]
```

SHA-256 and SHA-224 use 64 rounds, while SHA-512 and SHA-384 use 80 rounds. Each round involves bitwise operations, additions, and rotations, using 32-bit words in SHA-256 and 64-bit words in SHA-512.

For more information, please refer to the actual implementation of SHA-2.

The security parameters of SHA-2 are primarily determined by the length of the hash output, with options including 224, 256, 384, and 512 bits. These different output lengths offer varying levels of security, with longer outputs generally providing stronger resistance against attacks. 

But the Merkle-Damgard structure is what make SHA-1 and SHA-2 vulnerable to length extension attack. 

![image](https://github.com/user-attachments/assets/a2a44d41-28e7-4121-b01c-9733bfcdcee5)

### SHA-3
SHA-3 uses a sponge construction, instead of the Cipher Block Chaining mode of repeated compressions (each plaintext block is XORed with the previous ciphertext block before being encrypted) used by SHA-1, SHA-2, and Whirlpool. This is what make it resistant to length extension attack.

It use Absorb (24 rounds rounds, each round consists of these five functions θ, ρ, π, χ and ι, these rounds working on the full set of the state (1600 bits))

![image](https://github.com/user-attachments/assets/24e80995-222e-4fe6-b521-5e4403a446ac)

After 24 rounds, it start to Squeeze (take `r` bit from 1600 bits of the state):

![image](https://github.com/user-attachments/assets/254082c7-f3d4-4c57-a5fa-0e0b4a0c0430)

And then it take `n` bit from `r` (based on the length of the output), that's the digest we want.

The key parameters of SHA-3 are b: state size (b = r + c), r: bitrate (determine how much input data can be processed per round ), c: capacity and the number of round (can be deduced from b).

### HMAC
![image](https://github.com/user-attachments/assets/32a3f8a1-29cb-4197-85ea-120d281c2f5b)

![image](https://github.com/user-attachments/assets/cfdee0f2-24da-44d1-ae8b-c0e1ac1a0483)

Pros: HMAC is not vulnerable to length extension attacks, unlike certain MACs, especially those based directly on hash functions (like "Hash(message || key)")

Cons: requires secure channel for key exchange, reliance on underlying hash function...

`Note: From what I understand, using length extension on HMAC can only change the outer hash, not the actual message inside. As long as the original one is kept safe, we can know whether the message that the other side receive is our message or not.`

## Digital signature
`Note: the cost to compute digital signature is kinda big so it's not widely used like HMAC (like on devices with limited resources like IoT). Digital signature is not going to replace HMAC but it will be used in situation where you can't use HMAC (like when you need non-repudiation, HMAC with a single key can't provide you that)`

Main step of digital signature algorithms:
![image](https://github.com/user-attachments/assets/e9b90bc5-07a5-4b88-ad62-314e835ab8bc)

### RSASSA Signatures

### Digital Signature Algorithm
```
Key gen:
n bit prime q, 512-1024 bit prime p, (q-1) % p = 0 (public)
x in [1, q-1] (secret key) 
y = g^x (public key)

Signing:
r = (g^k mod p) mod q
s = k^-1 * (H(m) + x*r) mod q

Verify:
w = s^-1 mod q = k*(H(m) + x*r)^-1 mod q
u1 = H(m') * w mod q
u2 = r * w mod q
Check (g^u1 * y^u2 mod p) mod q = g^u1 * g^x*u2

We have u1 + x*u2 = H(m') * k*(H(m) + x*r)^-1 + x*r*k*(H(m) + x*r)^-1
= k*(H(m) + x*r)^-1 * (H(m') + x*r)
= k (if m == m')
g^k = r

So we just need to check whether g^u1 * y*u2 = r or not
```

![image](https://github.com/user-attachments/assets/cac5ada5-6aed-4870-8356-86e325cf58ca)

### Elliptic Curve Digital Signature Algorithm

```
n = ord(G)
order is the smallest number that nG = 0, in other words, n is the number of point on the curve
key generation:
d in [1, n-1] (secret)
Q = dG in E(Zp) (public)
Signing:
Choose k in [1, n-1]
R = kG = (x1, y1), r = x1
s = k^-1(H(m) + d*r) mod n
signature (r, s)
Verify:
w = s^-1 mod n = k*(H(m) + d*r) mod n
u1 = H(m') * w mod n = H(m') * k*(H(m) + d*r)^-1 mod n
u2 = r * w mod n = r * k*(H(m) + d*r)^-1 mod n

Verify v = u1*G + u2*Q mod n
= u1*G + u2*d*G = (u1 + u2*d)G mod n
= (H(m') * k*(H(m) + d*r)^-1 + r * d * k*(H(m) + d*r)^-1) * G mod n
= k*(H(m) + d*r)^-1 * (H(m') + r*d) * G mod n
= kG (if m == m')
```

## Cryptography Application
Scenario: Network system

Risk: 
- Transporting data: Everyone can read and modify (using packet sniffer tools like wireshark).
- Storing data: Can't encrypt everything, so we need access control to our system.
### Network Secure Protocols
#### Authentication
- Authentication: "Verifying the identity of a user, process, or device, often as a prerequisite to allowing access to resources in an information system."
- Authorization: "The right or a permission that is granted to a system entity to access a system resource."

Goal: Users authenticate Server (digital cert/pre-shared secret), Server authenticate Users (challenge response authentication

##### Certificate-based authentication
X.509 certificate includes the below elements:

- The public key
- The user or device’s name
- The name of the Certificate Authority (CA) that issued the certificate
- The date from which the certificate is valid
- The expiry date of the certificate
- The version number of the certificate data
- A serial number

![image](https://github.com/user-attachments/assets/2b8895ea-f764-4643-ab58-9ee497de2cb0)

##### Password-based authentication
If you store password as hash values, chances are if the database is leaked, the attacker can use dictionary attack. So password-based authentication alone can be insecure.

![image](https://github.com/user-attachments/assets/6eafa626-b3d0-4e80-84fc-9636235d3ce4)

#### Cryptographic algorithm negotiation
Cryptographic failure happen when in the negotiation step, one side choose an outdated or insecure algorithm and the other side agree with it (downgrade attack). For example this [Cryptohack challenge](https://github.com/san601/Cryptography-Bible/blob/main/Writeup/Diffie-Hellman.md#export-grade).

#### Key agreement

![image](https://github.com/user-attachments/assets/4dc16500-6a03-4701-83c2-759cc04e58ec)

Now User can verify Server using "tag", but how can Server vefify User?

##### Authenticate user/end device
Remember when I said that storing password hash alone is no longer secure because of the dictionary attack? It's time to use 2FA/MFA (biometric, smartcard, hardware-assisted, ...):
- Use OTP = H(sk, T) (through negotiation, DO NOT use SMS (SIM hijacking is a huge threat)
- Use smartcard (vincss fido2), masterkey with NFC
- Security module

![image](https://github.com/user-attachments/assets/b8cbcecc-da36-4ddd-8b0c-2693bf42929c)

#### Secure protocol implementation
In the Presentation layer (receive data from Application layer), TLS will add a TLS header into the packet

![image](https://github.com/user-attachments/assets/ae6f7592-6809-47a2-82f7-cfbb307c981d)

The reason we choose Presentation layer is because this implementation can be integrated into any application without modifying the application itself.

### Propose a secure network communication scenario
Key agreement:
- Use AES to encrypt data
- Use RSA/ECC/Elgama/Diffie Hellman to exchange AES key
- Add tag, certificate

End node authentication:
- Verify certificate with public key root trusted CA
- User verify public key from server
## Post-quantum cryptography (PQC)
To be updated
