#include <MemoryFree.h>
#include "Menu_Test.h"

// Global menu data and definitions
uint8_t fruit = 2;
bool bValue = true;
int8_t  int8Value = 99;
int16_t int16Value = 999;
int32_t int32Value = 9999;
float floatValue = 1.50;
MD_Menu::value_t engValue = { 1500, 0 };
char myText[] = "192.168.1.101";

const uint8_t EXT_PIN = A0;     // reads the value from here for INP_EXT testing

MD_Menu::value_t valueBuffer;  // interface buffer for values

// Menu Headers --------
// ID, Label, First mnuItem_t ID, Last mnuItem_t ID, CurrentItem (only used for internal track keeping so set to 0)
const PROGMEM MD_Menu::mnuHeader_t menuHeader[] =
{
  { 10, "MD_Menu",      10, 16, 0 },
  { 11, "Input Data",   20, 29, 0 },
  { 12, "Serial Setup", 30, 33, 0 },
  { 13, "LED Menu",     40, 41, 0 },
  { 14, "FF Menu",      50, 51, 0 },
  { 15, "Realtime FB",  60, 64, 0 },
  { 16, "User Text",    70, 70, 0 },
};

// Menu Items ----------
// ID, Label, 
// mnuAction_t = action to perform (Open a (sub)menu / change input value / input value with real time feedback, 
// ID of either the mnuHeader_t or menuInputsut_t depending on mnuAction_t
const PROGMEM MD_Menu::mnuItem_t menuItems[] =
{
  // Starting (Root) menu
  { 10, "Input Test",  MD_Menu::MNU_MENU, 11 },
  { 11, "Serial",      MD_Menu::MNU_MENU, 12 },
  { 12, "LED",         MD_Menu::MNU_MENU, 13 },
  { 13, "More Menu",   MD_Menu::MNU_MENU, 10 },
  { 14, "Flip-Flop",   MD_Menu::MNU_MENU, 14 },
  { 15, "Realtime FB", MD_Menu::MNU_MENU, 15 },
  { 16, "User Text",   MD_Menu::MNU_MENU, 16 },

  // Input Data submenu
  { 20, "Fruit List", MD_Menu::MNU_INPUT, 10 },
  { 21, "Boolean",    MD_Menu::MNU_INPUT, 11 },
  { 22, "Integer 8",  MD_Menu::MNU_INPUT, 12 },
  { 23, "Integer 16", MD_Menu::MNU_INPUT, 13 },
  { 24, "Integer 32", MD_Menu::MNU_INPUT, 14 },
  { 25, "Hex 16",     MD_Menu::MNU_INPUT, 15 },
  { 26, "Float",      MD_Menu::MNU_INPUT, 16 },
  { 27, "Eng Unit",   MD_Menu::MNU_INPUT, 17 },
  { 28, "External",   MD_Menu::MNU_INPUT, 18 },
  { 29, "Reset Menu", MD_Menu::MNU_INPUT, 19 },

  // Serial Setup
  { 30, "COM Port",  MD_Menu::MNU_INPUT, 30 },
  { 31, "Speed",     MD_Menu::MNU_INPUT, 31 },
  { 32, "Parity",    MD_Menu::MNU_INPUT, 32 },
  { 33, "Stop Bits", MD_Menu::MNU_INPUT, 33 },

  // LED
  { 40, "Turn Off", MD_Menu::MNU_INPUT, 40 },
  { 41, "Turn On",  MD_Menu::MNU_INPUT, 41 },

  // Flip-flop - boolean controls variable edit
  { 50, "Flip", MD_Menu::MNU_INPUT, 50 },
  { 51, "Flop", MD_Menu::MNU_INPUT, 51 },

  // Realtime feedback variable edit
  { 60, "Fruit List", MD_Menu::MNU_INPUT_FB, 10 },
  { 61, "Boolean",    MD_Menu::MNU_INPUT_FB, 11 },
  { 62, "Integer 8",  MD_Menu::MNU_INPUT_FB, 12 },
  { 63, "Float",      MD_Menu::MNU_INPUT_FB, 16 },
  { 64, "Eng Unit",   MD_Menu::MNU_INPUT_FB, 17 },

  // Output Data submenu
  { 70, "User Text",  MD_Menu::MNU_INPUT, 60 },
};

// Input Items ---------
const PROGMEM char listFruit[] = "Apple|Pear|Orange|Banana|Pineapple|Peach";
const PROGMEM char listCOM[] = "COM1|COM2|COM3|COM4";
const PROGMEM char listBaud[] = "9600|19200|57600|115200";
const PROGMEM char listParity[] = "O|E|N";
const PROGMEM char listStop[] = "0|1";
const PROGMEM char engUnit[] = "Hz";

