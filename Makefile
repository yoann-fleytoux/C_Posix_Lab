CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic -O2 -g
INCLUDE = ./
OBJETS_MON_SGF = mon_SGF.o
PROGRAMS = mon_formater  mon_affiche_SGF  mon_ls  mon_mkdir mon_rmdir

.SUFFIXES: .c .h .o

all: ${PROGRAMS}

mon_SGF.o :	mon_SGF.c  mon_SGF.h
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} -c $<

mon_formater:	mon_formater.c ${OBJETS_MON_SGF} mon_SGF.h
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} -o $@ $<   ${OBJETS_MON_SGF}

mon_affiche_SGF: mon_affiche_SGF.c ${OBJETS_MON_SGF} mon_SGF.h
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} -o $@ $<   ${OBJETS_MON_SGF}

mon_ls: 	mon_ls.c ${OBJETS_MON_SGF} mon_SGF.h
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} -o $@ $<   ${OBJETS_MON_SGF}

mon_mkdir:	mon_mkdir.c ${OBJETS_MON_SGF} mon_SGF.h
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} -o $@ $<   ${OBJETS_MON_SGF}

mon_rmdir:	mon_rmdir.c ${OBJETS_MON_SGF} mon_SGF.h
	@echo
	@echo "################  $@  ################"
	${CC} -I${INCLUDE} ${CFLAGS} -o $@ $<   ${OBJETS_MON_SGF}


clean :
	rm -f  ${PROGRAMS} ${OBJETS_MON_SGF} .device_name
