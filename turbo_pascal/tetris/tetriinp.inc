(*
******************************************************
* Tetris-Clone for Turbo Pascal 3.0
* ---------------------------------
*        Uwe Berger, 2019
*
* ...Input...
*
* ---------
* Have fun!
*
******************************************************
*)

const

  KEY_A_LOW = 97;
  KEY_D_LOW = 100;
  KEY_W_LOW = 119;
  KEY_X_LOW = 120;
  KEY_N_LOW = 110;
  KEY_P_LOW = 112;
  KEY_Q_LOW = 113;
  KEY_4 = 52;
  KEY_8 = 56;
  KEY_6 = 54;
  KEY_2 = 50;

  KEY_ARROW_LEFT  = 1068;
  KEY_ARROW_RIGHT = 1067;
  KEY_ARROW_UP    = 1065;
  KEY_ARROW_DOWN  = 1066;


(* ********************************** *)
function my_readkey : integer;
var
  c : char;
begin
  read(kbd, c);
  my_readkey := ord(c);
  (* Sondertaste? *)
  if c = #27 then
    begin
      read(kbd, c);
      (* bei Pfeiltastengruppe kommt eine 91 nach ESC... *)
      if c = #91 then
      begin
        read(kbd, c);
        my_readkey := ord(c) + 1000;
      end
      (* ansonsten andere Sondertaste... *)
      else my_readkey := 1000;
    end;
end;

(* ********************************** *)
(* ********************************** *)
(* ********************************** *)


