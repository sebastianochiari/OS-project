#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

#include "init.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// funzione generale che crea un file sulla base del parametro passato
void CreateFile(char *argv) {
    // lunghezza dell'argomento passato
    int len = strlen(argv);
    // lunghezza totale del comando passato a sprintf (6 è la lunghezza di "touch ")
    int totLen = 6 + len;
    // allocazione dinamica per la stringa command
    char *command = malloc(totLen + 1);
    // generazione del comando di creazione del file e controllo del codice di ritorno di sprintf
    int check = sprintf(command, "touch %s", argv);
    if(check < 0) { // sprintf restituisce intero minore di 0 in caso di fallimento
        printf("An error has occurred building the command into CreateFile() function\n");
        exit(1);
    }
    // esecuzione del comando 'command' e verifica del codice di ritorno
    check = system(command);
    if(check > 0) { // touch restituisce intero > 0 in caso di fallimento
        printf("An error has occurred creating the file %s\n", argv);
        exit(1);
    }
}

// gestore per la creazione dei file di default
void CreateDefaultFile() {
    if(flagO == -1) { // controllo dell'esistenza tramite flag del file di output
        // copia del file che si vuole creare all'interno della variabile di sicurezza fileName
        strcpy(fileName, "out.txt");

        wrapperCreateFile(fileName, flagO);
        
        // aggiornamento del flag 
        flagO = 0;
        
        OpenFile(fileName, STDOUT_FILENO);
    }
    if(flagE == -1) { // controllo dell'esistenza tramite flag del file di error
        // copia del file che si vuole creare all'interno della variabile di sicurezza fileName    
        strcpy(fileName, "err.txt");
        
        wrapperCreateFile(fileName, flagE);
        
        // aggiornamento del flag
        flagE = 0;
        
        OpenFile(fileName, STDERR_FILENO);
    }
}

// funzione generale che apre un file sulla base del parametero passato
void OpenFile(char *file, int reference) {
    // apertura del file passato alla funzione e verifica del codice di ritorno
    int fd = open(file, O_RDWR); // O_RDWR -> apertura del file sia in lettura che in scrittura
    if(fd == -1) { // gestione dell'errore
        printf("An error has occurred during OpenFile\n");
        exit(1);
    }
    // posizionamento del cursore all'interno del file e verifica del codice di ritorno
    int check = lseek(fd, 0, SEEK_END);
    if(check == -1) { // gestione dell'errore
        printf("An error has occurred moving the location of the read/write pointer of the file descriptor\n");
        exit(1);
    }
    // copia su variabili globali del file descriptor e del nome del file aperto tramite la variabile di controllo reference
    if(reference == STDOUT_FILENO) {
        fdOutput = fd;
        strcpy(logOutput, file);
    } else if(reference == STDERR_FILENO) {
        fdError = fd;
        strcpy(logError, file);
    } else if(reference == STDLOG_FILENO) {
        fdLog = fd;
        strcpy(logLog, file);
    }
}

// funzione di riconoscimento del file che si vuole aprire (output, error, log unico)
int checkParameters(char *argv) {
    char *parameter = argv;
    int result = 0;
    if(strcmp(parameter, outfile)==0) { // user ha inserito --outfile
        result = 1;
    } else if(strcmp(parameter, o)==0) { // user ha inserito -o
        result = 2;
    } else if(strcmp(parameter, errfile) == 0) { // user ha inserito --errfile
        result = 3;
    } else if(strcmp(parameter, e) == 0) { // user ha inserito -e
        result = 4;
    } else if(strcmp(parameter, log) == 0) { // user ha inserito --log
        result = 5;
    } else { // user ha inserito un parametro sconosciuto
        result = 6;
    }
    return result;
}

