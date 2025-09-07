CFLAGS := -g

compiler: reader.o token.o lexer.o main.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f reader.o token.o lexer.o main.o compiler

.PHONY: clean
