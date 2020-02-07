(*
*****************************************************
* Spielereien mit der Peripherie des Z80-MBC2-Boards
* --------------------------------------------------
*                 Uwe Berger, 2019
*
*
* ---------
* Have fun!
*
*****************************************************
*)

program my_io;

{$U+}

{$I mbc2io.inc}
{$I bitman.inc}
{$I max7219.inc}

const
  CURSOR_OFF = #27'[?25l';
  CURSOR_ON  = #27'[?25h';

(* ************************* *)
function int2zerostr (v, d : integer) : str_t;
var
  s : string[255];
  i : integer;
begin
  str(v:d, s);
  for i:=1 to d do if s[i] = ' ' then s[i] := '0';
  int2zerostr := s;
end;

(* ************************* *)
function toggle_value (var v : byte) : byte;
begin
  v := 1-v;
  toggle_value := v;
end;

(* ************************* *)
function byte2bin(v : byte) : str_t;
var
  ret, s : str_t;
  i : byte;
begin
  ret := '';
  for i := 0 to 7 do
  begin
    str(get_bit(v, i), s);
    ret := concat(s, ret);
  end;
  byte2bin := ret;
end;

(* ************************* *)
function delay_keypressed(v : integer) : char;
var
  i : integer;
  k : char;
begin
  k := char(0);
  for i:=0 to v do
  begin
    if keypressed then
    begin
      read(kbd, k);
      delay_keypressed := k;
      exit;
    end;
    delay(1);
  end;
  delay_keypressed := k;
end;

(* ************************* *)
procedure write_str_xy(x, y : byte; s : str_t);
begin
  gotoxy(x, y);
  write(s);
end;

(* ************************* *)
procedure write_num_xy(x, y : byte; v : integer);
begin
  gotoxy(x, y);
  write(v);
end;

(* ************************* *)
procedure init_screen;
begin
  clrscr;
  write_str_xy(1, 1,  'Z80-MBC2 I/O-Tests; Uwe Berger, 2019');
  lowvideo;
  write_str_xy(1, 4,  'DS3231 (RTC)');
  write_str_xy(1, 5,  '  Date/Time......:');
  write_str_xy(1, 6,  '  Temperature [C]:');
  write_str_xy(1, 9,  'User-LED...:');
  write_str_xy(1, 12, 'User-Switch:');
  write_str_xy(1, 15, 'GPIO-Ports');
  write_str_xy(1, 16, '    Bit: 76543210');
  write_str_xy(1, 17, '  GPIOA:');
  write_str_xy(1, 18, '  GPIOB:');
  normvideo;
end;

(* ************************* *)
(* ************************* *)
(* ************************* *)

var
  ts, old_ts : ds3231_t;
  key : char;
  led, gpioa, gpiob, bit : byte;
  s   : str_t;
  timer : integer;

  max7219 : max7219_pinout_t;


begin

  key   := ' ';
  led   := 0;
  bit   := 0;
  gpioa := 0;
  gpiob := 0;
  timer := 0;

  (* Cursor ausschalten *)
  write(CURSOR_OFF);
  init_screen;

  (* GPIOA als Output *)
  set_gpio(PORT_SET_IODIR_GPIOA, 0);
  set_gpio(PORT_WRITE_VALUE_GPIOA, gpioa);

