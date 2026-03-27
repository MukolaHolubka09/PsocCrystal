#include "project.h"

void SetLED(uint8_t r, uint8_t g, uint8_t b) {
    LED_R_Write(r);
    LED_G_Write(g);
    LED_B_Write(b);
}
int main(void)
{
    CyGlobalIntEnable; 

    SW_Tx_UART_Start();
    SW_Tx_UART_PutString("\r\n--- 10-Second Color Swap Started ---\r\n");

    uint8_t button_state = 1;
    uint8_t last_button_state = 1;
    
    uint8_t color_mode = 0; 
    uint8_t led_phase = 0;  
    uint16_t timer = 0;   

    for(;;)
    {
        button_state = Button_Read();

        if (button_state == 0 && last_button_state == 1) 
        {
            color_mode = !color_mode; 
            
            timer = 0; 
            led_phase = 0; 
            
            if (color_mode == 0) {
                SW_Tx_UART_PutString("Mode: RED / GREEN (10s swap)\r\n");
            } else {
                SW_Tx_UART_PutString("Mode: TURQUOISE / PURPLE (10s swap)\r\n");
            }
        }
        last_button_state = button_state;


        timer++;
        if (timer >= 1000) 
        {
            timer = 0;              
            led_phase = !led_phase; 
        }
    
        if (color_mode == 0) 
        {
   
            if (led_phase == 0) {
                SetLED(0, 1, 1); 
            } else {
                SetLED(1, 0, 1);
            }
        } 
        else 
        {
   
            if (led_phase == 0) {
                SetLED(1, 0, 0); 
            } else {
                SetLED(0, 1, 0);
            }
        }
        CyDelay(10); 
    }
}
