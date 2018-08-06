# splatsave
My C++ implementation of SciresM's Splatoon 2 [save crypto script](https://gist.github.com/SciresM/27f66899b9504343c32b71f05368c654) with a few additions including crc32 fix.

Requires [mbedtls](https://github.com/ARMmbed/mbedtls) and zlib to compile.

`make` to compile normally.

`make win` to compile a Windows build with mingw and static linking.

`make switch` to compile a Switch homebrew PoC version. It can be used with default file name or passing in args with nxlink.

### Thanks
- SciresM for the original script and csrngGetRandomBytes, guy knows magic.
- Rajko on suggesting mbedtls and csrngGetRandomBytes.

Shout out to WulfyStylez. I only found [his work](https://github.com/WulfyStylez/switch-save-work/tree/master/splatcrypt) on Splatoon 2 save crypto after I finished writing this, lol.
