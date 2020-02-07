(*
***************************************************
*
* Routinen zur Bitmanipulation
* ============================
*       Uwe Berger, 2019
*
*
* ---------
* Have fun!
*
***************************************************
*)


(* ************************* *)
function set_bit(v : byte; i : byte) : byte;
begin
  set_bit := v or (1 shl i);
end;

(* ************************* *)
function clear_bit(v : byte; i : byte) : byte;
begin
  clear_bit := v and not (1 shl i);
end;

(* ************************* *)
function put_bit(v : byte; i : byte; f : byte) : byte;
begin
   if f = 0 then
     put_bit := clear_bit(v, i)
   else
     put_bit := set_bit(v, i);
end;

(* ************************* *)
function toggle_bit(v : byte; i : byte) : byte;
begin
  toggle_bit := v xor (1 shl i);
end;

(* ************************* *)
function get_bit(v : byte; i : byte) : byte;
begin
  if (v and (1 shl i)) <> 0 then
    get_bit := 1
  else
    get_bit := 0;
end;

(* ************************* *)
(* ************************* *)
(* ************************* *)


