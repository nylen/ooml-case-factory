# Compiler, tools and options

CXX           = g++
CXXFLAGS      = -m64 -pipe -std=c++11 -Wall -Wno-sign-compare -W -fPIE
INCPATH       = -I/usr/include/ooml

LINK          = g++
LFLAGS        = -m64
LIBS          = -lOOMLCore -lOOMLComponents -lOOMLParts 

DEL_FILE      = rm -f



# Files

SOURCES       = main.cpp 
OBJECTS       = main.o

TARGET        = casefactory



# Rules

all: $(TARGET)


main.o: main.cpp geom.h cubieboard.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp


$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)


clean:
	-$(DEL_FILE) $(OBJECTS)
	
distclean: clean
	-$(DEL_FILE) $(TARGET)

