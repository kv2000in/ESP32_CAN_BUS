/*
 * 
 * ESP32 first boot output. 115200 baud rate
 * 
14:47:28.714 -> ⸮⸮⸮'⸮⸮0⸮⸮⸮b⸮⸮n⸮&ʶC⸮⸮⸮y⸮⸮ѐ⸮0⸮
⸮[0;32mI (30) boot: ESP-IDF v4.1-dirty 2nd stage bootloader[0m
14:47:28.729 -> [0;32mI (30) boot: compile time 16:15:01[0m
14:47:28.733 -> [0;32mI (30) boot: chip revision: 1[0m
14:47:28.736 -> [0;32mI (33) boot_comm: chip revision: 1, min. bootloader chip revision: 0[0m
14:47:28.741 -> [0;32mI (40) boot.esp32: SPI Speed      : 40MHz[0m
14:47:28.746 -> [0;32mI (45) boot.esp32: SPI Mode       : DIO[0m
14:47:28.751 -> [0;32mI (49) boot.esp32: SPI Flash Size : 4MB[0m
14:47:28.755 -> [0;32mI (54) boot: Enabling RNG early entropy source...[0m
14:47:28.759 -> [0;32mI (59) boot: Partition Table:[0m
14:47:28.763 -> [0;32mI (63) boot: ## Label            Usage          Type ST Offset   Length[0m
14:47:28.767 -> [0;32mI (70) boot:  0 nvs              WiFi data        01 02 00009000 00005000[0m
14:47:28.772 -> [0;32mI (77) boot:  1 otadata          OTA data         01 00 0000e000 00002000[0m
14:47:28.777 -> [0;32mI (85) boot:  2 app0             OTA app          00 10 00010000 00300000[0m
14:47:28.782 -> [0;32mI (92) boot:  3 spiffs           Unknown data     01 82 00310000 000f0000[0m
14:47:28.785 -> [0;32mI (100) boot: End of partition table[0m
14:47:28.790 -> [0;32mI (104) boot_comm: chip revision: 1, min. application chip revision: 0[0m
14:47:28.795 -> [0;32mI (111) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x1d2048 (1908808) map[0m
14:47:29.003 -> [0;32mI (847) esp_image: segment 1: paddr=0x001e2070 vaddr=0x3ffbdb60 size=0x04d3c ( 19772) load[0m
14:47:29.041 -> [0;32mI (856) esp_image: segment 2: paddr=0x001e6db4 vaddr=0x40080000 size=0x00400 (  1024) load[0m
14:47:29.048 -> [0;32mI (856) esp_image: segment 3: paddr=0x001e71bc vaddr=0x40080400 size=0x08e54 ( 36436) load[0m
14:47:29.055 -> [0;32mI (879) esp_image: segment 4: paddr=0x001f0018 vaddr=0x400d0018 size=0x9df74 (647028) map[0m
14:47:29.286 -> [0;32mI (1125) esp_image: segment 5: paddr=0x0028df94 vaddr=0x40089254 size=0x0b6a0 ( 46752) load[0m
14:47:29.323 -> [0;32mI (1157) boot: Loaded app from partition at offset 0x10000[0m
14:47:29.328 -> [0;32mI (1158) boot: Disabling RNG early entropy source...[0m
14:47:29.357 -> E (59) psram: PSRAM ID read error: 0xffffffff
14:47:29.390 -> 
14:47:29.393 -> E (116) sdmmc_common: sdmmc_init_ocr: send_op_cond (1) returned 0x107
14:47:29.427 -> Card Mount Failed
14:47:29.430 -> PSRAM Initialization Failure
14:47:29.605 -> [E][camera.c:1113] camera_probe(): Detected camera not supported.
14:47:29.607 -> [E][camera.c:1379] esp_camera_init(): Camera probe failed with error 0x20004
14:47:29.610 -> Camera init failed with error 0x20004


*/


/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-spi-communication-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

//Find the default SPI pins for your board
//Make sure you have the right board selected in Tools > Boards
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  
}

void loop() {
  // put your main code here, to run repeatedly:
}
