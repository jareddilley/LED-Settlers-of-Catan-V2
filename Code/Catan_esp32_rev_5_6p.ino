/**
Version 2 LED Catan - 5-6 person board size with desert included in board generation
Inludes additional rules and game aspects and a new resoruce hex (the all recource spot)
Board - 38 Pin ESP32 (Wi-fi and Bluetooth capable)
Author - Jared Dilley
**/

#include <FastLED.h>
#include <TM1637.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define NUM_LEDS 131  // Total num of LEDs
#define spotSize 4  // num of LEDs per hex
#define LED_PIN 14
#define Button 19   // Roll Dice Button 1
#define Button2 15  // Roll Dice Button 2   
#define Mode 5      // Switch for game mode
int ButtonState;
int ButtonState2;
int brightness = 50;
const int range = 30; // number of hexagons
const int num_ports = 11;  // number of ports
int tracker = 0;
unsigned long now = millis();
int CLK = 1;
int DIO = 3;  
TM1637 tm(CLK,DIO); // TX, RX
Adafruit_SSD1306 display1(-1);  // SCL P22, SDA P21
Adafruit_SSD1306 display2(-1);
int offset = 8;
int offset2 = 7;
int brick = 0; 
int wheat = 0; 
int wood = 0; 
int rock = 0; 
int sheep = 0;
int ModeState;
int rolls[11] = {0,0,0,0,0,0,0,0,0,0,0};  // tracker for the dice rolls 2-12
int button_display[12] = {0,1,2,3,4,14,15,25,26,27,28,29};
int button2_display[12] = {26,27,28,29,21,20,9,8,0,1,2,3};



// Brick::Red, Boulder::Aqua, Logs::Green, Hay::Orange, Sheep::white, Desert::Black
uint32_t resource[range];
uint32_t resource_rand[range] = {CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Aqua,CRGB::Aqua,CRGB::Aqua,CRGB::Aqua,CRGB::Aqua,
CRGB::Green,CRGB::Green,CRGB::Green,CRGB::Green,CRGB::Green,CRGB::Green,CRGB::Orange,CRGB::Orange,CRGB::Orange,CRGB::Orange,CRGB::Orange,CRGB::Orange,
CRGB::White,CRGB::White,CRGB::White,CRGB::White,CRGB::White,CRGB::White,CRGB::Black,CRGB::Purple};
uint32_t resource_spread[range] = {3,4,2,3,5,4,3,5,1,2,4,1,CRGB::Black,2,4,1,5,3,5,CRGB::Purple,1,3,2,1,4,2,5,3,4,5};
uint32_t five_spot[2] = {CRGB::Red,CRGB::Aqua};
uint32_t six_spot[3] = {CRGB::Green,CRGB::Orange,CRGB::White};
CRGB all[5] = {CRGB::Red,CRGB::Green,CRGB::Orange,CRGB::Aqua,CRGB::White};
int values[range] = {11,12,9,8,9,2,4,10,6,12,11,8,5,6,3,5,12,4,2,9,4,2,3,10,8,10,3,11,6,5}; // values of all 30 spots
int spotValues[range];
CRGB leds[NUM_LEDS];
CRGB currentColors[range];

// Function to swap two elements in an array
void swap_uint(uint32_t  *a, uint32_t  *b) {
    uint32_t  temp = *a;
    *a = *b;
    *b = temp;
}

// Function to the elements in a given array
void shuffle(uint32_t arr[], int size){
  for (int i = size - 1; i > 0; i--) {
    // Generate a random index between 0 and i (inclusive)
    int j = random(0,1000) % (i + 1);
    // Swap current element with randomly generated element
    swap_uint(&arr[i], &arr[j]);
  }
}

