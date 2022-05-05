#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include<time.h>

byte customChar[] = {
  B00100,
  B01100,
  B11100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111
};
int matrix[6][14] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int BOMB[6][14] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const int VRx = 2; // Connect to Analog Pin 0
const int VRy = 3; // Connect to Analog Pin 1
const int SW = 7; // Connect to Digital Pin 4

const int height = 8;
const int lenght = 5;
const int buttonPin = 9;
const int LedPin = 8;
int buttonState = 0;

int pozx = 7;
int pozy = 3;
int mod = 1;
int x = 4;
int y = 3;
int dificultate = 1;
int nr_bombe = 5;
int nr_marcari = 0;
int k = 0;

Adafruit_PCD8544 display = Adafruit_PCD8544(2, 3, 4, 5, 6);

void generate_matrix();
void generate_bombs(int dificult);
void afisare();
void miscare_X();
void selectare_Meniu();
void selectare_dificultate();
void game_over();
bool verificare_victorie();
void Victorie();
void maxi(int x, int y);
void afisare_Bombe();

void setup()   {
  pinMode(buttonPin, INPUT);
  pinMode(LedPin, OUTPUT);

  display.begin();
  // init done
  display.clearDisplay();
  display.display();
  display.setContrast(50);

  // Show inverted text
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);

  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  Serial.begin(9600);

  randomSeed(analogRead(5));
  generate_matrix();
  generate_bombs(1);
}


void loop() {
  switch (mod) {
    case 1://Meniu
      selectare_Meniu();
      break;
    case 2 : //Joc
      k = 0;
      miscare_X();
      afisare();
      break;
    case 3 : //GameOver
      game_over();
      break;
    case 4: //Select dificult
      selectare_dificultate();
      break;
    case 5://Victorie
      Victorie();
      break;
  }
  display.clearDisplay();
  delay(200);
}

void afisare_Bombe()
{
  for (int i = 0 ; i < 6; i++)
  {
    for (int j = 0 ; j < 14; j++)
    {
      if (BOMB[i][j] == 9)
        matrix[i][j] = 3;
    }
  }
}
void maxi(int x, int y)
{
  if (x < 0 || x > 5 || y < 0 || y > 13)
    return;

  if (BOMB[x][y] != 9 && BOMB[x][y] != 0)
  {
    matrix[x][y] = 2;
    return;
  }
  if (matrix[x][y] == 2)
    return;
  if (matrix[x][y] == 1 && BOMB[x][y] != 9)
  {
    nr_marcari--;
  }
  matrix[x][y] = 2;

  maxi(x - 1, y);
  maxi(x + 1, y);
  maxi(x, y - 1);
  maxi(x, y + 1);
}


void Victorie()
{
  display.setCursor(8, 15);
  display.setTextColor(BLACK, WHITE);
  display.println("Felicitari!!");
  display.setCursor(4, 24);
  display.println("Ai castigat!!");
  display.display();

  if (digitalRead(SW) == 0)
  {
    mod = 1;
    generate_bombs(dificultate);
    generate_matrix();
  }

}

bool verificare_victorie()
{
  for (int i = 0 ; i < 6; i++)
  {
    for (int j = 0; j < 14; j++)
    {
      if (matrix[i][j] == 1 && BOMB[i][j] != 9)
        return false;
      if (matrix[i][j] == 0 && BOMB[i][j] != 9)
        return false;
    }
  }
  return true;
}

void game_over()
{
  afisare_Bombe();

  if (k == 0)
  afisare();
  
  delay(1000);

  display.clearDisplay();
  digitalWrite(LedPin, HIGH);
  delay(500);
   k = 1;
  display.setCursor(30, 15);
  display.setTextColor(BLACK, WHITE);
  display.println("GAME");
  display.setCursor(30, 24);
  display.println("OVER");
  display.display();

  if (digitalRead(SW) == 0)
  { generate_bombs(dificultate);
    generate_matrix();
    digitalWrite(LedPin, LOW);
    delay(200);
    mod = 1;

  }
}

void selectare_dificultate()
{
  display.setCursor(12, 3);
  display.setTextColor(BLACK, WHITE);
  display.println("5 BOMBS");
  display.setCursor(12, 12);
  display.println("12 BOMBS");
  display.setCursor(12, 21);
  display.println("20 BOMBS");
  display.setCursor(x, y);
  display.print('>');
  display.display();

  if (analogRead(VRy) > 750 && y == 12) //12 - 21
  {
    y = 21;
  }
  if (analogRead(VRy) < 250 && y == 12)//12 -3
  {
    y = 3;
  }
  if (analogRead(VRy) > 750 && y == 3) // 3 -12
  {
    y = 12;
  }

  if (analogRead(VRy) < 250 && y == 21) //21 - 12
  {
    y = 12;
  }

  if (digitalRead(SW) == 0)
  {
    if (y == 3)
    {
      dificultate = 1;
      generate_bombs(dificultate);
      mod = 1;
    }
    if (y == 12)
    {
      y = 3;
      dificultate = 2;
      generate_bombs(dificultate);
      mod = 1;
    }
    if (y == 21)
    {
      y = 3;
      dificultate = 3;
      generate_bombs(dificultate);
      mod = 1;
    }

  }
}

