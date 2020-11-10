#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LEN 128 //127 + '\0'

char k[MAX_LEN]; //key
char M[MAX_LEN]; //cleartext
char C[MAX_LEN]; //ciphertext
char* generatek(void);
char* takeKey(void);
void crypt(char M[], char k[]);
void decrypt(char C[], char k[]);
void menu(void);
void haveKey(void);
void error(void);
void export(void);
void cleanStdin(void);

int main(void){                       // menu iniziale
  puts("\t[1] Create a new key.");
  puts("\t[2] I already have a key.");
  puts("\t[q] quit.");
  printf("\n\e[4;32mChoice:\e[0m");

  unsigned short c = getchar();
  switch (c) {
    case '1':
    fflush(stdin);
    getchar();
    menu();
    break;
    case '2':
    fflush(stdin);
    getchar();
    haveKey();
    break;
    case 'q':
    puts("");
    EOF;
    break;
    default:
    error();
    break;
  }
}

void haveKey() {           // opzione 2 (ho già la chiave)
  puts("Type your ciphertext:");
  printf("\t\033[0;36m");
  fgets(C, MAX_LEN, stdin);
  if(strlen(C) == MAX_LEN-1) //evito che l'input precedente sovrascriva il successivo
    cleanStdin();
  printf("\033[0m");
  puts("Type your key:");
  printf("\t\033[0;36m");
  fgets(k, MAX_LEN, stdin);
  if(strlen(k) == MAX_LEN-1)
    cleanStdin();
  printf("\033[0m");
  decrypt(C, k);

}

char* takeKey(void){            //funzione per prendere in ingresso una chiave
  getchar();
  do {
    puts("\nPlease type a string greater or equal to M (cleartext):");
    printf("\033[0;36m");
    fgets(k, MAX_LEN, stdin);
    printf("\033[0m");
  } while(strlen(k)<=strlen(M)-1);
  return k;
}

char* generatek(void) {         //funzione per genarare in automatico la chiave
  time_t t;
  srand((unsigned) time(&t));
  for (int i = 0; strlen(k)+2 <= strlen(M); i++) {
      char r = "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~]*"[rand() % 93]; // posso fare 32 + rand() % 96, ma così vedo i caratteri possibili
      k[i] = i + r;
  }
  printf("KEY (k): \033[0;32m%s\033[0m, Size: %ld\n", k, strlen(k));
  printf("HEX (k): \e[0;35m\n");
  short i = 0;
  while (k[i] != '\n') {  //stampa in hex
    printf("%2x ", k[i]);
    if((i%8)==0) // quando ne stampa 8 va a capo (più ordinato)
      puts("");
    if(i == 100) // limito l'output 
      break;

    ++i;
  }
  puts("\e[0m");
  fflush(stdin);
  getchar();
  return k;
}


void crypt(char M[], char k[]){   //funzione per cifrare
  for (int i = 0; i < strlen(k); i++) {
    C[i] = M[i]^k[i];
  }
  printf("%c[1m\n\t\tCiphertext (C)%c[0m\n", 27, 27);
  puts("=============================================");
  fflush(stdout);
  printf("%s\n", C);//fflush(stdout) -> evito che alcuni caratteri rovinino la stampa
  fflush(stdout);

  puts("=============================================");
  printf("HEX (C): \e[0;35m\n");
  short i = 0;
  while (C[i] != '\n') { //stampa in hex
    printf("%2x ", C[i]);
    if((i%8)==0)  // quando ne stampa 8 va a capo (più ordinato)
      puts("");
    if(i == 100) // limito la stampa
      break;
    ++i;
  }
  puts("\e[0m");
}

void decrypt(char C[], char k[]){  //funzione per decifrare
  for (int i = 0; i < strlen(k); i++) {
    M[i] = C[i]^k[i];
  }
  printf("%c[1mCleartext (M): %c[0m %s", 27, 27, M); // il 27 "grassetta" il cleartext
  printf("\nHEX (M): \e[0;35m\n");
  short i = 0;
  while (M[i] != '\n') { //stampa in hex
    printf("%2x ", M[i]);
    if((i%8)==0) // quando ne stampa 8 va a capo (più ordinato)
      puts("");
    if(i == 100) // limito la stampa
      break;
    ++i;
  }
  puts("\e[0m");
  puts("\n\e[0;30m[Enter] to continue\e[0m");
  export();
}

void menu(){
  printf("\033[01;33m \n\tPlease insert your cleartext (max 128 chars): \033[0m");
  fgets(M, MAX_LEN, stdin); // prendo il Cleartext e controllo non sia più di 128
  printf("\e[1;1H\e[2J\n"); // inizio Menu
  puts("\t(1) I'm going to choose a key.");
  puts("\t(2) Please generate me one.");
  printf("\n\033[01;33mHow do you want to proceed? \033[0m"); //Fine
  switch (getchar()) {
    case '1':
    takeKey();
    crypt(M, k);
    decrypt(C, k);
    break;
    case '2':
    generatek();
    crypt(M, k);
    decrypt(C, k);
    break;
    default:
    error(); //se la scelta è sbagliata o il cleartext inserito è > di 128 caratteri genera un errore
    break;
  }
}

void error(void){ // funzione per generare l'errore
  printf("\e[1;1H\e[2J\n"); // fa la stessa cosa di CTRL+L ("pulisce" l'output scorrendo), può non funzionare ovunque (ma è solo estetico), !! non usare system("cls") !!
  puts("\033[0;31mYour input is incorrect\a\033[0m");
  abort();
}

void export(void){ // funzione per salvare i risultati su file
  void write(void){
    FILE *fileptr;
    fileptr = fopen("export.txt", "w");
    if(!fileptr){
      puts("\033[0;31mSome problems occurred while opening/writing the file may be insufficient permissions. Try running as sudo.\033[0m");
    }else{
      fprintf(fileptr, "\t\t\tYour key(k) is:\n===============================================================\n%s\t\t(KEEP IT SECRET)\n===============================================================\n\n\t\t\tCleartext(M):\n===============================================================\n%s===============================================================\n\n\t\t\tCiphertext(C):\n===============================================================\n%s\n===============================================================\n\n"
      , k, M, C);
      puts("Exported.");
      fclose(fileptr);
    }
  }
  cleanStdin(); //evito che un precedente input troppo grandi skippi questa scelta
  printf("Do you want to export to a text file? (Y/n): ");
  unsigned short c = getchar();
  switch (c) {
    case 'n':
    case 'N':
    puts("\e[0;32m\nOk, bye :)\e[0");
    break;
    case 'y':
    case 'Y':
    default:
      write();
    break;
  }
}

void cleanStdin(void){ //pulisce lo stardard input
  unsigned short c;
  do{
    c = getchar();
  }  while (c != '\n' && c != EOF );
}
