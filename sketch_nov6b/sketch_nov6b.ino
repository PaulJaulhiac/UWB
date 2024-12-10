void setup() {
  pinMode(LED_GREEN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_GREEN, HIGH);
  Serial.println("HIGH");
  delay(2000);                      
  digitalWrite(LED_GREEN, LOW);   
  Serial.println("LOW");
  delay(2000);
}