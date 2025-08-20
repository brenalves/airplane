#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <string>

#define MENU_LCD_ADDRESS 0x27
#define MENU_LCD_COL 20
#define MENU_LCD_ROW 4

#define MENU_BTN1_PIN 12
#define MENU_BTN2_PIN 14
#define MENU_BTN3_PIN 27
#define MENU_BTN4_PIN 26
#define MENU_BTN5_PIN 33
#define MENU_BTN6_PIN 32

#define MENU_BUTTON_DEBOUNCE_TIME 200

#define MENU_STS_SCROLL_COUNT 2
#define MENU_NAV_SCROLL_COUNT 2
#define MENU_FPL_SCROLL_COUNT 2
#define MENU_PID_SCROLL_COUNT 0

enum Page
{
    STS,
    NAV,
    FPL,
    PID
};

class Menu
{
public:
    Menu();
    ~Menu();

    void setup();
    void update(unsigned long millis);

private:
    void drawStaticPage(Page page);
    void updateTelemetry();
    void scrollUpPage();
    void scrollDownPage();

    static int getPageScrollCount(Page page);
    static std::string stringifyPage(Page page);

private:
    LiquidCrystal_I2C m_LCD;
    Page m_currentPage;

    int m_scrollStep;

    unsigned long m_lastBounceMillis;
};