# This is my note for NT219.P21.ANTN-Cryptography
## Ciphers
### Symmetric (DES, AES)
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
- ShiftRows - the last three rows of the state matrix are transposed—shifted over a column or two or three.

![image](https://github.com/user-attachments/assets/d590a4db-6506-481c-890e-3cd43ad7f0ea)

- MixColumns - matrix multiplication is performed on the columns of the state, combining the four bytes in each column. This is skipped in the final round.

![image](https://github.com/user-attachments/assets/e977c1fb-a7e4-4757-9b07-749a0d08f6e2)

- AddRoundKey - the bytes of the current round key are XOR'd with the bytes of the state.

 ![image](https://github.com/user-attachments/assets/ca76bc38-d7ab-48d5-a2a0-bda46d1a23a1)

`Note: The ShiftRows and MixColumns work together to ensure every byte affects every other byte in the state within just two rounds. This creates non-linearity and stop cryptanalysts to attack each byte position in the state matrix separately.`

There are modes of operation:
- ECB: simple but can keep the structure of the original data, ciphertexts are the same if encrypted with the same key.

![image](https://github.com/user-attachments/assets/164b1e3a-dc36-4f4f-9b81-ecb34373768c)

- CBC: XOR the plaintext in the first block with an initialization vector (IV), then XOR the ciphertext of the previous block with the plaintext in the new block for the rest (chaining).

![image](https://github.com/user-attachments/assets/71a227d9-6394-4747-974f-8aa6fa2a546e)


### Asymmetric (RSA, ElGamar, ECC)
Asymmetric cryptography
- Key distribution: How to have secure communications in general without having to trust a KDC with your key
- Digital signatures: How to verify that a message comes intact from the claimed sender

Asymmetric encryption is not more secure than Symmetric encryption, in fact, quantum algorithms can attack RSA and ECC but not AES. It is used to distribute the key of symmetric algorithms and verify the identity of the sender (public key certificate).

But this come with a cost: the runtime of asymmetric encryptions are generally much longer than symmmetric one and the key size are often huge. For example, the security available with a 1024-bit key using asymmetric RSA is considered approximately equal in security to an 80-bit key in a symmetric algorithm.
#### RSA

The RSA is based on Integer factorization Problem

![image](https://github.com/user-attachments/assets/78ad128a-7e16-4375-a094-b3ca59774b53)

#### ElGamar

![image](https://github.com/user-attachments/assets/8e1cdd23-aa83-437d-b882-f3e4fefd1ca8)

This is based on the discrete logarithm problem

![image](https://github.com/user-attachments/assets/194d15a7-9dad-4eac-8a9e-350a1eb1abc5)

#### ECC

If non-standardized curve is used, the order of the curve can be factored and become 🍌. [For example](https://github.com/san601/Cryptography-Bible/blob/main/Writeup/Elliptic-Curves.md#smooth-criminal)

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

## Hash functions and MACs
A hash function maps a message of an arbitrary length to a l-bit output. MD5 and SHA-1 are phased out so we focus on SHA-2 and SHA-3

### SHA-2 (SHA-224, SHA-256, SHA-384, SHA-512)
SHA-2 processes input blocks sequentially. It divides the message into smaller blocks and process them sequentially, embedding one into the next. (Merkle-Damgard construction)

![image](https://github.com/user-attachments/assets/e26fe051-c5bd-4e35-aae3-d31afff8b63e)

The security parameters of SHA-2 are primarily determined by the length of the hash output, with options including 224, 256, 384, and 512 bits. These different output lengths offer varying levels of security, with longer outputs generally providing stronger resistance against attacks. 

But the Merkle-Damgard structure is what make SHA-1 and SHA-2 vulnerable to length extension attack. 

![image](https://github.com/user-attachments/assets/a2a44d41-28e7-4121-b01c-9733bfcdcee5)

### SHA-3
SHA-3 uses a sponge construction, instead of the Cipher Block Chaining mode of repeated compressions (each plaintext block is XORed with the previous ciphertext block before being encrypted) used by SHA-1, SHA-2, and Whirlpool. This is what make it resistant to length extension attack.

It use Absorb:
![image](https://github.com/user-attachments/assets/24e80995-222e-4fe6-b521-5e4403a446ac)

After n round, it start to Squeeze (take r bit from b bit of the whole state):
![image](https://github.com/user-attachments/assets/254082c7-f3d4-4c57-a5fa-0e0b4a0c0430)

The key parameters of SHA-3 are b: state size (b = r + c), r: bitrate (determine how much input data can be processed per round ), c: capacity and the number of round (can be deduced from b).

### HMAC
![image](https://github.com/user-attachments/assets/32a3f8a1-29cb-4197-85ea-120d281c2f5b)

![image](https://github.com/user-attachments/assets/cfdee0f2-24da-44d1-ae8b-c0e1ac1a0483)

Pros: HMAC is not vulnerable to length extension attacks, unlike certain MACs, especially those based directly on hash functions (like "Hash(message || key)")

Cons: requires secure channel for key exchange, reliance on underlying hash function...

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

## Post-quantum cryptography (PQC)
To be updated
