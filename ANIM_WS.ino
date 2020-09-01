/* === ALL +5V DC SERIAL LED STRIP ANIMATIONS (WS2812B) === */
//ANIMATION CATEGORIES
//A) RYTHM REGULAR
//B) RYTHM BUILDUP
//C) BACKGROUND

// LAST EDIT 23/07/2020

void addGlitter( fract8 chanceOfGlitter, uint8_t glitterhue, bool glittermode)
{
  if (glittermode == true)
  {
    RGBW1strip = CHSV( glitterhue, 255, 255);
  }
  else
  {
    wscolorstrip = CRGB::White;
  }

  if ( random8() < chanceOfGlitter)
  {
    leds[ random8(NUM_LEDS) ] += wscolorstrip;
  }
}

void beats2()
{
  if (animdelayer >= rythmdelayer)
  {
    animdelayer = 0;
    if (side == false)
    {
      for (uint8_t i = 0; i < NUM_LEDS / 2; i++)
      {
        leds[i] = CRGB::Orange;
      }
    }
    else
    {
      for (uint8_t i = NUM_LEDS / 2; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Cyan;
      }
    }
    side = !side;
  }
  fadeToBlackBy( leds, NUM_LEDS, 5); //higher number gives faster fade times
}

void beats2x2()
{
  // 128BPM
  if (posvar > ((NUM_LEDS / 2) - BEATSSIZE))
  {
    posvar = 0;
    animdelayer = 0;
  }

  if (animdelayer >= rythmdelayer)
  {
    wscolorstrip = CHSV( random8(255), 200, 255);
    animdelayer = 0;
    for (uint8_t i = 0; i < BEATSSIZE; i++)
    {
      leds[posvar] = wscolorstrip;
      leds[NUM_LEDS - posvar - 1] = leds[posvar];
      posvar++;
    }
  }
  fadeToBlackBy( leds, NUM_LEDS, 2);

}

void beats4_1()
{
  if (sctcount >= 4)
  {
    posvar = 0;
    animdelayer = 0;
    sctcount = 0;
  }

  if (animdelayer >= rythmdelayer / 2)
  {
    animdelayer = 0;
    switch (sctcount)
    {
      //60 60 60 60
      case 0:
        posvar = 0;
        sectionsize = 60;
        break;
      case 1:
        posvar = NUM_LEDS - 60;
        sectionsize = 60;
        break;
      case 2:
        posvar = 60;
        sectionsize = 60;
        break;
      case 3:
        posvar = NUM_LEDS - 60 - 60;
        sectionsize = 60;
        break;
    }

    wscolorstrip = CHSV( random8(255), 200, 255);
    beatscommon();
    sctcount++;
  }
  fadeToBlackBy( leds, NUM_LEDS, 7);
}

void beats4_2()
{
  if (sctcount >= 5)
  {
    posvar = 0;
    animdelayer = 0;
    sctcount = 0;
  }

  if (animdelayer >= rythmdelayer / 2)
  {
    animdelayer = 0;
    switch (sctcount)
    {
      //60 60 60 60
      case 0:
        posvar = 0;
        sectionsize = 60;
        break;
      case 1:
        posvar = NUM_LEDS - 60;
        sectionsize = 60;
        break;
      case 2:
        posvar = 60;
        sectionsize = 60;
        break;
      case 3:
        posvar = NUM_LEDS - 60 - 60;
        sectionsize = 60;
        break;
      case 4:
        posvar = 0;
        sectionsize = NUM_LEDS;
        break;
    }

    if (sctcount < 4)
      wscolorstrip = CHSV( random8(255), 200, 255);
    else if (sctcount >= 4)
      wscolorstrip = CRGB::White;

    beatscommon();
    sctcount++;
  }
  fadeToBlackBy( leds, NUM_LEDS, 7);
}