// ID, Label, inputAction_t = type of input, callback for get/set of value, width of field on display, 
// min value, power min value, max value, power max value, base, list pointer vfor lists or engineering units only 
const PROGMEM MD_Menu::mnuInput_t menuInputs[] =
{
  { 10, "List",     MD_Menu::INP_LIST,  mnuListValueRqst, 9,       0, 0,      0, 0,  0, listFruit }, // shorter and longer list labels
  { 11, "Bool",     MD_Menu::INP_BOOL,  mnuBoolValueRqst, 1,       0, 0,      0, 0,  0, nullptr },
  { 12, "Int8",     MD_Menu::INP_INT,   mnuIntValueRqst,  4,    -128, 0,    127, 0, 10, nullptr },
  { 13, "Int16",    MD_Menu::INP_INT,   mnuIntValueRqst,  4,  -32768, 0,  32767, 0, 10, nullptr },  // test field too small
  { 14, "Int32",    MD_Menu::INP_INT,   mnuIntValueRqst,  6,  -65536, 0,  65535, 0, 10, nullptr },
  { 15, "Hex16",    MD_Menu::INP_INT,   mnuIntValueRqst,  4,  0x0000, 0, 0xffff, 0, 16, nullptr },  // test hex display
  { 16, "Float",    MD_Menu::INP_FLOAT, mnuFloatValueRqst,7,  -10000, 0,  99950, 0, 10, nullptr },  // test float number
  { 17, "EU",       MD_Menu::INP_ENGU,  mnuEngValueRqst,  7,   -1100, 0,   1500, 0, 50, engUnit },  // test engineering units number
  { 18, "Extern",   MD_Menu::INP_EXT,   mnuExtValueRqst,  6,  -65536, 0,  65535, 0, 10, nullptr },  // test externally provided data
  { 19, "Confirm",  MD_Menu::INP_RUN,   myCode,           0,       0, 0,      0, 0, 10, nullptr },

  { 30, "Port",     MD_Menu::INP_LIST, mnuSerialValueRqst, 4, 0, 0, 0, 0, 0, listCOM },
  { 31, "Bits/s",   MD_Menu::INP_LIST, mnuSerialValueRqst, 6, 0, 0, 0, 0, 0, listBaud },
  { 32, "Parity",   MD_Menu::INP_LIST, mnuSerialValueRqst, 1, 0, 0, 0, 0, 0, listParity },
  { 33, "No. Bits", MD_Menu::INP_LIST, mnuSerialValueRqst, 1, 0, 0, 0, 0, 0, listStop },

  { 40, "Confirm", MD_Menu::INP_RUN, myLEDCode, 0, 0, 0, 0, 0, 0, nullptr },  // test using index in run code
  { 41, "Confirm", MD_Menu::INP_RUN, myLEDCode, 0, 0, 0, 0, 0, 0, nullptr },

  { 50, "Flip",    MD_Menu::INP_INT, mnuFFValueRqst, 4, -128, 0, 127, 0, 10, nullptr },
  { 51, "Flop",    MD_Menu::INP_INT, mnuFFValueRqst, 4, -128, 0, 127, 0, 16, nullptr },

  { 60, "Text",    MD_Menu::INP_RUN,  myCode,  0, 0, 0, 0, 0, 10, nullptr },  // test output TXT
};

// bring it all together in the global menu object
MD_Menu MainMenu(navigation, display,        // user navigation and display
          menuHeader, ARRAY_SIZE(menuHeader), // menu header data
          menuItems, ARRAY_SIZE(menuItems), // menu item data
          menuInputs, ARRAY_SIZE(menuInputs));// menu input data

// Callback code for menu set/get input values
MD_Menu::value_t *mnuListValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for list selection variable
{
  MD_Menu::value_t *r = &valueBuffer;

  if (id == 10)
  {
    if (reqType == MD_Menu::REQ_GET) {
      valueBuffer.value = fruit;
    }
    else if(reqType == MD_Menu::REQ_SET) {
      fruit = valueBuffer.value;
    }
    else if(MD_Menu::REQ_UPD) {
      Serial.print(F("\nList index changed to "));
      Serial.print(valueBuffer.value);
    }
  }
  else
    r = nullptr;

  return(r);
}

MD_Menu::value_t *mnuBoolValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for boolean variable
{
  MD_Menu::value_t *r = &valueBuffer;

  if (id == 11)
  {
    if (reqType == MD_Menu::REQ_GET) {
      valueBuffer.value = bValue;
    }
    else if(reqType == MD_Menu::REQ_SET) {
      bValue = valueBuffer.value;
    }
    else if(MD_Menu::REQ_UPD) {
      Serial.print(F("\nBoolean changed to "));
      Serial.print(valueBuffer.value);
    }
  }
  else
    r = nullptr;
  
  return(r);
}

