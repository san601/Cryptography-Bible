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
