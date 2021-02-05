const byte test_lenght = 12;
volatile byte test_portc[] = {1, 2, 4, 8, 16, 32, 0, 0, 0, 0,  0,  0};
volatile byte test_portb[] = {0, 0, 0, 0,  0,  0, 1, 2, 4, 8, 16, 32};

int soft_push_signals[] = {5, 7};
int hard_push_signals[] = {4, 6};
int synchro_signal = 3;
int switch_signal = 2;
const int sequence_length = 273;
//Za pomocą wait_time reguluje się czas między wykonaniem pary zdjęć
const unsigned long shots_wait_time = 3000;
const unsigned long impulse_length = 20;
volatile byte light_states[][2] = {{33, 0}, {2, 1}, {0, 0}, {33, 0}, {4, 1}, {0, 0}, {33, 0}, {6, 1}, {0, 0}, {33, 0}, {8, 1}, {0, 0}, {33, 0}, {10, 1}, {0, 0}, 
                                  {33, 0}, {12, 1}, {0, 0}, {33, 0}, {14, 1}, {0, 0}, {33, 0}, {16, 1}, {0, 0}, {33, 0}, {18, 1}, {0, 0}, {33, 0}, {20, 1}, {0, 0}, 
                                  {33, 0}, {22, 1}, {0, 0}, {33, 0}, {24, 1}, {0, 0}, {33, 0}, {26, 1}, {0, 0}, {33, 0}, {28, 1}, {0, 0}, {33, 0}, {0, 3}, {0, 0}, 
                                  {33, 0}, {2, 3}, {0, 0}, {33, 0}, {4, 3}, {0, 0}, {33, 0}, {6, 3}, {0, 0}, {33, 0}, {8, 3}, {0, 0}, {33, 0}, {10, 3}, {0, 0}, 
                                  {33, 0}, {12, 3}, {0, 0}, {33, 0}, {16, 3}, {0, 0}, {33, 0}, {18, 3}, {0, 0}, {33, 0}, {20, 3}, {0, 0}, {33, 0}, {24, 3}, {0, 0}, 
                                  {33, 0}, {0, 5}, {0, 0}, {33, 0}, {2, 5}, {0, 0}, {33, 0}, {4, 5}, {0, 0}, {33, 0}, {6, 5}, {0, 0}, {33, 0}, {8, 5}, {0, 0}, 
                                  {33, 0}, {10, 5}, {0, 0}, {33, 0}, {12, 5}, {0, 0}, {33, 0}, {16, 5}, {0, 0}, {33, 0}, {18, 5}, {0, 0}, {33, 0}, {20, 5}, {0, 0}, 
                                  {33, 0}, {24, 5}, {0, 0}, {33, 0}, {0, 7}, {0, 0}, {33, 0}, {2, 7}, {0, 0}, {33, 0}, {4, 7}, {0, 0}, {33, 0}, {8, 7}, {0, 0}, 
                                  {33, 0}, {16, 7}, {0, 0}, {33, 0}, {0, 9}, {0, 0}, {33, 0}, {2, 9}, {0, 0}, {33, 0}, {4, 9}, {0, 0}, {33, 0}, {6, 9}, {0, 0}, 
                                  {33, 0}, {8, 9}, {0, 0}, {33, 0}, {10, 9}, {0, 0}, {33, 0}, {12, 9}, {0, 0}, {33, 0}, {16, 9}, {0, 0}, {33, 0}, {18, 9}, {0, 0}, 
                                  {33, 0}, {20, 9}, {0, 0}, {33, 0}, {24, 9}, {0, 0}, {33, 0}, {0, 11}, {0, 0}, {33, 0}, {2, 11}, {0, 0}, {33, 0}, {4, 11}, {0, 0}, 
                                  {33, 0}, {8, 11}, {0, 0}, {33, 0}, {16, 11}, {0, 0}, {33, 0}, {0, 13}, {0, 0}, {33, 0}, {2, 13}, {0, 0}, {33, 0}, {4, 13}, {0, 0}, 
                                  {33, 0}, {8, 13}, {0, 0}, {33, 0}, {16, 13}, {0, 0}, {33, 0}, {0, 15}, {0, 0}, {33, 0}, {0, 33}, {0, 0}, 
                                  {33, 0}, {4, 33}, {0, 0}, {33, 0}, {6, 33}, {0, 0}, {33, 0}, {8, 33}, {0, 0}, {33, 0}, {10, 33}, {0, 0}, {33, 0}, {12, 33}, {0, 0}, 
                                  {33, 0}, {16, 33}, {0, 0}, {33, 0}, {18, 33}, {0, 0}, {33, 0}, {20, 33}, {0, 0}, {33, 0}, {24, 33}, {0, 0}, {33, 0}, {0, 35}, {0, 0}, 
                                  {33, 0}, {2, 35}, {0, 0}, {33, 0}, {4, 35}, {0, 0}, {33, 0}, {8, 35}, {0, 0}, {33, 0}, {16, 35}, {0, 0}, {33, 0}, {0, 37}, {0, 0}, 
                                  {33, 0}, {2, 37}, {0, 0}, {33, 0}, {4, 37}, {0, 0}, {33, 0}, {8, 37}, {0, 0}, {33, 0}, {16, 37}, {0, 0}, {33, 0}, {0, 39}, {0, 0}, 
                                  {33, 0}, {0, 41}, {0, 0}, {33, 0}, {2, 41}, {0, 0}, {33, 0}, {4, 41}, {0, 0}, {33, 0}, {8, 41}, {0, 0}, {33, 0}, {16, 41}, {0, 0}, 
                                  {33, 0}, {0, 43}, {0, 0}, {33, 0}, {0, 45}, {0, 0}};

