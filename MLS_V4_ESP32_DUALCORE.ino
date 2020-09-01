/* === MAIN MUSIC LIGHT SYSTEM PROGRAM === */
// LAST EDIT 30/08/2020

/*
  *QUOTE TO START THE DEVELOPMENT DAY*

  Some people are like Slinkies.

  Not really good for anything, but they bring a smile to your face when pushed down the stairs.
*/

//#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3003000
#error "Requires FastLED 3.3 or later; check github for latest code. Do not use 3.3.3"
#endif 

#define FASTADC 1
 
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define WS240 19
#define WS_EXT 23

//pins not to use: https://www.esp32.com/viewtopic.php?t=5970
//https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

//OUTPUTS
#define RGBW1_R 13
#define RGBW1_G 22
#define RGBW1_B 21
#define RGBW1_W 4

#define RGBW2_R 17
#define RGBW2_G 16
#define RGBW2_B 27
#define RGBW2_W 18

#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define NUM_LEDS 240
#define HALF (NUM_LEDS / 2)

#define NUM_LEDS_2M 120
#define HALF_2M 60

#define FRAMES_PER_SECOND 120 //max 277 LED's theoretically for 120Hz
#define BEATSSIZE 60
#define COLUMN 4

//#define PRINT
//#define MUSIC

#define left_channel 34
#define right_channel 35
#define CLIP 25 //for music sensitivity
#define MODEBTN 26

CRGB leds[NUM_LEDS] = 0;

CRGB left_array[HALF_2M];
CRGB right_array[HALF_2M];
CRGB wsstrip2[NUM_LEDS_2M] = 0;

//crossfade example: https://gist.github.com/StefanPetrick/0c0d54d0f35ea9cca983

CRGB wscolorstrip = 0;

CRGB RGBW1strip = 0;
CRGB RGBW2strip = 0;

int beatt, posvar, timingvar = 0;
int sectionsize = 30;
int incomingByte = 0;
int rythmdelayer = 0;

const int section = 30;
const int subsection = 10;

uint8_t brtrgbw = 0; 	//brightness (RGBW)
uint8_t brtws = 0;	//brightness serial LEDs

uint8_t animdelayer = 0;
uint8_t animdelayerrgbw = 0;
uint8_t leftbrt = 0;
uint8_t rightbrt = 0;
uint8_t bpm = 128; // 128 * delay correction
uint8_t flashdelayer = 0; //compare delay
uint8_t flashfreq = 0;
uint8_t x, y = 0;
uint8_t delcor = 1;
uint8_t chanbw = 500;
uint8_t jsine = 0;
uint8_t location, lastlocation = 0; //max location is 240
uint8_t rgbcolorstriphue = 0;
uint8_t RGBW1w = 0;
uint8_t RGBW2w = 0;
uint8_t sctcount = 0; //section counter
uint8_t extendedStates = 0;

uint8_t sectionleds[section] = {0};

bool flash = false;
bool side = false;
bool state = false;
bool flashstate = false;
bool buttonmode = false;

uint8_t globHue = 0; // rotating "base color" used by many of the patterns
int midval = 1870; //middle value for ADC

uint8_t flashfreqlookup[30] = {0};

//Byte 0 0 0   0 0 0 0 0
//     DEST  |   DATA
typedef enum datatypes //one byte contains all data, prevention of data corruption
{
  WS_ANIM,
  RGBW_ANIM,
  RGBW_COL,
  BRT_WS,
  BRT_RGBW,
  BLINDER,
  FLASH,
  BPM
} dtype;

TaskHandle_t communication;
TaskHandle_t lighting;

