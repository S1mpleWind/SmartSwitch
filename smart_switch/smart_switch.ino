#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>
#include <Servo.h>

Servo myservo;//定义舵机

char auth[] = "*********";  //点灯Key
char ssid[] = "*********";      //wifi名称
char pswd[] = "*********";     //wifi密码

// 新建组件对象
BlinkerButton Button1("test");


int counter = 0;

void miotPowerState(const String & state)//电源类操作
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
      
        myservo.write(120);
        delay(10);
        myservo.write(170);//收到“on”的指令后舵机旋转度
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();//反馈状态
        delay(500);//延时0.5秒
        myservo.write(85);//舵机归零，回到垂直状态
         
        
    }
    else if (state == BLINKER_CMD_OFF) {
      
         myservo.write(60); 
         delay(10);
         myservo.write(10); //舵机偏转70°
         BlinkerMIOT.powerState("off");
         BlinkerMIOT.print();
         delay(500);
         myservo.write(85);
         
      
    }
}

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    if (state=="on") 
    {
        myservo.write(180);//收到“on”的指令后舵机旋转150
        delay(500);//延时
        myservo.write(100);//舵机归零垂直
         
        
    } else if(state=="press"||state=="tap")
    {
       myservo.write(0);//长按开关按键后舵机旋转30
       delay(500);//延时
       myservo.write(100);//舵机归零垂直
    }
        
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
 
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    // 初始化舵机
    myservo.attach(2);//舵机的IO口，nodemcu的D4口
    myservo.write(90);//上电时舵机归零垂直
  BlinkerMIOT.attachPowerState(miotPowerState);
  
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);

    Button1.attach(button1_callback);
}

void loop() {
    Blinker.run();
}
