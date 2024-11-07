import machine as m

UART = m.UART(2, baudrate=9600, tx=m.Pin(17), rx=m.Pin(16), bits=8, parity=None, stop=1)

if UART.any():
    data = UART.read(2)
    print(data)
