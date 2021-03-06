/* ------------------------------------------------
 * Vorspann
 * ------------------------------------------------
 */

// CPP-Definition für Maximale Anzahl der Eingabeversuche

#define MAXVERSUCH 3

// Header-Dateien der C-Standardbibliothek einlesen

#include <stdio.h>     // enthält Deklarationen für
                       // printf(), scanf()

#include <stdlib.h>    // enthält CPP-Definitionen für
                       // EXIT_SUCCESS und EXIT_FAILURE
                       
#include <stdbool.h>   // enthält CPP-Definitionen für
                       // Typ bool und seine Werte true, false

/* ------------------------------------------------
 * Vorwärts-Deklaration der Hilfsfunktionen
 * ------------------------------------------------
 */

// Vorwärts-Deklaration durch Angabe der Funktionsköpfe
extern int ggT(int a, int b, int* addr_result);
extern int readInteger(char* prompt, int* n);

// Alternative: Vorwärts-Deklaration durch Angabe von Prototypen
extern int ggT(int, int, int*);
extern int readInteger(char*, int*);

/* ------------------------------------------------
 * Hauptfunktion main()
 * ------------------------------------------------
 */

int main (void) {
  int i1;
  int i2;
  int theGGT;

  // Lies zwei ganze Zahlen in i1 und i2 ein
  printf("Bitte geben Sie zwei ganze Zahlen ein\n");

  if(readInteger("Bitte die erste  Zahl i1= ", &i1) != EXIT_SUCCESS ) {
    printf("Fehler bei der Eingabe der Zahl i1\n");
    // Code für fehlerhafte Ausführung des gesamten Programms melden
    return EXIT_FAILURE;
  }

  if(readInteger("Bitte die zweite Zahl i2= ", &i2) != EXIT_SUCCESS ) {
    printf("Fehler bei der Eingabe der Zahl i2\n");
    // Code für fehlerhafte Ausführung des gesamten Programms melden
    return EXIT_FAILURE;
  }

  // Berechne GGT
  if(ggT(i1, i2, &theGGT) != EXIT_SUCCESS){
    printf("\nFehler bei der Berechnung des GGT\n"
           "Grund: i1 oder i2 war 0!\n");
    return EXIT_FAILURE;
  }

  // Ausgabe des Resultats
  printf ("\nDer größte gemeinsame Teiler von %d und %d ist: %d\n",
      i1, i2, theGGT);

  // Code für korrekte Ausführung des gesamten Programms melden.
  // Per Konvention:
  //     0 --> alles OK
  // 1-255 --> Fehler-Codes
  //
  return EXIT_SUCCESS;
}

/* ------------------------------------------------
 * Hilfsfunktionen
 * ------------------------------------------------
 */

// Berechne den größten gemeinsamen Teiler (GGT)
// nach dem Euklid'schen Algorithmus.
// 
// Achtung:
// Bei dieser Variante wird das Resultat per
// Referenzparameter addr_result zurückgegeben.
// Der Funktionswert ist ein FehlerCode.
//
int ggT(int a, int b, int* addr_result) {
  // Vermeide Endlosschleife; prüfe auf 0
  if (a == 0 || b == 0){
    // Code für fehlerhafte Ausführung melden
    return EXIT_FAILURE;
  }

  // Fix fuer negative Zahlen
  if (a < 0) { a = -a; }
  if (b < 0) { b = -b; }

  while (a != b) {
    if (a < b)
      b = b - a;
    else
      a = a - b;
  }
  // Zuweisung des GGT an Referenzparameter addr_result
  *addr_result = a;

  // Code für korrekte Ausführung melden
  return EXIT_SUCCESS;
}

// Lese eine ganze Zahl ein.
// Es sind MAXVERSUCH fehlerhafte Eingaben erlaubt.
//
int readInteger(char* prompt, int* n) {
  bool erfolg = false;
  int versuch = 1;    // Anzahl der Versuche

  while (!erfolg && versuch <= MAXVERSUCH ) {
    printf("%s",prompt);
    fflush(stdout);

    if(scanf("%d", n) != 1) {
      versuch++;
      printf("Sie müssen hier eine ganze Zahl eingeben!!!\n");
      while(getchar() != '\n');
      continue;
    } else {
      erfolg = true;
      while(getchar() != '\n');
    }
  }

  // Stelle fest, wieso wir die Schleife verlassen haben
  // und gibt entsprechenden Fehler-Code an den Aufrufer
  // der Funktion (hier main()) zurück.
  if (!erfolg) {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}

