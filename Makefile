CXXFLAGS = -g Wall -std=c++11
LDLIBS = -lglfw3 -lGLEW -lGL -lXrandr -lXinerama -lXcursor -lXi \
-lXxf86vm -lX11 -lpthread -lrt -lm ldl
OBJECTS = $(patsubst %.cpp,%.o,$(wildward *.cpp))
TARGET = slipAndSliding

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	-$(RM) $(TARGET) $(OBJECTS) *~ .*~ core
# -I/usr/local/include -L/usr/local/lib -lglfw3 \
# -lGLEW -framework OpenGL -framework CoreVideo -framework IOKit -framework Cocoa
