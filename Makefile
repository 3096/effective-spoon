main : main.cpp
	g++ main.cpp -o splatsave -std=c++11 -lcrypto

clean:
	rm splatsave
