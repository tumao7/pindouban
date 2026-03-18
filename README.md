# 智能拼豆板蓝牙模块开放协议

该开放协议是基于微信小程序：酷猫拼豆 开放的智能拼豆板蓝牙模块协议
![1.png](https://github.com/tumao7/pindouban/blob/main/gh_1bbe4fb7135a_258.jpg)


这是“智能拼豆板开放对接”文档入口，面向以下开发者：

- 使用任意 MCU/SoC（ESP32、RTL、Nordic、国产 BLE 芯片等）开发硬件的厂商
- 只希望按协议接入，不使用官方参考固件的团队
- 需要通过兼容性认证并在小程序内可识别的第三方设备开发者

本项目采用“协议开放 + 接入认证 + 兼容分级”的模式，不限制你使用的硬件方案，只要求符合协议与稳定性要求。

## 文档目录

- [01-开放能力与接入模型](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/01-开放能力与接入模型.md)
- [02-BLE开放协议V1](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/02-BLE开放协议V1.md)
- [03-小程序对接指南](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/03-小程序对接指南.md)
- [04-安全与稳定要求](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/04-安全与稳定要求.md)
- [05-兼容性认证流程](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/05-兼容性认证流程.md)
- [06-发布与运维清单](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/06-发布与运维清单.md)
- [device-manifest.schema.json](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/schemas/device-manifest.schema.json)

## 授权与合规

- [LICENSE.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/LICENSE.md)
- [COMMERCIAL_LICENSE.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/COMMERCIAL_LICENSE.md)
- [BUSINESS_CONTACT.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/BUSINESS_CONTACT.md)
- [ENFORCEMENT_POLICY.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/ENFORCEMENT_POLICY.md)
- [TERMS_OF_USE.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/TERMS_OF_USE.md)
- [CHANGELOG.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/CHANGELOG.md)

## 开发示例

- [examples/README.md](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/examples/README.md)
- [ESP32 Arduino 示例](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/examples/esp32-arduino/esp32_arduino_minimal.ino)
- [nRF52 Zephyr 示例](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/examples/nrf52-zephyr/nrf52_zephyr_gatt_sample.c)
- [Linux 网关示例](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/examples/linux-ble-proxy/python_ble_sender.py)

## 快速开始（最短路径）

1. 阅读 [02-BLE开放协议V1](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/02-BLE开放协议V1.md)
2. 按 [03-小程序对接指南](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/03-小程序对接指南.md) 完成握手与像素指令
3. 按 [04-安全与稳定要求](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/04-安全与稳定要求.md) 完成测试
4. 按 [05-兼容性认证流程](file:///d:/xiaochengxu/axcx/hardware/open-smart-pixel-board/docs/05-兼容性认证流程.md) 提交认证

## 接入原则

- 不限制硬件：你可以自研主控、自研驱动板、自研屏幕方案
- 协议优先：只要协议符合，就可以对接
- 安全可控：必须实现最小鉴权与限速策略
- 版本治理：必须声明协议版本，禁止无版本裸协议
