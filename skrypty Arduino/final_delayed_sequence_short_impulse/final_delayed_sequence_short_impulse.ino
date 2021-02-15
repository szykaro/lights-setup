const byte test_lenght = 12;
volatile byte test_portc[test_lenght] = {1, 2, 4, 8, 16, 32, 0, 0, 0, 0,  0,  0};
volatile byte test_portb[test_lenght] = {0, 0, 0, 0,  0,  0, 1, 2, 4, 8, 16, 32};

int soft_push_signals[] = {5, 7};
int hard_push_signals[] = {4, 6};
int synchro_signal = 3;
int switch_signal = 2;
const int photo_count = 273;
//Za pomocą wait_time reguluje się czas między wykonaniem trójki zdjęć
const unsigned long shots_wait_time = 4500;
const unsigned long impulse_length = 20;
volatile byte light_states[photo_count][2] = {{33, 0}, {2, 1}, {0, 0}, {33, 0}, {4, 1}, {0, 0}, {33, 0}, {6, 1}, {0, 0}, {33, 0}, {8, 1}, {0, 0}, {33, 0}, {10, 1}, {0, 0},
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
  {33, 0}, {0, 43}, {0, 0}, {33, 0}, {0, 45}, {0, 0}
};

byte process_state;
unsigned long start_time;

volatile unsigned long light_start_time;
volatile byte photo_taken;
volatile int photo_i;

void setup() {
  PORTB = 0;
  PORTC = 0;
  DDRB = B11111111;
  DDRC = B11111111;

  pinMode(synchro_signal, INPUT_PULLUP);
  pinMode(switch_signal, INPUT_PULLUP);

  for (byte i = 0; i < 2; i++) {
    digitalWrite(soft_push_signals[i], LOW);
    digitalWrite(hard_push_signals[i], LOW);
    pinMode(soft_push_signals[i], OUTPUT);
    pinMode(hard_push_signals[i], OUTPUT);

    digitalWrite(soft_push_signals[i], HIGH);
    delay(500);
    digitalWrite(hard_push_signals[i], HIGH);
    delay(100);

    digitalWrite(hard_push_signals[i], LOW);
    digitalWrite(soft_push_signals[i], LOW);
    delay(400);
  }

  for (photo_i = 0; photo_i < test_lenght; photo_i++) {
    PORTB = test_portb[photo_i];
    PORTC = test_portc[photo_i];
    delay(impulse_length);
    PORTB = 0;
    PORTC = 0;
    delay(500);
  }

  process_state = 0;
  photo_i = 2;

  attachInterrupt(digitalPinToInterrupt(synchro_signal), lights_change, FALLING);
}

void loop() {
  digitalWrite(soft_push_signals[0], HIGH);
  digitalWrite(soft_push_signals[1], HIGH);
  delay(1000);

  for (photo_i = 0; photo_i < photo_count;) {
    if (digitalRead(switch_signal) != 0) {
       goto idle_state;
    }
    
    start_time = millis();
    
    photo_taken = 0;
    digitalWrite(hard_push_signals[process_state], HIGH);
    for (byte i = 0; i < 3; i++) {
      while (!photo_taken);
      photo_taken = 0;
      photo_i++;

      while ((unsigned long) (millis() - light_start_time) < impulse_length);
      PORTB = 0;
      PORTC = 0;
    }

    digitalWrite(hard_push_signals[process_state], LOW);
    while (millis() - start_time < shots_wait_time);
  }

  delay(7500);
  process_state++;
  if (process_state < 2) {
    return;
  }

idle_state:
  digitalWrite(soft_push_signals[0], LOW);
  digitalWrite(hard_push_signals[0], LOW);
  digitalWrite(soft_push_signals[1], LOW);
  digitalWrite(hard_push_signals[1], LOW);
  PORTB = 0;
  PORTC = 0;
  process_state = 0;
  photo_i = 2;
  while (digitalRead(switch_signal) != 0);
}

void lights_change() {
  PORTC = light_states[photo_i][0];
  PORTB = light_states[photo_i][1];
  light_start_time = millis();
  photo_taken = 1;
}
