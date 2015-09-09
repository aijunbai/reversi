TEMPLATE	= app
TARGET		= rvrsmonitor
CONFIG		+= qt warn_on release
HEADERS		= monitor.h\
		  board.h\
		  common.h\
		  TCPClient.h\
		  comunication.h
SOURCES		= main.cpp\
		  monitor.cpp\
		  board.cpp\
		  TCPClient.cpp\
		  comunication.cpp
