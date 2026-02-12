# Encrypt a file using a passphrase (use flag -p to enter the passphrase)
$ age -p -o encrypted.txt plaintext.txt

# Decrypt the file (use flag -i to specify input file path)
$ age -d -i encrypted.txt

# Generate a public key pair
$ age-keygen -o key.txt

# Encrypt a file using public key
$ age -r <public_key> -o encrypted.txt plaintext.txt

# Decrypt a file using the private key
$ age -d -i encrypted.txt -k <private_key>

