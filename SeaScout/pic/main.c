#include "stdio.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "mcc_generated_files/i2c1_master.h"

//Humidity Sensor  
#define Humidity_Slave 0x27
#define Humidity_Data 0x00

//Temperature Sensor
#define Temp_Sensor 0x135

//Motor Driver
#define Motor_Slave
#define Motor_Data

int HumidityFormat(uint16_t data){
    int humidity_formatted = ((data * 1)/330);
    return humidity_formatted;
}

void main(void) {
    SYSTEM_Initialize(); 
    I2C1_Initialize();
    UART2_Initialize();

    uint16_t raw_temp, raw_hum = 0;
    
    
    raw_hum = I2C1_Read2ByteRegister(Humidity_Slave , Humidity_Data);
    int final_data = HumidityFormat(raw_hum);
    
    __delay_ms(1000); 

    while (1){
        if(UART2_is_tx_ready){
            printf("\nHumidity: %u \n\r", final_data);
        }
    }
}