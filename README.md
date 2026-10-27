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
(z. B. in der Debug-Buildkonfiguration der STM32CubeIDE). Im Release-Build sind die
Funktionsrümpfe leer, es findet keine UART-Ausgabe statt.

Die zu verwendende UART-Schnittstelle wird zur Laufzeit über `initBasicUart()` übergeben.

## Dateien

| Datei          | Beschreibung                                              |
|----------------|-----------------------------------------------------------|
| `basicuart.h`  | Header mit Funktionsdeklarationen und `uart_status`-Enum  |
| `basicuart.c`  | Implementierung aller Sende- und Empfangsfunktionen       |

## Konfigurationsparameter

| Symbol            | Gesetzt durch                                      | Wirkung                                                   |
|-------------------|----------------------------------------------------|-----------------------------------------------------------|
| `DEBUG`           | STM32CubeIDE im Debug-Build                        | Setzt in `basicuart.h` automatisch `DEBUG_BASICUART`      |
| `DEBUG_BASICUART` | `DEBUG` oder manuell in den Compiler-Defines       | Aktiviert alle Sende- und Empfangsfunktionen              |

Sollen UART-Ausgaben auch im Release-Build aktiv sein, wird `DEBUG_BASICUART` manuell
eingetragen unter
**Properties → C/C++ Build → Settings → MCU GCC Compiler → Preprocessor → Defined symbols**.

## API

### Initialisierung

```c
void initBasicUart (UART_HandleTypeDef *huart);                         // UART-Handle setzen
```

### Senden

```c
void uartTransmit (const char *str, const size_t size);                 // String bekannter Länge senden
void uartTransmitString (const char *str);                              // String senden (Länge wird automatisch ermittelt)
void uartTransmitChar (const char chr);                                 // Einzelnes Zeichen senden
void uartTransmitNumber (const uint32_t number, const uint32_t base);   // Alias für uartTransmitUNumber
void uartTransmitUNumber (const uint32_t number, const uint32_t base);  // Zahl ohne Vorzeichen senden
void uartTransmitVNumber (const int32_t number, const uint32_t base);   // Zahl mit Vorzeichen senden
```

### Empfangen

```c
uart_status uartReceive (uint8_t *data, uint16_t length);               // Daten empfangen
```

### Rückgabewerte

```c
typedef enum
{
	UART_OK		= 0x00,                                                    // Empfang erfolgreich
	UART_ERROR	= 0xFF,                                                    // Fehler oder Timeout beim Empfang
} uart_status;
```

### Funktionsdetails

| Funktion               | Parameter         | Beschreibung                                                                                              |
|------------------------|-------------------|-----------------------------------------------------------------------------------------------------------|
| `initBasicUart`        | `huart`           | Speichert das UART-Handle für alle nachfolgenden Sende- und Empfangsfunktionen. Ein erneuter Aufruf überschreibt das vorherige Handle |
| `uartTransmit`         | `str`, `size`     | Sendet `size` Bytes aus `str`. Es wird kein Nullterminator gesucht, die Länge muss stimmen                 |
| `uartTransmitString`   | `str`             | Sendet einen nullterminierten String, die Länge wird per `strlen()` ermittelt                              |
| `uartTransmitChar`     | `chr`             | Sendet ein einzelnes Zeichen                                                                              |
| `uartTransmitNumber`   | `number`, `base`  | Alias für `uartTransmitUNumber`                                                                           |
| `uartTransmitUNumber`  | `number`, `base`  | Wandelt eine vorzeichenlose 32-Bit-Zahl per `utoa()` zur Basis `base` (2 bis 36) um und sendet sie         |
| `uartTransmitVNumber`  | `number`, `base`  | Wandelt eine vorzeichenbehaftete 32-Bit-Zahl per `itoa()` um und sendet sie. Ein `-` wird nur zur Basis 10 ausgegeben, bei anderen Basen wird die Zahl als vorzeichenlos dargestellt (`-1` zur Basis 16 → `ffffffff`) |
| `uartReceive`          | `data`, `length`  | Empfängt `length` Bytes in den Puffer `data`. Gibt `UART_OK` zurück, wenn alle Bytes innerhalb des Timeouts eintreffen, sonst `UART_ERROR`. Ohne `DEBUG_BASICUART` immer `UART_ERROR` |

