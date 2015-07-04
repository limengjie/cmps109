# $Id: Makefile,v 1.14 2014-06-25 17:42:27-07 - - $

MKFILE      = Makefile
#DEPFILE     = ${MKFILE}.dep
#NOINCL      = ci clean spotless
#NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
#GMAKE       = ${MAKE} --no-print-directory

COMPILECPP  = g++ -g -O0 -Wall -Wextra -rdynamic -std=gnu++11
MAKEDEPCPP  = g++ -MM

#CPPSOURCE   = commands.cpp debug.cpp inode.cpp util.cpp main.cpp
#CPPHEADER   = commands.h debug.h inode.h util.h
CPPSOURCE   = phase10.cpp command.cpp play_phase10.cpp
CPPHEADER   = phase10.h   command.h
EXECBIN     = play_phase10
OBJECTS     = ${CPPSOURCE:.cpp=.o}
#OTHERS      = ${MKFILE} README
#ALLSOURCES  = ${CPPHEADER} ${CPPSOURCE} ${OTHERS}
ALLSOURCES  = ${CPPHEADER} ${CPPSOURCE} 
#LISTING     = Listing.ps

#all : ${EXECBIN}
	#- checksource ${ALLSOURCES}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	${COMPILECPP} -c $<

#ci : ${ALLSOURCES}
	#cid + ${ALLSOURCES}
	#- checksource ${ALLSOURCES}

#lis : ${ALLSOURCES}
	#mkpspdf ${LISTING} ${ALLSOURCES} ${DEPFILE}

#clean :
	#- rm ${OBJECTS} ${DEPFILE} core ${EXECBIN}.errs

#spotless : clean
	#- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

#dep : ${CPPSOURCE} ${CPPHEADER}
	#@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	#${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} : ${MKFILE}
	@ touch ${DEPFILE}
	${GMAKE} dep

#again :
	#${GMAKE} spotless dep ci all lis


