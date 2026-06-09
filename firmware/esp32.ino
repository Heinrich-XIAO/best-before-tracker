#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory

// Define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Flash LED pin
#define FLASH_LED_PIN      4

int pictureNumber = 0;

void eeprom_init() {
  // Initialize EEPROM first to get picture number
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber = EEPROM.read(0) + 1;
  
  // Handle overflow (reset to 1 if it exceeds 255)
  if (pictureNumber > 255) {
    pictureNumber = 1;
  }

  Serial.printf("Taking picture number: %d\n", pictureNumber);
}

void sd_card_init() {
  // Initialize SD Card BEFORE camera to reduce memory pressure
  Serial.println("Starting SD Card");
  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    goToSleep(); 
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD Card attached");
    goToSleep(); 
    return;
  }

  Serial.printf("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
}

void camera_config() {
  // Camera configuration - REDUCED settings to prevent stack overflow
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // CRITICAL: Reduced frame size and quality to prevent stack overflow
  if (psramFound()) {
    Serial.println("PSRAM found");
    config.frame_size = FRAMESIZE_XGA;  // Reduced from UXGA to XGA
    config.jpeg_quality = 12;           // Increased from 10 to 12 (lower quality, smaller file)
    config.fb_count = 1;                // Reduced from 2 to 1 to save memory
  } else {
    Serial.println("PSRAM not found");
    config.frame_size = FRAMESIZE_VGA;  // Reduced from SVGA to VGA
    config.jpeg_quality = 15;           // Lower quality for boards without PSRAM
    config.fb_count = 1;
  }
  
  // Additional memory-saving configurations
  config.grab_mode = CAMERA_GRAB_LATEST; // Changed from WHEN_EMPTY to LATEST
}

void camera_init() {
  // Initialize Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    goToSleep();
    return;
  }
  Serial.println("Camera initialized successfully");

  // Additional sensor settings to optimize for memory usage
  sensor_t * s = esp_camera_sensor_get();
  if (s != NULL) {
    // Lower the resolution if needed
    // s->set_framesize(s, FRAMESIZE_VGA);
    
    // Optimize other settings
    s->set_brightness(s, 0);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect)
    s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled
    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
    s->set_aec2(s, 0);           // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);       // -2 to 2
    s->set_aec_value(s, 300);    // 0 to 1200
    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);       // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);            // 0 = disable , 1 = enable
    s->set_wpc(s, 1);            // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
    s->set_lenc(s, 1);           // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
    s->set_vflip(s, 0);          // 0 = disable , 1 = enable
    s->set_dcw(s, 1);            // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
  }
}

void shoot_and_save() {
  // Take Picture with Camera
  Serial.println("Taking picture...");
  camera_fb_t *fb = NULL;
  
  // Try to take picture with retries
  for (int retry = 0; retry < 3; retry++) {
    fb = esp_camera_fb_get();
    if (fb) break;
    Serial.printf("Camera capture failed, retry %d\n", retry + 1);
    delay(500);
  }
  
  if (!fb) {
    Serial.println("Camera capture failed after retries");
    goToSleep();
    return;
  }
  Serial.printf("Picture taken! Size: %zu bytes\n", fb->len);

  // Create file path
  String path = "/picture" + String(pictureNumber) + ".jpg";
  
  // Save picture to SD card
  fs::FS &fs = SD_MMC;
  Serial.printf("Picture file name: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file in writing mode");
    esp_camera_fb_return(fb);
    goToSleep();
    return;
  }

  // Write file in chunks to prevent memory issues
  size_t totalBytes = fb->len;
  size_t bytesWritten = 0;
  const size_t chunkSize = 1024; // Write in 1KB chunks
  
  while (bytesWritten < totalBytes) {
    size_t toWrite = min(chunkSize, totalBytes - bytesWritten);
    size_t written = file.write(fb->buf + bytesWritten, toWrite);
    if (written != toWrite) {
      Serial.println("Write error occurred");
      break;
    }
    bytesWritten += written;
  }
  
  file.close();
  
  if (bytesWritten == totalBytes) {
    Serial.printf("Saved file to path: %s (%zu bytes)\n", path.c_str(), bytesWritten);
    
    // Update picture number in EEPROM only after successful save
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
    Serial.println("Picture saved successfully!");
  } else {
    Serial.printf("File write incomplete: %zu/%zu bytes\n", bytesWritten, totalBytes);
  }
  
  // Return the frame buffer immediately after use
  esp_camera_fb_return(fb);
}

void goToSleep() {
  // Deinitialize camera to free memory before sleep
  esp_camera_deinit();
  
  // Turn off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  Serial.println("Going to sleep now");
  Serial.flush();
  
  delay(100);
  
  esp_sleep_enable_ext0_wakeup(16, 0);
  esp_deep_sleep_start();
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector

  Serial.begin(115200);
  Serial.println("ESP32-CAM Photo Capture Starting...");

  Serial1.begin(115200);
  Serial1.setTX(0);
  Serial1.setRX(16);

  eeprom_init();
  sd_card_init();

  camera_config();
  camera_init();

  // Wait a moment for camera to stabilize
  delay(1000);
  shoot_and_save();
  // Brief delay before sleep
  delay(500);
  goToSleep();
}

void loop() {
    if (Serial1.available()) {
        String cmd = Serial1.readStringUntil('\n');
        cmd.trim();

        if (cmd == "SHOOT") {
          eeprom_init();
          sd_card_init();

          camera_config();
          camera_init();

          // Wait a moment for camera to stabilize
          delay(1000);
          shoot_and_save();
          // Brief delay before sleep
          delay(500);
          goToSleep();
        }
    } else {
        Serial.println("Serial1 not available.")
    }
}