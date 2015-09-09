TEMPLATE	= app
TARGET		= agent
CONFIG		+= qt warn_on release
HEADERS		= board.h\
		  common.h\
		  TCPClient.h\
		  eval.h\
		  cost.h\
		  agent.h\
		  version.h\
		  hash.h\
		  myrandom.h\
		  bitboard.h\
		  history.h
SOURCES		= main.cpp\
		  common.cpp\
		  board.cpp\
		  TCPClient.cpp\
		  eval.cpp\
		  cost.cpp\
		  agent.cpp\
		  hash.cpp\
		  myrandom.cpp\
		  bitboard.cpp\
		  history.cpp
