#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6  //this is the data pin connected to the LED strip.  If using WS2801 you also need a clock pin
#define NUM_LEDS 100 //change this for the number of LEDs in the strip
#define COLOR_ORDER RGB
#define BRIGHTNESS 120
#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ400);

int brightness = BRIGHTNESS;

int stopAtLight = -1;

int differentColors = 10;

uint32_t colors[] = {
                    strip.Color (0,255,255), // cyanish
                    strip.Color (153, 50, 204), // purpleish
                    strip.Color (255,255,0), // yellow
                    strip.Color (0,255,127), // green
                    strip.Color (255,165,0), // orange
                    strip.Color (65,105,255), // blue
                    strip.Color (76, 0, 153), // dark purple
                    strip.Color (255,105,180), // pink
                    strip.Color (0,128,0), // dark green
                    strip.Color (255,0,0) // red
                };

// number of lights to skip before getting to the letters
int skipLightStart = 26;
// number of lights to skip for the curve when going to the second row of letters
int numSkipLights = 5;
// number of lights between a letter
int lightsBetweenLetters = 2;

String phrases[] = {
    "help me",
    "im trapped",
    "hi mom"
};

// the order of the letters, in reverse order
// in my case z was at the very end of the light strip
// and m was at the "beginning"
String letters = "ZYXWVUTSRQPONABCDEFGHIJKLM";

void setup(){
    Serial.begin(9600);
    strip.setBrightness(BRIGHTNESS);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
    fullBright();
    flicker();
    followMe();
    stringToLights(phrases[random(0, arr_len(phrases) )]);
    delay(3000);
}

void testStrip() {
  for(int i = 0; i < NUM_LEDS; i++ )
  {
      strip.setPixelColor(i, 0, 100, 0);
      strip.show();
      delay(500);
      strip.clear();
  }
}

void followMe() {

    int delayTime = 100;

    for(int i = NUM_LEDS-1; i > stopAtLight; i-- )
    {
        strip.setPixelColor(i, colors[i % differentColors]);
        strip.show();
        // trying to get some kind of "speed up as you go along" effect going
        delayTime = delayTime-(100/i);
        if(delayTime < 10) {
            delayTime = 10;
        }
        delay(delayTime);
        strip.show();
    }

    delay(1000);
    strip.show();
    strip.clear();
}

void flicker() {
    for(int i = 0; i < 5; i++) {
        strip.setBrightness(BRIGHTNESS);
        strip.show();
        delay(100);
        strip.setBrightness(BRIGHTNESS/3);
        strip.show();
    }
    for(int i = 15; i < 255; i+=3) {
        int setBright = i;
        if(setBright > 255) {
            setBright = 255;
        }
        strip.setBrightness(setBright);
        strip.show();
        delay(20);
    }
    delay(3000);
    for(int i = 0; i < 5; i++) {
        strip.setBrightness(BRIGHTNESS);
        strip.show();
        delay(100);
        strip.setBrightness(BRIGHTNESS/3);
        strip.show();
    }
    strip.clear();
    strip.show();
    delay(1500);
}

void stringToLightsFull(String stringToPrint) {

    stringToPrint.toUpperCase();

    for(int i=0; i < stringToPrint.length(); i++) {
        int charIndex = letters.indexOf(stringToPrint[i]);

        // multiply by 2 so we skip every other light
        charIndex = charIndex * lightsBetweenLetters;

        // see if the charindex is greater than our skip start
        // if it is, add the number of skips to it
        if(skipLightStart <= charIndex) {
            charIndex += numSkipLights;
        }
        strip.setPixelColor(charIndex, colors[charIndex % differentColors]);
        strip.show();
    }
    delay(1500);
}

void stringToLights(String stringToPrint) {
    strip.clear();
    strip.show();
    delay(1000);
    stringToPrint.toUpperCase();

    for(int i=0; i < stringToPrint.length(); i++) {
        int charIndex = letters.indexOf(stringToPrint[i]);

        // multiply by 2 so we skip every other light
        charIndex = charIndex * lightsBetweenLetters;

        // see if the charindex is greater than our skip start
        // if it is, add the number of skips to it
        if(skipLightStart <= charIndex) {
            charIndex += numSkipLights;
        }
        strip.setPixelColor(charIndex, colors[charIndex % differentColors]);
        strip.show();
        delay(1000);
        strip.clear();
        strip.show();
        delay(300);
    }
}

void setLights() {
    for(int i = 0; i < NUM_LEDS; i++ ) {
        strip.setPixelColor(i, colors[i % differentColors]);
    }
}

void fullBright() {
    setLights();
    strip.show();
    delay(3000);
}
