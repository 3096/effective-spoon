main : main.cpp
	g++ main.cpp -o splatsave -std=c++11 -lmbedcrypto

# Windows rule to use with mingw, mingw openssl pre-built binary: https://wiki.openssl.org/index.php/Binaries
win: main.cpp
	g++ main.cpp -o splatsave.exe -std=c++11 -lmbedcrypto -static-libstdc++ -static-libgcc

clean:
	$(RM) splatsave 
	$(RM) splatsave.exe
