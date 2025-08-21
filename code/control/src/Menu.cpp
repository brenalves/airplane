#include "Menu.h"

float lat = 23.0324;
float lon = -54.8579;

Menu::Menu()
    : m_LCD(MENU_LCD_ADDRESS, MENU_LCD_COL, MENU_LCD_ROW), m_currentPage(STS), m_scrollStep(0), m_lastBounceMillis(0), m_lastTelemetryMillis(0)
{
}

Menu::~Menu()
{
    m_LCD.clear();
    m_LCD.setBacklight(false);
}

void Menu::setup()
{
    pinMode(MENU_BTN1_PIN, INPUT_PULLUP);
    pinMode(MENU_BTN2_PIN, INPUT_PULLUP);
    pinMode(MENU_BTN3_PIN, INPUT_PULLUP);
    pinMode(MENU_BTN4_PIN, INPUT_PULLUP);
    pinMode(MENU_BTN5_PIN, INPUT_PULLUP);
    pinMode(MENU_BTN6_PIN, INPUT_PULLUP);

    m_LCD.init();
    m_LCD.setBacklight(true);
    m_LCD.clear();
    m_LCD.setCursor(0, 0);

    byte scrollDown[8] = {B00100, B00100, B00100, B00100, B10101, B01110, B00100, B00000};
    byte scrollUp[8] = {B00100, B01110, B10101, B00100, B00100, B00100, B00100, B00000};
    byte star[8] = {B00100, B10101, B01110, B11111, B01110, B10101, B00100, B00000};
    byte at[8] = {B01110, B10001, B10101, B10101, B10111, B10000, B01110, B00000};

    m_LCD.createChar(0, scrollUp);
    m_LCD.createChar(1, scrollDown);
    m_LCD.createChar(2, star);
    m_LCD.createChar(3, at);

    // Start in Status Page
    drawStaticPage(m_currentPage);
}

void Menu::update(unsigned long millis)
{
    // Just press button when debounce time has been passed
    if (millis - m_lastBounceMillis > MENU_BUTTON_DEBOUNCE_TIME)
    {
        if (digitalRead(MENU_BTN1_PIN) == LOW && m_currentPage != STS)
        {
            m_scrollStep = 0; // Reset scroll step before draw
            drawStaticPage(STS);
            m_currentPage = STS;
            m_lastBounceMillis = millis;
        }
        else if (digitalRead(MENU_BTN2_PIN) == LOW && m_currentPage != NAV)
        {
            m_scrollStep = 0; // Reset scroll step before draw
            drawStaticPage(NAV);
            m_currentPage = NAV;
            m_lastBounceMillis = millis;
        }
        else if (digitalRead(MENU_BTN3_PIN) == LOW && m_currentPage != FPL)
        {
            m_scrollStep = 0; // Reset scroll step before draw
            drawStaticPage(FPL);
            m_currentPage = FPL;
            m_lastBounceMillis = millis;
        }
        else if (digitalRead(MENU_BTN4_PIN) == LOW && m_currentPage != PID)
        {
            m_scrollStep = 0; // Reset scroll step before draw
            drawStaticPage(PID);
            m_currentPage = PID;
            m_lastBounceMillis = millis;
        }

        if (digitalRead(MENU_BTN5_PIN) == LOW)
        {
            scrollDownPage();
        }
        else if (digitalRead(MENU_BTN6_PIN) == LOW)
        {
            scrollUpPage();
        }
    }

    // Update current page with telemetry data
    if (millis - m_lastTelemetryMillis > MENU_UPDATE_TELEMETRY_RATE)
    {
        lat += 0.0001f;
        lon += 0.0002f;
        
        m_lastTelemetryMillis = millis;
        updateTelemetry();
    }
}

void Menu::drawStaticPage(Page page)
{
    m_LCD.clear();
    m_LCD.setCursor(0, 0);

    switch (page)
    {
    case STS:
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.print(" STATUS PAGE ");
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));

        if (m_scrollStep == 0) // Scroll level 0 shows battery and error codes
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("Battery: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Errors: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 2);
            m_LCD.write(byte(1));
        }
        else if (m_scrollStep == 1) // Scroll level 1 shows system status part 1
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("GPS: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Radio: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 1);
            m_LCD.write(byte(0));
            m_LCD.setCursor(19, 2);
            m_LCD.write(byte(1));
        }
        else if (m_scrollStep == 2) // Scroll level 2 shows system status part 2
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("F. Controls: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Sensors: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 1);
            m_LCD.write(byte(0));
        }
        break;

    case NAV:
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.print(" NAV PAGE ");
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));

        if (m_scrollStep == 0) // Scroll level 0 shows lat and lon
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("Lat: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Lon: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 2);
            m_LCD.write(byte(1));
        }
        else if (m_scrollStep == 1) // Scroll level 1 shows alt and speed
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("Alt: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Speed: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 1);
            m_LCD.write(byte(0));
            m_LCD.setCursor(19, 2);
            m_LCD.write(byte(1));
        }
        else if (m_scrollStep == 2) // Scroll level 2 shows heading and satellites number
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("Heading: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Satellites: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 1);
            m_LCD.write(byte(0));
        }
        break;

    case FPL:
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.print(" FLIGHT PLAN ");
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));

        if (m_scrollStep == 0) // Scroll level 0 shows current waypoints from/to and ETA
        {
            m_LCD.setCursor(0, 2);
            m_LCD.print("ETA: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 2);
            m_LCD.write(byte(1));
        }
        else if (m_scrollStep == 1) // Scroll level 1 shows wind info and desired altitude
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("Wind: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Planned Alt: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 1);
            m_LCD.write(byte(0));
            m_LCD.setCursor(19, 2);
            m_LCD.write(byte(1));
        }
        else if (m_scrollStep == 2) // Scroll level 2 shows something idk yet
        {
            m_LCD.setCursor(0, 1);
            m_LCD.print("Planned HDG: ");
            m_LCD.setCursor(0, 2);
            m_LCD.print("Planned ETC: ");

            // Draw scroll arrows
            m_LCD.setCursor(19, 1);
            m_LCD.write(byte(0));
        }
        break;

    case PID:
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.print(" PID CONTROL ");
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));
        m_LCD.write(byte(2));

        // PID page don't have scroll so just print the values

        m_LCD.setCursor(0, 1);
        m_LCD.print("Kp=");
        m_LCD.setCursor(8, 1);
        m_LCD.print("Ki=");
        m_LCD.setCursor(0, 2);
        m_LCD.print("Kd=");
        break;
    }

    m_LCD.setCursor(0, 3);
    for (int i = 0; i < 4; i++)
    {
        m_LCD.print("[");
        if (i == page)
            m_LCD.print("---");
        else
            m_LCD.print(stringifyPage(static_cast<Page>(i)).c_str());
        m_LCD.print("]");
    }
}

