CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = ass1

SRCS = main.cpp B.cpp Bstar.cpp Bplus.cpp test.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)