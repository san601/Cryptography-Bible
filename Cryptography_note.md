# This is my note for NT219.P21.ANTN-Cryptography
## Ciphers
### Symmetric (DES, AES)
#### DES
#### AES
### Asymmetric (RSA, ElGamar, ECC)
#### RSA
#### ElGamar
#### ECC

If non-standardized curve is used, the order of the curve can be factored and become 🍌. [For example](https://github.com/san601/Cryptography-Bible/blob/main/Writeup/Elliptic-Curves.md#smooth-criminal)

## Hash functions and MACs
A hash function maps a message of an arbitrary length to a l-bit output. MD5 and SHA-1 are phased out so we focus on SHA-2 and SHA-3

If you store password as hash values, chances are if the database is leaked, the attacker can use dictionary attack. So password-based authentication alone can be insecure.
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
Goal: Users authenticate Server (digital cert/pre-shared secret), Server authenticate Users (challenge response authentication)


#### Cryptographic algorithm negotiation
#### Key agreement
#### Secure protocol implementation

## Digital Signatures
### Discrete logarithm-based
### ECC-based
## Digital certificate
