(*
************************************************
* Tetris-Clone for Turbo Pascal 3.0
* ---------------------------------
*         Uwe Berger, 2019
*
* ...Outouts...
*
* ---------
* Have fun!
*
************************************************
*)

type
  screen_line = string[80];

const
  GRID_X = 10;
  GRID_Y =  3;

  VKEY_X = 30;
  VKEY_Y = 10;

  VALUE_X = 41;
  VALUE_Y = 10;

  NEXT_BRICK_X = 30;
  NEXT_BRICK_Y = 3;

  ATTR_CURSOR_OFF = #27'[?25l';
  ATTR_CURSOR_ON  = #27'[?25h';
  ATTR_BLINK_ON   = #27'[5m';
  ATTR_INVERSE    = #27'[7m';
  ATTR_UNDERLINE  = #27'[4m';
  ATTR_HIGHLIGHT  = #27'[1m';
  ATTR_NORMAL     = #27'[0m';


(* ******************************** *)
procedure write_char_xy(x, y : byte; c : char);
begin
  gotoxy(x, y);
  write(c);
end;



(* ******************************** *)
procedure write_str_xy(x, y : byte; var s : screen_line);
begin
  gotoxy(x, y);
  write(s);
end;


(* ******************************** *)
procedure draw_grid;

var
  x, y : integer;

begin
  for x := 0 to GRID_DX-1 do
    for y := 0 to GRID_DY-1 do
    begin
      if grid[x][y] <> ' ' then write(ATTR_INVERSE);
      write_char_xy(GRID_X+1+x, GRID_Y+y, grid[x][y]);
      if grid[x][y] <> ' ' then write(ATTR_NORMAL, ATTR_HIGHLIGHT);
    end;
end;


(* ******************************** *)
procedure write_game_over;
begin
  gotoxy(VKEY_X, VKEY_Y+9);
  write(ATTR_INVERSE, ATTR_BLINK_ON);
  write('GAME OVER!');
  write(ATTR_NORMAL, ATTR_HIGHLIGHT)
end;


(* ******************************** *)
procedure write_pause;
begin
  gotoxy(VKEY_X, VKEY_Y+9);
  write(ATTR_INVERSE);
  write('Pause...');
  write(ATTR_NORMAL, ATTR_HIGHLIGHT);
end;


(* ******************************** *)
procedure write_pause_clear;
begin
  gotoxy(VKEY_X, VKEY_Y+9);
  write('        ');
end;


(* ******************************** *)
procedure write_score(s : score_t);
begin
  with s do
  begin
    gotoxy(VALUE_X, VALUE_Y);   write(level:6);
    gotoxy(VALUE_X, VALUE_Y+2); write(bricks:6);
    gotoxy(VALUE_X, VALUE_Y+4); write(lines:6);
    gotoxy(VALUE_X, VALUE_Y+6); write(points:6);
  end;
end;


(* ******************************** *)
procedure init_screen;

var
  i, x, y : integer;
  s : screen_line;

begin
  clrscr;
  (* Ego *)
  gotoxy(1,1);
  write(ATTR_UNDERLINE);
  write('Tetris-Clone with Turbo Pascal 3.0; Uwe Berger, 2019');
  write(ATTR_NORMAL, ATTR_HIGHLIGHT);
  (* Rahmen *)
  for i:= GRID_Y to GRID_Y+19 do
  begin
    write_char_xy(GRID_X     , i, '|');
    write_char_xy(GRID_X + 11, i, '|');
  end;
  write_char_xy(GRID_X     , GRID_Y + 20, '+');
  write_char_xy(GRID_X + 11, GRID_Y + 20, '+');
  for i := GRID_X + 1 to GRID_X + 10 do write_char_xy(i, GRID_Y + 20, '-');
  (* Spielstand *)
  write(ATTR_NORMAL);
  s := 'level....:';  write_str_xy(VKEY_x, VKEY_Y  ,  s);
  s := 'bricks...:';  write_str_xy(VKEY_x, VKEY_Y+2,  s);
  s := 'lines....:';  write_str_xy(VKEY_x, VKEY_Y+4,  s);
  s := 'points...:';  write_str_xy(VKEY_x, VKEY_Y+6,  s);
  s := 'q --> exit!'; write_str_xy(VKEY_x, VKEY_Y+12, s);
  write(ATTR_HIGHLIGHT);
  write_score(score);
end;


(* ******************************** *)
procedure write_next_brick;

var
  x, y : integer;

begin
  for y := 0 to 3 do
    for x := 0 to 3 do
    begin
      if next_brick.grid[x][y] <> ' ' then write(ATTR_INVERSE);
      write_char_xy(NEXT_BRICK_X+x, NEXT_BRICK_Y+y, next_brick.grid[x][y]);
      if next_brick.grid[x][y] <> ' ' then write(ATTR_NORMAL, ATTR_HIGHLIGHT);
    end;
end;


(* ******************************** *)
procedure write_brick;

var
  x, y : integer;

begin
  write(ATTR_INVERSE);
  for y := 0 to brick.dxy-1 do
    for x := 0 to brick.dxy-1 do
      if brick.grid[x][y] <> ' ' then
         write_char_xy(GRID_X+1+brick.x+x, GRID_Y+brick.y+y, brick.grid[x][y]);
  write(ATTR_NORMAL, ATTR_HIGHLIGHT);
end;



(* ******************************** *)
procedure clear_brick;

var
  x, y : integer;

begin
  for y := 0 to brick.dxy-1 do
    for x := 0 to brick.dxy-1 do
      if brick.grid[x][y] <> ' ' then
         write_char_xy(GRID_X+1+brick.x+x, GRID_Y+brick.y+y, ' ');
end;


(* ******************************** *)
(* ******************************** *)
(* ******************************** *)

