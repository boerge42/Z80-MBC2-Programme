(*
*********************************************************
* Tetris-Clone for Turbo Pascal 3.0
* ---------------------------------
*        Uwe Berger, 2019
*
* ...Main...
*
* ---------
* Have fun!
*
********************************************************
*)
program tetris;

{$B-}
{$I tetridef.inc}
{$I tetrigam.inc}
{$I tetriout.inc}
{$I tetriinp.inc}

var
  lines_del         : integer;
  free_iterations   : integer;
  iteration_time_ms : integer;
  timer             : integer;
  drop_key_pressed  : boolean;

(* ******************************* *)
procedure brick_is_falling;
begin
   temp_brick := brick;
   temp_brick.y := temp_brick.y + 1;
   if collision <> true then
   begin
     clear_brick;
     brick := temp_brick;
     write_brick;
     if drop_key_pressed <> true then free_iterations := free_iterations+1;
   end
   else     (* Ziegel ist in der Endlage angekommen *)
   begin
     copy_brick2grid;
     (* volle Zeilen loeschen *)
     lines_del := delete_full_lines;
     (* wenn Zeilen geloescht, Grid ausgeben *)
     if lines_del > 0 then
     begin
       draw_grid;
       score.lines := score.lines + lines_del;
     end;
     (* Level/Itearionszeit neu berechnen *)
     score.level := compute_level(score);
     iteration_time_ms := compute_iteration_time(score);
     (* neuer Spielstein *)
     new_brick;
     write_brick;
     write_next_brick;
     (* Game over? *)
     temp_brick := brick;
     if collision = true then
     begin
       score.game_over := true;
       write_game_over;
     end
     else
     begin
       score.bricks := score.bricks+1;
       score.points:=
          trunc(score.points+((21+(3*score.level))-free_iterations));
     end;
     (* Spielstand ausgeben *)
     write_score(score);
     free_iterations := 0;
     drop_key_pressed := false;
   end;
end;


(* ******************************* *)
(* ******************************* *)
(* ******************************* *)
begin
  write(ATTR_CURSOR_OFF);
  randomize;
  init_game;
  free_iterations := 0;
  drop_key_pressed := false;
  timer := 0;
  iteration_time_ms := compute_iteration_time(score);
  init_screen;
  write_next_brick;
  write_brick;
  (* Endlos-Loop *)
  while 1>0 do
  begin
    (* Tasteneingaben abarbeiten *)
    if keypressed then
    begin
      case my_readkey of
        KEY_Q_LOW:                          (* Spielende *)
          begin
            write(ATTR_CURSOR_ON);
            exit;
          end;
        KEY_ARROW_UP, KEY_W_LOW, KEY_8:     (* Ziegel drehen *)
          if (score.game_over = false) and (score.pause = false) then
          begin
            (* copy&rotate brick --> temp_brick *)
            rotate_brick;
            if collision <> true then
            begin
              clear_brick;
              brick := temp_brick;
              write_brick;
            end;
          end;
        KEY_ARROW_DOWN, KEY_X_LOW, KEY_2:   (* Ziegel fallen lassen *)
          if (score.game_over = false) and (score.pause = false) then
          begin
            drop_key_pressed := true;
          end;
        KEY_ARROW_LEFT, KEY_A_LOW, KEY_4:   (* Ziegel nach links *)
          if (score.game_over = false) and (score.pause = false) then
          begin
            temp_brick := brick;
            temp_brick.x := temp_brick.x - 1;
            if collision <> true then
            begin
              clear_brick;
              brick := temp_brick;
              write_brick;
            end;
          end;
        KEY_ARROW_RIGHT, KEY_D_LOW, KEY_6:  (* Ziegel nach rechts *)
          if (score.game_over = false) and (score.pause = false) then
          begin
            temp_brick := brick;
            temp_brick.x := temp_brick.x + 1;
            if collision <> true then
            begin
              clear_brick;
              brick := temp_brick;
              write_brick;
            end;
          end;
        KEY_P_LOW:    (* Pause *)
          if score.game_over = false then
          begin
            if score.pause = true then
              begin
                score.pause := false;
                write_pause_clear;
              end
            else
              begin
                score.pause := true;
                write_pause;
              end;
          end;
        KEY_N_LOW:     (* neues Spiel *)
          begin
            init_game;
            init_screen;
            free_iterations := 0;
            drop_key_pressed := false;
            timer := 0;
            iteration_time_ms := compute_iteration_time(score);
            write_next_brick;
            write_brick;
          end;
      end;
    end;

    (* Timer abarbeiten *)
    if score.game_over <> true then
    begin
      delay(1);
      timer := timer+1;
      if ((timer >= iteration_time_ms) or drop_key_pressed) then
      begin
        timer := 0;
        brick_is_falling;
      end;
    end;

  end;
  write(ATTR_CURSOR_ON);
end.


