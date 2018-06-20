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
posizionarsi nella directory del progetto ed eseguire
`mpirun -np 1_parametro ./WordCount 2_parametro`
dove:

1_parametro: è il numero di processi: Deve essere eseguito a partire da n>=2  (1 processo master ed n - 1 slave)
2_parametro: è una path relativa o assolta alla directory contentente i file di test escludendo lo slash finale.

Esempio: contare le parole di tutti i file presenti nella directory strong_scaling con un master ed uno slave (assumendo di trovarsi nella directory del progetto):

`mpirun -np 2 ./WordCount ./test/strong_scaling`


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
Ogni slave manda al master il proprio array con la funzione __MPI_Gatherv__ e quest'ultimo usa la funzione __reduce__ per calcolare le frequenze totali che ridirige sullo standard output. Anche in questo caso per inviare porzioni di memoria con contigue è stato utilizzato __MPI_create_struct__.

## Benchmark
Sono stati effettuati i benchmark con la seguente configurazione:

### Hardware

| Numero Istanze | VCore per istanza | RAM per istanza | Storage SSD per istanza | Larghezza di banda EBS dedicata(Mb/s) |
|----------------|-------------------|-----------------|-------------------------|---------------------------------------|
|8               |2                  |8                |Solo EBS                 |450                                    |

### Software

| Sistema Operativo | Versione OpenMPI | Versione GCC |
|-------------------|------------------|--------------|
| Ubuntu 12.04      |1.4.3             |4.6.3         |

## Weak Scaling
Il weak scaling è stato misurato mantenendo costante il numero di linee all'aumentare dei worker ed è pari a 300 linee.

![weak scaling image](/img/weak_scaling.png)

Dal grafico si nota che all'aumentare dei processori il tempo di esecuzione dell'algoritmo aumenta e questo è dovuto al fatto che il master deve leggere tutti i file per contare le linee complessive e decidere le porzioni che devono leggere i rispettivi slave. Quindi maggiori saranno le linee da processare, maggiore sarà il tempo necessario per il master a processarle tutte e di conseguenza il tempo di esecuzione del programma sarà maggiore. 
## Strong Scaling
Per quanto riguarda lo strong scaling è stato scelto come dataset un insieme di file da leggere dal peso complessivo pari a 5MB ottenendo i seguenti risultati:

![strong scaling image](/img/strong_scaling.png)

Dal grafico si evince che l'algoritmo aumenta sensibilmente passando da una versione sequenziale (ossia un unico slave che effettua il conteggio delle parole), ad una versione parallelizzata aumentando le prestazioni di oltre il 50% da 1 a 2 worker, ragggiungendo dei tempi stabili (all'incirca un minuto) a partire dagli 8 worker in poi. Questo è dovuto al fatto che seppure il tempo impiegato dal master (e quindi sequenziale) rimane costante, il tempo necessario per contare le parole migliora all'aumentare dei worker.