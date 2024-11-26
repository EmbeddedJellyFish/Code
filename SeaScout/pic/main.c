#include "mcc_generated_files/mcc.h"
#include "math.h"

//Humidity Sensor  
#define Humidity_Slave 0x27
#define Humidity_Data 0x00

//Temperature Sensor
#define Temp_Sensor 0x135

//Motor Driver
#define Motor_W 0xC8
#define Motor_R 0xC9
#define Control 0x00
#define Fault 0x01
uint8_t Voltage_ON = 0xA6;
uint8_t Voltage_OFF = 0x00;

//NEED TO FINISH THIS
typedef enum{
    Triangle = 1,
    Square = 2,
    Circle = 3
}Path_Type;

//Defining ID so that we can separate the data sent over UART
typedef enum{
    Temp_ID = 1,
    Hum_ID = 2,
    Motor_ID = 3
}Sensor_Type;

//Volatile because variable is being controlled outside of the compiler
volatile uint16_t raw_hum, raw_temp, raw_motor = 0xFF;

//Converts binary to a decimal number
float Binary_to_Decimal(uint16_t data){

    float decimal = 0;
    for (int i = 0; i < 16; i++){
        if(data & 1){
            decimal += (1 << i);
        }
        data >>= 1;
    }
    return decimal;
}

//Formats raw binary humidity to readable values using formula provided by data sheet
float Humidity_Format(uint16_t data){
    float humidity_formatted = ((data * 1)/330);
    return humidity_formatted;
}

//NEED TO FINISH THIS
void Timer(){
    int time_s;
    time_s++;
    printf(time_s);
}

//NEED TO FINISH THIS
void Boat_Path(Path_Type path){
    if(path == 1){
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_ON);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_OFF);
    }else if (path == 2){
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_ON);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_OFF);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_ON);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_OFF);
    }else if (path == 3){
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_ON);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_OFF);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_ON);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_OFF);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_ON);
        I2C2_Write1ByteRegister(Motor_W, Control, Voltage_OFF);
    }
}

//Prints data formatted properly over UART 
void Buffer_Format(float data, Sensor_Type sensor) {

    char buffer[20];
    if (sensor == Temp_ID) {
        sprintf(buffer, "Temperature: %.3f", data);
    } else if (sensor == Hum_ID) {
        sprintf(buffer, "Humidity: %.3f", Humidity_Format(data));
    } else if (sensor == Motor_ID) {
        sprintf(buffer, "Speed: %.3f", data);
    }
    printf("%s\n", buffer); 

}

//Reads data, make sure this is in the while loop
void Read_Data(void){
    raw_temp = I2C1_Read2ByteRegister(Temp_Sensor, Temp_Sensor);
    raw_hum = I2C1_Read2ByteRegister(Humidity_Slave, Humidity_Data);
}

void main(void) {

    SYSTEM_Initialize(); 
    I2C1_Initialize();
    UART1_Initialize();

    TMR0_Initialize();

    for(;;){

        //WIP
        Boat_Path();
        Timer();

        int i = 0;
        Read_Data();
        Buffer_Format(HumidityFormat(raw_hum), Hum_ID);
        Buffer_Format(binarytoDecimal(raw_temp), Temp_ID); 
        __delay_ms(1000);

        if(raw_hum == 0xFF && raw_motor == 0xFF && raw_temp == 0xFF && i == 20){
            printf("I2C broken :( ");
        }
        i++;
    }
}