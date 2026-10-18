# Runbook – STM32 Basic UART

Dieses Runbook beschreibt die Einrichtung, Konfiguration und Fehlerbehebung der
Bibliothek `basicuart` in einem STM32CubeIDE-Projekt.

---

## Voraussetzungen

- STM32CubeIDE (getestet ab Version 1.x)
- STM32 HAL-Treiber (werden von CubeMX generiert)
- Eine konfigurierte UART-Schnittstelle im `.ioc`-Projekt

---

## Einrichtung

### 1. Dateien in das Projekt kopieren

`basicuart.h` und `basicuart.c` in die entsprechenden Ordner des STM32-Projekts kopieren:

```
<Projektordner>/
├── Core/
│   ├── Inc/
│   │   └── basicuart.h      ← hier ablegen
│   └── Src/
│       └── basicuart.c      ← hier ablegen
```

Alternativ können beide Dateien in einem eigenen Unterordner (z. B. `Lib/basicuart/`)
abgelegt werden, sofern die Include-Pfade in den Projekteinstellungen entsprechend
angepasst werden.

### 2. Include-Pfad prüfen

In den Projekteinstellungen unter
**Properties → C/C++ Build → Settings → MCU GCC Compiler → Include paths**
sicherstellen, dass der Ordner mit `basicuart.h` eingetragen ist.

### 3. Header einbinden

In jeder Quelldatei, die UART-Ausgaben nutzt:

```c
#include "basicuart.h"
```

### 4. Bibliothek initialisieren

In `main.c` nach der UART-Initialisierung (nach dem Aufruf von `MX_USARTx_UART_Init()`):

```c
initBasicUart(&huart2);   // Oder das gewünschte UART-Handle
```

Es kann jedes beliebige UART-Handle übergeben werden. Die Bibliothek ist nicht auf
eine bestimmte Peripherie festgelegt.

---

## Build-Konfiguration

### Debug-Build (Standardverhalten)

Im Debug-Build setzt STM32CubeIDE automatisch das Symbol `DEBUG`. Die Bibliothek
definiert daraufhin `DEBUG_BASICUART`, und alle Funktionen sind aktiv.

Keine manuelle Konfiguration notwendig.

### Release-Build

Im Release-Build ist `DEBUG` nicht definiert. `DEBUG_BASICUART` wird nicht gesetzt,
alle Funktionsrümpfe werden wegkompiliert. UART-Ausgaben verursachen keinen
Code-Overhead im fertigen Produkt.

### UART dauerhaft aktivieren (unabhängig vom Build-Typ)

Falls UART-Ausgaben auch im Release-Build gewünscht sind, `DEBUG_BASICUART` manuell
in den Compiler-Defines eintragen:

**Properties → C/C++ Build → Settings → MCU GCC Compiler → Preprocessor → Defined symbols**

```
DEBUG_BASICUART
```

---

## UART-Schnittstelle wechseln

Die Bibliothek speichert das UART-Handle zur Laufzeit. Um eine andere Schnittstelle
zu verwenden, genügt es, `initBasicUart()` mit dem gewünschten Handle aufzurufen:

```c
// UART1 statt UART2 verwenden
initBasicUart(&huart1);
```

Ein erneuter Aufruf von `initBasicUart()` überschreibt das vorherige Handle.
Ein gleichzeitiger Betrieb mehrerer Schnittstellen ist nicht vorgesehen.

---

## Häufige Probleme

### Keine Ausgabe im Terminal

| Mögliche Ursache | Lösung |
|------------------|--------|
| `DEBUG_BASICUART` nicht definiert | Prüfen, ob Debug-Build aktiv ist, oder Symbol manuell setzen |
| `initBasicUart()` nicht aufgerufen | Aufruf vor der ersten Sende-/Empfangsfunktion einfügen |
| Falsches UART-Handle | Sicherstellen, dass das übergebene Handle zur konfigurierten Schnittstelle passt |
| Baudrate/Format falsch | CubeMX-Konfiguration mit der Terminaleinstellung vergleichen (typisch: 115200, 8N1) |
| USB-Seriell-Adapter nicht verbunden | Verdrahtung TX/RX prüfen (TX des STM32 → RX des Adapters) |

### `uartReceive` gibt immer `UART_ERROR` zurück

| Mögliche Ursache | Lösung |
|------------------|--------|
| Timeout überschritten | Sicherstellen, dass innerhalb von 1000 ms Daten eintreffen |
| `DEBUG_BASICUART` nicht definiert | Ohne dieses Symbol gibt die Funktion immer `UART_ERROR` zurück |
| Puffergröße zu groß | `length` darf nicht größer sein als die tatsächlich gesendeten Bytes |

### Compiler-Fehler: `HAL_UART_Transmit` undeclared

`main.h` oder `usart.h` fehlen im Include-Pfad. Sicherstellen, dass die von CubeMX
generierten Header eingebunden sind.

### Compiler-Fehler: `utoa`/`itoa` undeclared

`stdlib.h` fehlt. In `basicuart.c` muss `#include "stdlib.h"` vorhanden sein.

---

## Betriebshinweise

- Alle Übertragungs- und Empfangsfunktionen sind **blockierend** mit einem Timeout
  von 1000 ms. Bei zeitkritischen Anwendungen können lange Zeichenketten zu
  messbaren Verzögerungen führen.
- Die Bibliothek ist nicht thread-safe. Bei RTOS-Projekten muss der Zugriff auf die
  UART-Funktionen durch einen Mutex abgesichert werden.
- Der interne Zahlenpuffer in `uartTransmitUNumber` und `uartTransmitVNumber` hat eine
  Größe von 11 Bytes – ausreichend für eine 32-Bit-Zahl zur Basis 10. Bei anderen Basen
  (z. B. Basis 2) kann der benötigte Platz bis zu 32 Zeichen betragen; in diesem Fall
  sollte `uartTransmit` mit einem eigenen Puffer verwendet werden.
