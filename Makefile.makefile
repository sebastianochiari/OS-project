.PHONY: default, build, clean

ESEGUIBILE = $(wildcard ./src/main.c)

FILEC = $(wildcard ./src/init.c ./src/functions.c ./src/utilities.c)
FILEH = $(wildcard ./src/init.h ./src/functions.h ./src/utilities.h)

STDOUTFILE = $(wildcard out.txt)
STDERRFILE = $(wildcard err.txt)

# Default/senza target stampa nel terminale i comandi accettati dal programma e dal makefile
default:
	@echo ""
	@echo ""
	@echo "MAKEFILE COMMANDS:"
	@echo ""
	@echo "-> without a target: see default rule"
	@echo ""
	@echo "-> default : writes all the rules with a description"
	@echo ""
	@echo "-> build : compiles all the source code of the file 'main.c' in the src folder."
	@echo "To execute the program you should enter in the bin folder and then execute the file called 'shell'."
	@echo "There is a list of the optional parameters you can use:"
	@echo ""
	@echo "	-> --outfile=\"YourFile.txt\" or -o=\"YourFile.txt\" to specify the log for outputs"
	@echo "	-> --errfile=\"YourFile.txt\" or -e=\"YourFile.txt\" to specify the log for errors"
	@echo "	-> --log=\"YourFile\".txt to specify a unique log file"
	@echo "	-> -f to change log files detail level from SLIM (default) to FAT"
	@echo ""
	@echo "-> clean : deletes bin folder and its content"
	@echo ""
	@echo ""

# Genera il file eseguibili compilati.
# La dipendenza di build da clean sta nel fatto che 
# se esistono già i file, viene prima eseguita la regola clean e poi la regola build.
# Se errore in compilazione del file 'main.c' della MACRO, il file eseguibile non viene ovviamente generato
build: clean 
	@mkdir ./bin
	@gcc -std=gnu90 $(ESEGUIBILE) $(FILEC) $(FILEH)  -o ./bin/shell
	@echo ""
	@echo "** Your file has been succesfully compiled **"
	@echo ""
	@echo "To start the shell you need to enter in the bin directory."
	@echo ""
	@echo "Execute the file called 'shell'"
	@echo "You can use the following parameters:"
	@echo ""
	@echo "	-> --outfile=\"YourFile.txt\" or -o=\"YourFile.txt\" to specify the log for outputs"
	@echo "	-> --errfile=\"YourFile.txt\" or -e=\"YourFile.txt\" to specify the log for errors"
	@echo "	-> --log=\"YourFile\".txt to specify a unique log file"
	@echo "	-> -f to change log files detail level from SLIM (default) to FAT"
	@echo ""
	@echo "An example of execution code: ./shell -f --outfile=\"YourFile.txt\" --errfile=\"YourFile.txt\" "
	@echo ""

# Elimina i file e directory generate con la regola build (bin e il file eseguibile)
clean:
	@rm -rf ./bin
	@rm -f $(STDERRFILE)
	@rm -f $(STDOUTFILE)
	
