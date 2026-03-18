# 开发示例总览

本目录提供多芯片、多产品形态的对接示例，帮助开发者快速落地。

## 示例列表

- [ESP32 Arduino 最小可运行示例（HUB75）](esp32_arduino_minimal.ino)
- [nRF52 Zephyr GATT 示例（通用像素板）](nrf52_zephyr_gatt_sample.c)
- [Linux 网关发送示例（Python + Bleak）](linux-ble-proxy_python_ble_sender.py)
- [设备接入案例文档](DEVICE_CASES_CN.md)

## 对应产品形态

- HUB75 点阵屏控制板
- 自研 BLE 主控 + 自研显示驱动
- Linux 主机作为 BLE 网关转发器

## 使用建议

- 先跑通 ESP32 示例，验证协议链路
- 再按自身芯片迁移到 nRF52 或自研平台
- 如果你的产品是主机+外设架构，可直接参考 Linux 网关示例

