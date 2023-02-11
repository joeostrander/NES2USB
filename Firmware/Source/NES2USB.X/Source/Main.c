/*
 * All USB code was written by Szymon Roslowski and found at:
 * https://www.codeproject.com/Articles/830856/Microchip-PIC-F-USB-Stack
 *
 *
 * Joe Ostrander -- 20230211
 *
 */

#include <htc.h>
#include "Usb.h"
#include "nes_gamepad.h"

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is Digital Input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = NOCLKDIV // CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// Local Defines
#define LED_SetHigh()            do { PORTAbits.RA4 = 1; } while(0)
#define LED_SetLow()             do { PORTAbits.RA4 = 0; } while(0)
#define LED_Toggle()             do { PORTAbits.RA4 = ~LATCbits.LATC2; } while(0)

// Local Variables
uint8_t last_gamepad_reading;  // This is to hold last status of the gamepad so that we only report if it changes

// Interrupt
void __interrupt () ISRCode (void)
{
    if (UsbInterrupt) ProcessUSBTransactions();
}

static void InitializeSystem(void)
{
    ANSELA =        0x00;
    ANSELC =        0x00;
    OSCTUNE =       0x00;
    OSCCON =        0xFC;           // 16MHz HFINTOSC with 3x PLL enabled (48MHz operation)
    ACTCON =        0x90;           // Enable active clock tuning with USB
    OPTION_REG =    0xC3;           // Set prescaler to 256
    LATC =          0b00000000;     // Clear Port C Latches;
}

static void EnableInterrupts(void)
{
    UIE = 0x4B;             // Transaction complete, Start Of Frame, Error, Reset
    INTCONbits.PEIE = 1;    // Peripheral Interrupt enable
    INTCONbits.GIE = 1;     // Global Interrupt Enable
    PIE2bits.USBIE = 1;     // Enable Usb Global Interrupt
}

void PrepareTxBuffer(uint8_t gamepad_reading)
{
    uint8_t n;

    //Reset TX Buffer to zeroes
    for(n = 0 ; n < HidReportByteCount; n++)
    {
        HIDTxBuffer[n] = 0x00;
    }

    if ( gamepad_reading  > 0)
    {
        LED_SetHigh();

        int8_t x = 0;
        int8_t y = 0;

        if ((gamepad_reading & BUTTON_UP) > 0)
        {
            y = -128;
        }
        else if ((gamepad_reading & BUTTON_DOWN) > 0)
        {
            y = 127;
        }

        if ((gamepad_reading & BUTTON_LEFT) > 0)
        {
            x = -128;
        }
        else if ((gamepad_reading & BUTTON_RIGHT) > 0)
        {
            x = 127;
        }

        HIDTxBuffer[0] = x;
        HIDTxBuffer[1] = y;
        HIDTxBuffer[2] = gamepad_reading & 0x0F;
    }
    else
    {
        LED_SetLow();
    }
}

void ProcessIncomingData(void)
{
    // There is nothing to process
    // Host will not send anything to the gamepad
}

static void CheckUsb(void)
{
    if(IsUsbDataAvaialble(HidInterfaceNumber) > 0 )
    {
        ProcessIncomingData();
        ReArmInterface(HidInterfaceNumber);
    }
}

void ProcessIO(void)
{
    // Check USB for incomming Commands
    if (IsUsbReady) CheckUsb();

    // Check Status Of the gamepad
    uint8_t reading = NES_read_pad();
    if (reading == last_gamepad_reading ) return;

    // If gamepad reading changed - Report
    PrepareTxBuffer(reading);
    HIDSend(HidInterfaceNumber);

    // Save New Button Status
    last_gamepad_reading = reading;
}

void main(void)
{
    InitializeSystem();
    NES_GPIO_Initialize();
    InitializeUSB();
    EnableUSBModule();
    EnableInterrupts();

    while(1) { ProcessIO(); }
}
