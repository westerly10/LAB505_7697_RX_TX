from flask import Flask, render_template, request, jsonify
import paho.mqtt.client as mqtt

app = Flask(__name__)  # 沒有其它的用意，如此flask才會知道你的root在何處!

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))  # rc 為狀態碼 0為連接上
    client.subscribe("NTOU")

# 全域變數
motor_position = 0


def on_message(client, userdata, msg):
    print(msg.topic+" " + ":" + str(msg.payload, encoding="utf-8"))

    if msg.payload[0] == 'r':  # payload 回傳第0格表示為馬達回傳功能、後面表示馬達數值
        # value = msg.payload[1]
        motor_position = msg.payload[1]
    else:
        motor_position = 0

# @app.route('/MOTOR_GET_MOOTER_VALUE/<VAL>', methods=['POST'])#帶資料向網頁請求
# def MOTOR_GET_MOOTER_VALUE(VAL):
#     print(VAL)
#     # MQTT SUB接值
#     return motor_position

@app.route('/MOTOR_GET_MOOTER_VALUE/<VAL>')
def MOTOR_GET_MOOTER_VALUE(VAL):  # 對馬達讀取
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    read_motor = VAL
    publish.single(topic, read_motor, qos=1, hostname=host)  # 發布訊息 qos
    return""

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

if __name__ == '__main__':  # 程式進入點
    app.config['TEMPLATES_AUTO_RELOAD'] = True  # 當template有修改會自動更新
    app.run(host='0.0.0.0', debug=False, threaded=True)  # 將web server啟動