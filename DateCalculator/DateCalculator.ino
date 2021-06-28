#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include <PixelArt.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AsyncDelay.h>

// Definitions
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 
#define SCREEN_ADDRESS 0x3C 

RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// EPOCH TIME STARTING DATE
long dayStarted = [Add Your Starting Date]

String totalDays = " Days";

// for clock speed
AsyncDelay tick;
const int FPS = 1000 / 20;
// movement
const int X = 1;

// for dog
int dogPos_x = SCREEN_WIDTH;
int dogPos_y = (display.height() - dog_001_height);
int dog_animation_state = 0;

// for bunny
int bunnyPos_x = SCREEN_WIDTH;
int bunnyPos_y = (display.height() - bunny_000_height);
int bunny_animation_state = 0;

// for falling hearts
#define NUMFLAKES 5
#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

int8_t f, icons[NUMFLAKES][3];


void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  for(f=0; f< NUMFLAKES; f++) {
    
    icons[f][XPOS]   = random(1 - heart_width, display.width());
    icons[f][YPOS]   = -heart_height;
    icons[f][DELTAY] = random(1, 6);
  }
  totalDays = calculateDay() + totalDays;
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  tick.start(FPS, AsyncDelay::MILLIS);
}


void loop() {
  if (tick.isExpired()) {
      drawBunnyAnimation();
      
      if (bunny_animation_state < 5) {
        bunny_animation_state++;
      } else {
        bunny_animation_state = 0;
      }
      
      for(f=0; f< NUMFLAKES; f++) {
        icons[f][YPOS] += icons[f][DELTAY];
        if (icons[f][YPOS] >= display.height()) {
          icons[f][XPOS]   = random(1 - heart_width, display.width());
          icons[f][YPOS]   = -heart_height;
          icons[f][DELTAY] = random(1, 3);
        }
      }
  
      bunnyPos_x = bunnyPos_x - X;
      if (bunnyPos_x < 0 - 2*bunny_001_width) {
        bunnyPos_x = SCREEN_WIDTH + bunny_001_width;
      }
      tick.repeat();
    }
      
}


void drawBunnyAnimation() {
   int w = heart_width;
   int h = heart_height;

   int bh = bunny_000_height;
   int bw = bunny_000_width;

   int bunnyPos_x2 = bunnyPos_x + bw  - 5;
   int bunnyPos_y2 = bunnyPos_y + 2;
   
   switch (bunny_animation_state) {
      case 0:
          display.clearDisplay();
          
          display.drawXBitmap(bunnyPos_x, bunnyPos_y, bunny_000_xbm, bw, bh, 1);
          
          display.drawXBitmap(bunnyPos_x2,bunnyPos_y2, bunny_003_xbm, bw , bh , 1);
          
          drawCentreString(totalDays, 64, 10);
          
          for(f=0; f< NUMFLAKES; f++) {
            display.drawXBitmap(icons[f][XPOS], icons[f][YPOS], heart_xbm, w, h, 1);
          }
          
          display.display();
        break;
        
      case 1:
          display.clearDisplay();
          display.drawXBitmap(bunnyPos_x ,bunnyPos_y, bunny_001_xbm, bw, bh, 1);

          for(f=0; f< NUMFLAKES; f++) {
            display.drawXBitmap(icons[f][XPOS], icons[f][YPOS], heart_xbm, w, h, 1);
          }
          display.drawXBitmap(bunnyPos_x2 ,bunnyPos_y2, bunny_004_xbm, bw, bh, 1);
          
          drawCentreString(totalDays, 64, 10);
          display.display();
        break;
        
       case 2:
          display.clearDisplay();
          display.drawXBitmap(bunnyPos_x, bunnyPos_y, bunny_002_xbm, bw, bh, 1);
          
          for(f=0; f< NUMFLAKES; f++) {
            display.drawXBitmap(icons[f][XPOS], icons[f][YPOS], heart_xbm, w, h, 1);
          }
          
          display.drawXBitmap(bunnyPos_x2,bunnyPos_y2,bunny_000_xbm, bw, bh, 1);
          
          drawCentreString(totalDays, 64, 10);
          display.display();
        break;
        
        case 3:
          display.clearDisplay();
          display.drawXBitmap(bunnyPos_x, bunnyPos_y, bunny_003_xbm, bw, bh, 1);
          for(f=0; f< NUMFLAKES; f++) {
            display.drawXBitmap(icons[f][XPOS], icons[f][YPOS], heart_xbm, w, h, 1);
          }
          display.drawXBitmap(bunnyPos_x2  - 5,bunnyPos_y2, bunny_001_xbm, bw , bh, 1);
          
          drawCentreString(totalDays, 64, 10);
          display.display();
        break;
        
          case 4:
          display.clearDisplay();
          
          display.drawXBitmap(bunnyPos_x, bunnyPos_y, bunny_004_xbm, bw, bh, 1);
          
          // Draw each snowflake:
          for(f=0; f< NUMFLAKES; f++) {
            display.drawXBitmap(icons[f][XPOS], icons[f][YPOS], heart_xbm, w, h, 1);
          }
          display.drawXBitmap(bunnyPos_x2 ,bunnyPos_y2,bunny_002_xbm, bw ,bh , 1);
          
          drawCentreString(totalDays, 64, 10);
          display.display();
        break;
    }
}


// to center string 
void drawCentreString(const String &buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string 
    display.setCursor(x - w / 2, y);
    display.print(buf);
}

String calculateDay(){
    DateTime dt = rtc.now();
    int totalDays = 0;
    totalDays = (dt.unixtime()- dayWeStarted) / 86400L;
    Serial.println("Unix Time: ");
    Serial.print("elapsed ");
    Serial.print(dt.unixtime() - dayWeStarted);
    Serial.print(" seconds/");
    Serial.print((dt.unixtime()- dayWeStarted) / 86400L);
    Serial.println(" days since 2/14/2017");
    return String(totalDays);
}
