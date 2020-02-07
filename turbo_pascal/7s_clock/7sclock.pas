(*
*****************************************************
* Z80-MBC2-Board --> Uhr auf Basis DS3231 und MAX7219
* ---------------------------------------------------
*                 Uwe Berger, 2019
*
*
* ---------
* Have fun!
*
*****************************************************
*)

program seven_seg_clock;

{$U+}

{$I mbc2io.inc}
{$I bitman.inc}
{$I max7219.inc}
{$I my2wire.inc}


(* ************************* *)
(* ************************* *)
(* ************************* *)

var
  ts, old_ts  : ds3231_t;
  gpiob, i, t : byte;
  max7219     : max7219_pinout_t;
  my2w        : my2wire_pinout_t;


begin
  writeln('Z80-MBC-Board: 7-Seg-LED-Clock; Uwe Berger, 2019');
  writeln('Init...');
  old_ts.hour := 0;
  old_ts.min  := 0;
  old_ts.sec  := 0;
  old_ts.day  := 0;
  old_ts.mon  := 0;
  old_ts.year := 0;
  old_ts.temp := -100;


  writeln('...GPIOB --> Output');
  gpiob := 0;  (* GPIOB als Output (fuer MAX7219) *)
  set_gpio(PORT_SET_IODIR_GPIOB, 0);
  set_gpio(PORT_WRITE_VALUE_GPIOB, gpiob);

  (* MAX7219-Initialisierung *)
  (* ...MAX7219-Hardware definieren *)
  with max7219 do
  begin
    port := PORT_WRITE_VALUE_GPIOB;
    count := 3;
    din   := 0;
    cs    := 1;
    clk   := 2;
  end;

  max7219_init_hw(gpiob, max7219);
  writeln('...MAX7219-Intensity');
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_INTENSITY, 2);
  writeln('...MAX7219-Decode-Mode');
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_DECODE_MODE, CODE_B_7_0);
  max7219_cascade_send(gpiob, max7219, 3, REG_ADDR_DECODE_MODE, CODE_B_7_2);
  writeln('...MAX7219-Shutdown to normal');
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_SHUTDOWN, NORMAL_MODE);
  writeln('...MAX7219-Scanlimit');
  max7219_cascade_send_all(gpiob, max7219, REG_ADDR_SCAN_LIMIT, DISPLAY_0_TO_7);

  writeln('...Output template');
  max7219_cascade_send(gpiob, max7219, 1, 3, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 1, 6, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 2, 3, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 2, 6, CHAR_NEGATIV);
  max7219_cascade_send(gpiob, max7219, 3, 1, $4E); (* C    *)
  max7219_cascade_send(gpiob, max7219, 3, 2, $63); (* Grad *)

  (* My2Wire-Initialisierung *)
  writeln('...MY2WIRE');
  with my2w do
  begin
    port  := PORT_WRITE_VALUE_GPIOB;
    clock := 5;
    data  := 6;
  end;
  gpiob := my2wire_init(gpiob, my2w);

  writeln('Init ready!');
  writeln('');
  writeln('Exit: press ''userkey'' or CTRL+C...');


  (* solange bis Ende :-)... *)
  while userkey_pressed <> true do
  begin

    (* DS3231: Date/Time, Temperature ausgeben *)
    get_ds3231(ts);
    with ts do
    begin
      (* userled --> Visualisierung Dauer Portzugriff*)
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
        (* alle 10s Temperature senden *)
        if sec mod 10 = 0 then
        begin
          gpiob := my2wire_start(gpiob, my2w);
          my2wire_send_byte(gpiob, my2w, temp);
          gpiob := my2wire_stop(gpiob, my2w);
        end;
      end;
      if old_ts.day <> day then
      begin        old_ts.day := day;
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
      if old_ts.temp <> temp then
      begin
        old_ts.temp := temp;
        (* Temperatur wird etwas aufwaendiger... *)
        (* ...entspr. Display-'Zeile' loeschen (ausser Template) *)
        for i:=8 downto 3 do
          max7219_cascade_send(gpiob, max7219, 3, i, $0F);
        (* ...Annahme: Wertebereich -99...+99! *)
        if temp > $7f   (* 7.Bit ist Vorzeichen... *)
          then t := temp - 128
          else t := temp;
        i := 3;
        max7219_cascade_send(gpiob, max7219, 3, i, (t mod 10));
        if t > 9 then
        begin
          i := i+1;
          max7219_cascade_send(gpiob, max7219, 3, i, trunc(t/10));
        end;
        (* Temperatur negativ, dann noch Vorzeichen ausgeben *)
        if temp > $7F then
        begin
          i := i+1;
          max7219_cascade_send(gpiob, max7219, 3, i, CHAR_NEGATIV);
        end;
      end;
      set_userled(0);
    end;
    delay(50);
  end;
end.
