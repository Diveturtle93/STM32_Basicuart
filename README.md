# STM32 Basic UART
 
Eine schlanke C-Bibliothek für STM32-Mikrocontroller, die grundlegende Funktionen zum
Senden und Empfangen von Daten über die UART-Schnittstelle bereitstellt. Sie dient als
einfache Low-Level-Abstraktionsschicht über die STM32 HAL und wird von mehreren anderen
Bibliotheken als Abhängigkeit genutzt.
 
## Beschreibung
 
Die Bibliothek kapselt die direkte Verwendung der HAL-Funktionen `HAL_UART_Transmit`
und `HAL_UART_Receive` und bietet komfortable Hilfsfunktionen für die Ausgabe von
Strings, einzelnen Zeichen sowie vorzeichenbehafteten und vorzeichenlosen Ganzzahlen in
verschiedenen Zahlensystemen.
 
Alle Sende- und Empfangsfunktionen sind an das Präprozessorsymbol `DEBUG_BASICUART`
gebunden. Dieses wird automatisch gesetzt, wenn das Symbol `DEBUG` definiert ist
(z. B. in der Debug-Buildkonfiguration der STM32CubeIDE). Im Release-Build werden alle
UART-Ausgaben vollständig wegkompiliert.
 
Die Kommunikation erfolgt über **UART2** (`huart2`).
 
## Dateien
 
| Datei          | Beschreibung                                              |
|----------------|-----------------------------------------------------------|
| `basicuart.h`  | Header mit Funktionsdeklarationen und `uart_status`-Enum  |
| `basicuart.c`  | Implementierung aller Sende- und Empfangsfunktionen       |
 
## API
 
### Senden
 
```c
void uartTransmit(const char *str, const size_t size);          // String bekannter Länge senden
void uartTransmitString(const char *str);                       // String senden (Länge wird automatisch ermittelt)
void uartTransmitChar(const char chr);                          // Einzelnes Zeichen senden
void uartTransmitNumber(const uint32_t number, const uint32_t base);  // Zahl ohne Vorzeichen senden
void uartTransmitUNumber(const uint32_t number, const uint32_t base); // Zahl ohne Vorzeichen senden (identisch zu uartTransmitNumber)
void uartTransmitVNumber(const int32_t number, const uint32_t base);  // Zahl mit Vorzeichen senden
```
 
### Empfangen
 
```c
uart_status uartReceive(uint8_t *data, uint16_t length);        // Daten empfangen
```
 
### Rückgabewerte
 
```c
typedef enum {
    UART_OK    = 0x00,   // Empfang erfolgreich
    UART_ERROR = 0xFF,   // Fehler beim Empfang
} uart_status;
```
 
## Funktionen im Detail
 
| Funktion               | Parameter                        | Beschreibung                                                        |
|------------------------|----------------------------------|---------------------------------------------------------------------|
| `uartTransmit`         | `str`, `size`                    | Sendet `size` Bytes; Länge muss bekannt und korrekt übergeben werden |
| `uartTransmitString`   | `str`                            | Sendet einen nullterminierten String; Länge wird per `strlen` ermittelt |
| `uartTransmitChar`     | `chr`                            | Sendet ein einzelnes Zeichen                                        |
| `uartTransmitNumber`   | `number`, `base`                 | Sendet eine vorzeichenlose 32-Bit-Zahl zur Basis `base` (z. B. 10 oder 16) |
| `uartTransmitUNumber`  | `number`, `base`                 | Identisch zu `uartTransmitNumber`                                   |
| `uartTransmitVNumber`  | `number`, `base`                 | Sendet eine vorzeichenbehaftete 32-Bit-Zahl zur Basis `base`        |
| `uartReceive`          | `*data`, `length`                | Empfängt `length` Bytes in den Puffer `data`; Timeout: 1000 ms      |
 
## Verwendung
 
### 1. Dateien einbinden
 
`basicuart.h` und `basicuart.c` in das STM32-Projekt kopieren und den Header einbinden:
 
```c
#include "basicuart.h"
```
 
### 2. UART initialisieren
 
Die Initialisierung von `huart2` erfolgt wie üblich über den von CubeMX generierten Code
in `usart.c`. Die Bibliothek nutzt die bestehende Handle-Variable `huart2` direkt.
 
### 3. Funktionen aufrufen
 
```c
// String mit bekannter Länge senden
uartTransmit("Hallo\n", 6);
 
// String senden (Länge automatisch)
uartTransmitString("Systemstart\n");
 
// Zahl dezimal ausgeben
uartTransmitNumber(42, 10);
 
// Zahl hexadezimal ausgeben
uartTransmitNumber(255, 16);    // Ausgabe: ff
 
// Vorzeichenbehaftete Zahl ausgeben
uartTransmitVNumber(-17, 10);
 
// Einzelnes Zeichen senden
uartTransmitChar('A');
 
// Daten empfangen
uint8_t buffer[16];
uart_status status = uartReceive(buffer, sizeof(buffer));
if (status == UART_OK) {
    // Daten verarbeiten
}
```
 
## Hinweise
 
- Alle Funktionen verwenden einen blockierenden Ansatz (`HAL_UART_Transmit` / `HAL_UART_Receive`
 mit einem Timeout von 1000 ms.
- Die Funktionen sind nur aktiv, wenn das Symbol `DEBUG_BASICUART` definiert ist.
Dies ist standardmäßig der Fall, wenn `DEBUG` gesetzt ist. Für eine dauerhafte Aktivierung
unabhängig vom Build-Typ kann `DEBUG_BASICUART` manuell in den
Compiler-Präprozessordefinitionen gesetzt werden.
- Die Bibliothek ist fest auf `huart2` ausgelegt. Bei Bedarf kann die Handle-Variable in
`basicuart.c` angepasst werden.

## Abhängigkeiten
 
- `main.h` – STM32 HAL
- `usart.h` – generierte UART-Handle-Deklaration (`huart2`)

## Lizenz
 
Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).
