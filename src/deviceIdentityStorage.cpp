#include <Arduino.h>
#include <NimBLEDevice.h>

int scanTime = 5000;

void setup() {
    Serial.begin(115200);                           // initializes uart communication
    Serial.println("Starting NimBLE Server");       // 
    NimBLEDevice::init("NimBLE");                   // initializes the NimBLEDevice class with the NimBLE class

    NimBLEDevice::setSecurityAuth(true, true, false); /** bonding, MITM, don't need BLE secure connections as we are using passkey pairing */
    NimBLEDevice::setSecurityPasskey(123456);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY); /** Display only passkey */
    NimBLEServer*           pServer                  = NimBLEDevice::createServer();
    NimBLEService*          pService                 = pServer->createService("ABCD");
    NimBLECharacteristic*   pNonSecureCharacteristic = pService->createCharacteristic("1234", NIMBLE_PROPERTY::READ);
    NimBLECharacteristic*   pSecureCharacteristic =
        pService->createCharacteristic("1235",
                                       NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN);

    pNonSecureCharacteristic->setValue("Hello Non Secure BLE");
    pSecureCharacteristic->setValue("Hello Secure BLE");

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("ABCD");
    pAdvertising->setName("NimBLE");
    pAdvertising->start();
}

void loop() {}