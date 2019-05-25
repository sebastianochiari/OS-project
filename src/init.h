#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

#ifndef __INIT_H__

#define __INIT_H__

// definizione di MACRO: argomenti che l'utente può passare al programma
#define o "-o"
#define outfile "--outfile"
#define errfile "--errfile"
#define e "-e"
#define log "--log"
#define levelFat "-f"
// definizione di MACRO di servizio
#define LEN_BUFFER 1024
#define STDLOG_FILENO 3

// variabili globali
int flagO; // flag per l'esistenza del file di output
int flagE; // flag per l'esistenza del file di error
int flagL; // flag per l'esistenza del file di log unico
int fdOutput; // file descriptor del file di output
int fdError; // file descriptor del file di error
int fdLog; // file descriptor del file di log unico
char logOutput[LEN_BUFFER]; // stringa per la memorizzazione del nome del file di output
char logError[LEN_BUFFER]; // stringa per la memorizzazione del nome del file di error
char logLog[LEN_BUFFER]; // stringa per la memorizzazione del file di log unico
int level; // se è 0, si tratta di una formattazione SLIM, se è 1 abbiamo una formattazione FAT
char fileName[LEN_BUFFER]; // stringa di servizio per la memorizzazione in itinere del file da creare

/* FUNZIONI DI INIZIALIZZAZIONE */

// funzione generale che crea un file sulla base del parametro passato
void CreateFile(char *argv);

// gestore per la creazione dei file di default
void CreateDefaultFile();

// funzione generale che apre un file sulla base del parametero passato
void OpenFile(char *file, int reference);

// funzione di riconoscimento del file che si vuole aprire (output, error, log unico)
int checkParameters(char *argv);

/* gestore per la creazione del file passato alla funzione 
e per la risoluzione di conflitti nel caso il file esista già */
void wrapperCreateFile(char *argv, int fd);

// gestore per la gestione dei parametri aggiuntivi in coda all'eseguibile
void TakeInputUserFileTok(char * argv);

// funzione di riconoscimento dell'esitenza del file
int FileExist(char *name, int fd);

// funzione di chiusura del file in base al file descriptor passato
void closeFile(int fd);

#endif
