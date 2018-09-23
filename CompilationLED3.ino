#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    297
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
byte btn = 12;
#define UPDATES_PER_SECOND 1000

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(9600);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  pinMode(btn, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(1,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(14,INPUT);
}
uint8_t secondWhenCalled;
static uint16_t ind = 0;
static uint8_t fore = 1;
uint8_t colorind = 0;
uint8_t brightness = 255;
uint8_t topbar = 0;
uint8_t bottombar = 0;
byte whichShow = 0;
byte numShows = 5;
byte btnPushed;
void loop()
{
  digitalWrite(1, LOW);
  digitalWrite(3,HIGH);
  int potread = analogRead(14);
  //brightness = potread/4;
  //Serial.println(potread);
  digitalWrite(3,LOW);
  if (whichShow % numShows == 0) {
    rainbow();
  }
  
  else if (whichShow % numShows == 1) {
    Runner();
  }
  else if (whichShow % numShows == 2) {
    FastLED.setBrightness(  48 );
    mood();
    FastLED.setBrightness(  BRIGHTNESS );
  }
  else if (whichShow % numShows == 3) {
    Pong2();
  }
  else if (whichShow % numShows == 4) {
    whiteout();
  }

  btnPushed = digitalRead(btn);
  if (btnPushed == 0) {
    whichShow++;
    ind = 0;
    fore = 1;
    colorind = 0;
    topbar = 85;
    bottombar = 85;
    setBlack();
    delay(500);
    secondWhenCalled = (millis() / 1000) % 60;
  }
}

void setBlack() {
  for (int ii = 0; ii <= NUM_LEDS; ii++) {
    leds[ii] = CRGB::Black;
  }
  FastLED.show();
}

void Runner() {
  byte runnerspeed = 2;
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  leds[ind] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  if (fore == 1) {
    ind+=runnerspeed;
    leds[ind-1] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    leds[ind-2] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    if (ind >= NUM_LEDS - 1) {
      fore = 0;
      colorind = colorind + 80;
    }
  }
  else if (fore == 0) {
    ind-=runnerspeed;
    leds[ind+1] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    leds[ind+2] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    if (ind <= 0) {
      fore = 1;
      colorind = colorind + 80;
    }
  }
}

void Pong() {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  leds[ind] = CRGB::Black;
  if (fore == 1) {
    ind++;
    leds[ind - 1] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    if (ind >= (NUM_LEDS - 1) - topbar) {
      fore = 0;
      colorind = colorind + 80;
      topbar++;
    }
  }
  else if (fore == 0) {
    ind--;
    leds[ind + 1] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    if (ind <= bottombar) {
      fore = 1;
      colorind = colorind + 80;
      bottombar++;
    }
  }

  if (topbar + bottombar == NUM_LEDS) {
    topbar = 0;
    bottombar = 0;
  }

  leds[ind] = CRGB::Gray;;
  for (uint8_t ii = 0; ii < bottombar; ii++) {
    leds[ii] = CRGB::Gray;;
  }
  for (uint16_t jj = NUM_LEDS - 1; jj > NUM_LEDS - 1 - topbar; jj--) {
    leds[jj] = CRGB::Gray;;
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

void ChangePalettePeriodically() {
  uint8_t secondHand = ((millis() / 1000) % 60) - secondWhenCalled;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 5)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
  }
}

void SetupTotallyRandomPalette(){
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

void SetupBlackAndWhiteStripedPalette(){
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

void SetupPurpleAndGreenPalette(){
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}



void slide() {
  //ChangePalettePeriodically();
  static uint8_t colorIndex;
  ind = ind + 1; /* motion speed */
  colorind = ind;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    colorind += 2;
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex){
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 4;
  }
}

void whiteout() {
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Gray;
  }
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void rainbow() {
  //static uint8_t colorIndex;
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  ind = ind + 1; /* motion speed */
  colorind = ind;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    colorind += 2;
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void Pong2() {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  if (topbar + bottombar == NUM_LEDS) {
    topbar = 0;
    bottombar = 0;
  }
  colorind++;
  for (uint16_t kk = 0; kk < NUM_LEDS; kk++) {
    leds[kk] = CRGB::Black;
  }
  leds[ind] = CRGB::Gray;
  for (uint8_t ii = 0; ii < bottombar; ii++) {
    leds[ii] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
  }
  for (uint16_t jj = NUM_LEDS - 1; jj > NUM_LEDS - 1 - topbar; jj--) {
    leds[jj] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
  }
  //leds[ind] = ColorFromPalette( currentPalette, 0, brightness, currentBlending);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  if (fore == 1) {
    ind++;
    if (ind == NUM_LEDS - 1 - topbar) {
      fore = 0;
      topbar++;
    }
  }
  else if (fore == 0) {
    ind--;
    if (ind == 0 + bottombar) {
      fore = 1;
      bottombar++;
    }
  }
}


void mood() {
  CRGB red  = CRGB::Red;
  CRGB pink  = CHSV(255, 125, 200);
  CRGB gray  = CRGB::Gray;
  currentPalette = CRGBPalette16(
                     red,  red,  pink,  pink,
                     red,  red,  pink,  pink,
                     red,  red,  pink,  pink,
                     red,  red,  pink,  pink );
  currentBlending = LINEARBLEND;
  ind = ind + 1; /* motion speed */
  colorind = ind;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorind, brightness, currentBlending);
    colorind += 2;
  }

  FastLED.show();
  FastLED.delay((1000 / UPDATES_PER_SECOND)+8);
}





