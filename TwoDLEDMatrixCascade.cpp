#include "TwoDLEDMatrixCascade.h"

byte reverseBits(byte);

TwoDLEDMatrixCascade::TwoDLEDMatrixCascade(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, int numRowDevices, int numColumnDevices) : ledControlRef(dataPin, clkPin, csPin, numRowDevices * numColumnDevices) {
  this->numRowDevices = numRowDevices;
  this->numColumnDevices = numColumnDevices;
  this->maxRow = (numRowDevices * 8) - 1;
  this->maxColumn = (numColumnDevices * 8) - 1;
  for (int i = 0; i < MAX_DEVICES; i++) {
    this->rotations[i] = 0;
  }
}

void TwoDLEDMatrixCascade::shutdown(bool status) {
  int deviceCount = ledControlRef.getDeviceCount();
  for (int i = 0; i < deviceCount; i++) {
    ledControlRef.shutdown(i, status);
  }
}


void TwoDLEDMatrixCascade::setIntensity(uint8_t intensity) {
  int deviceCount = ledControlRef.getDeviceCount();
  for (int i = 0; i < deviceCount; i++) {
    ledControlRef.setIntensity(i, intensity);
  }
}


void TwoDLEDMatrixCascade::clearDisplay() {
  int deviceCount = ledControlRef.getDeviceCount();
  for (int i = 0; i < deviceCount; i++) {
    ledControlRef.clearDisplay(i);
  }
}

void TwoDLEDMatrixCascade::setRotation(int deviceNum, uint8_t value) {
  rotations[deviceNum] = value;
}

void TwoDLEDMatrixCascade::setLed(int row, int col, boolean state) {
  int deviceID = getDeviceIndex(row, col);
  row = row % 8;
  col = col % 8;
  switch (rotations[deviceID] % 4) {
    case 0:
      ledControlRef.setLed(deviceID, row, col, state);
      break;
    case 1:
      col = 7 - col;
      ledControlRef.setLed(deviceID, col, row, state);
      break;
    case 2:
      row = 7 - row;
      col = 7 - col;
      ledControlRef.setLed(deviceID, row, col, state);
      break;
    case 3:
      row = 7 - row;
      ledControlRef.setLed(deviceID, col, row, state);
      break;
  }

}

void TwoDLEDMatrixCascade::setRow(int row, uint8_t values[]) {
  int devices[MAX_COLUMNS];
  getDeviceIndexesInaRow(row, devices);
  row = row % 8;
  for (int i = 0; i < numColumnDevices; i++) {
    switch (rotations[devices[i]] % 4) {
      case 0:
        ledControlRef.setRow(devices[i], row, values[i]);
        break;
      case 1:
        ledControlRef.setColumn(devices[i], row, reverseBits(values[i]));
        break;
      case 2:
        ledControlRef.setRow(devices[i], 7 - row, reverseBits(values[i]));
        break;
      case 3:
        ledControlRef.setColumn(devices[i], 7 - row, values[i]);
        break;
    }
  }
}

void TwoDLEDMatrixCascade::setColumn(int col, uint8_t values[]) {
  int devices[MAX_ROWS];
  getDeviceIndexesInaColumn(col, devices);
  col = col % 8;
  for (int i = 0; i < numRowDevices; i++) {
    switch (rotations[devices[i]] % 4) {
      case 0:
        ledControlRef.setColumn(devices[i], col, values[i]);
        break;
      case 1:
        ledControlRef.setRow(devices[i], 7-col, values[i]);
        break;
      case 2:
        ledControlRef.setColumn(devices[i], 7 - col, reverseBits(values[i]));
        break;
      case 3:
        ledControlRef.setRow(devices[i], col, reverseBits(values[i]));
        break;
    }
  }
}

int TwoDLEDMatrixCascade::getDeviceIndex(int row, int col) {
  if (row > maxRow || col > maxColumn)
    return -1;
  int rowBlock = row / 8;
  //because of spiral wiring
  //even indexed row of devices are numbered from left to right
  //while odd indexed row of devices are numbered right to left
  if (rowBlock % 2 == 0) {
    return col / 8 + (rowBlock * numColumnDevices);
  } else {
    return (maxColumn - col) / 8 + (rowBlock * numColumnDevices);
  }
}

void TwoDLEDMatrixCascade::getDeviceIndexesInaRow(int row, int devices[]) {
  if (row > maxRow)
    return;
  int rowBlock = row / 8;
  int firstDevice;
  //because of spiral wiring
  //even indexed row of devices are numbered from left to right
  //while odd indexed row of devices are numbered right to left
  if (rowBlock % 2 == 0) {
    firstDevice = rowBlock * numColumnDevices;
    for (int i =  0; i < numColumnDevices; i++) {
      devices[i] = firstDevice++;
    }
  } else {
    int firstDevice = ((rowBlock * numColumnDevices) + numColumnDevices) - 1;
    for (int i =  0; i < numColumnDevices; i++) {
      devices[i] = firstDevice--;
    }
  }
}


void TwoDLEDMatrixCascade::getDeviceIndexesInaColumn(int col, int devices[]) {
  if (col > maxColumn)
    return;
  int columnBlock = col / 8;
  int firstDevice;
  for (int i = 0; i < numRowDevices; i++) {
    if (i % 2 == 0) {
      firstDevice = i * numColumnDevices;
      devices[i] = firstDevice + columnBlock;
    } else {
      firstDevice = ((i * numColumnDevices) + numColumnDevices) - 1;
      devices[i] = firstDevice - columnBlock;
    }
  }
}

byte reverseBits(byte num)
{
  byte count = 8 - 1;
  byte reverse_num = num;

  num >>= 1;
  while (num)
  {
    reverse_num <<= 1;
    reverse_num |= num & 1;
    num >>= 1;
    count--;
  }
  reverse_num <<= count;
  return reverse_num;
}
