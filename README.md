# Web Server HTTP/1.0

## Descrizione

Questo progetto consiste nell'implementazione di un Web Server HTTP/1.0 sviluppato in linguaggio C.

Il server è in grado di accettare connessioni da browser reali e client HTTP, interpretare richieste HTTP, servire file statici presenti nella directory del progetto e gestire più connessioni contemporaneamente tramite la funzione `select()`.

È inoltre presente un sistema di logging asincrono basato su thread e mutex che registra gli accessi all'interno del file `access.log` senza rallentare il thread principale del server.

---

## Funzionalità implementate

* Creazione di una socket TCP tramite API BSD Socket
* Supporto al protocollo HTTP/1.0
* Parsing delle richieste HTTP
* Supporto al metodo `GET`
* Servizio di file statici
* Gestione dei codici HTTP:

  * `200 OK`
  * `404 Not Found`
  * `405 Method Not Allowed`
* Gestione concorrente tramite `select()`
* Logging asincrono mediante thread dedicato
* Sincronizzazione tramite mutex

---

## Struttura del progetto

```text
.
├── src/
│   ├── server.c
│   ├── create_socket.c
│   ├── connection_manager.c
│   ├── client_handler.c
│   ├── http_parser.c
│   ├── file_handler.c
│   └── logger.c
│
├── include/
│   ├── create_socket.h
│   ├── connection_manager.h
│   ├── client_handler.h
│   ├── http_parser.h
│   ├── file_handler.h
│   └── logger.h
│
├── bin/
├── obj/
├── index.html
├── about.html
├── contact.html
├── Makefile
└── access.log
```

---

## Compilazione

Per compilare il progetto:

```bash
make
```

Per eliminare file oggetto ed eseguibile:

```bash
make clean
```

---

## Esecuzione

Avviare il server con:

```bash
./bin/server
```

Il server rimane in ascolto sulla porta:

```text
8080
```

---

## Test

Aprire un browser e visitare:

```text
http://localhost:8080
```

oppure utilizzare:

```bash
curl http://localhost:8080/index.html
```

Per testare la gestione concorrente:

```bash
for i in {1..20}; do
    curl -s http://localhost:8080/index.html > /dev/null &
done
wait
```

---

## Tecnologie utilizzate

* Linguaggio C
* POSIX Threads (pthread)
* BSD Sockets
* HTTP/1.0
* select()
* Mutex

---

## Autore

Emanuele De Francesco

Progetto sviluppato per il corso di Reti e Sistemi Distribuiti.
