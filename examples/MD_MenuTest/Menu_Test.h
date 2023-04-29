#pragma once

#include <MD_Menu.h>

const bool AUTO_START = true; // auto start the menu, manual detect and start if false

const uint32_t BAUD_RATE = 57600;   // Serial Monitor speed setting 
const uint16_t MENU_TIMEOUT = 5000; // in milliseconds

const uint8_t LED_PIN = LED_BUILTIN;  // for myLEDCode function

// Function prototypes for Navigation/Display
void setupNav(void);
bool display(MD_Menu::userDisplayAction_t, char* = nullptr);
MD_Menu::userNavAction_t navigation();

// Function prototypes for variable get/set functions
// These are split for demonstration purposes. They could all be in one function.
MD_Menu::value_t *mnuListValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuBoolValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuIntValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t* mnuExtValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuFloatValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuEngValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuFFValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuFBValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *mnuSerialValueRqst(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *myCode(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);
MD_Menu::value_t *myLEDCode(MD_Menu::mnuId_t id, MD_Menu::requestType_t reqType);

