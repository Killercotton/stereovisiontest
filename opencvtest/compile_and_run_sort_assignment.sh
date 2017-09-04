# This file should be placed at the top level of your eclipse project
# i.e. it should be in the same folder as the "src" folder (containing .cpp files)
# and the "include" folder (containing .h files)

g++ src/main.cpp -std=c++11 -I include -o assignment4 -Wall -pthread -lpthread && ./assignment4
