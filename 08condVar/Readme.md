

## Esercizi



### Soluzione fair per RW (20/3/25)

Modificare il file `rw.c` per implementare la soluzione fair per il problema lettori/scrittori che avete visto a AESOP (quella con il mutex `ordering` oltre alla condition variable e il mutex associato). Nell'esempio contenuto nel main dovrebbe l'accesso dovrebbe risultare effettivamente fair, nel senso che nessun thread "sorpassa" un altro nell'accesso alla strattura dati. Riporto qui sotto l'output che ottenuto io con questa variante (come a lezione ogni thread è identificato dalle ultime due cifre del tid):
```bash
40 read request
41 read request
42 write request
43 read request
44 read request
45 read request
46 read request
47 read request
48 read request
49 write request
50 read request
51 read request
52 read request
53 read request
40 read completed
41 read completed
42 write completed
43 read completed
44 read completed
45 read completed
46 read completed
47 read completed
48 read completed
49 write completed
50 read completed
51 read completed
52 read completed
53 read completed
```

