/*
 * FINAL PROJECT V15: FIX LỖI TREO MÀN HÌNH (STUCK ON 'G')
 * - Bỏ Startup Test.
 * - Chuyển sang Page Addressing Mode (0x20, 0x02).
 * - Kiểm tra lỗi đường truyền I2C.
 */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "driver/adc.h"
#include "model.h"

// --- 1. CẤU HÌNH ---
#define I2C_PORT I2C_NUM_0
#define OLED_ADDR 0x3C
#define MPU_ADDR 0x68
#define FLEX_THUMB ADC1_CHANNEL_4
#define FLEX_INDEX ADC1_CHANNEL_5
#define FLEX_MID ADC1_CHANNEL_7
#define BAT_PIN ADC1_CHANNEL_6
#define BAT_MULTIPLIER 2.0
#define ALPHA 0.6

// --- 2. KHAI BÁO AI ---
Eloquent::ML::Port::RandomForest clf;

// --- 3. FONT CHỮ CHUẨN (8x8) ---
const uint8_t FONT_A[] = {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00, 0x00};
const uint8_t FONT_B[] = {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00};
const uint8_t FONT_D[] = {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00};
const uint8_t FONT_G[] = {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, 0x00};
const uint8_t FONT_Q[] = {0x00, 0x60, 0x80, 0x8D, 0x90, 0x60, 0x00, 0x00};

// Font số Pin
const uint8_t FONT_NUM[][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
};

const char *get_gesture_name(int class_id)
{
    switch (class_id)
    {
    case 0:
        return "CHU A";
    case 1:
        return "CHU B";
    case 2:
        return "CHU D";
    case 3:
        return "CHU G";
    default:
        return "?";
    }
}

// --- 4. DRIVER OLED (CÓ TRẢ VỀ TRẠNG THÁI LỖI) ---
// Sửa hàm i2c_send để trả về kết quả (thành công hay thất bại)
esp_err_t i2c_send(uint8_t addr, uint8_t data, uint8_t mode)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, mode, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    // Tăng thời gian chờ lên 50ms để tránh timeout
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 50 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

void oled_init()
{
    uint8_t cmds[] = {
        0xAE,       // Display OFF
        0x20, 0x02, // PAGE ADDRESSING MODE (QUAN TRỌNG: Chống lỗi nhảy trang)
        0xB0,       // Page Start Address
        0xC8, 0xA1, // Flip Screen
        0x8D, 0x14, // Charge Pump
        0xAF        // Display ON
    };
    for (int i = 0; i < sizeof(cmds); i++)
        i2c_send(OLED_ADDR, cmds[i], 0x00);
}

// --- 5. HÀM UPDATE MÀN HÌNH ---
void oled_update_display(int class_id, int battery_percent)
{

    // Gán Font cứng
    const uint8_t *current_font;
    if (class_id == 0)
        current_font = FONT_A;
    else if (class_id == 1)
        current_font = FONT_B;
    else if (class_id == 2)
        current_font = FONT_D;
    else if (class_id == 3)
        current_font = FONT_G;
    else
        current_font = FONT_Q;

    int chuc = battery_percent / 10;
    int donvi = battery_percent % 10;
    if (chuc > 9)
        chuc = 9;

    uint8_t empty[128];
    memset(empty, 0, 128);
    esp_err_t ret = ESP_OK;

    for (int page = 0; page < 8; page++)
    {
        // Set Page Address
        i2c_send(OLED_ADDR, 0xB0 + page, 0x00);
        i2c_send(OLED_ADDR, 0x00, 0x00); // Lower Column Start
        i2c_send(OLED_ADDR, 0x10, 0x00); // Higher Column Start

        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, 0x40, true);

        if (page == 0)
        {
            // Vẽ Pin
            i2c_master_write(cmd, empty, 90, true);
            if (battery_percent == 100)
            {
                i2c_master_write(cmd, FONT_NUM[9], 5, true);
                uint8_t s = 0;
                i2c_master_write(cmd, &s, 1, true);
                i2c_master_write(cmd, FONT_NUM[9], 5, true);
            }
            else
            {
                if (chuc > 0)
                    i2c_master_write(cmd, FONT_NUM[chuc], 5, true);
                else
                    i2c_master_write(cmd, empty, 5, true);
                uint8_t s = 0;
                i2c_master_write(cmd, &s, 1, true);
                i2c_master_write(cmd, FONT_NUM[donvi], 5, true);
            }
            uint8_t s = 0;
            i2c_master_write(cmd, &s, 1, true);
            i2c_master_write(cmd, FONT_NUM[10], 5, true);
            i2c_master_write(cmd, empty, 10, true);
        }
        else if (page == 4)
        {
            // Vẽ Chữ
            i2c_master_write(cmd, empty, 56, true);
            for (int col = 0; col < 8; col++)
            {
                uint8_t pixel_col = current_font[col];
                i2c_master_write_byte(cmd, pixel_col, true);
                i2c_master_write_byte(cmd, pixel_col, true);
            }
            i2c_master_write(cmd, empty, 56, true);
        }
        else
        {
            i2c_master_write(cmd, empty, 128, true);
        }

        i2c_master_stop(cmd);
        // Gửi lệnh và kiểm tra lỗi
        esp_err_t err = i2c_master_cmd_begin(I2C_PORT, cmd, 50 / portTICK_PERIOD_MS);
        if (err != ESP_OK)
            ret = err;
        i2c_cmd_link_delete(cmd);
    }

    // Nếu lỗi, in ra Terminal để biết
    if (ret != ESP_OK)
    {
        printf("!!! OLED UPDATE ERROR: %s !!!\n", esp_err_to_name(ret));
    }
}