void trailup()
{
  fadeToBlackBy( leds, NUM_LEDS, 8);
  if (animdelayer >= rythmdelayer / 15)
  {
    animdelayer = 0;
    if (y >= section)
    {
      y = 0; //0
      x = (2 * section) - 1; //59
      wscolorstrip = CHSV(globHue, 200, 255);
    }

    for (uint8_t p = 0; p < 4; p++)
    {
      leds[y + (2 * p * section)] = wscolorstrip;
      leds[x + (2 * p * section)] = wscolorstrip;
    }
    x--;
    y++;
  }
}

void traildown()
{
  fadeToBlackBy( leds, NUM_LEDS, 8);
  if (animdelayer >= rythmdelayer / 15)
  {
    animdelayer = 0;
    if (y >= section)
    {
      y = section - 1; //0
      x = section; //59
      wscolorstrip = CHSV(globHue, 200, 255);
    }

    for (uint8_t p = 0; p < 4; p++)
    {
      leds[y + (2 * p * section)] = wscolorstrip;
      leds[x + (2 * p * section)] = wscolorstrip;
    }
    x++;
    y--;
  }
}

void tml()
{
  fadeToBlackBy( leds, NUM_LEDS, 5);
  if (animdelayer >= rythmdelayer / 4)
  {
    animdelayer = 0;
    location = random(8);

    do {
      location = random(8);
    } while (location == lastlocation);

    lastlocation = location;
    wscolorstrip = CHSV( random8(255), 255, 255);
    for (int i = 0; i < section; i++)
    {
      leds[(location * section) + i] = wscolorstrip;
    }
  }
}

void bpmanim()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, globHue + (i * 2), beat - globHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin8(i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void lines()
{
  // 4 random red dots 6 pixels distance, fading quick
  if (posvar >= NUM_LEDS)
  {
    wscolorstrip = CHSV( random8(0, 255), 200, 255);
    posvar = 0;
  }

  leds[posvar] = wscolorstrip;
  leds[NUM_LEDS - posvar - 1] = leds[posvar];
  posvar++;

  fadeToBlackBy( leds, NUM_LEDS, 8);
}

void beats_8_mix()
{
  fadeToBlackBy( leds, NUM_LEDS, 8);
  if (animdelayer >= rythmdelayer)
  {
    animdelayer = 0;
    int k = 0;
    for (uint8_t w = 0; w < 8; w++)
    {
      wscolorstrip = CHSV( random8(255), 255, 255);
      for (uint8_t i = 0; i < section; i++)
      {
        leds[(w * section) + i] = wscolorstrip;
      }
    }
  }
}

void glitter_rainbow()
{
  fadeToBlackBy( leds, NUM_LEDS, 25);
  if (animdelayer >= (2 * rythmdelayer))
  {
    state = !state;
    animdelayer = 0;
  }

  if (state == false)
  {
    fill_rainbow( leds, NUM_LEDS, globHue, 5);
  }
  else
  {
    addGlitter(255, 0, false);
  }
}

void glitter_switch()
{
  if (animdelayer >= (2 * rythmdelayer))
  {
    state = !state;
    animdelayer = 0;
  }

  if (state == false)
  {
    addGlitter(255, 0, false);
  }
  else
  {
    addGlitter(255, 0, true); //red glitter
  }
  fadeToBlackBy( leds, NUM_LEDS, 25);
}

void beats8_S()
{
  if (sctcount >= 8)
  {
    posvar = 0;
    animdelayer = 0;
    sctcount = 0;
  }
  if (animdelayer >= rythmdelayer / 4)
  {
    animdelayer = 0;
    wscolorstrip = CHSV( random8(128, 255), 200, 255);

    sectionsize = 30;
    beatscommon();
    sctcount++;
  }
  fadeToBlackBy( leds, NUM_LEDS, 8);
}

void beats8_F()
{
  if (sctcount >= 8)
  {
    posvar = 0;
    animdelayer = 0;
    sctcount = 0;
  }
  if (animdelayer >= (rythmdelayer / 8))
  {
    animdelayer = 0;
    wscolorstrip = CHSV( random8(128, 255), 200, 255);

    sectionsize = 30;
    beatscommon();
    sctcount++;
  }
  fadeToBlackBy( leds, NUM_LEDS, 12);
}

void strobe()
{
  // strobe
  if (animdelayer >= 3)
  {
    animdelayer = 0;
    if (state == false)
    {
      for (uint8_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::White;
      }
    }
    else
    {
      for (uint8_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
      }
    }
    state = !state;
  }
}

void dots()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 3); //higher number gives faster fade times
  int pos = random8(NUM_LEDS);
  leds[pos] += CHSV( random8(255), 255, 255);
}

