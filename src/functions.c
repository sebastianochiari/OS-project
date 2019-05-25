#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

#include "functions.h"
#include "init.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

// gestore dei comandi passati in input dall'utente nel prompt personalizzato
void wrapperCommand() {
    // comando intero digitato dall'utente
    char* line = NULL;
    // comando senza eventuali parametri
    char* command = NULL;
    // stringa con parametri passati dall'utente
	char* parameters = NULL;
    // lunghezza stringa
    int len;

    // lettura input utente
    getline(&line, (size_t*)&len, stdin);
	if (line[strlen(line) - 1] == '\n') {
        // inserimento di carattere di terminazione della stringa
        line[strlen(line) - 1] = '\0';
    }
	
    // estrazione del comando
    command = getCommand(line);
    // estrazione dei parametri
	parameters = getParameters(line);

    // esecuzione del comando con relativi parametri inseriti dall'utente
    executeCommand(command, parameters, line);
} 

// funzione che a partire dall'intero input estrae il comando
char* getCommand(char* input) {
    // variabile di controllo per la lunghezza parziale del comando
    int len;
    // variabile di controllo per la lunghezza totale dell'input
    int totalLen;
    // variabile di servizio
    int i;
    // variabile di servizio per la memorizzazione temporanea del comando
	char* command = NULL;
	// lunghezza del comando
	if (strchr(input, ' ') != NULL) { // cerca il carattere ' '
		len = strlen(strchr(input, ' '));
	} else {
		len = 0;
	}
	totalLen = strlen(input);
	// creazione della nuova stringa
	command = malloc((totalLen - len) + 1);
	for (i = 0; i < (totalLen - len); i++) {
        command[i] = input[i];
    }
	// aggiunta del carattere per concludere la stringa
	command[totalLen - len] = '\0';
	return command;
}

// funzione che a partire dall'intero input estrae gli eventuali parametri del comando
char* getParameters(char* input) {
    // variabile di controllo per la lunghezza parziale del parametro
    int len;
    // variabile di controllo per la lunghezza totale dell'input
    int totalLen;
    // variabile di servizio
    int i;
    // variabile di servizio per la memorizzazione temporanea dei parametri
	char* parameters = NULL;
	// lunghezza parametri -1 dovuto dallo spazio iniziale
	if (strchr(input, ' ') != NULL) { // cerca il carattere ' '
		len = strlen(strchr(input, ' ')) - 1;
	} else {
		len = 0;
	}
	totalLen = strlen(input);
	// creazione della nuova stringa
	if (len > 0) { // ci sono parametri
		parameters = malloc(len + 1);
		for (i = (totalLen - len); i < strlen(input); i++) {
            parameters[i-(totalLen - len)] = input[i];
        }
		// aggiunta del carattere per concludere la stringa
		parameters[len] = '\0';

	}
	return parameters;
}

