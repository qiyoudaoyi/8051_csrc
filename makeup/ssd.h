#ifndef SSD_H
#define SSD_H

#include "REGX51.h"
#include "delay.h"

#define UI unsigned int
#define UC unsigned char
#define DIN P2_4
#define LOAD P2_3
#define CLK P2_2
#define LED P1

// REGISTERS
#define DECODE_MODE     0x09
#define INTENSITY       0x0A
#define SCAN_LIMIT      0x0B
#define SHUTDOWN        0x0C
#define DISPLAY_TEST    0x0F

UC SSD_CODE[] = {
    0x7E,   // 0b01111110: 0
    0x30,   // 0b00110000: 1
    0x6D,   // 0b01101101: 2
    0x79,   // 0b01111001: 3
    0x33,   // 0b00110011: 4
    0x5B,   // 0b01011011: 5
    0x5F,   // 0b01011111: 6
    0x70,   // 0b01110000: 7
    0x7F,   // 0b01111111: 8
    0x7B,   // 0b01111011: 9
    0x77,   // 0b01110111: A
    0x1F,   // 0b00011111: b
    0x4E,   // 0b01001110: C
    0x3D,   // 0b00111101: d
    0x4F,   // 0b01001111: E
    0x47    // 0b01000111: F
};

void BitExtract(UC bits);
void SerialDIN(UC, UC);
void SSD_ShowTime(const UI*);
void SSD_INIT();

void BitExtract(UC bits)
{
    UC i;

    for (i = 0; i < 8; i++) {
        CLK = 0;
        DIN = (bits & 0x80);
        bits <<= 1;
        CLK = 1;
    }
}

void SerialDIN(UC address, UC dat)
{
    LOAD = 0;
    BitExtract(address);
    BitExtract(dat);
    LOAD = 1;
}

void SSD_ShowTime(const UI* t)
{
    UI HH = t[0], MM = t[1], SS = t[2];

    SerialDIN(1, SSD_CODE[SS%10]);
    SerialDIN(2, SSD_CODE[SS/10]);
    SerialDIN(3, 0x01);

    SerialDIN(4, SSD_CODE[MM%10]);
    SerialDIN(5, SSD_CODE[MM/10]);
    SerialDIN(6, 0x01);

    SerialDIN(7, SSD_CODE[HH%10]);
    SerialDIN(8, SSD_CODE[HH/10]);
}

void SSD_INIT()
{
    UC i;

    SerialDIN(SHUTDOWN,     0x01);  // Normal mode (0xX1)
    SerialDIN(DISPLAY_TEST, 0x00);  // Overwrite SHUTDOWN
    SerialDIN(DECODE_MODE,  0x00);  // no decoding
    SerialDIN(SCAN_LIMIT,   0x07);  // use all 8 digits
    SerialDIN(INTENSITY,    0x0E);  // brightness

    for (i = 1; i <= 8; i++)
        SerialDIN(i, 0);
}

#endif