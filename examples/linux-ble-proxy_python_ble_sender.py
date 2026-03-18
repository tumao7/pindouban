import asyncio
from bleak import BleakClient, BleakScanner

SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHAR_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
NAME_PREFIX = "kmpd-"


def cmd_clear():
    return bytes([0x01])


def cmd_show():
    return bytes([0x03])


def cmd_draw(r, g, b, points):
    payload = bytearray([0x02, r, g, b])
    for x, y in points:
        payload.append(x & 0xFF)
        payload.append(y & 0xFF)
    return bytes(payload)


async def find_device():
    devices = await BleakScanner.discover(timeout=5.0)
    for d in devices:
        name = d.name or ""
        if name.startswith(NAME_PREFIX):
            return d
    return None


async def main():
    dev = await find_device()
    if not dev:
        print("未找到设备")
        return

    async with BleakClient(dev.address) as client:
        await client.write_gatt_char(CHAR_UUID, cmd_clear(), response=False)
        await asyncio.sleep(0.05)

        points_red = [(1, 1), (2, 2), (3, 3), (4, 4)]
        points_green = [(10, 10), (11, 10), (12, 10), (13, 10)]

        await client.write_gatt_char(CHAR_UUID, cmd_draw(255, 0, 0, points_red), response=False)
        await asyncio.sleep(0.02)
        await client.write_gatt_char(CHAR_UUID, cmd_draw(0, 255, 0, points_green), response=False)
        await asyncio.sleep(0.02)
        await client.write_gatt_char(CHAR_UUID, cmd_show(), response=False)

        print("发送完成")


if __name__ == "__main__":
    asyncio.run(main())

