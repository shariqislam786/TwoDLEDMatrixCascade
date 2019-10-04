#ifndef TwoDLEDMatrixCascade_h
#define TwoDLEDMatrixCascade_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef MAX_DEVICES
#define MAX_DEVICES 6
#endif


#ifndef MAX_ROWS
#define MAX_ROWS 2
#endif

#ifndef MAX_COLUMNS
#define MAX_COLUMNS 3
#endif

#include "LedControl.h"

class TwoDLEDMatrixCascade {
  private:
    LedControl ledControlRef;
    int numRowDevices;
    int numColumnDevices;
    int maxRow;
    int maxColumn;
    uint8_t rotations[MAX_DEVICES];
    int getDeviceIndex(int row, int col);
    void getDeviceIndexesInaRow(int row, int devices[]);
    void getDeviceIndexesInaColumn(int col, int devices[]);
  public:
    TwoDLEDMatrixCascade(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, int numRowDevices, int numColumnDevices);

    void setRotation(int deviceNum, uint8_t value);
    /*
           Set the shutdown (power saving) mode for the matrix
           Params :
           status	If true the device goes into power-down mode. Set to false
         		for normal operation.
    */
    void shutdown(bool status);

    /*
       Set the brightness of the display.
       Params:
       intensity	the brightness of the display. (0..15)
    */
    void setIntensity(uint8_t intensity);

    /*
       Switch all Leds on the display off.
    */
    void clearDisplay();

    /*
       Set the status of a single Led.
       Params
       row	the row of the Led (0..((8 * numYDevices)-1)
       col	the column of the Led (0..((8 * numXDevices)-1)
       state	If true the led is switched on,
     		if false it is switched off
    */
    void setLed(int row, int col, boolean state);

    /*
       Set all Led's in a row to a new state
       Params:
       row	row which is to be set (0..(0..((8 * numYDevices)-1))
       value	each bit set to 1 will light up the
     		corresponding Led.
    */
    void setRow(int row, uint8_t values[]);

    /*
       Set all Led's in a column to a new state
       Params:
       addr	address of the display
       col	column which is to be set (0..(0..((8 * numXDevices)-1))
       value	each bit set to 1 will light up the
     		corresponding Led.
    */
    void setColumn(int col, uint8_t values[]);



};
#endif
