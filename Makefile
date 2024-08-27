# Compiler and flags
CXX = g++
CXXFLAGS = -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 \
            -I/opt/homebrew/Cellar/opencv/4.10.0_4/include/opencv4 \
            -I/opt/homebrew/Cellar/tesseract/5.4.1/include \
            -I/opt/homebrew/Cellar/leptonica/1.84.1/include \
            -std=c++11

# Linker flags
LDFLAGS = -L/opt/homebrew/Cellar/tesseract/5.4.1/lib \
          -L/opt/homebrew/Cellar/leptonica/1.84.1/lib \
          -L/opt/homebrew/Cellar/opencv/4.10.0_4/lib

LIBS = -ltesseract -lleptonica -lopencv_core -lopencv_imgproc -lopencv_highgui

# Target executable
TARGET = main

# Source files
SRC = main.cpp textToMatrix.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target from object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) $(LIBS) -o $(TARGET)

# Rule to compile source files into object files
$(OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)

# Rule to clean up build files
clean:
	rm -f $(TARGET) $(OBJ)

# Rule to clean, compile, and run
go: clean all run
