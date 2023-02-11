/*
 * File:   UsbDescriptors.h
 * Author: Szymon Roslowski
 *
 * Created on 13 October 2014, 18:44
 *
 * Device and configuration descriptors.  These are used as the
 * host enumerates the device and discovers what class of device
 * it is and what interfaces it supports.
 */

#ifndef USBDESCRIPTORS_H
#define	USBDESCRIPTORS_H

//#include <GenericTypeDefs.h>

// Vendor and Product Information
#define VendorId    0x04D8
#define ProductId   0x01A6
#define ReleaseNo   0x0001

// Definitions
#define InterfaceCount          0x01 // One Interface - Just Gamepad
#define StringDescriptorCount   0x03 // Three string descriptors - See Bottom of this file
#define Endpoint0BufferSize     0x08 // Endpoint 0 Buffer Size
#define HidDescriptorSize       0x19 // Size Of HID Descriptor
// HID
//#define HidReportByteCount      0x08 // Hid Report Size, also size of Buffers etc. ( Memory usage can go over the roof if not careful with this value)
#define HidReportByteCount      0x03 // Hid Report Size, also size of Buffers etc. ( Memory usage can go over the roof if not careful with this value)
#define HidInterfaceNumber      0x00 // Interface For our HID

// Strings
#define SMAN 0x01   // Manufacturer Name String Index
#define SPRD 0x02   // Product Name String Index
#define SSER 0x00   // Serial Number String Index
#define SCON 0x00   // Configuration String Index

// Actual USB Data Buffers
volatile uint8_t HIDRxBuffer[HidReportByteCount];
volatile uint8_t HIDTxBuffer[HidReportByteCount];

BufferInfo Buffers[(InterfaceCount * 2)] =
{
    { HidReportByteCount, (uint8_t*)&HIDTxBuffer },
    { HidReportByteCount, (uint8_t*)&HIDRxBuffer }
};

/***********************/
/* Descriptors         */
/***********************/

// Device Descriptor
const uint8_t DeviceDescriptor[]=
{
    0x12,   // Size of this descriptor in bytes
    0x01,   // DEVICE descriptor type
    0x00,   // USB Spec Release Number in BCD format LSB
    0x02,   // USB Spec Release Number in BCD format MSB
    0x00,   // Class Code
    0x00,   // Subclass code
    0x00,   // Protocol code
    E0SZ,   // Max packet size for EP0 (8)
    VIDL,   // Vendor ID LSB
    VIDH,   // Vendor ID MSB
    PIDL,   // Product ID: Custom HID device demo LSB
    PIDH,   // Product ID: Custom HID device demo MSB
    RELL,   // Device release number in BCD format LSB
    RELH,   // Device release number in BCD format MSB
    SMAN,   // Manufacturer string index
    SPRD,   // Product string index
    SSER,   // Device serial number string index
    0x01    // Number of possible configurations
};

// ...Stuck these here to keep the number of files to minimum
#define HRBC HidReportByteCount
typedef struct _configStruct
{
    uint8_t configHeader[CONFIG_HEADER_SIZE];
    uint8_t HIDDescriptor[HidDescriptorSize];
} ConfigStruct;

// Configuration descriptor
const ConfigStruct ConfigurationDescriptor =
{
    {
        // Configuration descriptor
    0x09,   // Size of this descriptor in bytes
    0x02,   // CONFIGURATION descriptor type
    0x22,   // Total length of data for this cfg LSB
    0x00,   // Total length of data for this cfg MSB
    INTF,   // Number of interfaces in this cfg
    0x01,   // Index value of this configuration
    SCON,   // Configuration string index
    0xA0,   // Attributes
    0x32,   // Max power consumption (50 mA)
    },
    {
        // Mouse HID Interface descriptor
    0x09,   // Size of this descriptor in bytes
    0x04,   // INTERFACE descriptor type
    IHID,   // Interface Number
    0x00,   // Alternate Setting Number
    0x01,   // Number of endpoints in this interface
    0x03,   // Class code (HID)
    0x01,   // Subclass code (Sublass Boot(1) as opposed to NONE(0) the rest reserved)
//    0x02,   // Protocol code 0-none, 1-Keyboard, 2- Mouse
    0x00,   // Protocol code 0-none, 1-Keyboard, 2- Mouse
    0x00,   // Interface String Descriptor Index

        // Mouse Class-Specific descriptor
    0x09,   // Size of this descriptor in bytes
    0x21,   // HID descriptor type
    0x11,   // HID Spec Release Number in BCD format (1.11) LSB
    0x01,   // HID Spec Release Number in BCD format (1.11) MSB
    0x00,   // Country Code (0x00 for Not supported)
    0x01,   // Number of class descriptors
    0x22,   // Report descriptor type
    0x2A,   // Report Size LSB  (42 bytes)
    0x00,   // Report Size MSB

    	// Gamepad Endpoint 1 In
    0x07,   // Size of this descriptor in bytes
    0x05,   // ENDPOINT descriptor type
    0x81,   // Endpoint Address
    0x03,   // Attributes (Interrupt)
    HRBC,   // Max Packet Size LSB
    0x00,   // Max Packet Size MSB
    0x01    // Interval (1 millisecond)
    }
};

// Report For Gamepad
const uint8_t HIDReport[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x04,                    //     USAGE_MAXIMUM (Button 4)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x08,                    //     REPORT_COUNT (8)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0xc0                           // END_COLLECTION
};


const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[1];}StringDescriptor0={sizeof(StringDescriptor0),0x03,{0x0409}};

const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[13];}StringDescriptor1={sizeof(StringDescriptor1),0x03,
{'J','o','e',' ','O','s','t','r','a','n','d','e','r'}};

const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[7];}StringDescriptor2={sizeof(StringDescriptor2),0x03,
{'N','E','S','2','U','S','B'}};

//Array of string descriptors
const uint8_t *const StringDescriptorPointers[StringDescriptorCount]=
{
    (const uint8_t *const)&StringDescriptor0,
    (const uint8_t *const)&StringDescriptor1,
    (const uint8_t *const)&StringDescriptor2
};

#endif	/* USBDESCRIPTORS_H */

