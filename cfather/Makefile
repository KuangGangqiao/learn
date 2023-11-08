NAME=hello
all:
	as -o $(NAME).o $(NAME).s
	ld -s -o $(NAME) $(NAME).o

clean:
	rm $(NAME)
	rm *.o

