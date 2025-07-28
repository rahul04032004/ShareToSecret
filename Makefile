all:
\tg++ src/main.cpp src/shamir.cpp -o shamir -std=c++17

run:
\t./shamir

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRC = src/main.cpp src/shamir.cpp
OUT = secret_share

all: $(SRC)
\t$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

run: all
\t./$(OUT)

clean:
\trm -f $(OUT)
