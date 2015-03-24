CXX = 		g++
CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =		broxy.o

LIBS =		-lthrift -lcurl 

TARGET =	broxy

SRC_DIR = gen-cpp/

DEP = $(SRC_DIR)broxy_constants.cpp $(SRC_DIR)broxy_types.cpp $(SRC_DIR)Service.cpp

CLIENT_SRC = $(SRC_DIR)Service_client.cpp

SERVER_SRC = $(SRC_DIR)Service_server.cpp $(SRC_DIR)Util.cpp

LD_FLAGS = "-Wl,-rpath,/usr/local/lib/"

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

client:	$(DEP) $(CLIENT_SRC)
	$(CXX) $^ -o $@ $(LIBS) $(CXXFLAGS) $(LD_FLAGS)
	
server: $(DEP) $(SERVER_SRC)
	$(CXX) $^ -o $@ $(LIBS) $(CXXFLAGS) $(LD_FLAGS)

clean:
	rm -f $(OBJS) $(TARGET) client server
