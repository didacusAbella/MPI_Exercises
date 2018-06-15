# WORD COUNT
Questo programma effettua un word count parallelo usando __MPI__ ed esclusivamente comunicazione collettiva.

## Requisiti
- Ambiente Unix Like
- Libreria MPI installata
- Make installato

## Installazione
Posizionarsi nella directory del progetto ed eseguire
`make`

## Esecuzione
Una volta compilato eseguire lo script
`run.sh`
oppure
`mpirun ./WordCount ./test`


## Table of Contents
1. [Algoritmo](#algoritmo)
2. [Benchmark](#benchmark)
    1. [Weak Scaling](#weak-scaling)
    2. [Strong Scaling](#strong-scaling)

## Algoritmo
La risoluzione di questo problema è stata effettuata cercando di simulare il paradigma __Map/Reduce__. L'architettura di questo programma è del tipo master/slave che si svolge in 4 fasi:

### Fase di Split
Il master riceve in input una path, assoluta o relativa, di una directory e scansiona tutti i file (ecludento path 'speciali' come ad esempio file nascosti) e conta le linee totali. Le linee sono mappate all'interno di un ADT che ha questa struttura:
```
typedef struct line_t{
    char file[PATH_LENGTH]; //path di ogi file
    unsigned long start; //offset inizio linea
    unsigned long end; //offset fine linea
}line_t;
```
Le linee sono raccole all'interno di un array e mandate ad ogni processore con la funzione __MPI_Scatterv__ Per inviare porzioni non contigue di memoria è stata usata la funzione __MPI_create_struct__. 

### Fase di Map
Gli slave, una volta ricevute le linee da processare, eseguono la funzione __map__ che tokenizza le linee secondo i delimitatori definiti nella macro __TOKENIZER__ ed estrae le parole in un ADT di questo tipo:
```
typedef struct word_t {
    char word[MAX_WORD]; // parola
    int frequency; // frequenza
}word_t;
```
Che vengono aggiunge ad un array.

### Fase di Combiner
Ogni slave effettua localmente il conteggio del proprio array di parole e il risultato viene trasferito in un nuovo array più compatto.

### Fase di Reduce
Ogni slave manda al master il proprio array con la funzione __MPI_Gatherv__ e quest'ultimo usa la funzione __reduce__ per calcolare le frequenze totali che ridirige sullo standard output. Anche in questo caso per inviare porzioni di memoria con contigue è stato utilizzato __MPI_crete_struct__.

## Benchmark
Qui si inseriranno le caratteristiche del cluster e la size del problema
## Weak Scaling
Qui si metterà su grafico il weak scaling
## Strong Scaling
Qui si metterà su grafico lo strong scaling