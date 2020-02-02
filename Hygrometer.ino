#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 7
#define DHTTYPE DHT11
#define MAX_HUMIDITY 65
#define INTERVALL_SECONDS 300000 //5 Minutes

const int buzzer = 8;

// initialize sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//last alarm time
unsigned long lastAlarmTime = 0;

//current alarm time
unsigned long currAlarmTime;

void setup() {
  Serial.begin(9600);
  dht.begin();
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hi Hygrometer!");
  delay(3000);

  pinMode(buzzer, OUTPUT);
  
}

void loop() {

  delay(2000);
  noTone(buzzer);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //TODO: function
  lcd.setCursor(0, 0);
  lcd.print("Temperature = ");
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity = ");
  lcd.print(h);

  //TODO: function
  Serial.print("Temperature = ");
  Serial.println(t);
  Serial.print("Humidity = ");
  Serial.println(h);

  // if the humitidy is bigger than the max acceppted
  if( h >= MAX_HUMIDITY ) {
    currAlarmTime = millis();
    if(lastAlarmTime == 0 || currAlarmTime - lastAlarmTime >= INTERVALL_SECONDS) {
      lastAlarmTime = currAlarmTime;

      for ( int i = 0; i<5; i++) {
        tone(buzzer, 1000); //play sound 
        delay(1000); //for 1 second
        noTone(buzzer); //stop sound
        delay(1000);  //for 1 second
      }

      noTone(buzzer);
        
    }
  }

  Serial.print("New cicle: ");
  Serial.println(millis());
}
