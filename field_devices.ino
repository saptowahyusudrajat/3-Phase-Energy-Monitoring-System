#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <PZEM004Tv30.h> // https://github.com/mandulaj/PZEM-004T-v30
#include <SoftwareSerial.h>

#include <ModbusIP_ESP8266.h>  // Modbus library
// ModbusIP object
ModbusIP mb;

#define Device_Name "3 Phase Power Monitoring PT. BYI 2024"
#define configWifi D0  // GPIO16 - D0 Config WiFi
#define PZEM_RX_PIN D6 // GPIO2  - D4 ESP8266
#define PZEM_TX_PIN D5 // GPIO0  - D3 ESP8266

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUM_PZEMS 3
PZEM004Tv30 pzems[NUM_PZEMS];
SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);

WiFiManager wifiManager;

float
    L1_voltage = 0,
    L1_current = 0,
    L1_power = 0,
    L1_energy = 0,
    L1_frequency = 0,
    L1_pf = 0,
    L2_voltage = 0,
    L2_current = 0,
    L2_power = 0,
    L2_energy = 0,
    L2_frequency = 0,
    L2_pf = 0,
    L3_voltage = 0,
    L3_current = 0,
    L3_power = 0,
    L3_energy = 0,
    L3_frequency = 0,
    L3_pf = 0;

unsigned long time1 = 0;
unsigned long time2 = 0;

unsigned long previousMillis = 0; // will store last time LED was updated

void ReadData()
{
  L1_voltage = pzems[0].voltage();
  L1_current = pzems[0].current();
  L1_power = pzems[0].power();
  L1_frequency = pzems[0].frequency();
  L1_pf = pzems[0].pf();
  L1_energy = pzems[0].energy();

  L2_voltage = pzems[1].voltage();
  L2_current = pzems[1].current();
  L2_power = pzems[1].power();
  L2_frequency = pzems[1].frequency();
  L2_pf = pzems[1].pf();
  L2_energy = pzems[1].energy();

  L3_voltage = pzems[2].voltage();
  L3_current = pzems[2].current();
  L3_power = pzems[2].power();
  L3_frequency = pzems[2].frequency();
  L3_pf = pzems[2].pf();
  L3_energy = pzems[2].energy();



}


//void sendNodeRed() {
//  if ((unsigned long)(millis() - time2) > 50) {
//    // Check for NaN and set to 0 if true for all L3 variables
//    if (isnan(L3_voltage)) L3_voltage = 0;
//    if (isnan(L3_current)) L3_current = 0;
//    if (isnan(L3_power)) L3_power = 0;
//    if (isnan(L3_frequency)) L3_frequency = 0;
//    if (isnan(L3_pf)) L3_pf = 0;
//    if (isnan(L3_energy)) L3_energy = 0;
//
//    // Send L3_voltage
//    int v_int = int(L3_voltage);
//    int v_dec = int((L3_voltage - v_int) * 100);
//    mb.Hreg(0, v_int);
//    mb.Hreg(1, v_dec);
//
//    // Send L3_current
//    int c_int = int(L3_current);
//    int c_dec = int((L3_current - c_int) * 100);
//    mb.Hreg(2, c_int);
//    mb.Hreg(3, c_dec);
//
//    // Send L3_power
//    int p_int = int(L3_power);
//    int p_dec = int((L3_power - p_int) * 100);
//    mb.Hreg(4, p_int);
//    mb.Hreg(5, p_dec);
//
//    // Send L3_energy
//    int e_int = int(L3_energy);
//    int e_dec = int((L3_energy - e_int) * 1000);
//    mb.Hreg(6, e_int);
//    mb.Hreg(7, e_dec);
//
//    // Send L3_frequency
//    int f_int = int(L3_frequency);
//    int f_dec = int((L3_frequency - f_int) * 10);
//    mb.Hreg(8, f_int);
//    mb.Hreg(9, f_dec);
//
//    // Send L3_power factor (pf)
//    int pf_int = int(L3_pf);
//    int pf_dec = int((L3_pf - pf_int) * 100);
//    mb.Hreg(10, pf_int);
//    mb.Hreg(11, pf_dec);
//
//    // Call once inside loop() to handle Modbus tasks
//    mb.task();
//
//    // Update the time for the next interval
//    time2 = millis();
//  }
//}


