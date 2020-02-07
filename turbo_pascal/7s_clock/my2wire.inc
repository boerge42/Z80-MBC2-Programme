(*
******************************************************
* Einfaches serielles Uebertragungsprotokoll
* ==========================================
*              Uwe Berger, 2020
*
* bitweise Uebertragung ueber 2 Leitungen:
*  -> clock
*  -> data
*
* Startbedingung:
*  * clock und data sind auf high
*  * data geht auf low
*  * clock geht auch auf low
*
* Datenuebertragung:
*  * clock ist low
*  * data wird nach Zustand des entsprechenden
*    Datenbits gesetzt
*  * Impuls auf clock (low -> high -> low)
*  * naechstes Datenbit...
*
* Stoppbedingung:
*  * clock und data sind low
*  * clock geht auf high
*  * data geht auf high
*
*
* ---------
* Have fun!
*
******************************************************
*)

type
  my2wire_pinout_t = record
                       port  : byte;
                       clock : byte;
                       data  : byte;
                     end;

const
  delay_val = 5;

(* ************************ *)
function my2wire_init(gpio_val : byte; hw : my2wire_pinout_t) : byte;
var t : byte;
begin
  (* clock und data auf high, aber Reihenfolge beachten *)

  (* clock --> low *)
  t := clear_bit(gpio_val, hw.clock);
  set_gpio(hw.port, t);

  (* data --> high *)
  t := set_bit(t, hw.data);
  set_gpio(hw.port, t);

  (* clock --> high *)
  t := set_bit(t, hw.clock);
  set_gpio(hw.port, t);

  my2wire_init := t;
end;

(* ************************ *)
function my2wire_start(gpio_val : byte; hw : my2wire_pinout_t) : byte;
var t : byte;
begin
  (* Annahme, clock ist auf high! *)

  (* data --> low *)
  t := clear_bit(gpio_val, hw.data);
  set_gpio(hw.port, t);

  (* clock --> low *)
  t := clear_bit(t, hw.clock);
  set_gpio(hw.port, t);

  my2wire_start := t;
end;

(* ************************ *)
function my2wire_stop(gpio_val : byte; hw : my2wire_pinout_t) : byte;
var t : byte;
begin
  (* clock ist bereits low;  data --> low *)
  t := clear_bit(gpio_val, hw.data);
  set_gpio(hw.port, t);

  (* clock --> high *)
  t := set_bit(t, hw.clock);
  set_gpio(hw.port, t);

  (* data --> high *)
  t := set_bit(t, hw.data);
  set_gpio(hw.port, t);

  my2wire_stop := t;
end;

(* ************************ *)
procedure my2wire_send_byte(gpio_val : byte; hw : my2wire_pinout_t; b : byte);
var i : byte;
begin
  for i:= 0 to 7 do
  begin
    (* data nach zu sendenden Bit von b setzen *)
    gpio_val := put_bit(gpio_val, hw.data, get_bit(b, i));
    set_gpio(hw.port, gpio_val);
    delay(delay_val);
    (* Impulse auf clock low --> high --> low *)
    set_gpio(hw.port, set_bit(gpio_val, hw.clock));
    delay(delay_val);
    set_gpio(hw.port, clear_bit(gpio_val, hw.clock));
    delay(delay_val);
  end;
end;

(* ************************ *)
(* ************************ *)
(* ************************ *)
