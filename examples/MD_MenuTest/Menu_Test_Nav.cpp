#include "Menu_Test.h"
#include <OneButton.h>

#define BUTTON_LEFT 7
#define BUTTON_RIGHT 8
#define BUTTON_ENTER 9
#define BUTTON_ESCAPE 10

OneButton ButtonLeft = OneButton(BUTTON_LEFT, true, true);
OneButton ButtonRight = OneButton(BUTTON_RIGHT, true, true);
OneButton ButtonEnter = OneButton(BUTTON_ENTER, true, true);
OneButton ButtonEscape = OneButton(BUTTON_ESCAPE, true, true);

MD_Menu::userNavAction_t buttonNav = MD_Menu::NAV_NULL;

void setupNav(void)
{
  ButtonLeft.attachClick([] { buttonNav = MD_Menu::NAV_DEC; });
  ButtonLeft.attachLongPressStart([] { buttonNav = MD_Menu::NAV_ESC; });
  ButtonRight.attachClick([] { buttonNav = MD_Menu::NAV_INC; });
  ButtonRight.attachLongPressStart([] { buttonNav = MD_Menu::NAV_SEL; });
  ButtonEnter.attachClick([] { buttonNav = MD_Menu::NAV_SEL; });
  ButtonEscape.attachClick([] { buttonNav = MD_Menu::NAV_ESC; });
}

MD_Menu::userNavAction_t navigation()
{
  ButtonLeft.tick();
  ButtonRight.tick();
  ButtonEnter.tick();
  ButtonEscape.tick();

  MD_Menu::userNavAction_t nav = buttonNav;
  buttonNav = MD_Menu::NAV_NULL;
  return nav;
}