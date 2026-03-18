#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

#define WIDTH 64
#define HEIGHT 64

static uint16_t frame[HEIGHT][WIDTH];

static struct bt_uuid_128 service_uuid = BT_UUID_INIT_128(
  0x4b,0x91,0x31,0xc9,0xc5,0xfc,0xcc,0x8f,0x5e,0x45,0xb5,0x1f,0x01,0xc2,0xaf,0x4f
);

static struct bt_uuid_128 char_uuid = BT_UUID_INIT_128(
  0xa8,0x26,0x1b,0x36,0x07,0xea,0xf5,0xb7,0x88,0x46,0xe1,0x36,0x3e,0x48,0x55,0xbe
);

static uint16_t to565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

static void clear_frame(void) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      frame[y][x] = 0;
    }
  }
}

static void apply_frame(void) {
}

static ssize_t cmd_write(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                         const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
  if (len == 0) return len;
  const uint8_t* d = (const uint8_t*)buf;
  uint8_t cmd = d[0];

  if (cmd == 0x01) {
    clear_frame();
    return len;
  }

  if (cmd == 0x02 && len >= 4) {
    uint16_t color = to565(d[1], d[2], d[3]);
    for (uint16_t i = 4; i + 1 < len; i += 2) {
      uint8_t x = d[i];
      uint8_t y = d[i + 1];
      if (x < WIDTH && y < HEIGHT) {
        frame[y][x] = color;
      }
    }
    return len;
  }

  if (cmd == 0x03) {
    apply_frame();
    return len;
  }

  return len;
}

BT_GATT_SERVICE_DEFINE(pixel_svc,
  BT_GATT_PRIMARY_SERVICE(&service_uuid),
  BT_GATT_CHARACTERISTIC(&char_uuid.uuid,
    BT_GATT_CHRC_WRITE_WITHOUT_RESP | BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE, NULL, cmd_write, NULL)
);

int main(void) {
  int err = bt_enable(NULL);
  if (err) return 0;
  clear_frame();

  struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
    BT_LE_ADV_OPT_CONNECTABLE,
    BT_GAP_ADV_FAST_INT_MIN_2,
    BT_GAP_ADV_FAST_INT_MAX_2,
    NULL
  );

  const struct bt_data ad[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, "kmpd-NRF52", 12)
  };

  bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), NULL, 0);

  while (1) {
    k_sleep(K_MSEC(20));
  }
  return 0;
}

