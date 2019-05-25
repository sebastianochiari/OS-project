#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

#include "init.h"
#include "functions.h"

#include <stdio.h>
#include <stdlib.h>

// funzione di inizializzazione delle variabili globali flag
void initializeFlags() {
    flagE = -1;
    flagO = -1;
    flagL = -1;
    level = 0;
}

// funzione di redirezionamento sulla base del file descriptor e del canale scelto tramite reference
void redirectFile(int fd, int reference) {
    // duplicazione dello stream prescelto sulla base del file descriptor e di reference
    int check = dup2(fd, reference);
    if(check == -1) { // gestione dell'errore
        printf("An error has occurred redirecting the output/error stream to the log file\n");
        exit(1);
    }
}

// funzione di gestione del prompt in caso di comando 'help'
void helpFunction() {
    printf("\nHi, I am Toby, your personal assistant. I am here to help you.\nHere is a list of the command supported and tested,but feel free to try command not in the list.\n");
    printf("Commands fully supported: \n");
    printf("-> ls\n");
    printf("-> df\n");
    printf("-> du\n");
    printf("-> cp\n");
    printf("-> clear\n");
    printf("-> pwd\n");
    printf("-> date\n");
    printf("-> cat 'nomefile'\n");
    printf("-> free\n");
    printf("-> rm\n");
    printf("-> rmdir (only for empty directory)\n");
    printf("-> find\n");
    printf("-> uname\n");
    printf("-> quit\n");
    printf("-> lshw\n");
    printf("Also the 'pipe' functionality is supported\n");
    
    printf("\nNow there is the list for the command in beta testing, they are not fully supported yet: \n");
    printf("-> cd\n");
    printf("-> mkdir\n");
    printf("-> time\n");
    printf("-> top\n");
    printf("-> less\n");

    printf("\nUnfortunately here there is the list of the command that are not supported yet: \n");
    printf("-> pico\n");
    printf("-> vim\n");

    printf("\n");
}

// funzione di gestione del livello di dettaglio del log in caso di comando 'changeLevel'
void changeLevel() {
    if(level == 0) {
        level = 1;
        printf("You've changed the level from SLIM to FAT\n");
        printf("Now, log files will display also DATE of execution and the RETURN CODE\n");
    } else {
        level = 0;
        printf("You've changed the level from FAT to SLIM\n");
        printf("Now, log files will display only the COMMAND you run and its OUTPUT\n");
    }
}