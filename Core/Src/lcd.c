#include "stm32f051x8.h"
#include "stm32f0xx_hal.h"
#include "lcd.h"


static void SendPulse(void);
static void LCD_OutNibble(uint8_t nibble); 

void LCD_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(LCD_PORT, LCD_RS_PIN | LCD_E_PIN |
                                 LCD_D4_PIN | LCD_D5_PIN |
                                 LCD_D6_PIN | LCD_D7_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LCD_RS_PIN | LCD_E_PIN |
                           LCD_D4_PIN | LCD_D5_PIN |
                           LCD_D6_PIN | LCD_D7_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);

    HAL_Delay(50);

    HAL_GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
    LCD_OutNibble(0x30);
    HAL_Delay(5);
    LCD_OutNibble(0x30);
    HAL_Delay(1);
    LCD_OutNibble(0x30);
    HAL_Delay(1);

    LCD_OutNibble(0x20); 
    HAL_Delay(1);

    LCD_OutCmd(0x28); 
    LCD_OutCmd(0x0C); 
    LCD_OutCmd(0x06); 

    LCD_Clear();
}

void LCD_Clear(void) {
    LCD_OutCmd(0x01);
    HAL_Delay(2);
}

void LCD_OutCmd(uint8_t command) { 
    HAL_GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_PIN_RESET); 
    LCD_OutNibble(command & 0xF0); 
    LCD_OutNibble((command << 4) & 0xF0); 
}

void LCD_OutChar(char letter) {
    HAL_GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_PIN_SET); 
    LCD_OutNibble(letter & 0xF0);
    LCD_OutNibble((letter << 4) & 0xF0);
}

void LCD_OutString(char *str) {
    while (*str) {
        LCD_OutChar(*str++);
    }
}

static void LCD_OutNibble(uint8_t nibble) {
    HAL_GPIO_WritePin(LCD_PORT, LCD_D4_PIN, (nibble & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, LCD_D5_PIN, (nibble & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, LCD_D6_PIN, (nibble & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, LCD_D7_PIN, (nibble & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    SendPulse();
}

static void SendPulse(void) {
    HAL_GPIO_WritePin(LCD_PORT, LCD_E_PIN, GPIO_PIN_SET); 
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, LCD_E_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_OutUDec(uint32_t n) {
    char buffer[10];
    int i = 0;

    if(n == 0) {
        LCD_OutChar('0');
        return;
    }
    while(n > 0) {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    }
    for(int j = i - 1; j >= 0; j--) {
        LCD_OutChar(buffer[j]);
    }
}

void LCD_OutUFix(uint32_t number) {
    if(number >= 10000) {
        LCD_OutString("*.*");
        LCD_OutChar('*'); 
        return;
    }

    uint32_t intPart = number / 1000;
    uint32_t fracPart = number % 1000;

    LCD_OutChar('0' + intPart);  
    LCD_OutChar('.');             

    LCD_OutChar('0' + (fracPart / 100));           
    LCD_OutChar('0' + ((fracPart / 10) % 10));    
    LCD_OutChar('0' + (fracPart % 10));           
}

