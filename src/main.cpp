#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

long world[15] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0};
long time = 500;
long jump_time = 1500;
int cactus_spawn_lim = 3;
int lvl = 1;

bool in_game = true;
bool jump_pressed = false;
long score = 1;
long current_time = 0;
long current_jump_time = 0;
long tmp_scroll_catus_state = 0;

byte empty_char[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
byte dino_char[8] = {
  0b00000,
  0b00011,
  0b00010,
  0b00010,
  0b01110,
  0b11110,
  0b01010,
  0b11111
};
byte floor_char[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
byte cactus_char[8] = {
  0b00000,
  0b00100,
  0b10101,
  0b10101,
  0b10111,
  0b11100,
  0b00100,
  0b11111
};
byte dino_jump_char[8] = {
  0b00011,
  0b00010,
  0b00010,
  0b00110,
  0b01110,
  0b11010,
  0b01000,
  0b00000
};
byte dead_dino[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11101,
  0b10100,
  0b11111
};
byte skull[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b10101,
  0b11111,
  0b01110,
  0b01010,
  0b00000
};

bool jumping = false;

void setup() {
  pinMode(7, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  lcd.createChar(0, empty_char);
  lcd.createChar(1, dino_char);
  lcd.createChar(2, dino_jump_char);
  lcd.createChar(3, floor_char);
  lcd.createChar(4, cactus_char);
  lcd.createChar(5, dead_dino);
  lcd.createChar(6, skull);

  lcd.setCursor(0, 1);
  lcd.write((byte)1); //afficher le dino à gauche
}

void loop() {
  if (in_game) {
    if (jumping && millis() - current_jump_time > jump_time) {
      jumping = false;
    }

    if (!digitalRead(7)) {
      if (!jump_pressed) {
        current_jump_time = millis();
        jump_pressed = true;
        jumping = true;
      }
    } else {
      jump_pressed = false;
    }

    if (jumping) {
      lcd.setCursor(0, 1);
      if (world[0] == 0) {
        lcd.write((byte)3);
      } else {
        lcd.write((byte)4);
      }
      lcd.setCursor(0, 0);
      lcd.write((byte)2);
    } else {
      lcd.setCursor(0, 1);
      lcd.write((byte)1);
      lcd.setCursor(0, 0);
      lcd.write((byte)0);
    }
    
    //death
    if (!jumping && world[0] == 1) {
      //in_game = false;
    }

    //scroll ground
    lcd.setCursor(1, 1);
    for (int i = 1; i < 15; i++) {
      if (world[i] == 0) {
        lcd.write((byte)3);
      } else {
        lcd.write((byte)4);
      }
    }

    //world
    if (millis()-current_time > time) {
      current_time = millis();
      for (int i = 0; i < 15; i++) {
        world[i-1] = world[i];
      }

      score += 1;
      tmp_scroll_catus_state += 1;

      if (random(0, 5) == 0 && tmp_scroll_catus_state > cactus_spawn_lim) {
          world[14] = 1;
          tmp_scroll_catus_state = 0;
      } else {
          world[14] = 0;
      }
    }

    //show score
    lcd.setCursor(12, 0);
    lcd.print(String(score));

  
    //manage lvls
    if (lvl != score/10) {
      lvl = score/10;
      //time -= 50;
      
      lcd.setCursor(5, 0);
      lcd.print("lvl ");
      lcd.print(lvl);
      //lcd.write((byte)6);
    }


  } else {
    lcd.setCursor(1, 0);
    lcd.print("You are so bad");
    lcd.write((byte)6);
    
    lcd.setCursor(0, 1);
    lcd.write((byte)5);
    lcd.setCursor(0, 0);
    lcd.write((byte)6);

    //restart
    if (!digitalRead(7)) {
      lcd.clear();
      in_game = true;
      score = 0;

      lcd.setCursor(0, 1);
      lcd.write((byte)1);
      lcd.setCursor(0, 0);
      lcd.write((byte)0);
      for (int i = 0; i < 15; i++) {
        world[i] = 0;
      }
      lcd.setCursor(0, 0);
    }
  }


}