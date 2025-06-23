import subprocess
import re

def run_aes_cryptopp(inputs):
    # Launch the binary
    proc = subprocess.Popen(
        ["aes_cryptopp.exe"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    # Join inputs with newline and encode as UTF-16 LE
    input_str = "\n".join(inputs) + "\n"  # Add final newline
    input_bytes = input_str.encode("utf-16-le")

    # Send input
    stdout, stderr = proc.communicate(input=input_bytes)

    # Decode output (program uses std::wcout -> UTF-16 LE)
    output_str = stdout.decode("utf-16-le", errors="ignore")

    match = re.search(
        r"Average time for (encryption|decryption) over 1000 rounds: [\d.]+ ms",
        output_str
    )

    if match:
        print(match.group(0))

def main():
    MODES = {
        "1": "ECB",
        "2": "CBC",
        "3": "OFB",
        "4": "CFB",
        "5": "CTR",
        "6": "XTS",
        "7": "CCM",
        "8": "GCM"
    }

    key_generate = [
        "2",           # Choose algorithm (AES, DES)
        "2",           # Choose mode
        "1",           # Generate key
        "keyfile.bin"
    ]

    encrypt = [
        "2",              # Choose algorithm (AES, DES)
        "2",              # Choose mode
        "2",              # Encrypt
        "keyfile.bin",
        "2",              
        "plaintext_10KB",
        "ciphertext"
    ]

    decrypt = [
        "2",           # Choose algorithm (AES, DES)
        "2",           # Choose mode
        "3",           # Decrypt
        "keyfile.bin",
        "2",           
        "ciphertext",
        "result"
    ]

    PLAINTEXT_FILES = [
        "plaintext_10KB",
        "plaintext_100KB",
        "plaintext_500KB",
        "plaintext_1024KB",
        "plaintext_3072KB",
        "plaintext_5120KB"
    ]
    for pt_file in PLAINTEXT_FILES:
        print(f"File {pt_file}")
        for mode_num in map(str, range(1, 6)):
            print(f"\n=== Testing DES Mode {mode_num} ({MODES[mode_num]}) ===")
            key_generate[1] = mode_num
            encrypt[1] = mode_num
            decrypt[1] = mode_num
            encrypt[5] = pt_file
            decrypt[5] = pt_file
            run_aes_cryptopp(key_generate)
            run_aes_cryptopp(encrypt)
            run_aes_cryptopp(decrypt)
        print()


if __name__ == "__main__":
    main()