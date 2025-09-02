
// Include libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> 

//USER DEFINE
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixel
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_SDA 0
#define OLED_SCL 1
#define BTN_UP 22
#define BTN_DOWN 23
#define BTN_RIGHT 24
#define BTN_LEFT 25
#define LED_RGB 13
#define NUM_LED 8
#define GPS_UART_TX 4
#define GPS_UART_RX 5
#define GPS_EN 18
#define GPS_RST 12
#define GPS_PPS 19
#define TEMP_SENSOR 26
#define BUZZER 21

// Function and struct declaration
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_NeoPixel strip(NUM_LED,LED_RGB,NEO_GRB+NEO_KHZ800);
SFE_UBLOX_GNSS GPS;

void setupHardware(); // init display and other hardware, wait for GPS FIX and other stuff
void showWaitStart(); // show "Press Button to Start"
void showSpeedMenu();
void showDistanceMenu();
void showTimeMenu();
void showTempMenu();
void showLoadingLED(); // show blue circling dot while user press start
void mainMenu(); // main logic of the program
void showLEDbasedOnSpeed();
void updateMeasure(); // update all measure time, speed and distance
void updateSpeed();
void updateTime();
void updateTemperature();
void updateDistance();
float haversine(float lat1, float lat2, float lon1, float lon2);


//Interrupt Subroutine
void BTN_RIGHT_ISR();
void BTN_UP_ISR();
void BTN_DOWN_ISR();
// user variable
int currentMenuWindow=0;
int state=0; // for the state machine
int showLED=1;
int currentLed=0; // for the loading LED maybe also when GPS lose fix
unsigned long previousMillis=0;
unsigned int startTime=0;
uint8_t displayH=0;
uint8_t displayM=0;
uint8_t displayS=0;
float displaySpeed=0;
float prevLat=0;
float prevLon=0;
float totalDistance=0; // total distance travelled in KM
float displayTemp=0;


void setup() {

  Serial.begin(115200);
  while(!Serial);
  
 
  setupHardware();
}

void loop() {
  
  switch(state)
  {
    case 0: // initialization state and wait for GPS fix 
      showLoadingLED();
      Serial.printf("GPS FIX TYPE : %d\n",GPS.getFixType());
      if(GPS.getFixType() == 3) // FIX=4 GPS+DEAD reckoning
      {
        state = 1;
        Serial.println("GPS has FIX");
      }
      break;
    case 1: // hardware init, wait for user input
      showWaitStart();
      showLoadingLED();
      break;
    case 2: // user input and main program running
      mainMenu(); // to show various display
      //updateMeasure(); // update speed and LED accordignly and distance travelled
      break;
    default: // we should never get here
      break;
  }

}

void setupHardware()
{
  // setup I2C bus
  Wire.setSDA(OLED_SDA);
  Wire.setSCL(OLED_SCL);
  Wire.begin();
  

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
   display.clearDisplay();

  // Imposta il colore e la dimensione
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); // Piccolo per prima riga
  display.setCursor(0, 0);
  display.println("Inizializzazione");

  display.setTextSize(2); // Grande per seconda riga
  display.setCursor(5, 35);
  display.println("Hardware");

  display.display();

  // Simula un piccolo effetto di "caricamento"
  for (int i = 0; i < 3; i++) {
    delay(400);
    display.print(".");
    display.display();
  }

  

  //setup BTN pin
  pinMode(BTN_UP,INPUT);
  pinMode(BTN_DOWN,INPUT);
  pinMode(BTN_LEFT,INPUT);
  pinMode(BTN_RIGHT,INPUT);
  attachInterrupt(digitalPinToInterrupt(BTN_RIGHT),BTN_RIGHT_ISR,FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_UP),BTN_UP_ISR,FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_DOWN),BTN_DOWN_ISR,FALLING);


  //setup GPS
  pinMode(GPS_RST,OUTPUT);
  pinMode(GPS_EN,OUTPUT);
  pinMode(GPS_PPS,INPUT);
  digitalWrite(GPS_EN,LOW);
  digitalWrite(GPS_RST,HIGH);
  Serial2.setTX(GPS_UART_TX);
  Serial2.setRX(GPS_UART_RX);
  Serial2.begin(9600);
  GPS.begin(Serial2);
  
  pinMode(TEMP_SENSOR, INPUT);
  //setup RGB LED
  strip.begin();
  strip.show();
  delay(1000); // Pausa prima di proseguire
  
}
void showSpeedMenu()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // Piccolo per prima riga
  display.setCursor(0, 0);
  display.println("Velocita'");
  display.setCursor(0,32);
  display.setTextSize(1);
  // get velocity and convert in kph
  display.printf("%.2f KM/H\n",displaySpeed);
  
  display.display();
}

void showDistanceMenu()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // Piccolo per prima riga
  display.setCursor(0, 0);
  display.println("Distanza");
  display.setCursor(0,32);
  display.setTextSize(1);
  display.printf("%.2f KM\n");
  display.display();
}

void showTimeMenu()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // Piccolo per prima riga
  display.setCursor(0, 0);
  display.println("Tempo");


  // print on screen
  display.setCursor(0, 32);
  display.setTextSize(1);
  display.printf("%d:%d:%d\n",displayH,displayM,displayS);
  display.display();
}