void Menu::updateTelemetry()
{
    switch (m_currentPage)
    {
    case STS:
        if (m_scrollStep == 0)
        {
            m_LCD.setCursor(8, 1);
            m_LCD.printf("%d%%", 100); // Battery
            m_LCD.setCursor(7, 2);
            m_LCD.printf("%s", "0b00101"); // Errors
        }
        else if (m_scrollStep == 1)
        {
            m_LCD.setCursor(4, 1);
            m_LCD.print("OK"); // GPS
            m_LCD.setCursor(6, 2);
            m_LCD.print("OK"); // Radio
        }
        else if (m_scrollStep == 2)
        {
            m_LCD.setCursor(12, 1);
            m_LCD.print("FAIL"); // Flight Controls
            m_LCD.setCursor(8, 2);
            m_LCD.print("FAIL"); // Sensors
        }
        break;

    case NAV:
        if (m_scrollStep == 0)
        {
            m_LCD.setCursor(4, 1);
            m_LCD.printf("%.4f", lat); // Lat
            m_LCD.setCursor(4, 2);
            m_LCD.printf("%.4f", lon); // Lon
        }
        else if (m_scrollStep == 1)
        {
            m_LCD.setCursor(4, 1);
            m_LCD.printf("%dm", 325); // Alt
            m_LCD.setCursor(6, 2);
            m_LCD.printf("%dkm/h", 30); // Speed
        }
        else if (m_scrollStep == 2)
        {
            m_LCD.setCursor(8, 1);
            m_LCD.printf("%03d", 90); // Heading
            m_LCD.write(byte(223));
            m_LCD.setCursor(11, 2);
            m_LCD.printf("%d", 16); // Satellites
        }
        break;

    case FPL:
        if (m_scrollStep == 0)
        {   
            m_LCD.setCursor(0, 1);
            m_LCD.printf("%s", "WPT2->WPT3"); // Route
            m_LCD.setCursor(4, 2);
            m_LCD.printf("%02d", 1); // ETA hours
            m_LCD.print(":");
            m_LCD.printf("%02d", 30); // ETA minutes
        }
        else if (m_scrollStep == 1)
        {
            m_LCD.setCursor(5, 1);
            m_LCD.printf("%03d", 90); // Wind direction
            m_LCD.write(byte(3));
            m_LCD.printf("%dkt", 2); // Wind speed
            m_LCD.setCursor(12, 2);
            m_LCD.printf("%dm", 300); // Planned altitude
        }
        else if (m_scrollStep == 2)
        {
            m_LCD.setCursor(12, 1);
            m_LCD.printf("%03d", 91); // Planned Heading
            m_LCD.write(byte(223));
            m_LCD.setCursor(12, 2);
            m_LCD.printf("%02d", 15); // Planned ETC hours
            m_LCD.print(":");
            m_LCD.printf("%02d", 24); // Planned ETC minutes
        }
        break;

    case PID:
        m_LCD.setCursor(3, 1);
        m_LCD.printf("%.2f", 1.20f);    // Kp
        m_LCD.setCursor(11, 1);
        m_LCD.printf("%.2f", 0.05f);    // Ki
        m_LCD.setCursor(3, 2);
        m_LCD.printf("%.2f", 0.80f);    // Kd
        break;
    }
}

void Menu::scrollUpPage()
{
    m_scrollStep--;
    if (m_scrollStep < 0)
        m_scrollStep = 0;

    drawStaticPage(m_currentPage);
}

void Menu::scrollDownPage()
{
    m_scrollStep++;

    int maxStep = getPageScrollCount(m_currentPage);
    if (m_scrollStep > maxStep)
        m_scrollStep = maxStep;

    drawStaticPage(m_currentPage);
}

int Menu::getPageScrollCount(Page page)
{
    switch (page)
    {
    case STS:
        return MENU_STS_SCROLL_COUNT;
    case NAV:
        return MENU_NAV_SCROLL_COUNT;
    case FPL:
        return MENU_FPL_SCROLL_COUNT;
    case PID:
        return MENU_PID_SCROLL_COUNT;
    default:
        return 0;
    }
}

std::string Menu::stringifyPage(Page page)
{
    switch (page)
    {
    case STS:
        return "STS";
    case NAV:
        return "NAV";
    case FPL:
        return "FPL";
    case PID:
        return "PID";
    default:
        return 0;
    }
}
