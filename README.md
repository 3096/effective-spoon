# splatsave
C++ implementation of Splatoon 2 save decoding/encoding, supports up to save version 8 (update 4.3.1).

Requires [mbedtls](https://github.com/ARMmbed/mbedtls) and zlib to compile.

`make` to compile.

`make switch` to compile a Switch homebrew PoC version. It can be used with default file name or passing in args with nxlink.

### Thanks
- Lean for bombing me with hints when I reversed ver 4 shuffle.
