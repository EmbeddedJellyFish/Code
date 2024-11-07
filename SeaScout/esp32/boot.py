from machine import time

#blinks LED so that we know if boot was successful 
led = time.Pin(2, time.Pin.OUT)     
while True:               
    led.value(1) 
    time.sleep(1)
    led.value(0)
    time.sleep(1)


