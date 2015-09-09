TEMPLATE	= app
TARGET		= rvrsserver
CONFIG		+= warn_on release
HEADERS		= TCPServer.h\
		  board.h
SOURCES		= main.cpp\
		  TCPServer.cpp\
		  board.cpp
