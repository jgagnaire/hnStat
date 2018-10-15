#
# JG - hnStat
#

CXX		= g++

INC_FOLDER	= inc

SRC_FOLDER	= src

TESTS_FOLDER	= unit_tests

CXXFLAGS	= -std=c++11 -W -Wall -Wextra -I$(INC_FOLDER)

LDFLAGS		= -lgtest -pthread

NAME		= hnStat

TESTS		= tests_$(NAME)

SRCS		= $(SRC_FOLDER)/TopXQueryContainer.cpp \
		$(SRC_FOLDER)/DistinctQueryContainer.cpp \
		$(SRC_FOLDER)/ReaderThread.cpp \
		$(SRC_FOLDER)/SearchFilter.cpp \
		$(SRC_FOLDER)/QueryProcessor.cpp \
		$(SRC_FOLDER)/File.cpp \
		$(SRC_FOLDER)/main.cpp

OBJS		= $(SRCS:.cpp=.o)

SRCS_TESTS	= $(TESTS_FOLDER)/TestFile.cpp \
		$(SRC_FOLDER)/File.cpp \
		$(TESTS_FOLDER)/TestSearchFilter.cpp \
		$(SRC_FOLDER)/SearchFilter.cpp \
		$(TESTS_FOLDER)/main.cpp

OBJS_TESTS	= $(SRCS_TESTS:.cpp=.o)

RM		= rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(NAME) $(LDFLAGS)

$(TESTS):	$(OBJS_TESTS)
	$(CXX) $(OBJS_TESTS) $(CXXFLAGS) -o $(TESTS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(OBJS_TESTS)

fclean:		clean
	$(RM) $(NAME) $(TESTS)

re:		fclean all

.PHONY:		re all clean fclean