void setup()
{
  delay(3000); // 3 second delay for recovery
  set_max_power_in_volts_and_milliamps(5, 5000);
  pinMode(WS240, OUTPUT);
  pinMode(WS_EXT, OUTPUT);
  pinMode(CLIP, OUTPUT);

  pinMode(RGBW1_R, OUTPUT);
  pinMode(RGBW1_G, OUTPUT);
  pinMode(RGBW1_B, OUTPUT);
  pinMode(RGBW1_W, OUTPUT);
  pinMode(RGBW2_R, OUTPUT);
  pinMode(RGBW2_G, OUTPUT);
  pinMode(RGBW2_B, OUTPUT);
  pinMode(RGBW2_W, OUTPUT);

  pinMode(MODEBTN, INPUT);

  //PWM
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcSetup(3, 5000, 8);
  ledcSetup(4, 5000, 8);
  ledcSetup(5, 5000, 8);
  ledcSetup(6, 5000, 8);
  ledcSetup(7, 5000, 8);
  ledcAttachPin(RGBW1_R, 0);
  ledcAttachPin(RGBW1_G, 1);
  ledcAttachPin(RGBW1_B, 2);
  ledcAttachPin(RGBW1_W, 3);
  ledcAttachPin(RGBW2_R, 4);
  ledcAttachPin(RGBW2_G, 5);
  ledcAttachPin(RGBW2_B, 6);
  ledcAttachPin(RGBW2_W, 7);

  rythmdelayer = FRAMES_PER_SECOND / (float(bpm) / 60); //calculate delay time to match BPM
  
  FastLED.addLeds<LED_TYPE, WS240, COLOR_ORDER>(leds, NUM_LEDS);
  #ifdef MUSIC
    FastLED.addLeds<LED_TYPE, WS_EXT, COLOR_ORDER>(wsstrip2, NUM_LEDS_2M);
  #endif MUSIC

  FastLED.setBrightness(brtws);
  FastLED.clear();

  fillsinesection(); //fill sine table to increase speed during runtime

  for (uint8_t z = 0; z < 30; z++)
  {
    flashfreqlookup[z] = rythmdelayer / 15 * z; //strobe flash frequency table, should make it logarithmic
  }

  buttonmode = digitalRead(MODEBTN);

  disableCore0WDT();
  disableCore1WDT();
  xTaskCreatePinnedToCore(
    loop1, /* Function to implement the task */
    "communication", /* Name of the task */
    5000, /* Stack size in words */
    NULL, /* Task input parameter */
    1, /* Priority of the task */
    &communication, /* Task handle. */
    0); /* Core where the task should run */

  xTaskCreatePinnedToCore(
    loop0, /* Function to implement the task */
    "lighting", /* Name of the task */
    5000, /* Stack size in words */
    NULL, /* Task input parameter */
    2, /* Priority of the task */
    &lighting, /* Task handle. */
    1); /* Core where the task should run */ //FASTLED on core 1

  if (buttonmode == true)
  {
    //NOT IMPLEMENTED, maybe WiFi?
    digitalWrite(CLIP, HIGH);
    delay(250);
    digitalWrite(CLIP, LOW);
    delay(250);
    digitalWrite(CLIP, HIGH);
    delay(250);
    digitalWrite(CLIP, LOW);
    Serial.begin(115200); //SERIAL LOGGING
    Serial.print("READY");
  }
  else
  {
    //SERIAL
    digitalWrite(CLIP, HIGH);
    delay(750);
    digitalWrite(CLIP, LOW);
    Serial.begin(115200); //SERIAL DATA
  }
}

void loop1(void * parameter)
{
  for (;;)
  {
    //COMMS HAPPENING HERE
    EVERY_N_MILLISECONDS(1)
    {
      if (Serial.available() > 0) //check if there is/are (a) byte(s) ready to be decoded
      {
        incomingByte = Serial.read(); //first byte is the target (what data is this?)
        processdata();
      }
      if (Serial.available() == 0)
      {
        //Serial.write(0xAA); //ready to receive from PC after decoding
      }
    }
    //Nothing happening, maybe a nop operation command for stability?
  }
}

