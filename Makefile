CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./include
TARGET   := eurosky_test

SRCS := src/main.cpp \
        src/Aircraft.cpp \
        src/Airport.cpp \
        src/Route.cpp \
        src/Graph.cpp \
        src/CostModel.cpp \
        src/Dijkstra.cpp \
        src/Bfs.cpp \
        src/Dfs.cpp \
        src/Montecarlo.cpp \
        src/Bellmanford.cpp \
        src/Pathfinder.cpp

build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: build run clean