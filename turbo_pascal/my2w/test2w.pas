(*
**********************************************************






**********************************************************
*)

program test2w;

{$U+}

{$I bitman.inc}
{$I mbc2io.inc}
{$I my2wire.inc}

var
  my2w  : my2wire_pinout_t;
  gpiob : byte;
  i     : byte;



(* ***************************** *)
(* ***************************** *)
(* ***************************** *)
begin
  writeln('Test my2wire; Uwe Berger, 2020');
  writeln('Init...');

  (* Konfig./Init HW etc. *)
  writeln('...GPIOB --> Output');
  gpiob := 0;
  set_gpio(PORT_SET_IODIR_GPIOB, 0);
  set_gpio(PORT_SET_GPPU_GPIOB, 1);
  set_gpio(PORT_WRITE_VALUE_GPIOB, gpiob);

  with my2w do
  begin
    port  := PORT_WRITE_VALUE_GPIOB;
    clock := 5;
    data  := 6;
  end;

  gpiob := my2wire_init(gpiob, my2w);
  i := 0;

  while userkey_pressed <> true do
  begin
    gpiob := my2wire_start(gpiob, my2w);
    my2wire_send_byte(gpiob, my2w, i);
    gpiob := my2wire_stop(gpiob, my2w);
    i := i + 1;
    delay(200);
  end;

end.
