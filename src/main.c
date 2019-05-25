#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

// includo file personali
#include "init.h"
#include "functions.h"
#include "utilities.h"

// includo librerie
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv) {

    // inizializza le variabili globali per la creazione dei file di log
    initializeFlags();
    
    // cambia la directory corrente, portandola a un livello superiore
    chdir("..");
    
    if(argc <= 1) { // non sono stati passati parametri
        printf("\n");
        printf("No parameters have been passed.\n");
        printf("I create the default file: out.txt and err.txt\n");

        // funzione che crea i file di default
        CreateDefaultFile();
        
        fflush(stdout); 
        fflush(stdin);
    } else { // sono stati passati parametri
        
        // contatore per scorrere gli argomenti passati
        int counter = 1;

        while(counter < argc) {
            // controllo se l'utente ha passato l'argomento -f per il livello di dettaglio dei file di log
            if(strcmp(argv[counter], levelFat) == 0) {
                // aggiornamento variabile globale per gestione livello di dettaglio dei file di log
                level = 1;
            } else {
                // funzione gestore creazione file di log in base ai parametri passati
                TakeInputUserFileTok(argv[counter]);
            }
            counter++;
        }
        
        // se è stato passato solo un file di log personalizzato si crea l'altro di default
        CreateDefaultFile();
        
        fflush(stdout); 
    }

    printf("Press ENTER to start the shell.\n");
    
    // per sicurezza, forziamo la pulizia dei buffer
    fflush(stdin);
    while(getchar() != '\n'); 
    fflush(stdout);

    // variabile di verifica delle chiamate di funzioni / system call
    int check;
    // puliamo la schermata del terminale
    check = system("clear");
    // controllo corretta esecuzione system call
    if(check == -1) {
        exit(1);
    }

    printf("Write 'help' if you want to get some help from Toby, your personal assistant.\n");
    printf("Log files detail level is set to default, SLIM.\nIf you want to change it, write 'changeLevel'\n\n");

    // avvia la Custom Shell
    while(1) {

        // controlli cautelari sull'esistenza dei file di log precedente creati in base agli argomenti passati
        if (flagL == 0) { // l'utente aveva precedentemente invocato la creazione di un file unico di log
            check = FileExist(logLog, flagL);
            if (check == 0) { // l'utente ha eliminato il file di log unico 
                printf("Oh dear! It seems you have deleted the log file!\nNow I create '%s' for you.\n", logLog);
                // viene creato nuovamente il file di log unico (con lo stesso nome dato)
                CreateFile(logLog);
                // il file viene nuovamente aperto
                OpenFile(logLog, STDLOG_FILENO);
            }
        } else { // l'utente aveva precedentemente invocato la creazione di file separati
            check = FileExist(logOutput, flagO);
            if (check == 0) { // l'utente ha eliminato il file di output
                printf("Oh dear! It seems you have deleted the output file!\nNow I create '%s' for you.\n", logOutput);
                // viene creato nuovamente il file di output (con lo stesso nome dato)
                CreateFile(logOutput);
                // il file viene nuovamente aperto
                OpenFile(logOutput, STDOUT_FILENO);
            }
            check = FileExist(logError, flagE);
            if (check == 0) { // l'utente ha eliminato il file di error
                printf("Oh dear! It seems you have deleted the error file!\nNow I create '%s' for you.\n", logError);
                // viene creato nuovamente il file di error (con lo stesso nome dato)
                CreateFile(logError);
                // il file viene nuovamente aperto
                OpenFile(logError, STDERR_FILENO);
            }
        } 

        fflush(stdout);

        printf("shell> ");

        // funzione che leggi l'input che l'utente scrive e lo processa
        wrapperCommand();

        printf("\n");
    }
    
    return 0;
}