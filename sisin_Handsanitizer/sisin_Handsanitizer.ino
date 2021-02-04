/* Mini Project Sistem Instrumentasi EL3013 K01
 * Nama : David Fauzi
 * NIM  : 13218043 
 * Nama Project : Automatic Hand Sanitizer Dispenser With Liquid Level Indicator 
 * Deskripsi : Program automatic hand sanitizer dispenser dengan liquid level indicator.
 *             Arduino akan menyalakan relay yang terhubung pada pompa DC setiap kali 
 *             sensor IR memberikan keluaran HIGH, yaitu ketika didekati tangan.
 *             Sistem juga akan membaca ketinggian air lalu menampilkan level ketinggian   
 *             cairan hand sanitizer pada layar LCD I2C.
 */
// Define Macro 
#include <LiquidCrystal_I2C.h>
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args, BYTE);
#endif
#define echoPin 9 
#define trigPin 10 
#define relay 2
#define ir 3
#define WINDOW_SIZE 5
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
// Variable
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement
int barLevel;
bool toggleRelay = HIGH;
int count;
void setup()
{
  Serial.begin(9600);
  Serial.print("Program Mulai");
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  introLcd();
  pinMode(ir, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(relay, toggleRelay);
  delay(100);
  showCurrentLevel(0);
}
// Prosedur untuk mengecek apakah sensor IR bernilai LOW atau tidak (Active Low), jika ya, maka sistem akan memompa cairan hand sanitizer
// dengan durasi yang dapat ditentukan
void detectIR(void)
{
  int readIR = digitalRead(ir);
  // Nyalakan Pompa DC selama 300 ms
  if (!readIR)
  {
    digitalWrite(relay, LOW);
    delay(300);
    digitalWrite(relay, HIGH);
    delay(200);
  }
}


// Menampilkan pesan pembuka pada layar LCD pada awal sistem dihidupan
void introLcd(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Automatic Hand");
  lcd.setCursor(0, 1);
  lcd.print("Sanitizer");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("David Fauzi");
  lcd.setCursor(0, 1);
  lcd.print("13218043/K01");
  delay(1000);
}
// menerima input argumen berupa persen ketinggian air lalu menunjukannya pada layar dengan skala 0-10 bar
void showCurrentLevel(int level)
{
  // Kalau tidak berubah tidak perlu melakukan update
  if(count%8==0)
  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Liquid Level :");
  lcd.setCursor(0, 1);
  barLevel = level / 10;
  for (int i = 0; i < barLevel; i++)
  lcd.printByte(255);
  lcd.setCursor(11, 1);
  if(level<0)level=0;
  if(level>100)level=100;
  lcd.print(level);
  lcd.print("%");
  count=0;
  }
  count++;

}
// Membaca ketinggian air 

float readDuration()
{
  float dur;
  float sum=0;
  int n=5;
  for(int i=0;i<n;i++)
  {
      digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  sum += pulseIn(echoPin, HIGH);
  delayMicroseconds(50);
  }
  dur = sum/n;
  return dur;
}
void readWaterLevel()
{
        digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
 
  duration =  pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.0343 / 2; // Speed of sound wave divided by 2 (go and back)
  distance =float((round(distance*2)))/2;
  // Displays the distance on the Serial Monitor
  Serial.print("Jarak yang terbaca : ");
  Serial.print(distance);
  Serial.println(" cm");
  float maxL = 7 ;
  float minL = 2 ;  
  if(distance>maxL)distance=maxL;
  if(distance<minL)distance=minL;
  float percentage = ((distance-maxL)/(minL-maxL))*100; 
  Serial.print(percentage);
  Serial.println(" percent");
  showCurrentLevel(percentage);
}
void loop()
{
  // Sistem akan mengupdate liquid level indicator pada LCD, lalu jika sensor IR ter-trigger atau tidak
  // Jika sensor IR ter-trigger, maka relay akan menyala dan pompa air DC akan memompakan cairan hand sanitizer
  readWaterLevel();
  detectIR();
  delay(100);
}
