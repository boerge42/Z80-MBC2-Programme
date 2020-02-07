(*
***************************************************
* Routinen zur Ansteuerung kaskadierter MAX7219
* ---------------------------------------------
*              Uwe Berger, 2019
*
* ---------
* Have fun!
*
***************************************************
*)

type
   max7219_pinout_t = record
                        count : byte;
                        port  : byte;
                        din   : byte;
                        cs    : byte;
                        clk   : byte
                      end;

const
  REG_ADDR_NOOP        = $00;
  REG_ADDR_DIGIT0      = $01;
  REG_ADDR_DIGIT1      = $02;
  REG_ADDR_DIGIT2      = $03;
  REG_ADDR_DIGIT3      = $04;
  REG_ADDR_DIGIT4      = $05;
  REG_ADDR_DIGIT5      = $06;
  REG_ADDR_DIGIT6      = $07;
  REG_ADDR_DIGIT7      = $08;
  REG_ADDR_DECODE_MODE = $09;
  REG_ADDR_INTENSITY   = $0A;
  REG_ADDR_SCAN_LIMIT  = $0B;
  REG_ADDR_SHUTDOWN    = $0C;
  REG_ADDR_TEST        = $0F;

  NO_DECODE            = $00;
  CODE_B_0             = $01;
  CODE_B_3_0           = $0F;
  CODE_B_7_2           = $FC;
  CODE_B_7_0           = $FF;

  SHUTDOWN_MODE        = $00;
  NORMAL_MODE          = $01;
  DISPLAY_0_ONLY       = $00;
  DISPLAY_0_TO_1       = $01;
  DISPLAY_0_TO_2       = $02;
  DISPLAY_0_TO_3       = $03;
  DISPLAY_0_TO_4       = $04;
  DISPLAY_0_TO_5       = $05;
  DISPLAY_0_TO_6       = $06;
  DISPLAY_0_TO_7       = $07;

  CHAR_BLANK           = $F;
  CHAR_NEGATIV         = $A;

(* ************************* *)
procedure max7219_init_hw(gpio_val : byte; hw : max7219_pinout_t);
begin
  (* CLK high *)
  set_gpio(hw.port, set_bit(gpio_val, hw.clk));
  (* CS high *)
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
end;

(* ************************* *)
procedure max7219_shiftout(gpio_val : byte; hw : max7219_pinout_t; b : byte);
var i, temp : byte;
begin
  for i:=7 downto 0 do
  begin
{
    (* CLK low *)
    set_gpio(hw.port, clear_bit(gpio_val, hw.clk));
    (* DIN setzen (i-tes Bit von b) *)
    set_gpio(hw.port, put_bit(gpio_val, hw.din, get_bit(b, i)));
}
    (* CLK low; DIN setzen (i-tes Bit von b) *)
    temp := clear_bit(gpio_val, hw.clk);
    temp := put_bit(temp, hw.din, get_bit(b, i));
    set_gpio(hw.port, temp);

    (* CLK high *)
    set_gpio(hw.port, set_bit(gpio_val, hw.clk));
  end;
end;

(* ************************* *)
procedure max7219_send(gpio_val : byte; hw : max7219_pinout_t; reg, data : byte
begin
{
  (* CS high *)
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
}
  (* send reg/data *)
  max7219_shiftout(gpio_val, hw, reg);
  max7219_shiftout(gpio_val, hw, data);
  (* CS low/high *)
  set_gpio(hw.port, clear_bit(gpio_val, hw.cs));
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
end;

(* ************************* *)
procedure max7219_cascade_send_all
   (gpio_val : byte; hw : max7219_pinout_t; reg, data : byte);
var
  i : byte;
begin
  for i:=1 to hw.count do
  begin
    (* send reg/data *)
    max7219_shiftout(gpio_val, hw, reg);
    max7219_shiftout(gpio_val, hw, data);
  end;
  (* CS low/high *)
  set_gpio(hw.port, clear_bit(gpio_val, hw.cs));
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
end;

(* ************************* *)
procedure max7219_cascade_send
   (gpio_val : byte; hw : max7219_pinout_t; ic, reg, data : byte);
var
  i : byte;
begin
  for i:=1 to hw.count do
  begin
    if i = ic then
    begin
      (* send reg/data *)
      max7219_shiftout(gpio_val, hw, reg);
      max7219_shiftout(gpio_val, hw, data);
    end
    else
    begin
      (* NOOP *)
      max7219_shiftout(gpio_val, hw, $00);
      max7219_shiftout(gpio_val, hw, $00);
    end;
  end;
  (* CS low/high *)
  set_gpio(hw.port, clear_bit(gpio_val, hw.cs));
  set_gpio(hw.port, set_bit(gpio_val, hw.cs));
end;



(* ************************* *)
procedure max7219_clear(gpio_val : byte; hw : max7219_pinout_t);
var
  i : byte;
begin
  for i:=1 to 8 do
    max7219_send(gpio_val, hw, i, CHAR_BLANK);
end;

(* ************************* *)
(* ************************* *)
(* ************************* *)