void sendNodeRed() {
  if ((unsigned long)(millis() - time2) > 50) {
    // Check for NaN and set to 0 if true for all L1 variables
    if (isnan(L1_voltage)) L1_voltage = 0;
    if (isnan(L1_current)) L1_current = 0;
    if (isnan(L1_power)) L1_power = 0;
    if (isnan(L1_frequency)) L1_frequency = 0;
    if (isnan(L1_pf)) L1_pf = 0;
    if (isnan(L1_energy)) L1_energy = 0;

    // Check for NaN and set to 0 if true for all L2 variables
    if (isnan(L2_voltage)) L2_voltage = 0;
    if (isnan(L2_current)) L2_current = 0;
    if (isnan(L2_power)) L2_power = 0;
    if (isnan(L2_frequency)) L2_frequency = 0;
    if (isnan(L2_pf)) L2_pf = 0;
    if (isnan(L2_energy)) L2_energy = 0;

    // Check for NaN and set to 0 if true for all L3 variables
    if (isnan(L3_voltage)) L3_voltage = 0;
    if (isnan(L3_current)) L3_current = 0;
    if (isnan(L3_power)) L3_power = 0;
    if (isnan(L3_frequency)) L3_frequency = 0;
    if (isnan(L3_pf)) L3_pf = 0;
    if (isnan(L3_energy)) L3_energy = 0;

    // Send L1_voltage
    int v1_int = int(L1_voltage);
    int v1_dec = int((L1_voltage - v1_int) * 100);
    mb.Hreg(0, v1_int);
    mb.Hreg(1, v1_dec);

    // Send L1_current
    int c1_int = int(L1_current);
    int c1_dec = int((L1_current - c1_int) * 100);
    mb.Hreg(2, c1_int);
    mb.Hreg(3, c1_dec);

    // Send L1_power
    int p1_int = int(L1_power);
    int p1_dec = int((L1_power - p1_int) * 100);
    mb.Hreg(4, p1_int);
    mb.Hreg(5, p1_dec);

    // Send L1_energy
    int e1_int = int(L1_energy);
    int e1_dec = int((L1_energy - e1_int) * 1000);
    mb.Hreg(6, e1_int);
    mb.Hreg(7, e1_dec);

    // Send L1_frequency
    int f1_int = int(L1_frequency);
    int f1_dec = int((L1_frequency - f1_int) * 10);
    mb.Hreg(8, f1_int);
    mb.Hreg(9, f1_dec);

    // Send L1_power factor (pf)
    int pf1_int = int(L1_pf);
    int pf1_dec = int((L1_pf - pf1_int) * 100);
    mb.Hreg(10, pf1_int);
    mb.Hreg(11, pf1_dec);

    // Send L2_voltage
    int v2_int = int(L2_voltage);
    int v2_dec = int((L2_voltage - v2_int) * 100);
    mb.Hreg(12, v2_int);
    mb.Hreg(13, v2_dec);

    // Send L2_current
    int c2_int = int(L2_current);
    int c2_dec = int((L2_current - c2_int) * 100);
    mb.Hreg(14, c2_int);
    mb.Hreg(15, c2_dec);

    // Send L2_power
    int p2_int = int(L2_power);
    int p2_dec = int((L2_power - p2_int) * 100);
    mb.Hreg(16, p2_int);
    mb.Hreg(17, p2_dec);

    // Send L2_energy
    int e2_int = int(L2_energy);
    int e2_dec = int((L2_energy - e2_int) * 1000);
    mb.Hreg(18, e2_int);
    mb.Hreg(19, e2_dec);

    // Send L2_frequency
    int f2_int = int(L2_frequency);
    int f2_dec = int((L2_frequency - f2_int) * 10);
    mb.Hreg(20, f2_int);
    mb.Hreg(21, f2_dec);

    // Send L2_power factor (pf)
    int pf2_int = int(L2_pf);
    int pf2_dec = int((L2_pf - pf2_int) * 100);
    mb.Hreg(22, pf2_int);
    mb.Hreg(23, pf2_dec);

    // Send L3_voltage
    int v3_int = int(L3_voltage);
    int v3_dec = int((L3_voltage - v3_int) * 100);
    mb.Hreg(24, v3_int);
    mb.Hreg(25, v3_dec);

    // Send L3_current
    int c3_int = int(L3_current);
    int c3_dec = int((L3_current - c3_int) * 100);
    mb.Hreg(26, c3_int);
    mb.Hreg(27, c3_dec);

    // Send L3_power
    int p3_int = int(L3_power);
    int p3_dec = int((L3_power - p3_int) * 100);
    mb.Hreg(28, p3_int);
    mb.Hreg(29, p3_dec);

    // Send L3_energy
    int e3_int = int(L3_energy);
    int e3_dec = int((L3_energy - e3_int) * 1000);
    mb.Hreg(30, e3_int);
    mb.Hreg(31, e3_dec);

    // Send L3_frequency
    int f3_int = int(L3_frequency);
    int f3_dec = int((L3_frequency - f3_int) * 10);
    mb.Hreg(32, f3_int);
    mb.Hreg(33, f3_dec);

    // Send L3_power factor (pf)
    int pf3_int = int(L3_pf);
    int pf3_dec = int((L3_pf - pf3_int) * 100);
    mb.Hreg(34, pf3_int);
    mb.Hreg(35, pf3_dec);

    // Call once inside loop() to handle Modbus tasks
    mb.task();

    // Update the time for the next interval
    time2 = millis();
  }
}



