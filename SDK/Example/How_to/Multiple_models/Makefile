#CROSS_COMPILE =aarch64-linux-gnu-
#CROSS_COMPILE =arm-hisiv500-linux-
CC = $(CROSS_COMPILE)gcc 
CPP = $(CROSS_COMPILE)g++ -std=c++11
CXX = $(CROSS_COMPILE)g++ -std=c++11
AR = $(CROSS_COMPILE)ar 
LINK=$(CROSS_COMPILE)g++

OPENCV = `pkg-config opencv --cflags --libs`
INCS =  -I./inc
COMMFLAG=  -DUSE_WD

LDFLAGS =   $(OPENCV) -lrt -ldl  -lusb-1.0  -lpthread  
# -lzlib
CFLAGS = -D_GNU_SOURCE -Wall  -O2 -g

PWD=$(shell pwd)

#TARGET =  $(notdir $(PWD))
TARGET =  Openncc
TARGET_LIB = lib/libOpenNCC.a
# change to you project file dir

# the obj dir
OBJDIR = obj
BINDIR = ../../bin
###########################################################################

TEST_SOURCES = $(wildcard *.cpp)
TEST_OBIS = $(TEST_SOURCES:%.cpp=%.o)
# head files

CFLAGS += $(INCS) $(COMMFLAG)
CXXFLAGS += $(INCS) $(COMMFLAG)

all: $(TARGET)
test:$(TARGET)

$(TARGET): $(TEST_OBIS)  
	@echo "TEST_SOURCES: $(TEST_SOURCES)"
	@test -d $(BINDIR) | mkdir -p $(BINDIR)
	$(CXX) -o $@ $^ $(TARGET_LIB) $(CXXFLAGS) $(LDFLAGS) 
	mv $@ $(BINDIR)/
	
%.o: %.c
	$(CC) -c $(CFLAGS)-o $@ $<

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
	
clean:
	rm -rf $(OBJDIR) *.o
	rm -f  $(BINDIR)/$(TARGET)
 
	
