CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = `sdl2-config --cflags --libs`

TARGET = pong

all: $(TARGET)

$(TARGET): pong.cpp
	$(CXX) $(CXXFLAGS) pong.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)