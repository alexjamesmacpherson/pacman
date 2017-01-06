LDLIBS = -lfreeglut -lopengl32 -lglu32  -lm -lpng

CXXFLAGS= -O3  -std=c++11
LDFLAGS= $(CXXFLAGS) $(LIBDIRS)

TARGETS = 

SRCS = 

OBJS =  $(SRCS:.cpp=.o)

CXX = g++

default: $(TARGETS)
