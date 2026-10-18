# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden hier dokumentiert.
Format nach [Keep a Changelog](https://keepachangelog.com/de/1.1.0/),
Versionierung nach [Semantic Versioning](https://semver.org/lang/de/).

## [Unveröffentlicht]

## [1.0.0] - 2026-10-18

Erste Veröffentlichung.

### Hinzugefügt

- `basicuart.h` und `basicuart.c` als schlanke Abstraktionsschicht über
  `HAL_UART_Transmit()` und `HAL_UART_Receive()` der STM32 HAL.
- `initBasicUart()`: Speichert das übergebene UART-Handle, das alle Sende- und
  Empfangsfunktionen verwenden. Die Schnittstelle ist damit frei wählbar und kann durch
  einen erneuten Aufruf zur Laufzeit gewechselt werden.
- `uartTransmit()`: Sendet eine Zeichenkette mit bekannter Länge, ohne nach einem
  Nullterminator zu suchen.
- `uartTransmitString()`: Sendet einen nullterminierten String, die Länge wird per
  `strlen()` ermittelt.
- `uartTransmitChar()`: Sendet ein einzelnes Zeichen.
- `uartTransmitUNumber()`: Wandelt eine vorzeichenlose 32-Bit-Zahl per `utoa()` zur Basis
  2 bis 36 um und sendet sie.
- `uartTransmitVNumber()`: Wandelt eine vorzeichenbehaftete 32-Bit-Zahl per `itoa()` zur
  Basis 2 bis 36 um und sendet sie. Ein Minuszeichen wird nur zur Basis 10 ausgegeben.
- `uartTransmitNumber()`: Alias für `uartTransmitUNumber()`.
- `uartReceive()`: Empfängt eine feste Anzahl Bytes in einen Puffer und meldet über
  `uart_status` mit `UART_OK` oder `UART_ERROR`, ob alle Bytes rechtzeitig eingetroffen sind.
- Alle Sende- und Empfangsfunktionen arbeiten blockierend mit einem Timeout von 1000 ms.
- Präprozessorsymbol `DEBUG_BASICUART`: Schaltet alle Sende- und Empfangsfunktionen ein.
  Es wird automatisch gesetzt, wenn `DEBUG` definiert ist (Debug-Build der STM32CubeIDE).
  Ohne das Symbol sind die Funktionsrümpfe leer und es findet keine UART-Ausgabe statt.
- Versionsmakros `BASICUART_MAJOR`, `BASICUART_MINOR`, `BASICUART_PATCH` und
  `BASICUART_DEV` zum Abfragen der Bibliotheksversion im Code.
- README mit Beschreibung der API, der Konfigurationsparameter, einer Anleitung zur
  Einbindung in ein STM32CubeIDE-Projekt, Codebeispielen und einer Tabelle mit typischen
  Fehlerbildern.
- Dieses Changelog.

[Unveröffentlicht]: https://github.com/Diveturtle93/STM32_Basicuart/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/Diveturtle93/STM32_Basicuart/releases/tag/v1.0.0
