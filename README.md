# MD_Menu

###I changed a few things and added a menu option for time based values since I needed that for my project.

#####Find the original documentation below.



[![arduino-library-badge](https://www.ardu-badge.com/badge/MD_Menu.svg?)](https://www.ardu-badge.com/MD_Menu)

This is a menu management library created as a front end to set parameters in embedded hardware control applications, leaving the back end under application control. It is suitable for text based displays (eg, LCD modules) with 1 or 2 lines available for display.

Menu managers in embedded systems are generally not the main function of the application software, so this library minimizes the use of RAM and has a small memory footprint overall, leaving more space 
for what really matters.

The library allows user code to define:
- Static menu definitions to minimized RAM footprint. 
- Callbacks for navigation and display control.
- Menu inactivity timeout.
- Auto start on key press, or manual start by user code.
- Input methods are available for
  - Boolean (Y/N) values.
  - Pick List selection.
  - Signed integers.
  - Time values (minutes, hours, days, seconds).
  - Decimal floating point.
  - Engineering units.

If you like and use this library please consider making a small donation using [PayPal](https://paypal.me/MajicDesigns/4USD)

[Library Documentation](https://MajicDesigns.github.io/MD_Menu/)
