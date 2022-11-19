all: serverM.cpp serverC.cpp serverEE.cpp serverCS.cpp client.cpp
	g++ -o serverM -std=c++11 serverM.cpp
	g++ -o serverC -std=c++11 serverC.cpp
	g++ -o serverEE -std=c++11 serverEE.cpp
	g++ -o serverCS -std=c++11 serverCS.cpp
	g++ -o client -std=c++11 client.cpp

clean:
	rm ./serverM
	rm ./serverC
	rm ./serverEE
	rm ./serverCS
	rm ./client