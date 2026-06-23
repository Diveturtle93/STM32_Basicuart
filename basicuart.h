//----------------------------------------------------------------------
// Titel	:	basicuart.h
//----------------------------------------------------------------------
// Sprache	:	C
// Datum	:	16.01.2021
// Version	:	1.0
// Autor	:	Diveturtle93
// Projekt	:	Basic Uart
// Quelle	:
//----------------------------------------------------------------------

// Saveguard symbol
//----------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------

// Dateiheader definieren
//----------------------------------------------------------------------
#ifndef INC_BASICUART_H_
#define INC_BASICUART_H_
//----------------------------------------------------------------------

// Einfuegen der standard Include-Dateien
//----------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
//----------------------------------------------------------------------

// Einfuegen der STM Include-Dateien
//----------------------------------------------------------------------
#include "usart.h"
//----------------------------------------------------------------------

// Einfuegen der eigenen Include Dateien
//----------------------------------------------------------------------

//----------------------------------------------------------------------

// Definiere Debug Symbols
//----------------------------------------------------------------------
#ifdef DEBUG
	#define DEBUG_BASICUART
#endif
//----------------------------------------------------------------------

// Version definieren
//----------------------------------------------------------------------
#define BASICUART_MAJOR					0
#define BASICUART_MINOR					0
#define BASICUART_PATCH					0
#define BASICUART_DEV					0
//----------------------------------------------------------------------

// Typedefines definieren
//----------------------------------------------------------------------
typedef enum
{
	UART_OK		= 0x00,
	UART_ERROR	= 0xFF,
} uart_status;
//----------------------------------------------------------------------

// Funktionen definieren
//----------------------------------------------------------------------
void initBasicUart (UART_HandleTypeDef *huart);								// Initialisiere Basic Uart mit UART Handle
void uartTransmit (const char *str, const size_t size);						// Sende Zeichenkette der Laenge "size"
void uartTransmitNumber (const uint32_t number, const uint32_t base);		// Sende Nummer ohne Vorzeichen mit Basis "base"
void uartTransmitUNumber (const uint32_t number, const uint32_t base);		// Sende Nummer ohne Vorzeichen mit Basis "base"
void uartTransmitVNumber (const int32_t number, const uint32_t base);		// Sende Nummer mit Vorzeichen mit Basis "base"
void uartTransmitString (const char *str);									// Sende String, Laenge wird ermittelt
void uartTransmitChar (const char str);										// Sende einzelnes Zeichen
uart_status uartReceive (uint8_t *data, uint16_t length);					// Empfange Daten
//----------------------------------------------------------------------

#endif /* INC_BASICUART_H_ */
//----------------------------------------------------------------------