void selectare_Meniu()
{
  display.setCursor(12, 3);
  display.setTextColor(BLACK, WHITE);
  display.println("Start Game");
  display.setCursor(12, 12);
  display.println("Dificult");

  display.setCursor(12, 25);
  display.println("Numar de");
  display.setCursor(12, 34);
  display.print("Bombe :");
  display.print(nr_bombe);

  display.setCursor(x, y);
  display.print('>');
  display.display();
  if (analogRead(VRy) < 250 && y == 12)
  {
    y = 3;
  }
  if (analogRead(VRy) > 750 && y == 3)
  {
    y = 12;
  }

  if (digitalRead(SW) == 0)
  {
    if (y == 3)
    {
      mod = 2;
      y = 3;
    }
    else if (y == 12)
    {
      mod = 4;
      y = 3;
    }
  }
}

void miscare_X()
{
  buttonState = digitalRead(buttonPin);
  if (analogRead(VRx) < 250 && pozx > 0)
    pozx = pozx - 1;

  if (analogRead(VRx) > 750 && pozx < 13)
    pozx = pozx + 1;

  if (analogRead(VRy) > 750 && pozy < 5)
    pozy = pozy + 1;

  if (analogRead(VRy) < 250 && pozy > 0)
    pozy = pozy - 1;

  if (buttonState == 1 && matrix[pozy][pozx] == 0 && nr_marcari < nr_bombe)
  {
    matrix[pozy][pozx] = 1;
    nr_marcari++;
  }
  else if (buttonState == 1 && matrix[pozy][pozx] == 1)
  {
    matrix[pozy][pozx] = 0;
    nr_marcari--;
  }
  if (digitalRead(SW) == 0 && matrix[pozy][pozx] == 0)
  {
    if (BOMB[pozy][pozx] == 9)
    {
      mod = 3;
      return;
    }
    else if (BOMB[pozy][pozx] == 0 )
      maxi(pozy, pozx);
    else
    {
      matrix[pozy][pozx] = 2;
    }
  }

  if (verificare_victorie() == true)
    mod = 5;
}



void afisare()
{
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 14; j++)
    {
      if (i == pozy && j == pozx)
      {
        display.print('X');
      }
      else if (matrix[i][j] == 1)
      {
        display.print('M');
      }
      else if ( matrix[i][j] == 2)
      {
        display.print(BOMB[i][j]);
      }
      else if (matrix[i][j] == 3)
      {
        display.print('B');
      }
      else if (matrix[i][j] == 0)
      {
        display.print('#');
      }
    }
  }
  display.display();
}
void generate_matrix()
{
  for (int i = 0 ; i < 6; i++)
  {
    for (int j = 0; j < 14; j++)
    {
      matrix[i][j] = 0;
    }
  }
}
void generate_bombs( int dificult)
{
  int x, y;
  switch (dificult)
  {
    case 1:
      nr_bombe = 5;
      break;
    case 2:
      nr_bombe = 12;
      break;
    case 3:
      nr_bombe = 20;
      break;
  }
  for (int i = 0 ; i < 6; i++)
  {
    for (int j = 0 ; j < 14; j++)
    {
      BOMB[i][j] = 0;
    }
  }
  for (int i = 0 ; i < nr_bombe; i++)
  {
    x = random(0, 6);
    y = random(0, 14);

    if (BOMB[x][y] == 0)
      BOMB[x][y] = 9;
    else
      i--;
  }
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 14; j++)
    {
      int bombe_vecine = 0;
      if (BOMB[i][j] != 9)
      {
        if (i != 0 && j != 0 && i != 5 && j != 13)//centru
        {
          if ( BOMB[i - 1][j - 1] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i - 1][j] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i - 1][j + 1] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i][j - 1] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i + 1][j - 1] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i][j + 1] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i + 1][j] == 9 )
          {
            bombe_vecine++;
          }
          if ( BOMB[i + 1][j + 1] == 9 )
          {
            bombe_vecine++;
          }
        }
        else if ( i == 0 && j != 0 && j != 13)//sus
        {
          if (BOMB[i][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j + 1] == 9)
          {
            bombe_vecine++;
          }
        }
        else if (i == 5 && j != 0 && j != 13)//jos
        {
          if (BOMB[i][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i][j + 1] == 9)
          {
            bombe_vecine++;
          }
        }
        else if (j == 0 && i != 0 && i != 5)
        {
          if (BOMB[i - 1][j] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j] == 9)
          {
            bombe_vecine++;
          }
        }
        else if (j == 13 && i != 0 && i != 5)
        {
          if (BOMB[i - 1][j] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j] == 9)
          {
            bombe_vecine++;
          }
        }
        else if (i == 0 && j == 0)
        {
          if (BOMB[i][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j] == 9)
          {
            bombe_vecine++;
          }
        }
        else if (i == 0 && j == 13)
        {
          if (BOMB[i][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i + 1][j] == 9)
          {
            bombe_vecine++;
          }
        }
        else if ( i == 5 && j == 0)
        {
          if (BOMB[i - 1][j] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j + 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i][j + 1] == 9)
          {
            bombe_vecine++;
          }
        }
        else if (i == 5 && j == 13)
        {
          if (BOMB[i - 1][j] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i - 1][j - 1] == 9)
          {
            bombe_vecine++;
          }
          if (BOMB[i][j - 1] == 9)
          {
            bombe_vecine++;
          }
        }
        BOMB[i][j] = bombe_vecine;
      }
    }
  }
}
