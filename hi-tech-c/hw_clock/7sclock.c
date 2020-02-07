#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define  PORT_READ_USER_KEY       0x80
#define  PORT_READ_GPIOA          0x81
#define  PORT_READ_GPIOB          0x82
#define  PORT_READ_DS3231         0x84

#define  PORT_WRITE_USER_LED      0x00
#define  PORT_WRITE_VALUE_GPIOA   0x03
#define  PORT_WRITE_VALUE_GPIOB   0x04
#define  PORT_SET_IODIR_GPIOA     0x05  /* IO-Direction        */
#define  PORT_SET_IODIR_GPIOB     0x06  /* 0-Output; 1-Input   */
#define  PORT_SET_GPPU_GPIOA      0x07  /* Pull-UP:            */
#define  PORT_SET_GPPU_GPIOB      0x08  /* 0-disable; 1-enable */

typedef struct {
               char sec;
               char min;
               char hour;
               char day;
               char mon;
               char year;
               char temp;
} ds3231_t;

#define  REG_ADDR_NOOP         0x00
#define  REG_ADDR_DIGIT0       0x01
#define  REG_ADDR_DIGIT1       0x02
#define  REG_ADDR_DIGIT2       0x03
#define  REG_ADDR_DIGIT3       0x04
#define  REG_ADDR_DIGIT4       0x05
#define  REG_ADDR_DIGIT5       0x06
#define  REG_ADDR_DIGIT6       0x07
#define  REG_ADDR_DIGIT7       0x08
#define  REG_ADDR_DECODE_MODE  0x09
#define  REG_ADDR_INTENSITY    0x0A
#define  REG_ADDR_SCAN_LIMIT   0x0B
#define  REG_ADDR_SHUTDOWN     0x0C
#define  REG_ADDR_TEST         0x0F

#define  NO_DECODE             0x00
#define  CODE_B_0              0x01
#define  CODE_B_3_0            0x0F
#define  CODE_B_7_2            0xFC
#define  CODE_B_7_0            0xFF

#define  SHUTDOWN_MODE         0x00
#define  NORMAL_MODE           0x01
#define  DISPLAY_0_ONLY        0x00
#define  DISPLAY_0_TO_1        0x01
#define  DISPLAY_0_TO_2        0x02
#define  DISPLAY_0_TO_3        0x03
#define  DISPLAY_0_TO_4        0x04
#define  DISPLAY_0_TO_5        0x05
#define  DISPLAY_0_TO_6        0x06
#define  DISPLAY_0_TO_7        0x07

#define  CHAR_BLANK            0xF
#define  CHAR_NEGATIV          0xA

typedef struct {
               char count;
               char port;
               char din;
               char cs;
               char clk;
} max7219_pinout_t;

/* ************************* */
char set_bit(char v, char i)
{
  return (v | (1 << i));
}

/* ************************* */
char clear_bit(char v, char i)
{
  return (v & ~(1 << i));
}

/* ************************* */
char put_bit(char v, char i, char f)
{
  if (f == 0) {
    return clear_bit(v, i);
  } else {
    return set_bit(v, i);
  }
}

/* ************************* */
char toggle_bit(char v, char i)
{
  return (v ^ (1 << i));
}

/* ************************* */
char get_bit(char v, char i)
{
  if ((v & (1 << i)) != 0) {
    return 1;
  } else {
    return 0;
  }
}

/* ************************* */
char get_gpio(char p)
{
  outp(1, p);
  return inp(0);  
}

/* ************************* */
void get_ds3231(ds3231_t *ts)
{
  outp(1, PORT_READ_DS3231);
  ts->sec  = inp(0);
  ts->min  = inp(0);
  ts->hour = inp(0);
  ts->day  = inp(0);
  ts->mon  = inp(0);
  ts->year = inp(0);
  ts->temp = inp(0);
}

/* ************************* */
void set_gpio(char p, char v)
{
  outp(1, p);
  outp(0, v);
}

