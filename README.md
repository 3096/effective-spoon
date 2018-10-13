# splatsave
C++ implementation of Splatoon 2 save decoding/encoding, with full encryption support and ver 4 shuffle.

Requires [mbedtls](https://github.com/ARMmbed/mbedtls) and zlib to compile.

`make` to compile.

`make switch` to compile a Switch homebrew PoC version. It can be used with default file name or passing in args with nxlink.

### Thanks
- Lean for bombing me with hints when I reversed ver 4 shuffle.
- SciresM for the original crypto script and csrngGetRandomBytes.
- Rajko on suggesting mbedtls and csrngGetRandomBytes.

Shout out to WulfyStylez. [Apparently he had save encryption figured out on day 1](https://github.com/WulfyStylez/switch-save-work/tree/master/splatcrypt) but nobody noticed, lol.
