all: compile

compile:
	@echo "Compiling..."
	gcc -pthread -o museum *.c *.h
