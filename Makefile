

PROJECT       = graphics
SOURCE        = $(wildcard *.cpp)
OBJECTS       = $(SOURCE:.cpp=.o)
DEPS          = $(SOURCE:.cpp=.d)


.PHONY: all clean

all: $(PROJECT).out

clean:
	rm -f $(PROJECT).out $(OBJECTS) $(DEPS)

.cpp.o:
	gcc -c -MD -std=gnu++11 -o $@ $<

$(PROJECT).out: $(OBJECTS)
	gcc -o $@ $^ -lGL -lglfw -lGLEW -lm -lstdc++

-include $(DEPS)