void rgb()
{
  wscolorstrip = CHSV( globHue, 255, 255);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = wscolorstrip;
  }
}

void ice()
{
  if (animdelayer >= 4)
  {
    animdelayer = 0;
    if (y >= section)
    {
      do {
        location = random8(8);
      } while (location == lastlocation);
      lastlocation = location;

      y = section - 1; //reset to 29 for even start
      x = 0; //reset to 0
    }

    if (location % 2 == 0) //is location even?
    {
      leds[y + (location * section)] = CRGB::White;
    }

    else
    {
      leds[x + (location * section)] = CRGB::White;
    }
    x++;
    y--;
    fadeToBlackBy( leds, NUM_LEDS, 50);
  }
}

void randomsines()
{
  fadeToBlackBy( leds, NUM_LEDS, 20);
  if (animdelayer >= rythmdelayer)
  {
    side = !side;
    animdelayer = 0;
  }

  int k = 0;
  uint8_t pos1 = beatsin8(16 , 0, (NUM_LEDS / 4) - 1);
  uint8_t pos2 = beatsin8(16 , (NUM_LEDS / 4) + 1, (NUM_LEDS / 2) - 1);
  uint8_t pos3 = beatsin8(16 , (NUM_LEDS / 2) + 1, (3 * NUM_LEDS) / 4 - 1);
  uint8_t pos4 = beatsin8(16 , (3 * NUM_LEDS / 4) + 1, NUM_LEDS - 1);

  jsine = 30;

  for (uint8_t i = 0; i < 12; i++)
  {
    jsine += 10;
    k = sin8(jsine);
    leds[pos1 + i].setRGB(k, 0, 0);
    leds[pos2 - 1 + i].setRGB(0, k, 0);
    leds[pos3 + i].setRGB(0, 0, k);
    leds[pos4 - 1 + i].setRGB(k, k, k);
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, globHue, 2); //higher number gives shorter rainbow chains
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80, 0, false);
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 5); //higher number gives faster fade times
  int pos = random8(NUM_LEDS);
  leds[pos] += CHSV( globHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin8( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( globHue, 255, 192);
}

void glitterbox()
{
  addGlitter(255, 0, false);
  fadeToBlackBy( leds, NUM_LEDS, 25);
}

void triplets()
{
  fadeToBlackBy( leds, NUM_LEDS, 7);
  if (sctcount >= 6)
  {
    sctcount = 0;
  }
  if (animdelayer >= (rythmdelayer / 3))
  {
    animdelayer = 0;
    wscolorstrip = CHSV( random8(255), 200, 255);
    sectionsize = subsection;
    switch (sctcount)
    {
      case 0:
        subbeatscommon(0);
        subbeatscommon(6);
        subbeatscommon(12);
        subbeatscommon(18);
        break;
      case 1:
        subbeatscommon(1);
        subbeatscommon(7);
        subbeatscommon(13);
        subbeatscommon(19);
        break;
      case 2:
        subbeatscommon(2);
        subbeatscommon(8);
        subbeatscommon(14);
        subbeatscommon(20);
        break;
      case 3:
        subbeatscommon(3);
        subbeatscommon(9);
        subbeatscommon(15);
        subbeatscommon(21);
        break;
      case 4:
        subbeatscommon(4);
        subbeatscommon(10);
        subbeatscommon(16);
        subbeatscommon(22);
        break;
      case 5:
        subbeatscommon(5);
        subbeatscommon(11);
        subbeatscommon(17);
        subbeatscommon(23);
        break;
    }
    sctcount++;
  }
}

void sinefadeout()
{
  fadeToBlackBy( leds, NUM_LEDS, 7);
  if (animdelayer >= rythmdelayer)
  {
    animdelayer = 0;
    int k = 0;
    for (uint8_t w = 0; w < 8; w++)
    {
      for (uint8_t i = 0; i < section; i++)
      {
        wscolorstrip = CHSV( globHue, 200, sectionleds[i]);
        if (w % 2 == 0) leds[(w * section) + i] = wscolorstrip;
        else leds[(w * section) + i] = wscolorstrip;
      }
    }
  }
}

void wavingsines()
{
  if (animdelayer >= rythmdelayer)
  {
    side = !side;
    animdelayer = 0;
  }

  for (uint8_t w = 0; w < 8; w++)
  {
    for (uint8_t i = 0; i < section; i++)
    {
      if (side == false)
      {
        if (w % 2 == 0) leds[(w * section) + i].setRGB(sectionleds[i], 0, 0); //red
        else leds[(w * section) + i].setRGB(0, 0, sectionleds[i]); //blue
      }
      else
      {
        if (w % 2 == 0) leds[(w * section) + i].setRGB(0, 0, sectionleds[i]); //blue
        else leds[(w * section) + i].setRGB(sectionleds[i], 0, 0); //red
      }
    }
  }
}

void movingsines()
{
  if (extendedStates >= section) extendedStates = 0; //displacement of animation
  if (animdelayer >= rythmdelayer / 15)
  {
    animdelayer = 0;
    for (uint8_t w = 0; w < 8; w++)
    {
      for (uint8_t i = 0; i < section; i++)
      {
        uint8_t x = i + extendedStates;
        if (x >= section) x = x - section; //start back from 0
        leds[(w * section) + i].setRGB(sectionleds[x], 0, 0); //red
      }
    }
    extendedStates++;
  }
}

void juggle8ind() {
  // eight colored dots, waving in and out of sync with each other on their own section
  fadeToBlackBy( leds, NUM_LEDS, 10);
  for ( int i = 0; i < 8; i++) {
    leds[beatsin8( (bpm / 8) + (i * 2), (section * i), (section * (i + 1)) - 1)] |= CHSV(globHue, 200, 255);
  }
}

void juggle8wave() {
  // eight colored dots in a horizontal sine wave
  fadeToBlackBy( leds, NUM_LEDS, 10);
  for ( int i = 0; i < 8; i++) {
    leds[beatsin8( bpm / 4, (section * i), (section * (i + 1)) - 1)] |= CHSV(globHue, 200, 255);
  }
}

void juggle8sine() {
  // eight colored dots in a horizontal sine wave
  fadeToBlackBy( leds, NUM_LEDS, 10);
  for ( int i = 0; i < 8; i++)
  {
    if (i % 2 == 0) //is location even?
    {
      leds[beatsin8( bpm / 4, (section * i), (section * (i + 1)) - 1, 0, i * 32)] |= CHSV(globHue, 200, 255);
    }

    else
    {
      leds[beatsin8( bpm / 4, (section * i), (section * (i + 1)) - 1, 0, (i * 32) - 128)] |= CHSV(globHue, 200, 255);
    }
  }
}

void tomid() {
  if (sctcount >= 4)
  {
    posvar = 0;
    animdelayer = 0;
    sctcount = 0;
    wscolorstrip = CHSV(globHue, 200, 255);
  }
  if (animdelayer >= rythmdelayer / 4)
  {
    animdelayer = 0;
    for (int i = 0; i < section; i++)
    {
      leds[(sctcount * section) + i] = wscolorstrip;
      leds[(NUM_LEDS - ((sctcount * section) + i) - 1)] = wscolorstrip;
    }
    sctcount++;
  }
  fadeToBlackBy( leds, NUM_LEDS, 20);
}

void allwhite() {
  // eight colored dots in a horizontal sine wave
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::White;
  }
}
