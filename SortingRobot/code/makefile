CXX = g++
CXXFLAGS = -std=c++11 -I$(RUNTIME) -I$(SRC) -I/usr/local/include -L/usr/local/lib -lwiringPi -lrt -lpthread -lmosquitto
CPPFLAGS = -I$(SRC) -I$(RUNTIME)
LDFLAGS = -lpthread
TARGET = dznpi
SRC = ./src
RUNTIME = ./lib
SRCS = $(wildcard $(SRC)/*.cc)
SRCS += $(wildcard $(RUNTIME)/*.cc)
OBJS = $(subst .cc,.o,$(SRCS))

all:
	make $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)