@; funzione che alloca e restituisce array dei primi fino a n
@; input r0: n
#;       r1: *num  indirizzo di dove scrivere la lunghezza dell'array
@; output r0: indirizzo array restituito

    .data
err_nomem:  .string "out of mem"

    .text
    .global array_primi
    .type array_primi, %function 

array_primi:
    cmp r0, #2      @; calcola r0-2
    blt stop1       @; r0<2 errore
    push {r1,r4,r5,r6,r7,r8,lr} 
    mov r6,r0       @; r6 non viene modificato nelle chiamate
    @; allochiamo array sufficientemente grande r7=(n+1)/2
    add r0,#1
    lsr r7,r0,#1    @; r7 dimensione array in int
    @; chiamiamo malloc
    lsl r0,r7,#2    @; numero di byte (moltiplico r7 per 4)
    bl malloc
    cmp r0,#0       @; controlla se r0 = NULL
    beq stop2
    @; inizio a riempire array 
    mov r4,r0   @; salvo inizio array in r4 (r0 serve per le chiamate)
    mov r0,#2   @; salvo il valore di 2 nell'array
    str r0,[r4]
    mov r5,#1   @; numero elementi messi in array
    @; loop for(r0=3;r0<=n;r0+=2)
    mov r0,#1
loop:   
    add r0,#2
    cmp r0,r6
    bhi finito  @; r0>n
    mov r8,r0
    bl primo    @; chiamata al mio test di primalità
    cmp r0,#0
    mov r0,r8
    beq loop    @; iterazione successiva
    str r0,[r4,r5,lsl #2] @; salva r0
    add r5,#1   @; incremento messi
    @; assert messi<= r7 (dimensione array)
    cmp r5,r7
    bhi stopa
    b loop
    
finito: @; realloc
    mov r0,r4
    lsl r1, r5,#2
    bl realloc     @; il nuovo indirizzo è in r0
    cmp r0,#0
    beq stop2
    @; return 
    mov r2,r5
    pop {r1,r4,r5,r6,r7,r8,lr}  @; recupero i valori
    str r2,[r1]                 @; salvo numero di elementi in *num
    mov pc, lr                  @; return al main

stop1:  
    mov r0,#2
    b exit       @; chiama exit(2) 
stopa:  
    mov r0,#3
    b exit       @; chiama exit(3) 
stop2:  
    ldr r0,=err_nomem
    b termina    @; chiama termina nel file .c che esegue exit(1)   
