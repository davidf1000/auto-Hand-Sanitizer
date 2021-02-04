/* Mini Project Sistem Instrumentasi EL3013 K01
 * Nama : David Fauzi
 * NIM  : 13218043 
 * Nama Project : Automatic Hand Sanitizer Dispenser With Liquid Level Indicator 
 * Deskripsi : program untuk mengambil data sampel pengukuran dari sensor
 */
// Define Macro 

#define echoPin 9 
#define trigPin 10 

double distance; // variable for the distance measurement
double deltaSmallest=1000000;
double distanceLast=0;
double delta;
void setup()
{
  Serial.begin(9600);
  Serial.print("Program Mulai");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void loop()
{
    digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  distance =  pulseIn(echoPin, HIGH)*0.0343/2;
  Serial.print(distance,8);
Serial.println();
  delay(200);
}