MD_Menu::value_t *mnuIntValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for integers variables
{
  MD_Menu::value_t *r = &valueBuffer;

  switch (id)
  {
    case 12:
      if (reqType == MD_Menu::REQ_GET) {
        valueBuffer.value = int8Value;
      }
      else if (reqType == MD_Menu::REQ_SET)
      {
        int8Value = valueBuffer.value;
      }    
      else if(MD_Menu::REQ_UPD) {
        Serial.print(F("\nInt8 value changed to "));
        Serial.print(valueBuffer.value);
      }
      break;

    case 13:
    case 15:
      if (reqType == MD_Menu::REQ_GET) {
        valueBuffer.value = int16Value;
      }
      else if (reqType == MD_Menu::REQ_SET) {
        int16Value = valueBuffer.value;
      }      
      else if(MD_Menu::REQ_UPD) {
        Serial.print(F("\nInt16 value changed to "));
        Serial.print(valueBuffer.value);
      }
      break;

    case 14:
      if (reqType == MD_Menu::REQ_GET) {
        valueBuffer.value = int32Value;
      }
      else if (reqType == MD_Menu::REQ_SET) {
        int32Value = valueBuffer.value;
        Serial.print(int32Value);
      }
      else if(MD_Menu::REQ_UPD) {
        Serial.print(F("\nInt32 value changed to "));
        Serial.print(valueBuffer.value);
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return (r);
}

MD_Menu::value_t* mnuExtValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for external 32 bit integers
{
  MD_Menu::value_t* r = &valueBuffer;

  switch (id)
  {
  case 18:
     if (reqType == MD_Menu::REQ_GET)
    {
      valueBuffer.value = analogRead(EXT_PIN);
      Serial.print(F("\nProviding ext value "));
      Serial.print(valueBuffer.value);
    }
    else if (reqType == MD_Menu::REQ_SET)
    {
      int32Value = valueBuffer.value;
      Serial.print(F("\nExt value set to "));
      Serial.print(int32Value);
    }
    break;

  default:
    r = nullptr;
    break;
  }

  return (r);
}

MD_Menu::value_t *mnuSerialValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for Serial parameters
{
  static uint8_t port = 0, speed = 0, parity = 0, stop = 0;
  MD_Menu::value_t *r = &valueBuffer;

  switch (id)
  {
    case 30:
       if (reqType == MD_Menu::REQ_GET)
        valueBuffer.value = port;
      else if (reqType == MD_Menu::REQ_SET)
      {
        port = valueBuffer.value;
        Serial.print(F("\nPort index="));
        Serial.print(port);
      }
      break;

    case 31:
      if (reqType == MD_Menu::REQ_GET)
        valueBuffer.value = speed;
      else if (reqType == MD_Menu::REQ_SET)
      {
        speed = valueBuffer.value;
        Serial.print(F("\nSpeed index="));
        Serial.print(speed);
      }
      break;

    case 32:
      if (reqType == MD_Menu::REQ_GET)
        valueBuffer.value = parity;
      else if (reqType == MD_Menu::REQ_SET)
      {
        parity = valueBuffer.value;
        Serial.print(F("\nParity index="));
        Serial.print(parity);
      }
      break;

    case 33:
      if (reqType == MD_Menu::REQ_GET)
        valueBuffer.value = stop;
      else if (reqType == MD_Menu::REQ_SET)
      {
        stop = valueBuffer.value;
        Serial.print(F("\nStop index="));
        Serial.print(stop);
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return (r);
}

MD_Menu::value_t *mnuFloatValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for floating value
{
  MD_Menu::value_t *r = &valueBuffer;

  if (id == 16)
  {
    if (reqType == MD_Menu::REQ_GET)
      valueBuffer.value = (uint32_t)(floatValue * 100.0);
    else if (reqType == MD_Menu::REQ_SET)
    {
      floatValue = (valueBuffer.value / 100.0);
      Serial.print(F("\nFloat changed to "));
      Serial.print(floatValue);
    }
  }
  else
    r = nullptr;

  return(r);
}

MD_Menu::value_t *mnuEngValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for engineering value
{
  if (id == 17)
  {
    if (reqType == MD_Menu::REQ_GET)
      return(&engValue);
    else if (reqType == MD_Menu::REQ_SET)
    {
      float f = (engValue.value / 1000.0);
      Serial.print(F("\nEng Unit changed to "));
      Serial.print(f);
      Serial.print("x10^");
      Serial.print(engValue.power);
    }
  }

  return(nullptr);
}

MD_Menu::value_t *mnuFFValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value edit allowed request depends on another value
{
  static bool gateKeeper = false;
  MD_Menu::value_t *r = &valueBuffer;

  switch (id)
  {
    case 50:
      if (reqType == MD_Menu::REQ_GET)
      {
        if (gateKeeper)
        {
          Serial.print(F("\nFlipFlop value blocked"));
          r = nullptr;
        }
        else
          valueBuffer.value = int8Value;
      }
      else if (reqType == MD_Menu::REQ_SET)
      {
        int8Value = valueBuffer.value;
        Serial.print(F("\nFlipFlop value changed to "));
        Serial.print(int8Value);
        gateKeeper = !gateKeeper;
      }
      break;

    case 51:
      if (reqType == MD_Menu::REQ_GET)
      {
        if (!gateKeeper)    // reverse the logic of above
        {
          Serial.print(F("\nFlipFlop value blocked"));
          r = nullptr;
        }
        else
          valueBuffer.value = int8Value;
      }
      else if (reqType == MD_Menu::REQ_SET)
      {
        int8Value = valueBuffer.value;
        Serial.print(F("\nFlipFlop value changed to "));
        Serial.print(int8Value);
        gateKeeper = !gateKeeper;
      }
      break;

    default:
      r = nullptr;
      break;
  }

  return(r);
}

MD_Menu::value_t *mnuFBValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback using real time feedback selection variable
{
  MD_Menu::value_t *r = &valueBuffer;

  switch (id)
  {
  case 60:
    if (reqType == MD_Menu::REQ_GET)
      valueBuffer.value = int8Value;
    else if (reqType == MD_Menu::REQ_SET)
    {
      int8Value = valueBuffer.value;
      Serial.print(F("\nUint8 value changed to "));
      Serial.print(int8Value);
    }
    break;

  case 61:
    if (reqType == MD_Menu::REQ_GET)
    {
      valueBuffer.value = fruit;
      return(&valueBuffer);
    }
    else if (reqType == MD_Menu::REQ_SET)
    {
      fruit = valueBuffer.value;
      Serial.print(F("\nFruit index changed to "));
      Serial.print(fruit);
    }
    break;

  default:
    r = nullptr;
    break;
  }

  return(r);
}

MD_Menu::value_t *myCode(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for run code input
{
  switch (id)
  {
    case 19:
      Serial.print(F("\nmyCode called id="));
      Serial.print(id);
      Serial.print(F(" to "));
      Serial.print(reqType ? F("GET") : F("SET - reset menu"));

      if (!reqType) MainMenu.reset();
      break;
  
    case 60:
      if (reqType) return ((MD_Menu::value_t *)&myText);
      break;
  }
  return (nullptr);
}

MD_Menu::value_t *myLEDCode(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType)
// Value request callback for run code input
// reqType is used to determine if a confirmation is required. Returning a nullptr 
// requires confirm, anything else does not.
{
  MD_Menu::value_t* r = &valueBuffer;

  if (reqType)
  {
    if (id == 40) r = nullptr;  // only apply confirm to turn off
  }
  else
  {
    digitalWrite(LED_PIN, id == 40 ? LOW : HIGH);
  }

  return(r);
}

// Standard setup() and loop()
void setup(void)
{
  Serial.begin(BAUD_RATE);
  pinMode(LED_PIN, OUTPUT);

  display(MD_Menu::DISP_INIT);
  setupNav();

  MainMenu.begin();
  MainMenu.setMenuWrap(true);
  MainMenu.setAutoStart(AUTO_START);
  MainMenu.setTimeout(MENU_TIMEOUT);
}

void loop(void)
{
  static bool prevMenuRun = true;

  // Detect if we need to initiate running normal user code
  if (prevMenuRun && !MainMenu.isInMenu()) {

    Serial.print("\n\nSWITCHING TO USER'S NORMAL OPERATION\n");
  }
  if (!prevMenuRun && MainMenu.isInMenu()) {

    Serial.print("\n\nSWITCHING TO RUNNING MENU\n");
  }
  prevMenuRun = MainMenu.isInMenu();

  
  // If we are not running and not autostart
  // check if there is a reason to start the menu
  if (!MainMenu.isInMenu() && !AUTO_START)
  {
    uint16_t dummy;

    if (navigation() == MD_Menu::NAV_SEL)
      MainMenu.runMenu(true);
  }

  MainMenu.runMenu();   // Must run in loop in order to work
}