void OLED096()
{
  if ((unsigned long)(millis() - time1) > 4000 && (unsigned long)(millis() - time1) < 8000 )
  {
    if (isnan(L1_voltage))
    {
      display.clearDisplay();
      display.setTextSize(2);
      // display.setTextColor(SSD1306_WHITE);  // Draw white text
      display.setCursor(8, 0); // Start at top-left corner
      display.print("    R   ");
      display.setCursor(0, 20);
      display.print("    NOT   ");
      display.setCursor(0, 40);
      display.print(" CONNECTED");
      display.display();
    }
    else
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE); // Draw white text
      display.setCursor(8, 0);             // Start at top-left corner
      display.print("    R   ");
      display.setTextSize(2);
      display.setCursor(0, 17);
      display.print(L1_power);
      display.print("W");
      display.setTextSize(1); // Normal 1:1 pixel scale
      display.setCursor(0, 35);
      display.print(L1_voltage);
      display.print("V / ");
      display.print(L1_current);
      display.print("A");
      display.setCursor(0, 45);
      display.print(L1_frequency, 1);
      display.println("Hz");
      display.print("Power factor: ");
      display.print(L1_pf);
//      display.print("/");
//      display.print(L1_energy);
//      display.print("kWh");
      display.display();
    }
    
  }
  if ((unsigned long)(millis() - time1) > 8000 && (unsigned long)(millis() - time1) < 12000 )
  {
    if (isnan(L2_voltage))
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(8, 0); // Start at top-left corner
      display.print("    S   ");
      display.setCursor(0, 20);
      display.print("    NOT   ");
      display.setCursor(0, 40);
      display.print(" CONNECTED");
      display.display();
    }
    else
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(8, 0); // Start at top-left corner
      display.print("    S   ");
      display.setTextSize(2);
      display.setCursor(0, 17);
      display.print(L2_power);
      display.print("W");
      display.setTextSize(1); // Normal 1:1 pixel scale
      display.setCursor(0, 35);
      display.print(L2_voltage);
      display.print("V / ");
      display.print(L2_current);
      display.print("A");
      display.setCursor(0, 45);
      display.print(L2_frequency, 1);
      display.println("Hz");
      display.print("Power factor: ");
      display.print(L2_pf);