int read_battery_stable()
{
    long sum = 0;
    for (int i = 0; i < 20; i++)
    {
        sum += adc1_get_raw(BAT_PIN);
        esp_rom_delay_us(100);
    }
    int raw = sum / 20;
    float voltage = (raw / 4095.0) * 3.3 * BAT_MULTIPLIER;
    int percent = (int)((voltage - 3.3) * 100 / (4.2 - 3.3));
    if (percent > 99)
        percent = 99;
    if (percent < 0)
        percent = 0;
    return percent;
}

extern "C" void app_main(void)
{
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = 21;
    conf.scl_io_num = 22;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    i2c_param_config(I2C_PORT, &conf);
    i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);

    i2c_send(MPU_ADDR, 0x00, 0x6B);
    oled_init();

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(FLEX_THUMB, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(FLEX_INDEX, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(FLEX_MID, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(BAT_PIN, ADC_ATTEN_DB_12);

    printf("--- SYSTEM READY V15 (NO STARTUP TEST) ---\n");
    float prev_input[6] = {0, 0, 0, 0, 0, 0};
    int displayed_bat = -1;

    while (1)
    {
        float raw[6];
        raw[0] = (float)adc1_get_raw(FLEX_INDEX);
        raw[1] = (float)adc1_get_raw(FLEX_THUMB);
        raw[2] = (float)adc1_get_raw(FLEX_MID);

        uint8_t mpu[6];
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (MPU_ADDR << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, 0x3B, true);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (MPU_ADDR << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmd, mpu, 6, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        // Tăng timeout lên 50ms cho MPU
        i2c_master_cmd_begin(I2C_PORT, cmd, 50 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        raw[3] = (float)((int16_t)(mpu[0] << 8 | mpu[1]));
        raw[4] = (float)((int16_t)(mpu[2] << 8 | mpu[3]));
        raw[5] = (float)((int16_t)(mpu[4] << 8 | mpu[5]));

        float input[6];
        for (int i = 0; i < 6; i++)
        {
            input[i] = (ALPHA * raw[i]) + ((1.0 - ALPHA) * prev_input[i]);
            prev_input[i] = input[i];
        }

        int current_bat = read_battery_stable();
        if (abs(current_bat - displayed_bat) > 1 || displayed_bat == -1)
        {
            displayed_bat = current_bat;
        }

        int result = clf.predict(input);

        // LOG RA MÀN HÌNH ĐỂ KIỂM TRA
        printf("TERMINAL: %s (%d) | Pin: %d\n", get_gesture_name(result), result, displayed_bat);

        oled_update_display(result, displayed_bat);

        // Giãn thời gian ra một chút để I2C kịp thở
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}