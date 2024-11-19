#include <mcc_generated_files/mcc.h>
#include <math.h>


//Humidity Sensor  
#define Humidity_Slave 0x27
#define Humidity_Data 0x00

//Temperature Sensor
#define Temp_Sensor 0x135

//Motor Driver
#define Motor_Slave
#define Motor_Data
#define Motor_Voltage 

//Defining ID so that we can separate the data sent over UART
typedef enum{
    Temp_ID = 1,
    Hum_ID = 2,
    Motor_ID = 3
}SensorType;

//Volatile because variable is being controlled outside of the compiler
volatile uint16_t raw_hum, raw_temp, raw_motor = 0;

//Converts binary to a decimal number
float binarytoDecimal(uint16_t data){

    float decima = 0;
    for (int i = 0; i < 16; i++){
        if(data & 1){
            decimal += (1 << i);
        }
        data >>= 1;
    }
    return decimal;
}

//Formats raw binary humidity to readable values using formula provided by data sheet
float HumidityFormat(uint16_t data){
    float humidity_formatted = ((data * 1)/330);
    return humidity_formatted;
}

//Prints data formatted properly over UART 
void send_Data(float data, SensorType sensor) {

    char buffer[20];
    if (sensor == Temp_ID) {
        sprintf(buffer, "Temperature: %.3f°C", data);
    } else if (sensor == Hum_ID) {
        sprintf(buffer, "Humidity: %.3f%", HumidityFormat(data));
    } else if (sensor == Motor_ID) {
        sprintf(buffer, "Speed: %.3frpm", data);
    }
    printf("%s\n", buffer); 

}

//Reads data, make sure this is in the while loop
void ReadData(void){
    raw_temp = I2C1_Read2ByteRegister(Temp_Sensor, Temp_Sensor);
    raw_hum = I2C1_Read2ByteRegister(Humidity_Slave, Humidity_Data);
}

void main(void) {

    SYSTEM_Initialize(); 
    I2C1_Initialize();
    UART1_Initialize();

    for(;;){
        ReadData();
        send_Data(HumidityFormat(raw_hum), Hum_ID);
        send_Data(binarytoDecimal(raw_temp), Temp_ID); 
        __delay_ms(1000);
    }
}