#include <Arduino.h>
#include <NimBLEDevice.h>

int scanTime = 5000; //In milli seconds

/*
class scanCallbacks : public NimBLEScanCallbacks {
    // Initial discovery, advertisement data only.
    void onDiscovered(const NimBLEAdvertisedDevice* advertisedDevice) override {
        printf("Discovered Device: %s\n", advertisedDevice->getAddress().toString().c_str());
    }

    /*
     *  If active scanning the result here will have the scan response data.
     *  If not active scanning then this will be the same as onDiscovered.
    
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
        printf("Device result: %s\n", advertisedDevice->toString().c_str());
    }
    */

    /*
    void onScanEnd(const NimBLEScanResults& results, int reason) override {
        printf("Scan ended reason = %d; restarting scan\n", reason);
        NimBLEDevice::getScan()->start(scanTime, false, true);
      }
} scanCallbacks;
*/

class ClientCallbacks : public NimBLEClientCallbacks {
    void onPassKeyEntry(NimBLEConnInfo& connInfo) override {
        Serial.printf("Server Passkey Entry\n");
        /**
         * This should prompt the user to enter the passkey displayed
         * on the peer device.
         */
        NimBLEDevice::injectPassKey(connInfo, 123456);
    }
} clientCallbacks;

/*
void setup() {
    NimBLEDevice::deleteAllBonds();
    NimBLEDevice::init("");                             // Initialize the device, you can specify a device name if you want.
    NimBLEScan* pBLEScan = NimBLEDevice::getScan();     // Create the scan object.
    pBLEScan->setScanCallbacks(&scanCallbacks, false);  // Set the callback for when devices are discovered, no duplicates.
    pBLEScan->setActiveScan(false);                     // Set active scanning, this will get more data from the advertiser.
    pBLEScan->setMaxResults(0);                         // Do not store the scan results, use callback only.
    pBLEScan->start(scanTime, false, true);             // duration, not a continuation of last scan, restart to get all devices again.
    printf("Scanning...\n");
}
*/

void setup() {
    Serial.begin(115200);
    Serial.println("Starting NimBLE Client");

    NimBLEDevice::init("");
    NimBLEDevice::setPower(3);                               /** +3db */
    NimBLEDevice::setSecurityAuth(true, true, false);        /** bonding, MITM, don't need BLE secure connections as we are using passkey pairing */
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_ONLY); /** passkey */
    NimBLEScan*       pScan   = NimBLEDevice::getScan();
    NimBLEScanResults results = pScan->getResults(5 * 1000);

    NimBLEUUID serviceUuid("ABCD");

    for (int i = 0; i < results.getCount(); i++) {
        const NimBLEAdvertisedDevice* device = results.getDevice(i);
        Serial.println(device->getName().c_str());

        if (device->isAdvertisingService(serviceUuid)) {
            NimBLEClient* pClient = NimBLEDevice::createClient();
            pClient->setClientCallbacks(&clientCallbacks, false);

            if (pClient->connect(&device)) {
                pClient->secureConnection();
                NimBLERemoteService* pService = pClient->getService(serviceUuid);
                if (pService != nullptr) {
                    NimBLERemoteCharacteristic* pNonSecureCharacteristic = pService->getCharacteristic("1234");

                    if (pNonSecureCharacteristic != nullptr) {
                        // Testing to read a non secured characteristic, you should be able to read this even if you have mismatching passkeys.
                        std::string value = pNonSecureCharacteristic->readValue();
                        // print or do whatever you need with the value
                        Serial.println(value.c_str());
                    }

                    NimBLERemoteCharacteristic* pSecureCharacteristic = pService->getCharacteristic("1235");

                    if (pSecureCharacteristic != nullptr) {
                        // Testing to read a secured characteristic, you should be able to read this only if you have
                        // matching passkeys, otherwise you should get an error like this. E NimBLERemoteCharacteristic:
                        // "<< readValue rc=261" This means you are trying to do something without the proper
                        // permissions.
                        std::string value = pSecureCharacteristic->readValue();
                        // print or do whatever you need with the value
                        Serial.println(value.c_str());
                    }
                }
            } else {
                // failed to connect
                Serial.println("failed to connect");
            }

            NimBLEDevice::deleteClient(pClient);
        }
    }
}

void loop() {
}