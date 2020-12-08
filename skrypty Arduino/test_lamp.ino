const byte sequence_length = 12;
volatile byte light_states_portc[] = {1, 2, 4, 8, 16, 32,
                                      0, 0, 0, 0, 0, 0};
volatile byte light_states_portb[] = {0, 0, 0, 0, 0, 0,
                                      1, 2, 4, 8, 16, 32};
volatile byte sequence_state;

void setup() {
  DDRB = B11111111;
  DDRC = B11111111;
  PORTB = 0;
  PORTC = 0;
  sequence_state = 0;
}

void loop() {
  PORTB = light_states_portb[sequence_state % sequence_length];
  PORTC = light_states_portc[sequence_state % sequence_length];
  sequence_state += 1;
  delay(1000);
}
