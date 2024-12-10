#include "mcc_generated_files/mcc.h"
#include <math.h>
#include "mcc_generated_files/examples/i2c2_master_example.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Humidity Sensor  
#define Humidity_Slave 0x27
#define Humidity_Data 0x00

// Temperature Sensor
#define Temp_Sensor 0b1001100
#define Temp_Data 0x00

// Declare global variables for raw sensor data
volatile uint8_t raw_hum = 0, raw_temp = 0;


// Function to format the raw humidity data to a readable value
float Humidity_Format(uint8_t data) {
    return ((float)data / 330);
}

// Main function
void main(void) {
    SYSTEM_Initialize(); 
    I2C2_Initialize();
    UART1_Initialize();
    
    INTERRUPT_GlobalInterruptEnable();

    while (1) {
        // Read raw data from humidity and temperature sensors
        raw_hum = I2C2_Read1ByteRegister(Humidity_Slave, Humidity_Data);
        raw_temp = I2C2_Read1ByteRegister(Temp_Sensor, Temp_Data);
        
        // Convert raw humidity data to a readable format
        float hum = Humidity_Format(raw_hum);

        // Control motor and display randomized speed
        Motor_SetHigh();
        printf("Motor Speed: %.2f RPM\r\n", 0.0);    // Initial value
        __delay_ms(1000);

        // Generate a randomized motor speed and display it
        float randomizedSpeed = generateRandomSpeed(8675.0f, 40.0f);
        printf("Motor Speed: %.2f RPM\r\n", randomizedSpeed); // Randomized float value
        Motor_SetLow();
        __delay_ms(1000);
    }
}