/* ************************* */
char userkey_pressed(void)
{
  if (get_gpio(PORT_READ_USER_KEY) == 0) {
    return 0;
  }else {
	return 1;
  }
}

/* ************************* */
void set_userled(char v)
{
  set_gpio(PORT_WRITE_USER_LED, v);
}


/* ************************* */
void max7219_init_hw(char gpio_val, max7219_pinout_t hw)
{
  /* CLK high */
  set_gpio(hw.port, set_bit(gpio_val, hw.clk));
  /* CS high */
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
}

/* ************************* */
void max7219_shiftout(char gpio_val, max7219_pinout_t hw, char b)
{
  char i; 
  char temp;
  
  for (i=7; i>=0; i--) {
    /* CLK low; DIN setzen (i-tes Bit von b) */
    temp = clear_bit(gpio_val, hw.clk);
    temp = put_bit(temp, hw.din, get_bit(b, i));
    set_gpio(hw.port, temp);
    /* CLK high */
    set_gpio(hw.port, set_bit(gpio_val, hw.clk));
  }
}

/* ************************* */
void max7219_send(char gpio_val, max7219_pinout_t hw, char reg, char data)
{
  /* send reg/data */
  max7219_shiftout(gpio_val, hw, reg);
  max7219_shiftout(gpio_val, hw, data);
  /* CS low/high */
  set_gpio(hw.port, clear_bit(gpio_val, hw.cs));
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
}

/* ************************* */
void max7219_cascade_send_all
   (char gpio_val, max7219_pinout_t hw, char reg, char data)
{
  char i;
  /* for i:=1 to hw.count do */
  for (i=1; i < (hw.count+1); i++) {
    /* send reg/data */
    max7219_shiftout(gpio_val, hw, reg);
    max7219_shiftout(gpio_val, hw, data);
  }
  /* CS low/high */
  set_gpio(hw.port, clear_bit(gpio_val, hw.cs));
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
}

/* ************************* */
void max7219_cascade_send
   (char gpio_val, max7219_pinout_t hw, char ic, char reg, char data)
{
  char i;
  /* for i:=1 to hw.count do */
  for (i=1; i < (hw.count+1); i++) {
    if (i == ic) {
      /* send reg/data */
      max7219_shiftout(gpio_val, hw, reg);
      max7219_shiftout(gpio_val, hw, data);
    }else {
      /* NOOP */
      max7219_shiftout(gpio_val, hw, 0x00);
      max7219_shiftout(gpio_val, hw, 0x00);
    }
  }
  /* CS low/high */
  set_gpio(hw.port, clear_bit(gpio_val, hw.cs));
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
}

/* ************************* */
void max7219_clear(char gpio_val, max7219_pinout_t hw)
{
  char i;
  /* for i:=1 to 8 do */
  for (i=1; i<9; i++) max7219_send(gpio_val, hw, i, CHAR_BLANK);
}


