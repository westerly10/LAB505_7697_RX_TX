#include <LWiFi.h>
#include <PubSubClient.h>
#define MQTT_SUB_TOPIC "Jimmy"	// web->7697->master
#define MQTT_SUB_TOPIC_1 "Bill" // master->7697-> web
#define MQTT_SERVER_IP "140.121.198.196"
#define MQTT_SERVER_PORT 1883
#define MQTT_CLIENT_ID "linklt7697_111dsf"

char _lwifi_ssid[] = "Lab505_Router"; //wifi名稱
char _lwifi_pass[] = "lab505lab505";  //wifi名稱

WiFiClient mqttClient;			 //建立wifi用戶端物件
PubSubClient client(mqttClient); //以wifi用戶端物件進行MQTT連線

String s = "";
String motor_position = "";
String ledmode = "";

void callback(char *topic, byte *payload, unsigned int length); //宣告接收訂閱內容的回傳參數

void setup()
{									  //初始配置 只做一次
	Serial.begin(9600);				  //設定7697與電腦溝通的頻率
	Serial1.begin(9600);			  //設定7697與master(RX、TX)溝通的頻率
	pinMode(2, INPUT);				  //外部按鈕中斷
	attachInterrupt(2, TX_STM, HIGH); //高電位觸發 RISING比較好?
	while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED)
	{
		delay(1000);
	}

	Serial.print(WiFi.localIP().toString()); //列印7697分配到的IO

	client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT); //設置MQTT Broker
	client.setCallback(callback);						//設置MQTT訂閱呼叫函數
	connectMQTT();										//連接MQTT Broker
}

void TX_STM()
{
	if ((digitalRead(2) == HIGH))
	{ //當按下按鈕時的測試\\\\\\\\\\];
		Serial.print("TX:send message to stm32. ");
		Serial.write("AAAAAAAAAA\n");	//debug
		Serial1.write("aaaaaaaaaa"); //TX
    delay(500);
    //if(Serial1.available()){
      Serial.println(Serial1.read());      //debug
    // }
								  //RX有無收到訊號
	}
}

void loop()
{ //無限執行區塊
	//delay(5000);
	connectMQTT();		//若有斷線會再度連接動作
	client.loop();		//保持與MQTT Broker的連線 並且更新用戶狀態
	//Serial.write("Tx"); //TX
}
void connectMQTT()
{
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect(MQTT_CLIENT_ID))
		{
			Serial.println("connected");
			client.subscribe(MQTT_SUB_TOPIC_1);
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}
void callback(char *topic, byte *payload, unsigned int length)
{ 
	Serial.print("Input Message arrived [");
	Serial.print(MQTT_SUB_TOPIC_1);
	Serial.print("] ");

  if(payload[0] == 'r'){
    //Serial.print(payload);//debug 
    //Serial1.write(payload);//TX
    //接收回傳馬達距離值 再MQTT OUT給網頁端
    int motor_position = Serial1.read();
    //Serial.print("Position：%d", motor_position);
  }
  else if(payload[0] == 'w'){//送資料給Master
    //Serial.print(payload);//debug 
    //Serial1.write(payload);//TX
  }
  else if(payload[0] == '@'){//改變LED
    //Serial.print(payload);//debug 
    //Serial1.write(payload);//TX
  }
}