/* gestore per la creazione del file passato alla funzione 
e per la risoluzione di conflitti nel caso il file esista già */
void wrapperCreateFile(char *argv, int fd) {
    // controllo dell'esistenza del file
    int check = FileExist(argv, fd);
    if(check == 1) { // il file esiste
        // variabile inizializzata a runtime in base alla scelta dell'utente
        int choice;
        printf("The file %s you're attempting to create already exists!\n", argv);
        printf("What do you want to do? (Make sure you don't write \" \")\n");
        printf("Options (Default choice is number 2):\n");
        printf("[1] - Overwrite file content with new file content\n");
        printf("[2] - Append new content to old file\n");
        printf("[3] - Insert new file name\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1 : { // overwrite del file
                // eliminazione del file pre-esistente e controllo del return code
                int delete = unlink(argv);
                if(delete != 0) { // gestione dell'errore
                    printf("An error has occurred during the overwriting of the file\n");
                    exit(1);
                }
                // creazione del nuovo file
                wrapperCreateFile(argv, fd);
                break;
            }
            case 2 : default : { // append
                break;
            }
            case 3 : { // nuovo file in base alla scelta dell'utente
                fflush(stdin);
                // variabile temporanea per il salvataggio del nuovo nome del file
                char file[LEN_BUFFER];
                // lunghezza del nome del file passato
                int len;
                printf("Insert file name: ");
                // lettura da linea di comando del nuovo nome del file
                scanf("%s", file);
                // calcolo della lunghezza del nuovo file
                len = strlen(file);
                if ( file[len-1] == '\n') {
                    // inserimento del carattere di terminazione della stringa
                    file[len-1] = '\0';
                }

                printf("\nNome inserito: %s", file);
                // creazione del nuovo file sulla base dell'input passato dall'utente
                wrapperCreateFile(file, fd);
                // salvataggio del nome del file nella variabile globale di servizio
                strcpy(fileName, file);
                break;
            }

        }
        fflush(stdin);
    }
    else { // il file non esiste
        printf("\nThe file you're attempting to create doesn't exist\nNow I create the file %s\n\n", argv);
        // creazione del file sulla base dell'input passato
        CreateFile(argv);
    }
}


// gestore per la gestione dei parametri aggiuntivi in coda all'eseguibile
void TakeInputUserFileTok(char *argv) {
    // stringa temporanea per la gestione dei token
    char *input;
    // estrazione tipologia file
    input = strtok(argv, "=");
    // controllo del parametro passato in input e salvataggio del ritorno della funzione nella variabile check
    int check = checkParameters(input);
    if (check == 6) { // argomento a caso
        printf("\nLoL! Syntax is wrong. Check makefile for usage rules.\n\n");
        exit(1);
    }
    // divisone di input in token ed estrazione del nome del file
    input = strtok(NULL,"=");
    // salvataggio del nome del file nella variabile globale di servizio
    strcpy(fileName, input);
    switch(check) {
        case 1 : { // utente ha inserito --outfile
            // creazione del nuovo file sulla base dell'input passato dall'utente
            wrapperCreateFile(fileName, flagO);
            // aggiornamento del flag
            flagO = 0;
            // apertura del file appena creato
            OpenFile(fileName, STDOUT_FILENO);
        } break;
        case 2 : { // utente ha inserito -o
            // creazione del nuovo file sulla base dell'input passato dall'utente
            wrapperCreateFile(fileName, flagO);
            // aggiornamento del flag
            flagO = 0;
            // apertura del file appena creato
            OpenFile(fileName, STDOUT_FILENO);
        } break;
        case 3 : { // utente ha inserito --errfile
            // creazione del nuovo file sulla base dell'input passato dall'utente
            wrapperCreateFile(fileName, flagE);
            // aggiornamento del flag
            flagE = 0;
            // apertura del file appena creato
            OpenFile(fileName, STDERR_FILENO);
        } break;
        case 4 : { // utente ha inserito -e
            // creazione del nuovo file sulla base dell'input passato dall'utente
            wrapperCreateFile(fileName, flagE);
            // aggiornamento del flag
            flagE = 0;
            // apertura del file appena creato
            OpenFile(fileName, STDERR_FILENO);
        } break;
        case 5 : { // utente ha inserito --log
            // creazione del nuovo file sulla base dell'input passato dall'utente
            wrapperCreateFile(fileName, flagL);
            // aggiornamento del flag
            flagL = 0;
            flagE = 0;
            flagO = 0;
            // apertura del file appena creato
            OpenFile(fileName, STDLOG_FILENO);
        } break;
        case 0 : default : { // nessun argomento passato
            // creazione file di default
            CreateDefaultFile();
        } break;
    }
}

// funzione di riconoscimento dell'esitenza del file
int FileExist(char *name, int fd) {
    // variabile di controllo  
    int res;
    if(access(name, F_OK) != -1) { // il file esiste
        res = 1;
    } else { // il file non esiste
        res = 0;
        fd = -1;
    }
    return res;
}

// funzione di chiusura del file in base al file descriptor passato
void closeFile(int fd) {
    // chiusura del file tramite file descriptor e controllo del codice di ritorno
    if(close(fd) < 0) { // gestione dell'errore
        perror("An error has occurred during closeFile().\n");
        exit(1);
    }
}