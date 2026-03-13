CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML_INCLUDE = -I/opt/homebrew/include
SFML_LIBS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lpthread

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = graphics_editor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
