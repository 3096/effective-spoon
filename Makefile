main : main.cpp
	g++ main.cpp -o splatsave -std=c++11 -lcrypto

# Windows rule to use with mingw, mingw openssl pre-built binary: https://wiki.openssl.org/index.php/Binaries
win: main.cpp
	g++ main.cpp -o splatsave.exe -std=c++11 -lcrypto -static-libstdc++ -static-libgcc

clean:
	$(RM) splatsave 
	$(RM) splatsave.exe
