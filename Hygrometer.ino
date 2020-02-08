#include <DHT.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

#define DHTPIN 7

// the sensor type: DHT11 or DHT22
#define DHTTYPE DHT22
//5 Minutes intervall
#define INTERVALL_SECONDS 300000

// BUZZER PIN
const int BUZZER = 8;

// RECEIVER PIN
const int IRRECVPIN = 9;

// Available modes
const int SHOW_MODE = 1;
const int ALARM_MODE = 2;

int currentMode = SHOW_MODE;

int humidityThreshold = 60;

byte AlarmIcon[8] = {
        0b00100,
        0b01110,
        0b01110,
        0b01110,
        0b11111,
        0b00000,
        0b00100,
        0b00000
};

// irrecv(pin)
IRrecv irrecv(IRRECVPIN);

// dht(pin, type)
DHT dht(DHTPIN, DHTTYPE);

// LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

decode_results irResults;
unsigned long timePreChoicePressed = 0;

// last alarm time
unsigned long lastAlarmTime = 0;
// current alarm time
unsigned long currAlarmTime;

void showModeOnLcd();

void setup() {
    Serial.begin(9600);
    dht.begin();

    lcd.begin(16, 2);
    lcd.createChar(0, AlarmIcon);
    lcd.setCursor(0, 0);
    lcd.print("IgorMeter");
    lcd.setCursor(0, 1);
    lcd.print("by Aliss");
    
    delay(3000);

    pinMode(BUZZER, OUTPUT);

    irrecv.enableIRIn();
    irrecv.blink13(true);
}

void loop() {

    delay(2000);
    noTone(BUZZER);

    // no FUNC following command 
    // in the following 4 seconds 
    if(timePreChoicePressed != 0 
      && (timePreChoicePressed + 8000) < millis()) {
        timePreChoicePressed = 0;
      }

    if (irrecv.decode(&irResults)){
        Serial.println(irResults.value, HEX);
        irrecv.resume();

        switch(irResults.value){
            case 0xFF6897:
                Serial.println("Pressed 0");
                showCredits();
                break;
            case 0xFF30CF:
                Serial.println("Pressed 1");
                currentMode = SHOW_MODE;
                timePreChoicePressed = 0;
                break;
            case 0xFF18E7:
                Serial.println("Pressed 2");
                currentMode = ALARM_MODE;
                timePreChoicePressed = 0;
                break;
            case 0xFFE21D:
                Serial.println("Pressed FUNC");
                timePreChoicePressed = millis();
                Serial.println(timePreChoicePressed);
                break;
            case 0xFF22DD:
                Serial.println("Pressed <<");
                if (timePreChoicePressed != 0 ) {
                    humidityThreshold--;
                } else {
                    timePreChoicePressed = 0;
                }
                break;
            case 0xFFC23D:
                Serial.println("Pressed >>");
                if (timePreChoicePressed != 0 ) {
                    humidityThreshold++;
                } else {
                    timePreChoicePressed = 0;
                }
                break;
            default:
                timePreChoicePressed = 0;
        }
    }

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    //printOnConsole(h, t);
    showOnLcd(h, t);

    // play a sound if the humidity value is over the threshold
    // and the currentMode is equal alarm_mode
    if(currentMode == ALARM_MODE && h >= humidityThreshold ) {
        currAlarmTime = millis();
        if(lastAlarmTime == 0 || currAlarmTime - lastAlarmTime >= INTERVALL_SECONDS) {
            lastAlarmTime = currAlarmTime;

            int i =0;
            while (currentMode == ALARM_MODE && i < 5) {
                tone(BUZZER, 1000); //play sound
                delay(1000); //for 1 second
                noTone(BUZZER); //stop sound
                delay(1000);  //for 1 second

                i++;
            }

            noTone(BUZZER); //stop sound

        }
    }
}

void printOnConsole(float h, float t) {
    Serial.print("Temperature = ");
    Serial.println(t);
    Serial.print("Humidity = ");
    Serial.println(h);
    Serial.println(millis());
}

void showOnLcd(float h, float t) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("RH: ");
    lcd.print(h);
    lcd.print(" %");
    showModeOnLcd();
}

void showModeOnLcd() {
    if (currentMode == ALARM_MODE) {
        lcd.setCursor(15, 0);
        lcd.write(byte(0));
        lcd.setCursor(13, 1);
        lcd.print(humidityThreshold);
        lcd.print("%");
    }
}

void showCredits() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IgorMeter ");
  lcd.setCursor(0, 1);
  lcd.print("by Aliss");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 to SHOW mode");
  lcd.setCursor(0, 1);
  lcd.print("2 to ALARM mode");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Func + << or >>");
  lcd.setCursor(0, 1);
  lcd.print("change threshold");
  delay(5000);
}
