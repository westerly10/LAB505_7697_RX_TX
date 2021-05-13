import paho.mqtt.client as mqtt

# 當地端程式連線伺服器得到回應時，要做的動作
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("NTOU")
# 當接收到從伺服器發送的訊息時要進行的動作
def on_message(client, userdata, msg):
    print(msg.topic+" " + ":" + str(msg.payload, encoding = "utf-8"))

# 連線設定
# 初始化地端程式
client = mqtt.Client()

# 設定連線的動作
client.on_connect = on_connect

# 設定接收訊息的動作
client.on_message = on_message

# 設定連線資訊(IP, Port, 連線時間)
client.connect("140.121.198.196", 1883, 60)

# 開始連線，執行設定的動作和處理重新連線問題
# 也可以手動使用其他loop函式來進行連接
client.loop_forever()