void loop0(void * parameter)
{
  for (;;)
  {
    // ============== SERIAL LED STRIP FUNCTIONALITIES ==============
  #ifdef MUSIC //audio sampling on core 0 -> logaritmic calc
    int lval = analogRead(left_channel);
    int rval = analogRead(right_channel);

    #ifdef PRINT
        Serial.print("L:");
        Serial.print(lval);
        Serial.print(" R:");
        Serial.println(rval);
    #endif

    //blink LED to show that the audio level is clipping to max shown value
    if (lval > midval + chanbw || rval > midval + chanbw || lval < midval - chanbw || rval < midval - chanbw)
    {
      digitalWrite(CLIP, HIGH);
    }
    else
    {
      digitalWrite(CLIP, LOW);
    }

    // ============== SERIAL LED STRIP FUNCTIONALITIES ==============

    EVERY_N_MILLISECONDS_I(timingObj, 25)
    {
      timingObj.setPeriod( animdelayer / 30 );

      byte hue_left = map(lval, midval - chanbw, midval + chanbw, 0, 255);
      byte hue_right = map(rval, midval - chanbw, midval + chanbw, 0, 255);

      //Shift the current values.
      for (int i = 0; i < HALF_2M - 1; i++)
      {
        left_array[i] = left_array[i + 1];
        right_array[i] = right_array[i + 1];
      }

      //Fill in the new value at the end of each array

      left_array[HALF_2M - 1] = CHSV( hue_left, 255, 255);
      right_array[HALF_2M - 1] = CHSV( hue_right, 255, 255);

      //Go through each Pixel on the strip and set its color
      for (int i = 0; i < HALF_2M; i++)
      {
        //set pixel color
        wsstrip2[i] = left_array[i];
        wsstrip2[NUM_LEDS_2M - i - 1] = right_array[i];
      }
    }
  #endif

    if (flash == true) //WW strobe function
    {
      if (flashdelayer >= 3 && flashstate == false)
      {
        flashdelayer = 0;
        flashstate = !flashstate;
        RGBW1w = 0;
        RGBW2w = 0;
      }
      else if (flashdelayer >= flashfreq && flashstate == true)
      {
        flashdelayer = 0;
        flashstate = !flashstate;
        RGBW1w = brtrgbw;
        RGBW2w = brtrgbw;
      }
      flashdelayer++;
    }

    EVERY_N_MILLISECONDS( 25 )
    {
      globHue++;  // slowly cycle the "base color" through the rainbow
    }

    EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND)
    {
      patternselector();
      ledcWrite(0, RGBW1strip.r);
      ledcWrite(1, RGBW1strip.g);
      ledcWrite(2, RGBW1strip.b);
      ledcWrite(3, RGBW1w);

      ledcWrite(4, RGBW2strip.r);
      ledcWrite(5, RGBW2strip.g);
      ledcWrite(6, RGBW2strip.b);
      ledcWrite(7, RGBW2w);
      FastLED.setBrightness(brtws);
      FastLED.show();
    }
  }
}

// List of patterns to cycle through.  Each is defined as a separate function below.

//ANIMATION CATEGORIES
//A) RYTHM REGULAR
//B) RYTHM BUILDUP
//C) BACKGROUND
//D) RGBW FUNCTIONS

//16/06/2020 changed beatsin16 to beatsin8 and random16 to random8 for speed increase
//https://github.com/FastLED/FastLED/issues/82
typedef void (*SimplePatternList[])();
SimplePatternList wsPatterns =
{
  black,
  nows, //empty function for light test
  //A RYTHM REGULAR
  beats2,
  beats2x2,
  beats4_1,
  beats4_2,
  beats_8_mix,
  wavingsines,
  tml,
  trailup,
  traildown,

  //B RYTHM BUILDUP
  beats8_S,
  beats8_F,
  strobe,

  //C BACKGROUND
  juggle,
  lines,
  bpmanim,
  rgb,
  rainbowWithGlitter,
  confetti,
  dots,
  ice,
  sinelon,
  glitter_switch,
  glitterbox,
  sinefadeout,
  movingsines,
  triplets,
  juggle8ind,
  juggle8wave,
  juggle8sine,
  tomid
};

SimplePatternList rgbwPatterns =
{
  resetRGBW,
  manualRGBW,
  manualCOL, //good for light test
  RGBW_switch,
  RGBW_Wfade,
  doublergbflash,
  twofasttwoslow,
  hueflash,
  dnb,
  RGBW_Woffsetfade,
  CWflash,
  steadywhite
};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t wsCurrentPatternNumber = 1; // Index number of which pattern is current
uint8_t rgbwCurrentPatternNumber = 1;

void fillsinesection()
{
  int k = 0;
  jsine = 196;
  for (int i = 0; i < section; i++)
  {
    jsine += 8;
    k = sin8(jsine);
    sectionleds[i] = k;
  }
}

//fill a section
void beatscommon()
{
  for (int i = 0; i < sectionsize; i++) //animation used for strip sections
  {
    leds[posvar] = wscolorstrip;
    posvar++;
  }
}

//fill a subsection
void subbeatscommon(uint8_t sectionnumber)
{
  posvar = subsection * sectionnumber;
  beatscommon();
}