Alle Sende- und Empfangsfunktionen arbeiten blockierend mit einem Timeout von 1000 ms.

## Verwendung

### 1. Dateien einbinden

`basicuart.h` und `basicuart.c` in das STM32-Projekt kopieren, z. B. nach `Core/Inc/`
und `Core/Src/`. Liegen die Dateien in einem eigenen Ordner (z. B. `Lib/basicuart/`),
muss dieser unter
**Properties → C/C++ Build → Settings → MCU GCC Compiler → Include paths**
eingetragen werden. Anschließend den Header einbinden:

```c
#include "basicuart.h"
```

### 2. UART initialisieren

Die Initialisierung der UART-Schnittstelle erfolgt wie üblich über den von CubeMX
generierten Code. Das Handle wird anschließend einmalig an die Bibliothek übergeben,
typischerweise in `main()` nach `MX_USART2_UART_Init()`:

```c
initBasicUart(&huart2);
```

Es kann jedes beliebige UART-Handle übergeben werden – die Bibliothek ist nicht auf
eine bestimmte Schnittstelle festgelegt.

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
if (status == UART_OK)
{
	// Daten verarbeiten
}
```

## Hinweise

- `initBasicUart()` muss vor dem ersten Aufruf einer Sende- oder Empfangsfunktion
  aufgerufen werden.
- Es wird immer nur eine Schnittstelle gleichzeitig bedient. Ein erneuter Aufruf von
  `initBasicUart()` wechselt die Schnittstelle.
- Alle Funktionen sind blockierend. Lange Zeichenketten oder niedrige Baudraten führen
  zu messbaren Verzögerungen; bei 9600 Baud passen in 1000 ms nur etwa 960 Zeichen.
- Die Bibliothek ist nicht thread-safe. In RTOS-Projekten muss der Zugriff durch einen
  Mutex abgesichert werden.
- Ohne `DEBUG_BASICUART` sind nur die Funktionsrümpfe leer. Die Funktionen, ihre Aufrufe
  und übergebene String-Literale bleiben im Programm erhalten, sofern der Linker sie
  nicht entfernt (z. B. mit LTO).
- `utoa()` und `itoa()` stammen aus newlib und sind nur mit `-std=gnu11` (Standard der
  STM32CubeIDE) sichtbar, nicht mit `-std=c11`.

### Fehlerbilder

| Symptom                              | Ursache                                    | Abhilfe                                                                 |
|--------------------------------------|--------------------------------------------|-------------------------------------------------------------------------|
| Keine Ausgabe im Terminal            | `DEBUG_BASICUART` nicht definiert          | Debug-Build verwenden oder Symbol manuell setzen                        |
| Keine Ausgabe im Terminal            | `initBasicUart()` nicht aufgerufen         | Aufruf vor der ersten Sende- oder Empfangsfunktion einfügen             |
| Keine Ausgabe im Terminal            | Falsches UART-Handle                       | Handle der konfigurierten Schnittstelle übergeben                       |
| Unlesbare Zeichen im Terminal        | Baudrate oder Format falsch                | CubeMX-Einstellung mit dem Terminal vergleichen (typisch: 115200, 8N1)  |
| Keine Ausgabe im Terminal            | Adapter falsch verdrahtet                  | TX des STM32 auf RX des Adapters                                        |
| `uartReceive` liefert `UART_ERROR`   | Weniger als `length` Bytes in 1000 ms      | `length` an die gesendete Datenmenge anpassen                           |
| `uartReceive` liefert `UART_ERROR`   | `DEBUG_BASICUART` nicht definiert          | Debug-Build verwenden oder Symbol manuell setzen                        |
| `HAL_UART_Transmit` undeclared       | `main.h` oder `usart.h` nicht gefunden     | Von CubeMX generierte Header in den Include-Pfad aufnehmen              |

## Abhängigkeiten

- `main.h` – STM32 HAL
- `usart.h` – generierte UART-Handle-Deklaration

## Lizenz

Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).
