int soft_push_signals[] = {5, 7};
int hard_push_signals[] = {4, 6};
int synchro_signal = 3;
int switch_signal = 2;
const byte sequence_length = 20;
//Za pomocą wait_time reguluje się czas między wykonaniem pary zdjęć
const unsigned long wait_time = 2000;
volatile byte light_states[][2] = {{33, 0}, {2, 1}, {33, 0}, {4, 1}, {33, 0}, {6, 1}, {33, 0}, {8, 1}, {33, 0}, {10, 1}, {33, 0}, {12, 1}, 
                                  {33, 0}, {14, 1}, {33, 0}, {16, 1}, {33, 0}, {18, 1}, {33, 0}, {20, 1}, {33, 0}, {22, 1}, {33, 0}, {24, 1}, 
                                  {33, 0}, {26, 1}, {33, 0}, {28, 1}, {33, 0}, {0, 3}, {33, 0}, {2, 3}, {33, 0}, {4, 3}, {33, 0}, {6, 3}, 
                                  {33, 0}, {8, 3}, {33, 0}, {10, 3}, {33, 0}, {12, 3}, {33, 0}, {16, 3}, {33, 0}, {18, 3}, {33, 0}, {20, 3}, 
                                  {33, 0}, {24, 3}, {33, 0}, {0, 5}, {33, 0}, {2, 5}, {33, 0}, {4, 5}, {33, 0}, {6, 5}, {33, 0}, {8, 5}, 
                                  {33, 0}, {10, 5}, {33, 0}, {12, 5}, {33, 0}, {16, 5}, {33, 0}, {18, 5}, {33, 0}, {20, 5}, {33, 0}, {24, 5}, 
                                  {33, 0}, {0, 7}, {33, 0}, {2, 7}, {33, 0}, {4, 7}, {33, 0}, {8, 7}, {33, 0}, {16, 7}, {33, 0}, {0, 9}, 
                                  {33, 0}, {2, 9}, {33, 0}, {4, 9}, {33, 0}, {6, 9}, {33, 0}, {8, 9}, {33, 0}, {10, 9}, {33, 0}, {12, 9}, 
                                  {33, 0}, {16, 9}, {33, 0}, {18, 9}, {33, 0}, {20, 9}, {33, 0}, {24, 9}, {33, 0}, {0, 11}, {33, 0}, {2, 11}, 
                                  {33, 0}, {4, 11}, {33, 0}, {8, 11}, {33, 0}, {16, 11}, {33, 0}, {0, 13}, {33, 0}, {2, 13}, {33, 0}, {4, 13}, 
                                  {33, 0}, {8, 13}, {33, 0}, {16, 13}, {33, 0}, {0, 15}, {33, 0}, {0, 33}, {33, 0}, {2, 33}, {33, 0}, {4, 33}, 
                                  {33, 0}, {6, 33}, {33, 0}, {8, 33}, {33, 0}, {10, 33}, {33, 0}, {12, 33}, {33, 0}, {16, 33}, {33, 0}, {18, 33}, 
                                  {33, 0}, {20, 33}, {33, 0}, {24, 33}, {33, 0}, {0, 35}, {33, 0}, {2, 35}, {33, 0}, {4, 35}, {33, 0}, {8, 35}, 
                                  {33, 0}, {16, 35}, {33, 0}, {0, 37}, {33, 0}, {2, 37}, {33, 0}, {4, 37}, {33, 0}, {8, 37}, {33, 0}, {16, 37}, 
                                  {33, 0}, {0, 39}, {33, 0}, {0, 41}, {33, 0}, {2, 41}, {33, 0}, {4, 41}, {33, 0}, {8, 41}, {33, 0}, {16, 41}, 
                                  {33, 0}, {0, 43}, {33, 0}, {0, 45}};
volatile byte sequence_state;
volatile byte process_state;
byte old_sequence_state;
unsigned long start_time;
byte switch_state;


void setup() {
  for(int i = 0; i < 2; i++)
  {
    pinMode(soft_push_signals[i], OUTPUT);
    pinMode(hard_push_signals[i], OUTPUT);
    digitalWrite(soft_push_signals[i], LOW);
    digitalWrite(hard_push_signals[i], LOW);
  }
  DDRB = B11111111;
  DDRC = B11111111;
  PORTB = 0;
  PORTC = 0;
  pinMode(synchro_signal, INPUT_PULLUP);
  pinMode(switch_signal, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(synchro_signal), lights_change, FALLING);
  attachInterrupt(digitalPinToInterrupt(switch_signal), button_off, RISING);
  process_state = 2;
  sequence_state = 0;
}

void loop() {
  if(process_state < 2)
  {
    digitalWrite(soft_push_signals[0], HIGH);
    digitalWrite(soft_push_signals[1], HIGH);
    delay(800);
    while(process_state < 2) 
    {
      old_sequence_state = sequence_state;
      digitalWrite(hard_push_signals[process_state], HIGH);
      while(sequence_state - old_sequence_state == 0)
      {
        if(process_state == 3){
          goto idle_state;
        }
      }
      start_time = millis();
      while(sequence_state - old_sequence_state == 1)
      {
        if(process_state == 3){
          goto idle_state;
        }
      }
      digitalWrite(hard_push_signals[process_state], LOW);
      while(millis() - start_time < wait_time)
      {
        if(process_state == 3){
          goto idle_state;
        }
      }
      if(sequence_state == sequence_length)
      {
        process_state += 1;
        if(process_state == 1)
        {
//          przerwa po zakończeniu sesji aparatem numer 1
          delay(500);
          PORTB = 0;
          PORTC = 0;
          delay(7500);
        }
        sequence_state = 0;
      }
    }
  }
  idle_state:
  digitalWrite(soft_push_signals[0], LOW);
  digitalWrite(hard_push_signals[0], LOW);
  digitalWrite(soft_push_signals[1], LOW);
  digitalWrite(hard_push_signals[1], LOW);
  PORTB = 0;
  PORTC = 0;
  while(process_state == 2)
  {
    delay(5);
  }
  while(process_state == 3)
  {
    PORTB = 0;
    PORTC = 0;
    switch_state = digitalRead(switch_signal);
    if(switch_state == 0)
    {
      process_state = 0;
      sequence_state = 0; 
    }
  }
}

void lights_change() {
  PORTC = light_states[sequence_state][0];
  PORTB = light_states[sequence_state][1];
  sequence_state += 1;
}

void button_off() {
  process_state = 3;
}
