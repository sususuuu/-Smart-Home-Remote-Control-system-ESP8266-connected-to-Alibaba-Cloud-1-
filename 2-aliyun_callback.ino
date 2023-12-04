// 引入 wifi 模块，并实例化，不同的芯片这里的依赖可能不同
#include <ESP8266WiFi.h>
static WiFiClient espClient;

// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>

// 设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY "a15OpX2Imz4"
#define DEVICE_NAME "test_1"
#define DEVICE_SECRET "d4a3d967d601e9674d547af261e9ca1d"
#define REGION_ID "cn-shanghai"

// 设置 wifi 信息
#define WIFI_SSID "206"
#define WIFI_PASSWD "12345678"

void setup()
{
    // set the digital pin as output:
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);
    
    Serial.begin(115200);
    
    // 初始化 wifi
    wifiInit(WIFI_SSID, WIFI_PASSWD);
    
    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 ledSwitchCallback
    // PowerSwitch 是在设备产品中定义的物联网模型的 id
    AliyunIoTSDK::bindData("LEDSwitch", LEDSwitchCallback);
}

void loop()
{
    AliyunIoTSDK::loop();
}

// 初始化 wifi 连接
void wifiInit(const char *ssid, const char *passphrase)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passphrase);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
    Serial.println("Connected to AP");
}

// LED状态修改的回调函数
void LEDSwitchCallback(JsonVariant p)
{
    int LEDSwitch = p["LEDSwitch"];
    // 变更LED状态
    digitalWrite(LED_BUILTIN, LEDSwitch);
}

// test的回调函数
void deviceNameCallback(JsonVariant p)
{
    int deviceName = p["deviceName"];
    Serial.println(deviceName);
}
