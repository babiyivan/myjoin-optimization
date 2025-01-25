# The optimization we want to build / test
SRCS = myjoin8.cpp

CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17 -pthread

TARGET = myjoin

OBJS = $(SRCS:.cpp=.o)

# Define default target
.PHONY: all build compare-small compare-large benchmark-local benchmark-small benchmark-large clean

all: build

# Build target
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compare targets
compare-small:
	cd .. && cd /localtmp/efficient24 && ~/EP-Group-12/optimized_implementations/$(TARGET) a.csv b.csv c.csv d.csv | sort | diff - abcd.csv
compare-large:
	cd .. && cd /localtmp/efficient24 && ~/EP-Group-12/optimized_implementations/$(TARGET) f1.csv f2.csv f3.csv f4.csv | sort | diff - output.csv

# Benchmark targets
benchmark-small:
	cd .. && cd /localtmp/efficient24 && perf stat ~/EP-Group-12/optimized_implementations/$(TARGET) a.csv b.csv c.csv d.csv > /dev/null
benchmark-large:
	cd .. && cd /localtmp/efficient24 && perf stat ~/EP-Group-12/optimized_implementations/$(TARGET) f1.csv f2.csv f3.csv f4.csv > /dev/null

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)