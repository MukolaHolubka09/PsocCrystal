#include "project.h"
static uint8_t LED_NUM[] = {
    0xC0, 
    0xF9,
    0xA4, 
    0xB0,
    0x99, 
    0x92,
    0x82, 
    0xF8, 
    0x80, 
    0x90, 
    0x88,
    0x83  
};
static void FourDigit74HC595_sendData(uint8_t data) {
    for(int8_t i = 0; i < 8; i++) {
        if (data & (0x80 >> i)) {
            Pin_DO_Write(1);  
        } else {
            Pin_DO_Write(0);  
        }
        Pin_CLK_Write(1);  
        Pin_CLK_Write(0);  
    }
}
static void FourDigit74HC595_sendOneDigit(uint8_t pos, uint8_t digit, uint8_t dot) {
    FourDigit74HC595_sendData(0xFF & ~(1 << pos));
    if (dot) {
        FourDigit74HC595_sendData(LED_NUM[digit] & 0x7F); 
    } else {
        FourDigit74HC595_sendData(LED_NUM[digit]); 
    }
    Pin_Latch_Write(1);
    Pin_Latch_Write(0);
}
static void (*COLx_SetDriveMode[3])(uint8_t mode) = { C_1_SetDriveMode, C_2_SetDriveMode, C_3_SetDriveMode };
static void (*COLx_Write[3])(uint8_t value) = { C_1_Write, C_2_Write, C_3_Write };
static uint8 (*ROWx_Read[4])(void) = { R_1_Read, R_2_Read, R_3_Read, R_4_Read };
static uint8_t keys[4][3];
static uint8_t keys_act[4][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 },
    { 10, 0, 11 } // 10 = '*', 11 = '#'
};
static void matrixInit() {
    for (int i = 0; i < 3; i++) {
        COLx_SetDriveMode[i](C_1_DM_DIG_HIZ); 
    }
}
static void readMatrix() {
    uint8_t row_count = 4;
    uint8_t col_count = 3;

    for (int col_index=0; col_index < col_count; col_index++) {
        COLx_SetDriveMode[col_index](C_1_DM_STRONG);
        COLx_Write[col_index](0);

        for (int row_index=0; row_index < row_count; row_index++) {
            keys[row_index][col_index] = ROWx_Read[row_index]();
        }
        
        COLx_SetDriveMode[col_index](C_1_DM_DIG_HIZ);
    }
}  
int main(void) {
    CyGlobalIntEnable; 
        SW_Tx_UART_Start();
    SW_Tx_UART_PutString("Lab 4 Started\r\n");

    matrixInit();
        FourDigit74HC595_sendOneDigit(0, 0, 0); 
    
    for (;;) {
        readMatrix();

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 3; col++) {
                if (keys[row][col] == 0) // (Active Low)
                {
                    uint8_t button_number = keys_act[row][col]; 
                    
                    if (button_number < 12) { 
                        FourDigit74HC595_sendOneDigit(0, button_number, 0); 
                        SW_Tx_UART_PutString("Key Pressed\r\n");
                    }
                }
            }
        }
        CyDelay(50);
    }
}