// funzione che esegue il comando e stampa nei file di log
void executeCommand(char* command, char* parameters, char* line) {
    int result = 1;
    int pidFiglio, pidNipote, cFiglio, cNipote;

    int defout = dup(STDOUT_FILENO);
    if(defout == -1) {
        printf("An error has occurred during standard output duplication\n");
    }
    int deferr = dup(STDERR_FILENO);
    if(deferr == -1) {
        printf("An error has occurred during standard error duplication\n");
    }

    if(strcmp(command, "quit") == 0) {
        if(flagL == 0) {
            closeFile(fdLog);
        }
        else {
            closeFile(fdOutput);
            closeFile(fdError);
        }
        exit(1);
	} else if(strcmp(command, "help") == 0) {
        helpFunction();
    } else if(strcmp(command, "changeLevel") == 0) {
        changeLevel();
    } else if(strcmp(command, "pico") == 0 || (strcmp(command, "vim")==0)) {
        printf("\nI am sorry, but the command '%s' is not yet supported. Write 'help' to see the full list of commands\n", command);
    } else {
        pidFiglio = fork();
        if (pidFiglio == -1) {
            perror("An error has occurred during forking");
            exit(1);
        }       
        
        if (pidFiglio == 0) {
            // stampa output/error su shell
            redirectFile(defout, STDOUT_FILENO);
            redirectFile(deferr, STDERR_FILENO);

            cFiglio = system(line);
            if(cFiglio == -1) {
                fprintf(stderr, "An error has occurred during '%s' execution\n", line);
            } else if(WEXITSTATUS(cFiglio) == 127 || cFiglio == 256 || WEXITSTATUS(cFiglio) == 2) {
                fprintf(stderr, "Shell command '%s' is not found. You are attempting to execute a non valid command\n", line);
            }

            if(flagL == 0) {
                redirectFile(fdLog, STDOUT_FILENO);
                redirectFile(fdLog, STDERR_FILENO);
            } else {
                // stampa output/error su file
                redirectFile(fdOutput, STDOUT_FILENO);
                redirectFile(fdError, STDERR_FILENO);
            }

            time_t t = time(NULL);

            if(cFiglio == -1) { // comando non eseguito correttamente
                //impaginazione error
                if(flagL == 0) {
                    fprintf(stderr, "--------------------------- ERROR ---------------------------\n");
                } else {
                    fprintf(stderr, "\n-------------------------------------------------\n");
                }
                fprintf(stderr, "COMMAND: \t\t\t%s\n", line);
                if(level == 1) {
                    fprintf(stderr, "DATE: \t\t\t\t%s", asctime(localtime(&t)));
                    fprintf(stderr, "RETURN CODE: \t\t\t%d\n", WEXITSTATUS(cFiglio));
                }
                fprintf(stderr, "TYPE ERROR: An error has occurred during '%s' execution\n", line);
            } else if(WEXITSTATUS(cFiglio) == 127 || cFiglio == 256 || WEXITSTATUS(cFiglio) == 2) {
                //impaginazione error
                if(flagL == 0) {
                    fprintf(stderr, "--------------------------- ERROR ---------------------------\n");
                } else {
                    fprintf(stderr, "\n-------------------------------------------------\n");
                }
                fprintf(stderr, "COMMAND: \t\t\t%s\n", line);
                if(level == 1) {
                    fprintf(stderr, "DATE: \t\t\t\t%s", asctime(localtime(&t)));
                    fprintf(stderr, "RETURN CODE: \t\t\t%d\n", WEXITSTATUS(cFiglio));
                }
                fprintf(stderr, "TYPE ERROR: Shell command '%s' not found\n", line);
            } else {
                //impaginazione output
                if(flagL == 0) {
                    printf("--------------------------- OUTPUT ---------------------------\n");
                } else {
                    printf("\n-------------------------------------------------\n");
                }
                printf("COMMAND: \t\t\t%s\n", line);
                if(level == 1) {
                    printf("DATE: \t\t\t\t%s", asctime(localtime(&t)));
                    printf("RETURN CODE: \t\t%d\n", WEXITSTATUS(cFiglio));   
                }
                fflush(stdout);

                if (strcmp(command, "cd") == 0) {
                    result = chdir(parameters);
                    if(result == -1) {
                        fprintf(stderr, "An error has occured during 'cd' execution\n");
                    } else {
                        printf("OUTPUT: \n");
                        //stampa su file della directory corrente in cui sono entrato in modo temporaneo
                        char cwd[1024];
                        if (getcwd(cwd, sizeof(cwd)) != NULL){
                            printf("Current working dir: %s\n\n", cwd);
                        }
                    }
                } else {
                    printf("OUTPUT: \n");
                }

                int pidNipote = fork();
                if (pidNipote == -1) {
                    perror("An error has occurred during forking");
                    exit(1);
                }  

                if (pidNipote == 0) {
                    cNipote = system(line);
                    exit(1);
                }

            }

            wait (NULL);

            fflush(stdout);

            exit(1);
        } else {
            wait (NULL);
        }
    }
}