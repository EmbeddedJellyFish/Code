import ssl
from mqtt_as.mqtt_as import MQTTClient
from mqtt_as.mqtt_local import wifi_led, blue_led, config
import uasyncio as asyncio
from machine import UART
import time
import logging
logging.basicConfig(level=logging.DEBUG)
from config import *

MAXTX = 4

uart = UART(2, 9600, tx=17, rx=16)
uart.init(9600, bits=8, parity=None, stop=1, flow=0)

async def receiver():
    b = b''
    sreader = asyncio.StreamReader(uart)
    while True:
        res = await sreader.read(1)
        if res == b'\r':
            await client.publish(TOPIC_PUB, b, qos=1)
            print('published', b)
            b = b''
        else:
            b += res

def sub_cb(topic, msg, retained):
    print(f'Topic: "{topic.decode()}" Message: "{msg.decode()}" Retained: {retained}')
    uart.write(msg)
    uart.write('\r\n')
    time.sleep(0.01)

async def wifi_han(state):
    wifi_led(not state)
    print('Wifi is ', 'up' if state else 'down')
    await asyncio.sleep(1)

async def conn_han(client):
    await client.subscribe(TOPIC_SUB, 1)

async def main(client):
    try:
        await client.connect()
    except OSError:
        print('Connection failed.')
        return
    while True:
        await asyncio.sleep(1)

config['server'] = MQTT_SERVER
config['ssid'] = WIFI_SSID
config['wifi_pw'] = WIFI_PASSWORD

config['ssl'] = True
with open('certs/student_key.pem', 'rb') as f:
    key_data = f.read()
with open('certs/student_crt.pem', 'rb') as f:
    cert_data = f.read()
with open('certs/ca_crt.pem', 'rb') as f:
    ca_data = f.read()
ssl_params = {}
ssl_params["cert"] = cert_data
ssl_params["key"] = key_data
ssl_params["cadata"] = ca_data
ssl_params["server_hostname"] = MQTT_SERVER
ssl_params["cert_reqs"] = ssl.CERT_REQUIRED
config["time_server"] = MQTT_SERVER
config["time_server_timeout"] = 10

config['ssl_params'] = ssl_params

config['subs_cb'] = sub_cb
config['wifi_coro'] = wifi_han
config['connect_coro'] = conn_han
config['clean'] = True
config['user'] = MQTT_USER
config["password"] = MQTT_PASSWORD

MQTTClient.DEBUG = True
client = MQTTClient(config)

asyncio.create_task(receiver())
try:
    asyncio.run(main(client))
finally:
    client.close()
    asyncio.new_event_loop()

