const int in1 = 5;
const int in2 = 6;
String incoming;
int incoming_int;
int mapped;
void setup() {
  Serial.begin(115200);
  //Serial.println("Give a value between -10 and 10");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
}

void loop() {
  if (Serial.available() > 0){
    incoming = Serial.readString();
    Serial.print("I recived: ");
    Serial.println(incoming);
    incoming_int = incoming.toInt();
    Serial.print("as an int: ");
    Serial.println(incoming_int);
    if (incoming_int > 0){
      mapped = map(incoming_int, 0, 10, 0, 255);
      analogWrite(in1, mapped);
      digitalWrite(in2, LOW);
    }else if (incoming_int < 0 ){
      mapped = map(incoming_int, 0, -10, 0, 255);
      digitalWrite(in1, LOW);
      analogWrite(in2, mapped);
    }else{
      digitalWrite(in1, HIGH);
      digitalWrite(in2, HIGH);
    }

    
  }

}