// Function to find an element in an array and swap it with a random element
void swapWithRandom(CRGB arr[], int size, CRGB target) {
  // Search for the target element in the array
  int found = 0;
  int targetIndex;
  for (int i = 0; i < size; i++) {
    if (arr[i] == target) {
      targetIndex = i; // Store the index of the target element
      found = 1;
      break;
    }
  }
  // If the target element was found, swap it with a random element
  if (found) {
    int randomIndex = random(0,1000) % size;
    CRGB target_spot = currentColors[targetIndex];
    CRGB rand_spot = currentColors[randomIndex];
    for (int i = 0; i < 2; i++) {  
      setSpot(targetIndex + 1, CRGB::Black, false);
      setSpot(randomIndex + 1, CRGB::Black, false);
      FastLED.show();
      delay(250);
      setSpot(targetIndex + 1, target_spot, false);
      setSpot(randomIndex + 1, rand_spot, false);
      FastLED.show();
      delay(250);
    }
    for (int i = 0; i < 2; i++) {  
      setSpot(targetIndex + 1, CRGB::Black, false);
      setSpot(randomIndex + 1, CRGB::Black, false);
      FastLED.show();
      delay(250);
      setSpot(targetIndex + 1, rand_spot, true);
      setSpot(randomIndex + 1, target_spot, true);
      FastLED.show();
      delay(250);
    }
  }
}

// Sets the specified hexagon, with an option to save changes to memory
void setSpot (int spot, CRGB color, boolean save){  
    int currentLED = (spot - 1)*spotSize + num_ports;
    int lastLED = currentLED + spotSize; 
    for (currentLED; currentLED < lastLED; currentLED++) {   
      leds[currentLED] = color;
    }
    if (save){
      currentColors[spot-1] = color; 
    }
}

// Returns True if element in array
boolean arrayIncludeElement(int randArray[], int input, int range) {
  for (int i = 0; i < range; i++) {   
    if (randArray[i] == input) {
      return true;
    }
  }
  return false;
}

// Fuction for rolling the dice and playing animations
void rollDice() {
  // Roll dice
  int roll = random(1,7) + random(1,7);
  // roll = 7; // uncomment for testing 7's
  rolls[roll-2]++;
  tm.init(); // clear
  if (roll < 10){
    tm.display(3,roll);
  } else{
    tm.display(2,1);
    tm.display(3,roll-10);
  }
  // show dice value
  for (int i = 0; i < range; i++) {   
    setSpot(i+1, CRGB::Black, false);
  }
  FastLED.show();  
  
  for (int i = 0; i < roll; i++) {   // rev
    if(ButtonState == LOW){
      setSpot(button_display[i]+1, CRGB::White, false);
    } else if (ButtonState2 == LOW){
      setSpot(button2_display[i]+1, CRGB::White, false);
    }
    FastLED.show();
    delay(100);
  }
  delay(500);
  // show only spots with that roll
  int locations[range];
  int count = 0;
  if (roll == 7){
    updateDisplay(brick, wheat, wood, rock, sheep);
    for (int a = 0; a < 3; a++) {
      for (int i = 0; i < range; i++) {   
        setSpot(i+1, CRGB::Red, false);
      }
      FastLED.show();
      delay(500);
      if (a < 2){
        for (int i = 0; i < range; i++) {   
          setSpot(i+1, CRGB::Black, false);
        }
        FastLED.show();
        delay(500);
      }
    }
    if (ModeState == HIGH){ // randomly swap desert and all piece with a random piece
      swapWithRandom(currentColors, range, CRGB::Black);
      swapWithRandom(currentColors, range, CRGB::Purple);
    } else{ // swap desert and all rec in even spread
      CRGB spot_19 = currentColors[19];
      CRGB spot_12 = currentColors[12];
      for (int i = 0; i < 2; i++) {  
        setSpot(19 + 1, CRGB::Black, false);
        setSpot(12 + 1, CRGB::Black, false);
        FastLED.show();
        delay(250);
        setSpot(19 + 1, spot_19, false);
        setSpot(12 + 1, spot_12, false);
        FastLED.show();
        delay(250);
      }
      for (int i = 0; i < 2; i++) {  
        setSpot(19 + 1, CRGB::Black, false);
        setSpot(12 + 1, CRGB::Black, false);
        FastLED.show();
        delay(250);
        setSpot(19 + 1, spot_12, true);
        setSpot(12 + 1, spot_19, true);
        FastLED.show();
        delay(250);
      }
    }
  } else{
    for (int i = 0; i < range; i++) {   
      if (values[i] == roll){
        setSpot(i+1, currentColors[i], false);
        if (currentColors[i] == all[0]){
          brick++;
        }
        if (currentColors[i] == all[1]){
          wood++;
        }
        if (currentColors[i] == all[2]){
          wheat++;
        }
        if (currentColors[i] == all[3]){
          rock++;
        }
        if (currentColors[i] == all[4]){
          sheep++;
        }
      }
      else{
        setSpot(i+1, CRGB::Black, false);
      }
    }
    updateDisplay(brick, wheat, wood, rock, sheep);
    FastLED.show();
    delay(5000);
  }
  // reset board
  for (int i = 0; i < range; i++) {   
    setSpot(i+1, currentColors[i], true);
  }
  FastLED.show();
}