void showTempMenu()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // Piccolo per prima riga
  display.setCursor(0, 0);
  display.println("Temperatura");
  display.setCursor(0,32);
  display.setTextSize(1);
  display.printf("%.1f C\n",displayTemp);
  display.display();
}

void showLoadingLED()
{
  if (showLoadingLED) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 150) {
      previousMillis = currentMillis;

      strip.clear();
      strip.setPixelColor(currentLed, strip.Color(0, 0, 255)); // Blu
      strip.show();

      currentLed++;
      if (currentLed >= NUM_LED) {
        currentLed = 0;
      }
    }
  }
  else
  {
    strip.clear();
    strip.show();
  }
}

void showWaitStart()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); // Piccolo per prima riga
  display.setCursor(0, 0);
  display.println("Press Right To Start Journey");
  display.display();
}

void mainMenu()
{
  Serial.printf("GPS FIX TYPE : %d\n",GPS.getFixType());
  updateMeasure();
  switch(currentMenuWindow)
  {
    case 0:
      showSpeedMenu();
      break;
    case 1:
      showDistanceMenu();
      break;
    case 2:
      showTimeMenu();
      break;
    case 3:
      showTempMenu();
      break;
    default:
      break;
  }
  delay(500);
}

void BTN_RIGHT_ISR()
{ 
    strip.clear();
    strip.show();
    showLED=0;
    // get time of start trip
    uint8_t startHour=GPS.getHour();
    uint8_t startMinute=GPS.getMinute();
    uint8_t startSecond=GPS.getSecond();
    startTime=startHour*3600+startMinute*60+startSecond;
    Serial.printf("Start Time : %d:%d:%d\n",startHour,startMinute,startSecond);
    prevLat=GPS.getLatitude();
    prevLon=GPS.getLongitude();

    Serial.printf("Lat:%f - Lon:%f\n",prevLat,prevLon);
    // need to put starting position for disatance computing later
    state = 2;
}
void setAllLEDs(uint32_t color) {
  for (int i = 0; i < NUM_LED; i++) {
    strip.setPixelColor(i, color);
  }
}
void showLEDbasedOnSpeed()
{
  strip.clear();

  // Example thresholds — adjust for your needs
  if (displaySpeed < 30) {
    // Slow - blue
    setAllLEDs(strip.Color(0, 0, 255));
  } else if (displaySpeed < 60) {
    // Medium - green
    setAllLEDs(strip.Color(0, 255, 0));
  } else if (displaySpeed < 80) {
    // Fast - yellow
    setAllLEDs(strip.Color(255, 255, 0));
  } else {
    // Very fast - red
    setAllLEDs(strip.Color(255, 0, 0));
  }

  strip.show();
}


void updateMeasure()
{
  updateSpeed();
  updateTime();
  updateDistance();
  updateTemperature();
  showLEDbasedOnSpeed();
  
}
void updateSpeed()
{
  // here update speed
  int32_t speed = GPS.getGroundSpeed();
  Serial.printf("Ground Speed %d mm/s\n",speed);
  displaySpeed = (speed/1000)*3.6;
  Serial.printf("Ground Speed %.2f km/h\n",displaySpeed);
  if(displaySpeed > 70)
    tone(BUZZER,440,500);

}
void updateDistance()
{
  float tmpLat=GPS.getLatitude();
  float tmpLon=GPS.getLongitude();
  float partialDist=haversine(tmpLat,prevLat,tmpLon,prevLon);
  totalDistance+= partialDist/1000;

  Serial.printf("Partial Dist:%.2f - Total Dist: %.2f\n",partialDist/1000,totalDistance);

}
void updateTime()
{
  // need to update display[H:M:S] variable
  uint8_t tmpH= GPS.getHour();
  uint8_t tmpM = GPS.getMinute();
  uint8_t tmpS= GPS.getSecond();
  unsigned int tmpTime= tmpH*3600+tmpM*60+tmpS;
  unsigned int diff = tmpTime-startTime;
  Serial.printf("Now time : %d:%d:%d\n",tmpH,tmpM,tmpS);
  displayH=diff/3600;
  displayM=(diff%3600)/60;
  displayS=diff%60;
}


void BTN_UP_ISR()
{
  if(currentMenuWindow <3 )
  {
    currentMenuWindow++;
    delay(200);
  }
}

void BTN_DOWN_ISR()
{
  if(currentMenuWindow >0)
  {
    currentMenuWindow--;
    delay(200);
  }
}

float haversine(float lat1, float lat2, float lon1, float lon2)
{
    const double R = 6371000; // Earth radius in meters
    double dLat = radians(lat2 - lat1);
    double dLon = radians(lon2 - lon1);
    lat1 = radians(lat1);
    lat2 = radians(lat2);

    double a = sin(dLat / 2) * sin(dLat / 2) +
              cos(lat1) * cos(lat2) *
              sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c; // in meters
}

void updateTemperature()
{
  int analogValue = analogRead(TEMP_SENSOR); // 0 to 1023
  float voltage = analogValue * (3.3 / 1023.0); // Convert to voltage (assuming 5V Vcc)

  displayTemp = (voltage - 0.5) * 100.0; // Convert to °C
}
