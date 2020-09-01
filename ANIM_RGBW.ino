/* === ALL +12V DC RGBW LED STRIP ANIMATIONS === */
// LAST EDIT 30/06/2020

void RGBW_switch()
{
  if (animdelayerrgbw >= rythmdelayer)
  {
    animdelayerrgbw = 0;
    resetRGB();

    if (beatt >= 4) beatt = 0;
    switch (beatt)
    {
      case 0:
        RGBW1strip.r = brtrgbw;
        RGBW2strip.g = brtrgbw;
        break;
      case 1:
        RGBW1strip.g = brtrgbw;
        RGBW2strip.b = brtrgbw;
        break;
      case 2:
        RGBW1strip.b = brtrgbw;
        RGBW2strip.r = brtrgbw;
        break;
      case 3:
        RGBW1strip.r = brtrgbw;
        RGBW1strip.g = brtrgbw;
        RGBW1strip.b = brtrgbw;
        RGBW2strip.r = brtrgbw;
        RGBW2strip.g = brtrgbw;
        RGBW2strip.b = brtrgbw;
        break;
    }
    beatt++;
  }
}

void RGBW_Wfade()
{
  RGBW1strip = CHSV( globHue, 255, brtrgbw);
  RGBW2strip = CHSV( globHue, 255, brtrgbw);
}

void doublergbflash()
{
  if (animdelayerrgbw >= (rythmdelayer / 4))
  {
    animdelayerrgbw = 0;
    resetRGB();

    if (beatt >= 8) beatt = 0;
    switch (beatt)
    {
      case 0:
        RGBW1strip.r = brtrgbw;
        break;
      case 2:
        RGBW1strip.r = brtrgbw;
        break;
      case 4:
        RGBW2strip.b = brtrgbw;
        break;
      case 6:
        RGBW2strip.b = brtrgbw;
        break;
    }
    beatt++;
  }
}

void twofasttwoslow()
{
  if (animdelayerrgbw >= (rythmdelayer / 6))
  {
    animdelayerrgbw = 0;
    resetRGB();

    if (beatt >= 12) beatt = 0;
    switch (beatt)
    {
      case 0:
        RGBW1strip.g = brtrgbw;
        break;
      case 2:
        RGBW1strip.g = brtrgbw;
        break;
      case 4:
        RGBW2strip.r = brtrgbw;
        break;
      case 5:
        RGBW2strip.r = brtrgbw;
        break;
      case 8:
        RGBW2strip.r = brtrgbw;
        break;
      case 9:
        RGBW2strip.r = brtrgbw;
        break;
    }
    beatt++;
  }
}


void hueflash()
{
  if (animdelayerrgbw >= (rythmdelayer / 4))
  {
    animdelayerrgbw = 0;
    resetRGB();

    if (beatt >= 12) beatt = 0;
    switch (beatt)
    {
      case 0:
        RGBW1strip = CHSV( globHue - 64, 255, 255);
        break;
      case 2:
        RGBW2strip = CHSV( globHue + 64, 255, 255);
        break;
      case 4:
        RGBW1strip = CHSV( globHue - 64, 255, 255);
        break;
      case 5:
        RGBW2strip = CHSV( globHue + 64, 255, 255);
        break;
      case 8:
        RGBW1strip = CHSV( globHue - 64, 255, 255);
        break;
      case 9:
        RGBW2strip = CHSV( globHue + 64, 255, 255);
        break;
    }
    beatt++;
  }
}

void dnb()
{
  if (animdelayerrgbw >= (rythmdelayer / 4))
  {
    animdelayerrgbw = 0;
    resetRGB();

    if (beatt >= 14) beatt = 0;
    switch (beatt)
    {
      case 0:
        RGBW1strip = CHSV( globHue - 64, 255, 255);
        break;
      case 2:
        RGBW1strip = CHSV( globHue + 64, 255, 255);
        break;
      case 4:
        RGBW2strip = CHSV( globHue - 64, 255, 255);
        break;
      case 8:
        RGBW1strip = CHSV( globHue + 64, 255, 255);
        break;
      case 10:
        RGBW2strip = CHSV( globHue - 64, 255, 255);
        break;
    }
    beatt++;
  }
}

void RGBW_Woffsetfade()
{
  RGBW1strip = CHSV( globHue - 64, 255, brtrgbw);
  RGBW2strip = CHSV( globHue + 64, 255, brtrgbw);
}

void CWflash()
{
  if (animdelayerrgbw >= (rythmdelayer / 2))
  {
    animdelayerrgbw = 0;
    resetRGB();

    if (beatt >= 2) beatt = 0;
    switch (beatt)
    {
      case 0:
        RGBW1strip.r = brtrgbw;
        RGBW1strip.g = brtrgbw;
        RGBW1strip.b = brtrgbw;
        break;
      case 1:
        RGBW2strip.r = brtrgbw;
        RGBW2strip.g = brtrgbw;
        RGBW2strip.b = brtrgbw;
        break;
    }
    beatt++;
  }
}

void steadywhite()
{
  RGBW1strip.r = brtrgbw;
  RGBW1strip.g = brtrgbw;
  RGBW1strip.b = brtrgbw;
  RGBW2strip.r = brtrgbw;
  RGBW2strip.g = brtrgbw;
  RGBW2strip.b = brtrgbw;
}
