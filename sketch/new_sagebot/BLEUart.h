#ifndef BLE_UART_H
#define BLE_UART_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// These UUIDS correspond to Nordic Semiconductor's UART profile, and
// allow interoperation with e.g. the Bluefruit app.
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// To make defining functions easier:
// Accepts a std::string& and returns void.
typedef void (*handlerFunc_t) (std::string &);

/**
 * The BLEUart class defines an object that works similiarly to a software UART,
 * compatible with nRF's BLE UART protocol.  Outgoing strings are sent with .write(),
 * and the handler function provided in the constructor is invoked whenever an incoming
 * string appears.
 * 
 * To use this class, just initialize it with the device name and incoming handler function,
 * then call .init() to setup the device and start advertising it.  Note that you CANNOT
 * call .write() before calling init()!
 */
class BLEUart {
    private:
        BLEServer *pServer = NULL;
        BLECharacteristic * pTxCharacteristic;
        bool deviceConnected = false;
        bool oldDeviceConnected = false;
        uint8_t txValue = 0;
        std::string name;
        handlerFunc_t handler;
    public:
        BLEUart(std::string name, handlerFunc_t incomingStringHandler)
            : name(name), handler(incomingStringHandler) {}

        void init();

        /**
         * Write the given string to the virtual serial port.
         */
        void write(std::string str);


        class MyServerCallbacks: public BLEServerCallbacks {
            private:
                bool *parentDeviceConnected;
                BLEServer *pServer;
            public:
                MyServerCallbacks(bool *deviceConnected, BLEServer *pServer)
                    : parentDeviceConnected(deviceConnected), pServer(pServer) {}

                void onConnect(BLEServer* pServer);

                void onDisconnect(BLEServer* pServer);
        };

        class IncomingCallback : public BLECharacteristicCallbacks {
            private:
                handlerFunc_t handleIncoming;
            public:
                IncomingCallback(handlerFunc_t handler) 
                    : handleIncoming(handler) {}

                void onWrite(BLECharacteristic *pCharacteristic);
        };
};
#endif
