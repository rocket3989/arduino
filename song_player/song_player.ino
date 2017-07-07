#include <avr/pgmspace.h>
#include <Tone.h>
#include "FastLED.h"
Tone tone1;
#define NUM_LEDS 1

#define DATA_PIN 4
CRGB leds[NUM_LEDS];
#define OCTAVE_OFFSET 0
int k;
int player_state = 0;
int play_button = 2;
int select_button = 3;
int pulse_led = 4;
int speaker = 8;
int song_selected = 0;
int number_songs = 6;
int notes[] = { 0,
                NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
                NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
                NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
                NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
              };
void showRGB(int color)
{
	int redIntensity;
	int greenIntensity;
	int blueIntensity;


	if (color <= 255)          // zone 1
	{
		redIntensity = 255 - color;    // red goes from on to off
		greenIntensity = color;        // green goes from off to on
		blueIntensity = 0;             // blue is always off
	}
	else if (color <= 511)     // zone 2
	{
		redIntensity = 0;                     // red is always off
		greenIntensity = 255 - (color - 256); // green on to off
		blueIntensity = (color - 256);        // blue off to on
	}
	else // color >= 512       // zone 3
	{
		redIntensity = (color - 512);         // red off to on
		greenIntensity = 0;                   // green is always off
		blueIntensity = 255 - (color - 512);  // blue on to off
	}

	// Now that the brightness values have been set, command the LED
	// to those values

	leds[0] = CRGB(redIntensity, greenIntensity, blueIntensity);
	FastLED.show();
}


const char  song0 [] PROGMEM = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char  song1 [] PROGMEM = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char  song2 [] PROGMEM = "Bolero:d=4,o=5,b=80:c6,8c6,16b,16c6,16d6,16c6,16b,16a,8c6,16c6,16a,c6,8c6,16b,16c6,16a,16g,16e,16f,2g,16g,16f,16e,16d,16e,16f,16g,16a,g,g,16g,16a,16b,16a,16g,16f,16e,16d,16e,16d,8c,8c,16c,16d,8e,8f,d,2g";
const char  song3 [] PROGMEM = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char *song = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
//char *song = "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
//char *song4 = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
//char *song4 = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
//char *song = "20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
//char *song = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
//char *song4 = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
const char  song5 [] PROGMEM = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
//char *song = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//char *song6 = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
//char *song = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//char *song = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//char *song4 = "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
//char *song = "Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
//char *song = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//char *song = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
//char *song4 = "smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
//char *song = "smb_under:d=4,o=6,b=100:32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#";
//char *song = "smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c";
//char *song4 = "ducktales:d=4,o=5,b=112:8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8d6,8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8g6,8e6,8e6";
//char *song4 = "Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f");
//char *song9 = "smario2:d=4,o=5,b=125:8g,16c,8e,8g.,16c,8e,16g,16c,16e,16g,8b,a,8p,16c,8g,16c,8e,8g.,16c,8e,16g,16c#,16e,16g,8b,a,8p,16b,8c6,16b,8c6,8a.,16c6,8b,16a,8g,16f#,8g,8e.,16c,8d,16e,8f,16e,8f,8b.4,16e,8d.,c";
//char *song4 = "smb3lvl1:d=4,o=5,b=80:16g,32c,16g.,16a,32c,16a.,16b,32c,16b,16a.,32g#,16a.,16g,32c,16g.,16a,32c,16a,4b.,32p,16c6,32f,16c.6,16d6,32f,16d.6,16e6,32f,16e6,16d.6,32c#6,16d.6,16c6,32f,16c.6,16d6,32f,16d6,4e.6,32p,16g,32c,16g.,16a,32c,16a.,16b,32c,16b,16a.,32g#,16a.,16c6,8c.6,32p,16c6,4c.6";
//char *song = "shinobi:d=4,o=5,b=140:b,f#6,d6,b,g,f#,e,2f#.,a,1f#,p,b,f#6,d6,b,g,f#,e,1f#.,8a,1b.,8a,1f#.,8a,1b.,8a,1f#.";
//char *song = "outrun_magic:d=4,o=5,b=160:f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,8f.,a#,16c.6,1a#,f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,16f.,16g#.,c6,2a#.";
//char *song = "Popeye:d=4,o=5,b=140:16g.,16f.,16g.,16p,32p,16c.,16p,32p,16c.,16p,32p,16e.,16d.,16c.,16d.,16e.,16f.,g,8p,16a,16f,16a,16c6,16b,16a,16g,16a,16g,8e,16g,16g,16g,16g,8a,16b,32c6,32b,32c6,32b,32c6,32b,8c6";
//char *song = "Wonderboy:d=4,o=5,b=225:f6,d6,f6,8d6,f6,32p,8f6,d6,f6,d6,e6,c6,e6,8c6,e6,32p,8e6,c6,e6,c6";
//char *song4 = "smwwd1:d=4,o=5,b=125:a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16f,16p,8c6,8a.,g,16c,a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16a#,16a,16g,2f,16p,8a.,8f.,8c,8a.,f,16g#,16f,16c,16p,8g#.,2g,8a.,8f.,8c,8a.,f,16g#,16f,8c,2c6";
//char *song = "dkong:d=4,o=5,b=160:2c,8d.,d#.,c.,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,2c6";
//char *song11 = "dkong:d=4,o=5,b=160:2c,8d.,d#.,c.,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,2c6";
const char  song4 [] PROGMEM = "LordDance:d=4,o=5,b=225:8p,2a6,f#.6,8e6,8f#6,8g6,8f#6,8e6,2d6,f#6,8f#6,8g6,a6,8g6,8f#6,e6,a,a,8p,8a,d6,d6,d.6,8e6,8f#6,8e6,8f#6,8g6,a6,8g6,8f#6,e6,e6,8e6,8g6,8f#6,8e6,2d6";
//char *song4 = "Who:d=4,o=6,b=56:e,16p,16d_,16c,16b5,e,16p,16d_,16c,16b5,e,16p,16d_,16c,16b5,e5,32d_5,32e5,32f_5,32g_5,32a5,32b5,32c_,32d_,16p,e,16p,16d_,16c,16b5,e,16p,16d_,16c,16b5,e,16p,16d_,16c,16b5,e5,32d_5,32e5,32f_5,32g_5,32a5,32b5,32c_,32d_";

