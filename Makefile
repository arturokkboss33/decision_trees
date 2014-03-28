CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm


dectree_exe : dectree_bst.o dectree_class.o dectree_test.o
	${CC} ${CFLAGS} dectree_bst.o dectree_class.o dectree_test.o -o dectree_exe

dectree_bst.o : dectree_bst.cpp dectree_class.h dectree_node.h
	${CC} ${CFLAGS} -c dectree_bst.cpp

dectree_class.o : dectree_class.cpp dectree_class.h dectree_bst.h dectree_node.h
	${CC} ${CFLAGS} -c dectree_class.cpp

dectree_test.o : dectree_test.cpp dectree_class.h dectree_bst.h
	${CC} ${CFLAGS} -c dectree_test.cpp

clean :
	rm -rf *o dectree_exe