{
  (* Pull-ps GPIOB einschalten (...und als Input belassen) *)
  set_gpio(PORT_SET_GPPU_GPIOB, 255);
}
  (* GPIOB als Output (fuer MAX7219) *)
  set_gpio(PORT_SET_IODIR_GPIOB, 0);
  set_gpio(PORT_WRITE_VALUE_GPIOB, gpiob);

  (* MAX7219-Initialisierung *)
  (* ...MAX7219-Hardware definieren *)
  with max7219 do
  begin
    port := PORT_WRITE_VALUE_GPIOB;
    count := 2;
    din   := 0;
    cs    := 1;
    clk   := 2;
  end;
  max7219_init_hw(gpiob, max7219);
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_INTENSITY, 2);
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_DECODE_MODE, CODE_B_7_0);
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_SHUTDOWN, NORMAL_MODE);
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_SCAN_LIMIT, DISPLAY_0_TO_7)

  max7219_cascade_send(gpiob, max7219, 1, 3, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 1, 6, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 2, 3, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 2, 6, CHAR_NEGATIV);

  (* solange bis Ende :-)... *)
  while key <> 'q' do
  begin

    timer := timer+1;

    (* Ausgaben an GPIOA *)
    (* ...Test set_bit/clear_bit *)
    if (timer mod 2) = 0 then
      gpioa := set_bit(gpioa, 1)
    else
      gpioa := clear_bit(gpioa, 1);
    (* ...Test toggle_bit *)
    gpioa := toggle_bit(gpioa, 2);
    (* ...Test put_bit *)
    if (timer mod 5) = 0 then
      gpioa := put_bit(gpioa, 0, toggle_value(bit));
    (* ...und ausgeben *)
    set_gpio(PORT_WRITE_VALUE_GPIOA, gpioa);

    (* DS3231: Date/Time, Temperature ausgeben *)
    get_ds3231(ts);
    gotoxy(20, 5);
    with ts do
    begin
      write(
            int2zerostr(day,  2), '.',
            int2zerostr(mon,  2), '.',
            int2zerostr(year, 2), ', ',
            int2zerostr(hour, 2), ':',
            int2zerostr(min,  2), ':',
            int2zerostr(sec,  2)
           );
      (* ...Uhrzeit auf MAX7219 ausgeben *)
      set_userled(1);
      if old_ts.hour <> hour then
      begin
        old_ts.hour := hour;
        max7219_cascade_send(gpiob, max7219, 1, 8, trunc(hour/10));
        max7219_cascade_send(gpiob, max7219, 1, 7, (hour mod 10));
      end;
      if old_ts.min <> min then
      begin
        old_ts.min := min;
        max7219_cascade_send(gpiob, max7219, 1, 5, trunc(min/10));
        max7219_cascade_send(gpiob, max7219, 1, 4, (min mod 10));
      end;
      if old_ts.sec <> sec then
      begin
        old_ts.sec := sec;
        max7219_cascade_send(gpiob, max7219, 1, 2, trunc(sec/10));
        max7219_cascade_send(gpiob, max7219, 1, 1, (sec mod 10));
      end;
      if old_ts.day <> day then
      begin
        old_ts.day := day;
        max7219_cascade_send(gpiob, max7219, 2, 8, trunc(day/10));
        max7219_cascade_send(gpiob, max7219, 2, 7, (day mod 10));
      end;
      if old_ts.mon <> mon then
      begin
        old_ts.mon := mon;
        max7219_cascade_send(gpiob, max7219, 2, 5, trunc(mon/10));
        max7219_cascade_send(gpiob, max7219, 2, 4, (mon mod 10));
      end;
      if old_ts.year <> year then
      begin
        old_ts.year := year;
        max7219_cascade_send(gpiob, max7219, 2, 2, trunc(year/10));
        max7219_cascade_send(gpiob, max7219, 2, 1, (year mod 10));
      end;
      set_userled(0);
    end;
    write_num_xy(20, 6, ts.temp);

    (* Status User-Switch (onboard) ausgeben *)
    write_num_xy(14, 12, get_gpio(PORT_READ_USER_KEY));

    (* Status GPIOs (read) ausgeben *)
    (* ...indirekt Test get_bit (in byte2bin) *)
    write_str_xy(10, 17, byte2bin(get_gpio(PORT_READ_GPIOA)));
    write_str_xy(10, 18, byte2bin(get_gpio(PORT_READ_GPIOB)));

    (* Pause! bzw. Taste?*)
    key:=delay_keypressed(100);

  end;

  (* Cursor einschalten *)
  write(CURSOR_ON);

end.