void setup(void)
{
	pinMode(play_button, INPUT_PULLUP);
	pinMode(select_button, INPUT_PULLUP);
	Serial.begin(9600);
	tone1.begin(speaker);
	FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
}

#define isdigit(n) (n >= '0' && n <= '9')

void play_rtttl(char *p)
{
	

	byte default_dur = 4;
	byte default_oct = 6;
	int bpm = 63;
	int num;
	long wholenote;
	long duration;
	byte note;
	byte scale;

	// format: d=N,o=N,b=NNN:
	// find the start (skip name, etc)

	while(*p != ':') p++;    // ignore name
	p++;                     // skip ':'

	// get default duration
	if(*p == 'd')
	{
		p++;
		p++;              // skip "d="
		num = 0;
		while(isdigit(*p))
		{
			num = (num * 10) + (*p++ - '0');
		}
		if(num > 0) default_dur = num;
		p++;                   // skip comma
	}

	//Serial.print("ddur: "); Serial.println(default_dur, 10);

	// get default octave
	if(*p == 'o')
	{
		p++;
		p++;              // skip "o="
		num = *p++ - '0';
		if(num >= 3 && num <= 7) default_oct = num;
		p++;                   // skip comma
	}

// Serial.print("doct: "); Serial.println(default_oct, 10);

	// get BPM
	if(*p == 'b')
	{
		p++;
		p++;              // skip "b="
		num = 0;
		while(isdigit(*p))
		{
			num = (num * 10) + (*p++ - '0');
		}
		bpm = num;
		p++;                   // skip colon
	}

// Serial.print("bpm: "); Serial.println(bpm, 10);

	// BPM usually expresses the number of quarter notes per minute
	wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

// Serial.print("wn: "); Serial.println(wholenote, 10);


	// now begin note loop
	while(*p)
	{
		// first, get note duration, if available
		num = 0;
		while(isdigit(*p))
		{
			num = (num * 10) + (*p++ - '0');
		}

		if(num) duration = wholenote / num;
		else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

		// now get the note
		note = 0;

		switch(*p)
		{
			case 'c':
				note = 1;
				break;
			case 'd':
				note = 3;
				break;
			case 'e':
				note = 5;
				break;
			case 'f':
				note = 6;
				break;
			case 'g':
				note = 8;
				break;
			case 'a':
				note = 10;
				break;
			case 'b':
				note = 12;
				break;
			case 'p':
			default:
				note = 0;
		}
		p++;

		// now, get optional '#' sharp
		if(*p == '#')
		{
			note++;
			p++;
		}

		// now, get optional '.' dotted note
		if(*p == '.')
		{
			duration += duration / 2;
			p++;
		}

		// now, get scale
		if(isdigit(*p))
		{
			scale = *p - '0';
			p++;
		}
		else
		{
			scale = default_oct;
		}

		scale += OCTAVE_OFFSET;

		if(*p == ',')
			p++;       // skip comma for next note (or we may be at the end)

		// now play the note

		if(note)
		{
			Serial.print("note: ");
			Serial.println(duration);
			tone1.play(notes[(scale - 4) * 12 + note]);
			long previous_millis = millis();
			showRGB(duration);
			boolean stay = true;
			while ((millis() <= previous_millis + duration) && stay)
			{
				//	Serial.print(millis());
				//	Serial.print(",");
				//	Serial.print(previous_millis);
				//	Serial.print(",");
				//	Serial.println(duration);
				//Serial.println((previous_millis + duration) - millis() );
				if ((digitalRead(play_button) == 0) || (digitalRead(select_button) == 0))
				{
					tone1.stop();
					Serial.println("button pressed in note loop!");
					while (*p)
					{
						stay = false;
						p++;
					}
					if (digitalRead(play_button) == 0)
					{
						player_state = 0;
					}
					else
					{
						player_state = 2;
					}
					while ((digitalRead(play_button) == 0) || (digitalRead(select_button) == 0)) {}
					delay(500);
				}
			}
		}
		else
		{
			Serial.print("rest: ");
			Serial.println(duration);
			boolean stay = true;
			long previous_millis = millis();
			showRGB(duration);
			while ((millis() <= previous_millis + duration) && stay)
			{
				if ((digitalRead(play_button) == 0) || (digitalRead(select_button) == 0))
				{
					Serial.println("button pressed in rest loop!");
					while (*p)
					{
						stay = false;
						p++;
					}
					if (digitalRead(play_button) == 0)
					{
						player_state = 0;
					}
					else
					{
						player_state = 2;
					}
					while ((digitalRead(play_button) == 0) || (digitalRead(select_button) == 0)) {}
					delay(500);
				}
			}
		}
		tone1.stop();
	}
}

