HDIR = .
CFLAGS = -g -I$(HDIR)
mybc: lexer.o main.o parser.o 
		$(CC) $^ -o $@

clean:
		$(RM) *.o

mostlyclean: clean
		$(RM) *~

targz: # para entregar
		tar zcvf mybc.tar.gz Makefile *.[ch]

# para extrair
# tar zxvf mybc.tar.gz