# API-Referenz – STM32 Basic UART

Diese Datei beschreibt alle öffentlichen Schnittstellen der Bibliothek `basicuart`.

---

## Präprozessorsymbole

### `DEBUG`

Wird von der STM32CubeIDE im Debug-Build automatisch als Compiler-Define gesetzt.
Löst die Definition von `DEBUG_BASICUART` aus.

### `DEBUG_BASICUART`

Steuert die Kompilierung aller Bibliotheksfunktionen. Ist dieses Symbol **nicht**
definiert, werden alle Funktionsrümpfe wegkompiliert – die Symbole existieren weiterhin
(leere Definitionen oder Trivialwerte), verursachen aber keinen Code.

Kann unabhängig von `DEBUG` manuell in den Präprozessordefinitionen des Projekts
gesetzt werden, um UART-Ausgaben dauerhaft zu aktivieren.

---

## Versionsmakros

Definiert in `basicuart.h`:

| Makro               | Typ    | Beschreibung          |
|---------------------|--------|-----------------------|
| `BASICUART_MAJOR`   | `int`  | Hauptversionsnummer   |
| `BASICUART_MINOR`   | `int`  | Nebenversionsnummer   |
| `BASICUART_PATCH`   | `int`  | Patch-Nummer          |
| `BASICUART_DEV`     | `int`  | Entwicklungsstand     |

---

## Typen

### `uart_status`

```c
typedef enum {
    UART_OK    = 0x00,
    UART_ERROR = 0xFF,
} uart_status;
```

Rückgabetyp von `uartReceive()`.

| Wert          | Numerisch | Bedeutung               |
|---------------|-----------|-------------------------|
| `UART_OK`     | `0x00`    | Operation erfolgreich   |
| `UART_ERROR`  | `0xFF`    | Fehler oder Timeout     |

---

## Funktionen

### `initBasicUart`

```c
void initBasicUart(UART_HandleTypeDef *huart);
```

Speichert das übergebene UART-Handle in einer internen Variable. Muss einmalig vor dem
ersten Aufruf einer Sende- oder Empfangsfunktion aufgerufen werden.

| Parameter | Typ                      | Beschreibung                            |
|-----------|--------------------------|-----------------------------------------|
| `huart`   | `UART_HandleTypeDef *`   | Zeiger auf ein initialisiertes UART-Handle |

**Rückgabe:** –

**Verhalten bei deaktiviertem `DEBUG_BASICUART`:** Funktionsrumpf ist leer; das Handle
wird nicht gespeichert.

---

### `uartTransmit`

```c
void uartTransmit(const char *str, const size_t size);
```

Sendet `size` Bytes des Puffers `str` über UART. Die Länge muss vom Aufrufer korrekt
übergeben werden; es wird kein Nullterminator gesucht.

| Parameter | Typ            | Beschreibung                                 |
|-----------|----------------|----------------------------------------------|
| `str`     | `const char *` | Zeiger auf den Sendepuffer                   |
| `size`    | `size_t`       | Anzahl der zu sendenden Bytes                |

**Rückgabe:** –

**Timeout:** 1000 ms (blockierend)

---

### `uartTransmitString`

```c
void uartTransmitString(const char *str);
```

Sendet einen nullterminierten String. Die Länge wird intern per `strlen()` ermittelt.
Ruft intern `uartTransmit()` auf.

| Parameter | Typ            | Beschreibung                          |
|-----------|----------------|---------------------------------------|
| `str`     | `const char *` | Nullterminierter String               |

**Rückgabe:** –

---

### `uartTransmitChar`

```c
void uartTransmitChar(const char chr);
```

Sendet ein einzelnes Zeichen. Das Zeichen wird intern in ein Byte-Array kopiert und
über `uartTransmit()` gesendet.

| Parameter | Typ    | Beschreibung              |
|-----------|--------|---------------------------|
| `chr`     | `char` | Das zu sendende Zeichen   |

**Rückgabe:** –

---

### `uartTransmitNumber`

```c
void uartTransmitNumber(const uint32_t number, const uint32_t base);
```

Alias für `uartTransmitUNumber()`. Sendet eine vorzeichenlose 32-Bit-Zahl als ASCII-
Zeichenkette zur angegebenen Basis.

| Parameter | Typ        | Beschreibung                                     |
|-----------|------------|--------------------------------------------------|
| `number`  | `uint32_t` | Die auszugebende Zahl                            |
| `base`    | `uint32_t` | Zahlenbasis (z. B. `10` für dezimal, `16` für hex)|

**Rückgabe:** –

---

### `uartTransmitUNumber`

```c
void uartTransmitUNumber(const uint32_t number, const uint32_t base);
```

Wandelt `number` per `utoa()` in einen String zur Basis `base` um und sendet ihn.
Intern wird ein Puffer der Größe 11 Bytes verwendet (ausreichend für `UINT32_MAX` zur
Basis 10).

| Parameter | Typ        | Beschreibung                                     |
|-----------|------------|--------------------------------------------------|
| `number`  | `uint32_t` | Die auszugebende vorzeichenlose Zahl             |
| `base`    | `uint32_t` | Zahlenbasis                                      |

**Rückgabe:** –

---

### `uartTransmitVNumber`

```c
void uartTransmitVNumber(const int32_t number, const uint32_t base);
```

Wandelt `number` per `itoa()` in einen String zur Basis `base` um und sendet ihn.
Bei negativen Zahlen zur Basis 10 wird ein führendes `-` ausgegeben.

| Parameter | Typ       | Beschreibung                                      |
|-----------|-----------|---------------------------------------------------|
| `number`  | `int32_t` | Die auszugebende vorzeichenbehaftete Zahl         |
| `base`    | `uint32_t`| Zahlenbasis                                       |

**Rückgabe:** –

---

### `uartReceive`

```c
uart_status uartReceive(uint8_t *data, uint16_t length);
```

Empfängt `length` Bytes blockierend in den Puffer `data`. Gibt `UART_OK` zurück, wenn
`HAL_UART_Receive()` `HAL_OK` meldet, sonst `UART_ERROR`.

Wenn `DEBUG_BASICUART` nicht definiert ist, gibt die Funktion immer `UART_ERROR` zurück.

| Parameter | Typ        | Beschreibung                              |
|-----------|------------|-------------------------------------------|
| `data`    | `uint8_t *`| Zeiger auf den Empfangspuffer             |
| `length`  | `uint16_t` | Anzahl der zu empfangenden Bytes          |

**Rückgabe:** `uart_status` (`UART_OK` oder `UART_ERROR`)

**Timeout:** 1000 ms (blockierend)
