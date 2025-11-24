CXX = g++
TARGET = snake

SRC = src/main.cpp

# Ralib
RAYLIB_DIR = C:/raylib
INC = -I$(RAYLIB_DIR)/include
LIB = -L$(RAYLIB_DIR)/lib

# Linker flags
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -static

# Build
$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(INC) $(LIB) $(LDFLAGS)

run: $(TARGET)
	make
	./$(TARGET)

clean:
	rm -f $(TARGET).exe $(TARGET)