volatile int sequence_state;
volatile byte process_state;
volatile unsigned long light_start_time;
int old_sequence_state;
unsigned long start_time;
byte switch_state;


void setup() {
  for(int i = 0; i < 2; i++)
  {
    digitalWrite(soft_push_signals[i], LOW);
    digitalWrite(hard_push_signals[i], LOW);
    pinMode(soft_push_signals[i], OUTPUT);
    pinMode(hard_push_signals[i], OUTPUT);
  }
  PORTB = 0;
  PORTC = 0;
  DDRB = B11111111;
  DDRC = B11111111;
  pinMode(synchro_signal, INPUT_PULLUP);
  pinMode(switch_signal, INPUT_PULLUP);


  for (process_state = 0; process_state < 2; process_state++) {
     digitalWrite(soft_push_signals[process_state], HIGH);
     delay(500);
     digitalWrite(hard_push_signals[process_state], HIGH);
     delay(100);

     digitalWrite(hard_push_signals[process_state], LOW);
     digitalWrite(soft_push_signals[process_state], LOW);
     delay(400);
  }

  for (sequence_state = 0; sequence_state < test_lenght; sequence_state++) {
    PORTB = test_portb[sequence_state % sequence_length];
    PORTC = test_portc[sequence_state % sequence_length];
    delay(impulse_length);
    PORTB = 0;
    PORTC = 0;
    delay(500);
  }
  
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
      while(millis() - light_start_time < impulse_length)
      {
        delay(1);
      }
      PORTB = 0;
      PORTC = 0;
      while(sequence_state - old_sequence_state == 1)
      {
        if(process_state == 3){
          goto idle_state;
        }
      }
      digitalWrite(hard_push_signals[process_state], LOW);
      while(millis() - light_start_time < impulse_length)
      {
        delay(1);
      }
      PORTB = 0;
      PORTC = 0;

      while(sequence_state - old_sequence_state == 2)
      {
        if(process_state == 3){
          goto idle_state;
        }
      }
      digitalWrite(hard_push_signals[process_state], LOW);
      while(millis() - light_start_time < impulse_length)
      {
        delay(1);
      }
      PORTB = 0;
      PORTC = 0;
      
      while(millis() - start_time < shots_wait_time)
      {
        if(process_state == 3){
          goto idle_state;
        }
      }
      if(sequence_state >= sequence_length)
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
  if (sequence_state >= sequence_length) {
    return;
  }
  PORTC = light_states[sequence_state][0];
  PORTB = light_states[sequence_state][1];
  sequence_state += 1;
  light_start_time = millis();
}

void button_off() {
  process_state = 3;
}