//      display.print("/");
//      display.print(L2_energy);
//      display.print("kWh");
      display.display();
    }
    
  }
  if ((unsigned long)(millis() - time1) > 12000 && (unsigned long)(millis() - time1) < 16000 )
  {
    if (isnan(L3_voltage))
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(8, 0); // Start at top-left corner
      display.print("    T   ");
      display.setCursor(0, 20);
      display.print("    NOT   ");
      display.setCursor(0, 40);
      display.print(" CONNECTED");
      display.display();
    }
    else
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(8, 0); // Start at top-left corner
      display.print("    T   ");
      display.setTextSize(2);
      display.setCursor(0, 17);
      display.print(L3_power);
      display.print("W");
      display.setTextSize(1); // Normal 1:1 pixel scale
      display.setCursor(0, 35);
      display.print(L3_voltage);
      display.print("V / ");
      display.print(L3_current);
      display.print("A");
      display.setCursor(0, 45);
      display.print(L3_frequency, 1);
      display.println("Hz");
      display.print("Power factor: ");
      display.print(L3_pf);
//      display.print("/");
//      display.print(L3_energy); //L3_energy
//      display.print("kWh");
      display.display();
    }
    
  }

if ((unsigned long)(millis() - time1) > 16000 && (unsigned long)(millis() - time1) < 16500){
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("host@modbus:~$ ip ");
      display.println(".");
      display.println(".");
      display.println(".");
      display.println(".");
      display.println(".");
      display.print(WiFi.localIP());
      display.display();
  time1 = millis();
  }


}
// end hienthiLCD

void SerialMonitor()
{
  // Check if the data is valid
  if (isnan(L1_voltage))
  {
    Serial.println("============================ ERROR READING L1 ============================");
  }
  else
  {
    Serial.print("V1: ");
    Serial.print(L1_voltage, 2);
    Serial.print("V | ");
    Serial.print("A1: ");
    Serial.print(L1_current, 2);
    Serial.print("A | ");
    Serial.print("P1: ");
    Serial.print(L1_power, 2);
    Serial.print("W | ");
    Serial.print("E1: ");
    Serial.print(L1_energy, 2);
    Serial.print("kWh | ");
    Serial.print("F1: ");
    Serial.print(L1_frequency, 2);
    Serial.print("Hz | ");
    Serial.print("PF1: ");
    Serial.println(L1_pf, 2);
  }
  if (isnan(L2_voltage))
  {
    Serial.println("============================ ERROR READING L2 ============================");
  }
  else
  {
    Serial.print("V2: ");
    Serial.print(L2_voltage, 2);
    Serial.print("V | ");
    Serial.print("A2: ");
    Serial.print(L2_current, 2);
    Serial.print("A | ");
    Serial.print("P2: ");
    Serial.print(L2_power, 2);
    Serial.print("W | ");
    Serial.print("E2: ");
    Serial.print(L2_energy, 2);
    Serial.print("kWh | ");
    Serial.print("F2: ");
    Serial.print(L2_frequency, 2);
    Serial.print("Hz | ");
    Serial.print("PF2: ");
    Serial.println(L2_pf, 2);
  }
  if (isnan(L3_voltage))
  {
    Serial.println("============================ ERROR READING L3 ============================");
  }
  else
  {
    Serial.print("V3: ");
    Serial.print(L3_voltage, 2);
    Serial.print("V | ");
    Serial.print("A3: ");
    Serial.print(L3_current, 2);
    Serial.print("A | ");
    Serial.print("P3: ");
    Serial.print(L3_power, 2);
    Serial.print("W | ");
    Serial.print("E3: ");
    Serial.print(L3_energy, 2);
    Serial.print("kWh | ");
    Serial.print("F3: ");
    Serial.print(L3_frequency, 2);
    Serial.print("Hz | ");
    Serial.print("PF3: ");
    Serial.println(L3_pf, 2);
  }

  Serial.println();
} // end SerialMonitor


