# Overview

CryptoTools is a simple node-editor utility for debugging/developing cryptographic applications. It is powered by the Crypto++ library for any computation, and serves as a convenient graphical interface for building algorithms and cryptographic workflows from Crypto++ components. As an example, here is a first principles implementation of RSA encryption/decryption within the application which can be inspected at any stage for debugging purposes:

<img width="1792" alt="Screenshot 2023-07-24 at 12 18 59 AM" src="https://github.com/LevKruglyak/CryptoTools/assets/13054020/3c761a01-a9cc-4517-a0ac-087058e4f013">

# Building

CryptoTools uses CMake, so to build and run:

```sh
mkdir build
cd build
cmake ..
make
./cryptotools
```
