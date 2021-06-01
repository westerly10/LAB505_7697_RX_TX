from flask import Flask, render_template, request, jsonify
# render_template 內建函數，渲染模板，它需要一個『模板』來提供渲染的格式
import pymysql
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import datetime
import demjson
app = Flask(__name__)  # 沒有其它的用意，如此flask才會知道你的root在何處!

# test sub


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))  # rc 為狀態碼 0為連接上
    client.subscribe("NTOU")


def on_message(client, userdata, msg):
    print(msg.topic+" " + ":" + str(msg.payload, encoding="utf-8"))
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    # publish.single(topic, "xxx", qos=1, hostname=host)  # 發布訊息 qos
    if msg.payload[0] == 'r':  # payload 回傳第0格表示為馬達回傳功能、後面表示馬達數值
        # value = msg.payload[1]
        motor_position = msg.payload[1]
    else:
        motor_position = 0

# @app.route('/MOTOR_GET_MOOTER_VALUE/<VAL>', methods=['POST'])
# def MOTOR_GET_MOOTER_VALUE(VAL):
#     print(VAL)
#     # MQRR SUB接值
#     return "3000"


client = mqtt.Client()

# 設定連線的動作
client.on_connect = on_connect

# 設定接收訊息的動作
client.on_message = on_message

# 設定連線資訊(IP, Port, 連線時間)
client.connect("140.121.198.196", 1883, 60)

# 開始連線，執行設定的動作和處理重新連線問題
# 也可以手動使用其他loop函式來進行連接


# 單一LED控制
@app.route('/LED/<VAL>')
def LED(VAL):  # 對單一LED控制
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    # client.username_pw_set("lab505", "lab505")
    if VAL == "1":           # 開啟LED
        status = "1"
    elif VAL == "0":  # 關閉LED
        status = "0"
    else:  # 自定義
        status = "3"
    publish.single(topic, status, qos=1, hostname=host)  # 發布訊息 qos
    return""


@app.route('/LED_CONTROL/<VAL>')
def LED_CONTROL(VAL):  # 對單一LED控制
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    led_value = "@" + VAL
    publish.single(topic, led_value, qos=1, hostname=host)  # 發布訊息 qos
    return""


@app.route('/MOTOR_READ/<VAL>')
def MOTOR_RITE(VAL):  # 對馬達讀取
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    read_motor = VAL
    publish.single(topic, read_motor, qos=1, hostname=host)  # 發布訊息 qos
    return""


@app.route('/MOTOR_WRITE/<VAL>')
def MOTOR_WRITE(VAL):  # 對馬達寫入
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    motor_write = VAL
    publish.single(topic, motor_write, qos=1, hostname=host)  # 發布訊息 qos
    return""


@app.route('/CHART')  # 路由
def CHART():
    # return render_template('CHART.html')
    return render_template('show.html')


if __name__ == '__main__':  # 程式進入點
    app.config['TEMPLATES_AUTO_RELOAD'] = True  # 當template有修改會自動更新
    app.run(host='0.0.0.0', debug=False, threaded=True)  # 將web server啟動

client.loop_forever()  # 放最後一行!!
