

PROJECT       = graphics
SOURCE        = $(notdir $(wildcard src/*.cpp))
OBJECTS       = $(addprefix bin/,$(SOURCE:.cpp=.o))
DEPS          = $(addprefix bin/,$(SOURCE:.cpp=.d))


.PHONY: all clean

all: $(PROJECT).out

clean:
	rm -f $(PROJECT).out $(OBJECTS) $(DEPS)

bin/%.o: src/%.cpp
	g++ -c -MD -std=gnu++11 -I./include -o $@ $<

$(PROJECT).out: $(OBJECTS)
	gcc -o $@ $^ -lGL -lglfw -lGLEW -lm -lstdc++

-include $(DEPS)