// Fuction in charge of updating both OLED displays for dice and recouce roll ttracking
void updateDisplay(int brick, int wheat, int wood, int rock, int sheep){
  float maxVal = 0;
  int local = 0;
  
  display2.clearDisplay();
  display2.setCursor(2, 25);
  display2.println("2 3 4 5 6 7 8 9 A B C");
  display2.drawLine(0, 23, 127, 23, WHITE);
  for (int i = 0; i < (sizeof(rolls) / sizeof(rolls[0])); i++) {
    if (rolls[i] > maxVal) {
      maxVal = rolls[i];
    }
  }
  for (int i = 0; i < (sizeof(rolls) / sizeof(rolls[0])); i++) {
    if (rolls[i] > 0) {
      int barTop = 24 - (float(rolls[i])/maxVal)*(24);
      int barHieght = (float(rolls[i])/maxVal)*(24);
      if((barTop + barHieght) != 24)
      {
        barHieght = 24 - barTop;
      }
      display2.fillRoundRect(local, barTop, 8, barHieght, 2, WHITE);
    }
    else{
      display2.fillRoundRect(local, 0, 8, 0, 2, WHITE);
    }
    local += 12;
  }
  display2.display();

  int rec[5] = {brick,wheat,sheep,wood,rock};
  maxVal = 0;
  local = 0;

  display1.clearDisplay();
  display1.setCursor(0+offset, 25);
  display1.println("Br");
  display1.setCursor(25+offset, 25);
  display1.println("Wh");
  display1.setCursor(50+offset, 25);
  display1.println("Sh");
  display1.setCursor(75+offset, 25);
  display1.println("Wo");
  display1.setCursor(100+offset, 25);
  display1.println("Ro");
  display1.drawLine(0, 23, 127, 23, WHITE);
  for (int i = 0; i < (sizeof(rec) / sizeof(rec[0])); i++) {
    if (rec[i] > maxVal) {
      maxVal = rec[i];
    }
  }
  for (int i = 0; i < (sizeof(rec) / sizeof(rec[0])); i++) {
    if (rec[i] > 0) {
      int barTop = 24 - (float(rec[i])/maxVal)*(24);
      int barHieght = (float(rec[i])/maxVal)*(24);
      if((barTop + barHieght) != 24)
      {
        barHieght = 24 - barTop;
      }
      display1.fillRoundRect(local+offset2, barTop, 12, barHieght, 2, WHITE);
    }
    else{
      display1.fillRoundRect(local+offset2, 0, 12, 0, 2, WHITE);
    }
    local += 25;
  }
  display1.display();

}

