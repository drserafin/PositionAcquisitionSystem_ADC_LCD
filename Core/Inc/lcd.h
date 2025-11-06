#ifndef __LCD_H__
#define __LCD_H__

#define LCD_PORT        GPIOB
#define LCD_RS_PIN      GPIO_PIN_0
#define LCD_E_PIN       GPIO_PIN_1
#define LCD_D4_PIN      GPIO_PIN_4
#define LCD_D5_PIN      GPIO_PIN_5
#define LCD_D6_PIN      GPIO_PIN_6
#define LCD_D7_PIN      GPIO_PIN_7

void LCD_Init(void);
void LCD_Clear(void);
void LCD_OutCmd(uint8_t command);
void LCD_OutChar(char letter);
void LCD_OutString(char *str);
void LCD_OutUDec(uint32_t n);
void LCD_OutUFix(uint32_t number);

#endif