void loop(void)
{
	showRGB(0);
	Serial.print("state (top of loop)= ");
	Serial.println(player_state);
	if (player_state == 0)
	{
		while ((digitalRead(play_button) == 1) && (digitalRead(select_button) == 1)) {}
		if (digitalRead(play_button) == 0)
		{
			player_state = 1;
		}
		else
		{
			player_state = 2;
		}
		while ((digitalRead(play_button) == 0) || (digitalRead(select_button) == 0)) {}
		delay(500);
	}
	Serial.print("state (after 0)= ");
	Serial.println(player_state);
	if (player_state == 1)
	{
		player_state = 0;
		switch (song_selected)
		{
			case 0:
				Serial.println("playing song 1");
				play_rtttl(pgm_read_byte_near(song0 + k));
				break;
			case 1:
				Serial.println("playing song 2");
				play_rtttl(pgm_read_byte_near(song1 + k));
				break;
			case 2:
				Serial.println("playing song 3");
				play_rtttl(pgm_read_byte_near(song2 + k));
				break;
			case 3:
				Serial.println("playing song 4");
				play_rtttl(pgm_read_byte_near(song3 + k));
				break;
			case 4:
				Serial.println("playing song 5");
				play_rtttl(pgm_read_byte_near(song4 + k));
				break;

			case 5:
				Serial.println("playing song 6");
				play_rtttl(pgm_read_byte_near(song5 + k));
				break;
				/*
				case 6:
				Serial.println("playing song 7");
				play_rtttl(song6);
				break;

				case 7:
				Serial.println("playing song 8");
				play_rtttl(song7);
				break;
				case 8:
				Serial.println("playing song 9");
				play_rtttl(song8);
				break;
				case 9:
				Serial.println("playing song 10");
				play_rtttl(song9);
				break;
				case 10:
				Serial.println("playing song 11");
				play_rtttl(song10);
				break;
				case 11:
				Serial.println("playing song 12");
				play_rtttl(song11);
				break;
				*/
		}

	}
	Serial.print("state (after 1)= ");
	Serial.println(player_state);
	if (player_state == 2)
	{
		song_selected ++;
		if (song_selected == number_songs)
		{
			song_selected = 0;
		}
		player_state = 1;
	}

}
