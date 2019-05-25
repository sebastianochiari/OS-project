#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

#ifndef __FUNCTIONS_H__

#define __FUNCTIONS_H__

/* FUNZIONI D'ESECUZIONE */

// gestore dei comandi passati in input dall'utente nel prompt personalizzato
void wrapperCommand();

// funzione che a partire dall'intero input estrae il comando
char* getCommand(char* input);

// funzione che a partire dall'intero input estrae gli eventuali parametri del comando
char* getParameters(char* input);

// funzione che esegue il comando e stampa nei file di log
void executeCommand(char* command, char* params, char* line);

#endif