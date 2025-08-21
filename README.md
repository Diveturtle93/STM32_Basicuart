# STM32 Basicuart

Diese Bibliothek stellt die grundlegenden Funktionen zur Verfügung, um auf einem STM32
über die serielle Schnittstelle (UART) Zeichen, Zahlen oder ganze Zeichenketten zu
senden und zu empfangen.

## Funktionen
Folgende Funktionen sind bisher verfügbar.

- uartTransmitNumber
- uartTransmitUNumber
- uartTransmitVNumber
- uartTransmitString
- uartTransmitChar

Die Funktionen schreiben Zeichenketten, Zahlen mit Vorzeiche oder ohne und einzelne
Zeichen auf den UART. Dabei ist die Länge der Zeichenkette die übergeben wird irrelevant.
Diese wird aus den übergebenen Daten ermittelt und wird während des Programmablaufs
bestimmt.

Für eine Zeichenkette ohne Vorzeichen können die beiden Funktionen "uartTransmitNumber"
und "uartTransmitUNumber" zur Ausgabe einer Zahl ohne Vorzeichen genutzt werden. 
Dem gegenüber wird die Funktion "uartTransmitVNumber" für die Ausgabe eine Zahl mit
Vorzeichen benötigt. Die letzte Funktion ist "uartTransmitChar". Diese gibt nur ein
einzelnes Zeichen aus.

---

- uartTransmit

Mit dieser weiteren Funktion kann ebenfalls eine Zeichenkette oder ein einzelnes Zeichen
ausgegeben werden. Dabei ist aber notwendig die Größer der zu übergebenen Zeichen
zu wissen.

---

- uartReceive

Hier werden Daten die über die serielle Schnittstelle ankommen eingelesen und zurückgegeben.
Die Daten können nach dem Aufruf der Funktion weiter verarbeitet werden.