void resetvars() //reset some animation varibles
{
  animdelayer = 0;
  sctcount = 0;
  posvar = 0;
  sectionsize = 0;
}

void patternselector()
{
  // Call the current pattern function once, updating the 'leds' array
  wsPatterns[wsCurrentPatternNumber]();
  animdelayer++;
  rgbwPatterns[rgbwCurrentPatternNumber]();
  animdelayerrgbw++;
}

void processdata()
{
  dtype target = (dtype)(incomingByte >> 5); //000 first 3 bits
  uint8_t datavar = incomingByte & 0x1F; //00000 last 5 bits

  switch (target) //assign data to correct target
  {
    case WS_ANIM: //set animation for serial LEDs
      if (datavar < (ARRAY_SIZE(wsPatterns)))
      {
        wsCurrentPatternNumber = datavar;
      }
      break;

    case RGBW_ANIM: //set animation for RGBWW LEDs
      if (datavar < (ARRAY_SIZE(rgbwPatterns)))
      {
        rgbwCurrentPatternNumber = datavar;
        if (datavar == 1)
        {
          resetRGB();
        }
      }
      else if (datavar == 31)
      {
        rgbwCurrentPatternNumber = 2;
        wsCurrentPatternNumber = 1;
        allwhite();
        RGBW1strip.r = brtrgbw;
        RGBW1strip.g = brtrgbw;
        RGBW1strip.b = brtrgbw;
        RGBW2strip.r = brtrgbw;
        RGBW2strip.g = brtrgbw;
        RGBW2strip.b = brtrgbw;
        RGBW1w = brtrgbw;
        RGBW2w = brtrgbw;
      }
      break;

    case RGBW_COL: //set color hue for RGBWW LEDs
      if (rgbwCurrentPatternNumber == 2) //check if no automatic animation is running
      {
        RGBW1strip = CHSV((datavar * 8), 255, brtrgbw);
        RGBW2strip = RGBW1strip;
      }
      break;

    case BRT_WS: //set max brightness for serial LEDs
      brtws = datavar * 8;
      break;

    case BRT_RGBW: //set max brightness for RGBWW LEDs
      brtrgbw = datavar * 8; //convert 5 to 8 bit
      break;

    case BLINDER: //set relative brightness for WW LEDs
      if (flash == false) //strobe has priority over static white
      {
        RGBW1w = map(datavar, 0, 31, 0, brtrgbw); //clip to max set brightness
        RGBW2w = RGBW1w;
      }
      break;

    case FLASH: //strobe WW LEDs, higher value is higher frequency
      if (datavar > 0)
      {
        flashfreq = flashfreqlookup[31 - datavar];
        //make exponential: https://forum.arduino.cc/index.php?topic=145443.0
        flash = true;
      }
      else
      {
        flash = false;
        RGBW1w = 0;
        RGBW2w = 0;
      }
      break;

    case BPM: //set BPM and apply correction of non precision controller timing (no interrupts available due to strict WS2812B timing)
      bpm = (60 + (datavar * 4)); //* delcor; //delay correction
      rythmdelayer = FRAMES_PER_SECOND / (float(bpm) / 60);
      for (uint8_t z = 0; z < 30; z++)
      {
        flashfreqlookup[z] = rythmdelayer / 15 * z;
      }
      break;
  }
}

//RESET FUNCTIONS

//WS2812B
void black()
{ //fade to black, creates smoother transition
  fadeToBlackBy( leds, NUM_LEDS, 10); //fade out which gives a smoother effect
}

//empty function for driving the LED's outside an animation function
void nows()
{}

//RGBW

//only blinder allowed (Warm White)
void manualRGBW()
{}

//same as nows
void manualCOL()
{}

void resetRGBW()
{
  RGBW1strip.r = 0;
  RGBW1strip.g = 0;
  RGBW1strip.b = 0;
  RGBW1w = 0;
  RGBW2strip.r = 0;
  RGBW2strip.g = 0;
  RGBW2strip.b = 0;
  RGBW2w = 0;
}

void resetRGB()
{
  RGBW1strip.r = 0;
  RGBW1strip.g = 0;
  RGBW1strip.b = 0;
  RGBW2strip.r = 0;
  RGBW2strip.g = 0;
  RGBW2strip.b = 0;
}

//empty main loop
void loop() { }