int buttonState = 0;               // Trạng thái của nút SW1
int previousButtonState = 1;       // Trạng thái trước đó của nút SW1
unsigned long buttonPressTime = 0; // Thời điểm nút SW1 được nhấn
void ButtonScan()
{
  buttonState = digitalRead(configWifi); // Đọc trạng thái của nút SW1

  // Kiểm tra nếu nút SW1 được nhấn
  if (buttonState != previousButtonState)
  {
    if (buttonState == LOW)
    {
      buttonPressTime = millis(); // Lưu thời điểm nút SW1 được nhấn
    }
    else
    {
      unsigned long buttonHoldTime = millis() - buttonPressTime; // Tính thời gian nút SW1 được nhấn giữ

      // Kiểm tra nếu thời gian giữ nút SW1 là 5 giây (5000ms)
      if (buttonHoldTime >= 3000)
      {
        // thực hiện lệnh nhấn giữ 5s tại đây
        WiFiManager wifiManager;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println(" WIFI NEW ");
        display.display();

        Serial.println("Erasing Config, restarting");
        wifiManager.resetSettings();
        wifiManager.autoConnect(Device_Name);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(" COMPLETE ");
        display.display();
        delay(1000);
        // ESP.restart();
      }
    }
  }
  previousButtonState = buttonState; // Lưu trạng thái trước đó của nút SW1
}

void setup()
{
  Serial.begin(115200);
  pinMode(configWifi, INPUT);


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  wifiManager.setConfigPortalTimeout(60); // 1 minute
  if (!wifiManager.autoConnect(Device_Name))
  {
    Serial.println("failed to connect and hit timeout");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("mtvgsapto@gmail.com");
    display.display();
    delay(3000);
    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Wifi connecting to ");
  display.println(WiFi.SSID().c_str());

 

  display.println("\nConnecting");
  display.display();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    display.print(".");
    display.display();
  }

  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);

  display.println("Wifi Connected!");
  display.print("IP:");
  display.println(WiFi.localIP());
  display.display();
  delay(4000);

  display.clearDisplay();
  display.display();

  // For each PZEM, initialize it
  for (int i = 0; i < NUM_PZEMS; i++)
  {
    // Initialize the PZEMs with Software Serial
    pzems[i] = PZEM004Tv30(pzemSWSerial, 0x10 + i);
  }


// Initialize Modbus server
  mb.server();

  // Configure holding registers for dummy data
  mb.addHreg(0);  // Voltage integer part
  mb.addHreg(1);  // Voltage decimal part
  mb.addHreg(2);  // Current integer part
  mb.addHreg(3);  // Current decimal part
  mb.addHreg(4);  // Power integer part
  mb.addHreg(5);  // Power decimal part
  mb.addHreg(6);  // Energy integer part
  mb.addHreg(7);  // Energy decimal part
  mb.addHreg(8);  // Frequency integer part
  mb.addHreg(9);  // Frequency decimal part
  mb.addHreg(10);  // Power factor integer part
  mb.addHreg(11);  // Power factor decimal part


  mb.addHreg(12);  // Voltage integer part
  mb.addHreg(13);  // Voltage decimal part
  mb.addHreg(14);  // Current integer part
  mb.addHreg(15);  // Current decimal part
  mb.addHreg(16);  // Power integer part
  mb.addHreg(17);  // Power decimal part
  mb.addHreg(18);  // Energy integer part
  mb.addHreg(19);  // Energy decimal part
  mb.addHreg(20);  // Frequency integer part
  mb.addHreg(21);  // Frequency decimal part
  mb.addHreg(22);  // Power factor integer part
  mb.addHreg(23);  // Power factor decimal part

  mb.addHreg(24);  // Voltage integer part
  mb.addHreg(25);  // Voltage decimal part
  mb.addHreg(26);  // Current integer part
  mb.addHreg(27);  // Current decimal part
  mb.addHreg(28);  // Power integer part
  mb.addHreg(29);  // Power decimal part
  mb.addHreg(30);  // Energy integer part
  mb.addHreg(31);  // Energy decimal part
  mb.addHreg(32);  // Frequency integer part
  mb.addHreg(33);  // Frequency decimal part
  mb.addHreg(34);  // Power factor integer part
  mb.addHreg(35);  // Power factor decimal part

} // end setup

void loop()
{

  ButtonScan();
  ReadData();
  SerialMonitor();
  OLED096(); // Select if using OLED 0.96 display
  sendNodeRed();
  

} // end loop
