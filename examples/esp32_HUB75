/*
 * 适配 P2.5/P3/P4/P5 等 HUB75 接口 LED 点阵屏
 * 
 * 依赖库 (需要在 Arduino IDE 库管理器安装):
 * 1. ESP32-HUB75-MatrixPanel-I2S-DMA (作者: mrfaptastic)
 * 2. Adafruit GFX Library
 * 
 * 接线指南 (ESP32 -> HUB75 接口):
 * 注意：面板背面的接口通常是 2x8 排针，请对应丝印连接
 * 
 * [HUB75 面板]    [ESP32 引脚]
 * R1  ----------> GPIO 25
 * G1  ----------> GPIO 26
 * B1  ----------> GPIO 27
 * R2  ----------> GPIO 14
 * G2  ----------> GPIO 12
 * B2  ----------> GPIO 13
 * A   ----------> GPIO 23
 * B   ----------> GPIO 19
 * C   ----------> GPIO 5
 * D   ----------> GPIO 17
 * E   ----------> GPIO 18 (64x64屏必须接，64x32屏通常接地或不接)
 * CLK ----------> GPIO 16
 * LAT (STB) ----> GPIO 4
 * OE  ----------> GPIO 15
 * GND ----------> GND (必须共地！)
 * 
 * 供电注意：
 * 面板 VCC 接 5V 大电流电源 (建议 5V/5A 以上)，不要直接用 ESP32 供电！
 */

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ==========================================
//           用户配置区
// ==========================================

// 1. 设置屏幕分辨率
#define PANEL_RES_X 64      // 宽
#define PANEL_RES_Y 64      // 高 (如果是 64x32 的屏，请改为 32)
#define PANEL_CHAIN 1       // 级联数量

// 2. 设备名称
const String baseName = "pindou"; 
const String brandText = "kmpd";

// 3. 亮度设置 (0-255)
#define DEFAULT_BRIGHTNESS 64 

// ==========================================

// BLE UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

MatrixPanel_I2S_DMA *dma_display = nullptr;

bool deviceConnected = false;
bool oldDeviceConnected = false;
String fullDeviceName;
String deviceSuffix; 
String loopText; 

// Animation State
unsigned long lastFrameTime = 0;
int scrollX = PANEL_RES_X; 

// --- Helper: Draw Text Centered or Scrolling ---
void drawStatusText(String text, uint16_t color) {
  dma_display->fillScreen(0);
  
  // Handle black text (invisible on black bg) -> Change to White
  if (color == 0) {
      color = dma_display->color565(255, 255, 255);
  }
  
  dma_display->setTextSize(1);
  dma_display->setTextColor(color);
  
  // Center text roughly
  int16_t x1, y1;
  uint16_t w, h;
  dma_display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  
  int x = (PANEL_RES_X - w) / 2;
  int y = (PANEL_RES_Y - h) / 2;
  
  dma_display->setCursor(x, y);
  dma_display->print(text);
}

// --- BLE Callbacks ---
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Client connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Client disconnected");
      pServer->getAdvertising()->start();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String value = pCharacteristic->getValue();
      if (value.length() == 0) return;
      uint8_t* data = (uint8_t*)value.c_str();
      uint8_t cmd = data[0];

      if (cmd == 0x01) { // Clear
        dma_display->fillScreen(0);
      } 
      else if (cmd == 0x02) { // Draw Pixels
        // Format: [0x02, R, G, B, X1, Y1, X2, Y2, ...]
        if (value.length() >= 4) {
          uint8_t r = data[1];
          uint8_t g = data[2];
          uint8_t b = data[3];
          uint16_t color = dma_display->color565(r, g, b);
          
          // Fix: Convert near-black colors to white
          // Check if total brightness is too low (e.g. sum of R+G+B < 30)
          // Black is (0,0,0), but H07 might be very dark grey
          if ((r + g + b) < 40) { // Threshold: Adjust if needed. 40 is roughly 15% brightness
              color = dma_display->color565(255, 255, 255);
          }
          
          for (int i = 4; i < value.length(); i += 2) {
            if (i + 1 >= value.length()) break;
            uint8_t x = data[i];
            uint8_t y = data[i+1];
            if (x < PANEL_RES_X && y < PANEL_RES_Y) {
              dma_display->drawPixel(x, y, color);
            }
          }
        }
      }
      else if (cmd == 0x03) { // Show (Swap Buffer if enabled, else ignored)
        // dma_display->flipDMABuffer(); 
      }
    }
};

void setup() {
  Serial.begin(115200);
  
  // Increase MTU
  BLEDevice::setMTU(517);
  
  // Generate Name
  uint64_t chipid = ESP.getEfuseMac(); 
  uint16_t chip = (uint16_t)(chipid >> 32);
  char suffix[5];
  snprintf(suffix, 5, "%04X", chip); 
  deviceSuffix = String(suffix);
  fullDeviceName = baseName + "-" + deviceSuffix;
  loopText = brandText + " " + deviceSuffix; 
  
  Serial.print("Device Name: ");
  Serial.println(fullDeviceName);

  // Matrix Config
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // Module width
    PANEL_RES_Y,   // Module height
    PANEL_CHAIN    // Chain length
  );
  
  // Pin Mapping (Standard ESP32-WROOM)
  mxconfig.gpio.r1 = 25;
  mxconfig.gpio.g1 = 26;
  mxconfig.gpio.b1 = 27;
  mxconfig.gpio.r2 = 14;
  mxconfig.gpio.g2 = 12;
  mxconfig.gpio.b2 = 13;
  mxconfig.gpio.a = 23;
  mxconfig.gpio.b = 19;
  mxconfig.gpio.c = 5;
  mxconfig.gpio.d = 17;
  mxconfig.gpio.e = 18; // IMPORTANT for 64x64 panels
  mxconfig.gpio.lat = 4;
  mxconfig.gpio.oe = 15;
  mxconfig.gpio.clk = 16;

  // Clock Config
  mxconfig.clkphase = false; // Optional

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(DEFAULT_BRIGHTNESS);
  dma_display->clearScreen();
  
  // drawStatusText(fullDeviceName, dma_display->color565(0, 0, 255)); // Blue text on startup (OLD)
  drawStatusText(loopText, dma_display->color565(255, 255, 255)); // Show standard ID in White on startup

  // BLE Init
  BLEDevice::init(fullDeviceName.c_str()); 
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMaxPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("BLE Ready");
}

void loop() {
  // Connection Status Logic
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = true;
    dma_display->fillScreen(0); // Clear on connect
    Serial.println("Connected - Cleared Screen");
  }
  
  if (!deviceConnected && oldDeviceConnected) {
    oldDeviceConnected = false;
    dma_display->fillScreen(0);
    drawStatusText(loopText, dma_display->color565(255, 255, 255)); // Show ID on disconnect
    Serial.println("Disconnected - Show ID");
  }

  // Idle Animation (Optional: Heartbeat dot)
  if (!deviceConnected) {
      unsigned long now = millis();
      if (now - lastFrameTime > 1000) { 
          lastFrameTime = now;
          // Blink a small dot in corner to show alive
          static bool blink = false;
          dma_display->drawPixel(0, 0, blink ? dma_display->color565(255, 0, 0) : 0);
          blink = !blink;
      }
  }
  
  delay(50);
}