// Initialize board
void setup() {
  // set up display
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);

  display1.ssd1306_command(SSD1306_SETCONTRAST);
  display1.ssd1306_command(128);  // brightness 0-255
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(WHITE);

  display2.ssd1306_command(SSD1306_SETCONTRAST);
  display2.ssd1306_command(128);  // brightness 0-255
  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  updateDisplay(brick, wheat, wood, rock, sheep);
  // put your setup code here, to run once:
  tm.init();
  // set brightness; 0-7
  tm.set(1);
  // init roll display to "BEEF"
  tm.display(3,0xf);
  tm.display(2,0xe);
  tm.display(1,0xe);
  tm.display(0,8);

  pinMode(Mode, INPUT);
  ModeState = digitalRead(Mode);
  if (ModeState == HIGH){ 
    memcpy(resource, resource_rand, sizeof(resource));
  }
  else{
    memcpy(resource, resource_spread, sizeof(resource));
  }

  pinMode(Button, INPUT);
  pinMode(Button2, INPUT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  // set up random board if selected
  if(ModeState == HIGH){
    int first = random(range);
    int randArray[range] = {first};
    int num = 1;
    while(num < range){
      int input = random(range);
      if (!arrayIncludeElement(randArray, input, num)){
        randArray[num] = input;
        
        num++;
      }
    }
    uint32_t temp_resource[range];
    for (int i = 0; i < range; i++) {
      temp_resource[i] = resource[randArray[i]];
    }
    memcpy(resource, temp_resource, sizeof(resource));
  }else{
    shuffle(five_spot,2);
    shuffle(six_spot,3);
    for (int i = 0; i < range; i++) {   
      if(resource[i] == 1){
        resource[i] = five_spot[0];
      }else if(resource[i] == 2){
        resource[i] = five_spot[1];
      }else if(resource[i] == 3){
        resource[i] = six_spot[0];
      }else if(resource[i] == 4){
        resource[i] = six_spot[1];
      }else if(resource[i] == 5){
        resource[i] = six_spot[2];
      }
    }
  }
  // Animated power up sequence
  for (int i = 0; i < 1; i++) { 
    for (int i = 0; i < range; i++) {   
      setSpot(i + 1, CRGB::Black, false);
    }
    FastLED.show(); 
    delay(250);
    setSpot(11 + 1, resource[11], true);
    setSpot(17 + 1, resource[17], true);
    FastLED.show(); 
    delay(250);
    setSpot(6 + 1, resource[6], true);
    setSpot(7 + 1, resource[7], true);
    setSpot(10 + 1, resource[10], true);
    setSpot(12 + 1, resource[12], true);
    setSpot(18 + 1, resource[18], true);
    setSpot(16 + 1, resource[16], true);
    setSpot(24 + 1, resource[24], true);
    setSpot(23 + 1, resource[23], true);
    FastLED.show(); 
    delay(250);
    setSpot(0 + 1, resource[0], true);
    setSpot(1 + 1, resource[1], true);
    setSpot(2 + 1, resource[2], true);
    setSpot(5 + 1, resource[5], true);
    setSpot(13 + 1, resource[13], true);
    setSpot(15 + 1, resource[15], true);
    setSpot(25 + 1, resource[25], true);
    setSpot(26 + 1, resource[26], true);
    setSpot(27 + 1, resource[27], true);
    setSpot(28 + 1, resource[28], true);
    setSpot(22 + 1, resource[22], true);
    setSpot(19 + 1, resource[19], true);
    setSpot(9 + 1, resource[9], true);
    setSpot(8 + 1, resource[8], true);
    FastLED.show(); 
    delay(250);
    setSpot(3 + 1, resource[3], true);
    setSpot(4 + 1, resource[4], true);
    setSpot(14 + 1, resource[14], true);
    setSpot(20 + 1, resource[20], true);
    setSpot(21 + 1, resource[21], true);
    setSpot(29 + 1, resource[29], true);
    FastLED.show(); 
    delay(250);
    if(ModeState == HIGH){  // set up fully random ports
      uint32_t ports[num_ports] = {CRGB::White,CRGB::Orange,CRGB::Purple,CRGB::Green,CRGB::Purple,CRGB::Red,CRGB::Purple,CRGB::Purple,CRGB::White,CRGB::Purple,CRGB::Aqua};
      shuffle(ports,num_ports);
      for (int i = 0; i < num_ports; i++) {
        leds[i] = (CRGB) ports[i];
      } 
    } else{ // set up fixed 3:1 ports and random recource ports
      uint32_t ports[6] = {CRGB::Red,CRGB::Green,CRGB::Orange,CRGB::Aqua,CRGB::White,CRGB::White};
      shuffle(ports,6);
      leds[0] = (CRGB) ports[0];
      leds[1] = (CRGB) ports[1];
      leds[2] = CRGB::Purple;
      leds[3] = (CRGB) ports[2];
      leds[4] = CRGB::Purple;
      leds[5] = (CRGB) ports[3];
      leds[6] = CRGB::Purple;
      leds[7] = CRGB::Purple;
      leds[8] = (CRGB) ports[4];
      leds[9] = CRGB::Purple;
      leds[10] = (CRGB) ports[5];
    }
    FastLED.show(); 
  }
  // Serial.begin(115200); // higher rate for ESP32
}

// Main loop
void loop() { 
  // Get status of the two buttons
  ButtonState = digitalRead(Button);
  ButtonState2 = digitalRead(Button2);
  if (ButtonState == LOW || ButtonState2 == LOW){ // roll dice when one of the buttons is clicked
    delay(250);
    rollDice();   
  }
}
