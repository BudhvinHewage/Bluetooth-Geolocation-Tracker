#include <Arduino.h>
#include <NimBLEDevice.h>

int scanTime = 5000;

class scanCallbacks : public NimBLEScanCallbacks {
    // Initial discovery, advertisement data only.
    void onDiscovered(const NimBLEAdvertisedDevice* advertisedDevice) override {
        if (NimBLEDevice::isBonded(advertisedDevice->getAddress())) {
            printf("Found previously connected device")
        }
    }
} scanCallbacks;

void setup() {
    NimBLEScan* pBLEScan = NimBLEDevice::getScan();
    pBLEScan->setScanCallbacks(&scanCallbacks, false);
    pBLEScan->setActiveScan(false);                     // Set active scanning, this will get more data from the advertiser.
    pBLEScan->setMaxResults(0);                         // Do not store the scan results, use callback only.
    pBLEScan->start(scanTime, false, true);             // duration, not a continuation of last scan, restart to get all devices again.
    printf("Scanning...\n");   
}

void loop() {}