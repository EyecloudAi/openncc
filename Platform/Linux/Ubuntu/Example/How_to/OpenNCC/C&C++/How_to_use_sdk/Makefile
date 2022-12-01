#CROSS_COMPILE =aarch64-linux-gnu-
#CROSS_COMPILE =arm-hisiv500-linux-
CC = $(CROSS_COMPILE)gcc 
CPP = $(CROSS_COMPILE)g++ -std=c++11
CXX = $(CROSS_COMPILE)g++ -std=c++11
AR = $(CROSS_COMPILE)ar 
LINK=$(CROSS_COMPILE)g++

OPENCV = `pkg-config opencv --cflags --libs`
INCS =  -I./inc  -I/usr/include/opencv2
LIBCUR = `pkg-config libcurl --cflags --libs`
COMMFLAG=  -DUSE_WD 

LDFLAGS =   $(OPENCV) $(LIBCUR)  -lrt -ldl  -lusb-1.0  -lpthread  
#LDFLAGS += -L/opt/libusb/lib  -L/opt/opencv/lib 
# -lzlib
CFLAGS = -D_GNU_SOURCE -Wall  -O2 -g

PWD=$(shell pwd)

#TARGET =  $(notdir $(PWD))
TARGET =  OpenNCC
TARGET_LIB = lib/libOpenNCC.a
# change to you project file dir

# the obj dir
OBJDIR = obj
BINDIR = bin
###########################################################################

TEST_SOURCES = $(wildcard *.cpp)
TEST_OBIS = $(TEST_SOURCES:%.cpp=%.o)
# head files

CFLAGS += $(INCS) $(COMMFLAG)
CXXFLAGS += $(INCS) $(COMMFLAG)

help:
	@echo
	@echo "\t|--------------------------------------------------------------|"
	@echo "\t| Help                                                         |"
	@echo "\t| ----                                                         |"
	@echo "\t| make clone : Clone or update lib                             |"
	@echo "\t| make all   : Clone lib and build application                 |"
	@echo "\t| make run   : Clone lib and run application after build       |"
	@echo "\t| make clean : Clean application                               |"
	@echo "\t|--------------------------------------------------------------|"
	@echo

all: $(TARGET)
run: $(TARGET) 
	sudo ./script/run.sh
clone:
	./script/clone.sh

$(TARGET): $(TEST_OBIS)  
	./script/clone.sh	
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

