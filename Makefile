all: compile

compile:
	@echo "Compiling..."
	gcc -o museum *.c *.h
