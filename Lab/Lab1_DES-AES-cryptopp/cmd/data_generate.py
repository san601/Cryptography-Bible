import os

def generate_scaled_files(input_file, target_sizes_kb):
    # Read the contents of the original file
    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()

    content_size = len(content.encode('utf-8'))  # original size in bytes
    print(f"Original file size: {content_size / 1024:.2f} KB")

    for size_kb in target_sizes_kb:
        target_size_bytes = size_kb * 1024
        repeat_count = target_size_bytes // content_size

        # Construct the new content
        new_content = content * int(repeat_count)

        # Pad the remaining bytes if needed
        remaining = target_size_bytes - len(new_content.encode('utf-8'))
        if remaining > 0:
            new_content += content[:remaining]

        output_file = f"{os.path.splitext(input_file)[0]}_{size_kb}KB"
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(new_content)

        print(f"Generated {output_file} of size ~{os.path.getsize(output_file) / 1024:.2f} KB")

# Example usage
generate_scaled_files("plaintext", [3072])
