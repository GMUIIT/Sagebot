#include <Arduino.h>
#include "BLEUart.h"

#ifndef BLE_UART_CPP
#define BLE_UART_CPP

void BLEUart::init() {
    //////// BLE init
    // Create the BLE Device
    BLEDevice::init("Sagebot");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(&deviceConnected, pServer));

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID_TX,
            BLECharacteristic::PROPERTY_NOTIFY
            );

    pTxCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID_RX,
            BLECharacteristic::PROPERTY_WRITE
            );

    pRxCharacteristic->setCallbacks(new IncomingCallback(handler));

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
}


void BLEUart::write(std::string str) {
    pTxCharacteristic->setValue((uint8_t*)str.c_str(), str.length());
    pTxCharacteristic->notify();
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
}

void BLEUart::MyServerCallbacks::onConnect(BLEServer* pServer) {
    *parentDeviceConnected = true;
}
void BLEUart::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    *parentDeviceConnected = false;
    // TODO: this may not work, original sketch waits ~500ms before restarting
    // advertising
    pServer->startAdvertising();
}

void BLEUart::IncomingCallback::onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        handleIncoming(rxValue);
    }
}
#endif
