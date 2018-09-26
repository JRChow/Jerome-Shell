# Macro
CC = gcc
#CC = gcc217m
#CFLAGS = -D NDEBUG -O
CFLAGS = -g

# Pattern rule
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Dependency rules for non-file targets
all: ishlex ishsyn ish
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f ishlex ishsyn ish *.o

# Dependency rules for file targets 
ishlex: synAnalyzer.o lexAnalyzer.o token.o dynarray.o  command.o ishlex.o
	$(CC) $(CFLAGS) $< lexAnalyzer.o token.o dynarray.o  command.o ishlex.o -o $@

ishsyn: synAnalyzer.o lexAnalyzer.o token.o dynarray.o command.o ishsyn.o
	$(CC) $(CFLAGS) $< lexAnalyzer.o token.o dynarray.o command.o ishsyn.o -o $@

ish: synAnalyzer.o lexAnalyzer.o token.o dynarray.o  command.o ish.o
	$(CC) $(CFLAGS) $< lexAnalyzer.o token.o dynarray.o  command.o ish.o -o $@

token.o: token.h ish.h

command.o: dynarray.h ish.h

dynarray.o: dynarray.h

lexAnalyzer.o: lexAnalyzer.h dynarray.h token.h ish.h

synAnalyzer.o: synAnalyzer.h dynarray.h token.h command.h ish.h

ishlex.o: lexAnalyzer.h dynarray.h token.h ish.h

ishsyn.o: lexAnalyzer.h synAnalyzer.h dynarray.h token.h command.h ish.h

ish.o: lexAnalyzer.h synAnalyzer.h command.h
