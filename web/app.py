from flask import Flask, render_template, request, jsonify
# render_template 內建函數，渲染模板，它需要一個『模板』來提供渲染的格式
import pymysql
import paho.mqtt.publish as publish
import datetime
import demjson
app = Flask(__name__)  # 沒有其它的用意，如此flask才會知道你的root在何處!


# 單一LED控制
@app.route('/LED/<VAL>')
def LED(VAL):  # 對單一LED控制
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
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
    publish.single(topic, motor_cycle, qos=1, hostname=host)  # 發布訊息 qos
    return""


@app.route('/MOTOR_GET_MOOTER_VALUE/<VAL>')
def MOTOR_RITE(VAL):
    print(VAL)
    return "This is Home Page"


@app.route('/MOTOR_WRITE/<VAL>')
def MOTOR_WRITE(VAL):  # 對馬達寫入
    print(VAL)
    host = "140.121.198.196"    # Broker's IP
    topic = "NTOU_1"           # Topic
    motor_cycle = VAL
    publish.single(topic, motor_cycle, qos=1, hostname=host)  # 發布訊息 qos
    return""


@app.route('/CHART')  # 路由
def CHART():
    # return render_template('CHART.html')
    return render_template('show.html')


if __name__ == '__main__':  # 程式進入點
    app.config['TEMPLATES_AUTO_RELOAD'] = True  # 當template有修改會自動更新
    app.run(host='0.0.0.0', debug=False, threaded=True)  # 將web server啟動
