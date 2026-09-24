CXX = g++
CC = gcc

CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

main: src/*.cpp
	$(CXX) $(CXXFLAGS) -o build/$@ $^ $(LDFLAGS)

clean:
	rm -f ./build/main
