#include <MD_MAX72xx.h>
#include <ezButton.h>

// matrix controls
const byte data_pin = 11;
const byte chip_select_pin = 10;
const byte clock_pin = 13;
const byte max_devices = 1;

ezButton button(8);

int head_y = 0;
int head_x = 0;
int flag = 0;

int snake_x[64];
int snake_y[64];

int snake_length = 2;

long int current_time, prev_time = 0;
int threshold = 800;


String direction = "", prev_direction = "";
int max_x = 7, max_y = 7;

int food_x = random(0, 8);
int food_y = random(0, 8);

// creating matrix object
MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, chip_select_pin, max_devices);

// define the joystick controls
const byte hpin = A0;
const byte vpin = A1;

void setup() {
  Serial.begin(9600);

  matrix.begin();
  matrix.clear();

  button.setDebounceTime(25);
  matrix.setPoint(head_x, head_y, true);

}

void loop() {

  button.loop();
  current_time = millis();
  check_direction();
  // put your main code here, to run repeatedly:
  if (current_time - prev_time >= threshold) {
    prev_time = current_time;

    matrix.clear();
    move_sprite();
    update_snake();
    draw_sprites();
  }
  food_eat_check();

}

void window_check() {
  if (head_x > max_x) head_x = 0;
  else if (head_x < 0) head_x = max_x;
  else if (head_y > max_y) head_y = 0;
  else if (head_y < 0) head_y = max_y;

}


void check_direction() {

  if (analogRead(hpin) > 512 &&  prev_direction  !=  "right")
    direction = "left";
  else if (analogRead(hpin) < 512 &&  prev_direction  !=  "left")
    direction = "right";
  else if (analogRead(vpin) > 512 &&  prev_direction  !=  "down")
    direction = "up";
  else if (analogRead(vpin) < 512 &&  prev_direction  !=  "up")
    direction = "down";

  //  updating prev_direction
  prev_direction = direction;

}

void move_sprite() {
  if (direction == "left") head_x++;
  else if (direction == "right") head_x--;
  else if (direction == "up") head_y--;
  else if (direction == "down") head_y++;

  window_check();

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    flag = 1;
  }
}

void draw_sprites() {

  matrix.setPoint(food_y, food_x, true);

  for (int i = 0; i <= snake_length; i++) {
    matrix.setPoint(snake_y[i], snake_x[i], true);
  }
}

void update_snake() {
  for (int i = snake_length; i >= 0; i--) {
    if (i != 0) {
      snake_x[i] = snake_x[i - 1];
      snake_y[i] = snake_y[i - 1];
    } else {
      snake_x[i] = head_x;
      snake_y[i] = head_y;
    }
  }
}

void food_eat_check() {
  if (head_x  ==  food_x  &&  head_y  ==  food_y) {
    food_x = random(0, 8);
    food_y = random(0, 8);

    snake_length++;
    threshold = threshold - 50;
    if (threshold < 100)
          threshold = 80;
  }

}