/* ************************* */
/* ************************* */
/* ************************* */
void main(void)
{
  ds3231_t ts, old_ts;
  char gpiob, i, neg;
  max7219_pinout_t max7219;

  puts("Z80-MBC-Board: 7-Seg-LED-Clock; Uwe Berger, 2019");
  puts("Init...");
  old_ts.hour = 0;
  old_ts.min  = 0;
  old_ts.sec  = 0;
  old_ts.day  = 0;
  old_ts.mon  = 0;
  old_ts.year = 0;
  old_ts.temp = -100;

  puts("...GPIOB --> Output");
  gpiob = 0;         /* GPIOB als Output (fuer MAX7219) */
  set_gpio(PORT_SET_IODIR_GPIOB, 0);
  set_gpio(PORT_WRITE_VALUE_GPIOB, gpiob);

  /* MAX7219-Initialisierung */
  /* ...MAX7219-Hardware definieren */
  max7219.port  = PORT_WRITE_VALUE_GPIOB;
  max7219.count = 3;
  max7219.din   = 0;
  max7219.cs    = 1;
  max7219.clk   = 2;

  max7219_init_hw(gpiob, max7219);
  puts("...MAX7219-Intensity");
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_INTENSITY, 2);
  puts("...MAX7219-Decode-Mode");
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_DECODE_MODE, CODE_B_7_0);
  max7219_cascade_send(gpiob, max7219, 3, REG_ADDR_DECODE_MODE, CODE_B_7_2);
  puts("...MAX7219-Shutdown to normal");
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_SHUTDOWN, NORMAL_MODE);
  puts("...MAX7219-Scanlimit");
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_SCAN_LIMIT, DISPLAY_0_TO_7);

  printf("...Output template\n");
  max7219_cascade_send(gpiob, max7219, 1, 3, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 1, 6, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 2, 3, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 2, 6, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 3, 1, 0x4E);           /* C    */
  max7219_cascade_send(gpiob, max7219, 3, 2, 0x63);           /* Grad */

  printf("Init ready!\n\n");
  printf("Exit: press 'userkey' or 'any key'...\n");

  /* solange bis onboard-'userkey'... */
  while (userkey_pressed() == 0) {

    /* DS3231: Date/Time, Temperature ausgeben */
    get_ds3231(&ts);
    
    /* userled --> Visualisierung Dauer Portzugriff*/
    set_userled(1);
    if (old_ts.hour != ts.hour) {
      old_ts.hour = ts.hour;
      max7219_cascade_send(gpiob, max7219, 1, 8, ts.hour/10);
      max7219_cascade_send(gpiob, max7219, 1, 7, ts.hour%10);
    }
    if (old_ts.min != ts.min) {
      old_ts.min = ts.min;
      max7219_cascade_send(gpiob, max7219, 1, 5, ts.min/10);
      max7219_cascade_send(gpiob, max7219, 1, 4, ts.min%10);
    }
    if (old_ts.sec != ts.sec) {
      old_ts.sec = ts.sec;
      max7219_cascade_send(gpiob, max7219, 1, 2, ts.sec/10);
      max7219_cascade_send(gpiob, max7219, 1, 1, ts.sec%10);
    }
    if (old_ts.day != ts.day) {
      old_ts.day = ts.day;
      max7219_cascade_send(gpiob, max7219, 2, 8, ts.day/10);
      max7219_cascade_send(gpiob, max7219, 2, 7, ts.day%10);
    }
    if (old_ts.mon != ts.mon) {
      old_ts.mon = ts.mon;
      max7219_cascade_send(gpiob, max7219, 2, 5, ts.mon/10);
      max7219_cascade_send(gpiob, max7219, 2, 4, ts.mon%10);
    }
    if (old_ts.year != ts.year) {
      old_ts.year = ts.year;
      max7219_cascade_send(gpiob, max7219, 2, 2, ts.year/10);
      max7219_cascade_send(gpiob, max7219, 2, 1, ts.year%10);
    }
    if (old_ts.temp != ts.temp) {
      old_ts.temp = ts.temp;
      /* Temperatur wird etwas aufwaendiger... */
      /* ...entspr. Display-'Zeile' loeschen (ausser Template) */
      for (i=8; i>=3; i--) 
        max7219_cascade_send(gpiob, max7219, 3, i, CHAR_BLANK);
      /* ...Annahme: Wertebereich -99...+99! */
      if (ts.temp > 0x7f) {   /* 7.Bit ist Vorzeichen... */
        ts.temp = ts.temp - 128;
        neg = 1;
      }else {
        neg = 0;  
     }        
	  
      i = 3;
      max7219_cascade_send(gpiob, max7219, 3, i, ts.temp%10);
      if (ts.temp > 9) {
        i = i+1;
        max7219_cascade_send(gpiob, max7219, 3, i, ts.temp/10);
      }
      /* Temperatur negativ, dann noch Vorzeichen ausgeben */
      if (neg) {
        i = i+1;
        max7219_cascade_send(gpiob, max7219, 3, i, CHAR_NEGATIV);
      }
    }
    set_userled(0);
    /* delay(50); */
  }
}
