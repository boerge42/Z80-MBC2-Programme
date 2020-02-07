(*
***************************************************
*  Routinen zum Ansteuern der Peripherie
*         des Z80-MBC2-Boards
*  -------------------------------------
*            Uwe Berger; 2019
*
*  ---------
*  Have fun!
*
***************************************************
*)

const
  PORT_READ_USER_KEY      = $80;
  PORT_READ_GPIOA         = $81;
  PORT_READ_GPIOB         = $82;
  PORT_READ_DS3231        = $84;

  PORT_WRITE_USER_LED     = $00;
  PORT_WRITE_VALUE_GPIOA  = $03;
  PORT_WRITE_VALUE_GPIOB  = $04;
  PORT_SET_IODIR_GPIOA    = $05;  (* IO-Direction        *)
  PORT_SET_IODIR_GPIOB    = $06;  (* 0-Output; 1-Input   *)
  PORT_SET_GPPU_GPIOA     = $07;  (* Pull-UP:            *)
  PORT_SET_GPPU_GPIOB     = $08;  (* 0-disable; 1-enable *)

type
  ds3231_t = record
               sec  : byte;
               min  : byte;
               hour : byte;
               day  : byte;
               mon  : byte;
               year : byte;
               temp : byte;
             end;

  str_t = string[255];


(* ************************* *)
function get_gpio(p : byte) : byte;
begin
  port[1]  := p;
  get_gpio := port[0];
end;

(* ************************* *)
procedure get_ds3231(var v : ds3231_t);
begin
  port[1] := PORT_READ_DS3231;
  with v do
  begin
    sec  := port[0];
    min  := port[0];
    hour := port[0];
    day  := port[0];
    mon  := port[0];
    year := port[0];
    temp := port[0];
  end;
end;

(* ************************* *)
procedure set_gpio(p, v : byte);
begin
  port[1] := p;
  port[0] := v;
end;

(* ************************* *)
procedure set_userled(v : byte);
begin
  set_gpio(PORT_WRITE_USER_LED, v);
end;

(* ************************* *)
function userkey_pressed : boolean;
begin
  if get_gpio(PORT_READ_USER_KEY) = 0 then userkey_pressed := false
  else userkey_pressed := true;
end;


