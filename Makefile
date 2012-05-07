
#LIBLAB FLAGS
LLABCPPFLAGS  	:= 	-I$(HOME)/include/liblab 
LLABCPPFLAGS	+= 	-I$(HOME)/lib
LLABLDFLAGS		:= 	-L$(HOME)/include/liblab   
LLABLDFLAGS		+= 	-L$(HOME)/lib -llab -lsubsystem -lpthread -lusb

#MODBUSFLAGS
MODBUSCFLAGS	+= 	-I/home/veloxid/sdvlp/libmodbus-3.0.1/src
MODBUSLDFLAGS	+= 	-lmodbus

#SCPI-Interpreter FLAGS
#SCPICPPFLAGS		+= -I../scpiInterpreter
#SCPILDFLAGS			+= ../scpiInterpreter/scpiInterpreter.o


ROOTCFLAGS		:= 	$(shell root-config --cflags)
ROOTGLIBS   	:=	$(shell root-config --glibs)

LDFLAGS	      	:= 	-g $(LLABLDFLAGS) $(ROOGTLIBS)  $(SCPILDFLAGS)
#$(MODBUSLDFLAGS)
SOFLAGS       	:= 	-shared

CPP         	:= 	g++
CPPFLAGS    	:= 	-fPIC $(LLABCPPFLAGS) $(ROOTCFLAGS) $(SCPICPPFLAGS)

CXXFLAGS 		:=	-O4 -g  -fmessage-length=0 $(CPPFLAGS) -Wall

OBJS 			=	keithleyClient.o keithleyInterface.o keithleySubClientHandler.o
OBJS			+=	scpiInterpreter.o

TARGET =	keithleyClient

$(TARGET):	$(OBJS)
	$(CPP) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o:	%.cpp | %.h| %.hh
	@echo -n ">>> Compiling "
	$(CPP) $(CXXFLAGS) $<


all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
