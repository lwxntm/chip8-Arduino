#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "CPU.h"
#include "roms.h"
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/SCL, /* data=*/SDA);
CPU *cpu;

void DrawDisplay() {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 32; ++j) {
            if ((cpu->Display[i + j * 64]) != 0)
                u8g2.drawBox(2 * i, 2 * j, 2, 2);
        }
    }
}

void execCPU(void *parameter){
    while (true){
        cpu->ExecuteOpcode();
        delay(2);
    }

}
void setup() {
    Serial.begin(115200);//打开串口波特率115200
    u8g2.begin();
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setFontDirection(0);
    cpu = new CPU();
    //cpu->LoadProgram(Pong1,246);
   // cpu->LoadProgram(Tetris,494);
    cpu->LoadProgram(heart_monitor, 149);
    xTaskCreate(execCPU,"exec",30000,nullptr,1,nullptr);
}
void loop() {
   u8g2.clearBuffer();
   DrawDisplay();
   u8g2.sendBuffer();
}

