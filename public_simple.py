import paho.mqtt.publish as publish

host = "140.121.198.196"              # Broker's IP
topic = "NTOU_1"           # Topic
var = 1                          # Declare variable
while var == 1:                  # Infinite loop
	payload = input('Please enter your message：')  # content of the message
	publish.single(topic, payload, qos=1, hostname=host) #publish message

