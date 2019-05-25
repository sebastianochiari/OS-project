#if 0
Anno accademico: 2017/2018 - Università degli Studi di Trento
Progetto Laboratorio Sistemi Operativi 2018 - Custom Shell
Realizzazione a cura di: Tommaso Bosetti 185286 - Sebastiano Chiari 185858 - Marta Toniolli 187839
#endif

#ifndef __UTILITIES_H__

#define __UTILITIES_H__

/* FUNZIONALITÀ AGGIUNTIVE */

// funzione di inizializzazione delle variabili globali flag
void initializeFlags();
// funzione di redirezionamento sulla base del file descriptor e del canale scelto tramite reference
void redirectFile(int fd, int reference);
// funzione di gestione del prompt in caso di comando 'help'
void helpFunction();
// funzione di gestione del livello di dettaglio del log in caso di comando 'changeLevel'
void changeLevel();

#endif