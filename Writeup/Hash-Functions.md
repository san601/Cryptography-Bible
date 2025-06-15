# Cryptohack
## MD0
Desc: I've invented a nice simple version of HMAC authentication, hopefully it isn't vulnerable to the same problems as Merkle–Damgård construction hash functions...

From the provided source code, we can see that the `hash` function take a block of 16 bytes to process at a time, involving the previous state.

```python
def bxor(a, b):
    return bytes(x ^ y for x, y in zip(a, b))


def hash(data):
    data = pad(data, 16)
    out = b"\x00" * 16
    for i in range(0, len(data), 16):
        blk = data[i:i+16]
        out = bxor(AES.new(blk, AES.MODE_ECB).encrypt(out), out)
    return out
```

So we just need to send a dummy message, take the signature that the server returns and use it as the previous state like this:

```python
msg = b"aaaaaaaaaaaaaaaa"

request = {
    "option": "sign",
    "message": msg.hex()
}
print(request)
json_send(request)
r.readline()
response = json_recv()
sign = bytes.fromhex(response["signature"])

blk = pad(b"admin=True", 16)

# Start a new round with my message and the signature as the previous state
out = bxor(AES.new(blk, AES.MODE_ECB).encrypt(sign), sign)
```

Next, just send the message that contains your dummy message and `"admin=True"`:

```python
msg = pad(b"aaaaaaaaaaaaaaaa", 16) + b"admin=True"
payload = {"option": "get_flag", "signature": f"{out.hex()}", "message": f"{msg.hex()}"}
json_send(payload)
r.readline()
response = json_recv()
print("Flag:", response["flag"])
```

Result:

![image](https://github.com/user-attachments/assets/256b1368-a057-4acb-bea2-59b659be4915)
