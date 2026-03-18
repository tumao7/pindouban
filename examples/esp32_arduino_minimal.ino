#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic* gCharacteristic = nullptr;
bool gConnected = false;
const uint8_t WIDTH = 64;
const uint8_t HEIGHT = 64;
uint16_t gFrame[64][64];

void clearFrame() {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      gFrame[y][x] = 0;
    }
  }
}

uint16_t to565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void applyFrame() {
}

class ServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    gConnected = true;
  }
  void onDisconnect(BLEServer* pServer) {
    gConnected = false;
    pServer->getAdvertising()->start();
  }
};

class WriteCB : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string raw = pCharacteristic->getValue();
    if (raw.size() == 0) return;
    const uint8_t* data = (const uint8_t*)raw.data();
    uint8_t cmd = data[0];

    if (cmd == 0x01) {
      clearFrame();
      return;
    }

    if (cmd == 0x02) {
      if (raw.size() < 4) return;
      uint16_t color = to565(data[1], data[2], data[3]);
      for (size_t i = 4; i + 1 < raw.size(); i += 2) {
        uint8_t x = data[i];
        uint8_t y = data[i + 1];
        if (x < WIDTH && y < HEIGHT) {
          gFrame[y][x] = color;
        }
      }
      return;
    }

    if (cmd == 0x03) {
      applyFrame();
      return;
    }
  }
};

void setup() {
  BLEDevice::init("kmpd-ESP32");
  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new ServerCB());

  BLEService* service = server->createService(SERVICE_UUID);
  gCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
  );
  gCharacteristic->setCallbacks(new WriteCB());
  service->start();
  server->getAdvertising()->addServiceUUID(SERVICE_UUID);
  server->getAdvertising()->start();
  clearFrame();
}

void loop() {
  delay(20);
}

