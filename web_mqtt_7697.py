import pymysql
import paho.mqtt.client as mqtt
import datetime
import demjson

# 當地端程式連線伺服器得到回應時，要做的動作
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc)) # rc 為狀態碼 0為連接上
    client.subscribe("NTOU")

def on_message(client, userdata, msg):
    print(msg.topic+" " + ":" + str(msg.payload, encoding = "utf-8"))
    datetime_str = datetime.datetime.today().strftime("%Y/%m/%d %H:%M:%S")# 獲得當地時間
    message =demjson.decode(str(msg.payload, encoding = "utf-8"))
    db = pymysql.connect("localhost","root","lab505","ntou")
    cursor = db.cursor()
    cursor.execute("select * from sensor")
    db.commit()
    results = cursor.fetchall() #return 多條rows, null則返回0
    db.close()
    KEY=0
    if len(results)==0:
        KEY=1
    else:
        KEY=results[len(results)-1][0]+1 #key是計算在資料庫第幾筆,因為資料庫index = 1開始排序，所以+1
    print("KEY : ",KEY)
    db = pymysql.connect("localhost", "root", "lab505", "ntou")
    cursor = db.cursor()
    cursor.execute("insert into sensor(NO,TEMP,HUMID,DATETIME) VALUES("+str(KEY)+",'"+str(message['temp'])+"','"+str(message['humid'])+"','"+datetime_str+"')")
    db.commit()
    db.close()

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
