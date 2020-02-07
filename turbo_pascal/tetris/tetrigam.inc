(*
***************************************************
* Tetris-Clone for Turbo Pascal 3.0
* ---------------------------------
*         Uwe Berger, 2019
*
* ...Gameroutines...
*
* ---------
* Have fun!
*
***************************************************
*)

var
  grid : grid_t;
  score : score_t;
  brick, temp_brick, next_brick : brick_t;


(* ********************************* *)
function compute_iteration_time(s : score_t) : integer;
begin
  compute_iteration_time := 500-((s.level-1)*50);
end;


(* ********************************* *)
function compute_level(s : score_t) : integer;
begin
  with s do
  begin
    if lines <= 0 then level:=1
    else if (lines >= 1) and (lines <= 90) then level:=trunc(1+((lines-1)/10))
         else if lines >= 91 then level:=10;
  compute_level := level;
  end;
end;


(* ********************************* *)
function delete_full_lines : integer;

var
  count, i, j, k : integer;
  full           : boolean;

begin
  count := 0;
  for j:=0 to GRID_DY-1 do
  begin
    full:=true;
    for i:=0 to GRID_DX-1 do if grid[i][j] = ' ' then full := false;
    if full = true then
    begin
      count := count+1;
      for k:=j downto 0 do
        for i:=0 to GRID_DX-1 do
          if k>0 then grid[i][k] := grid[i][k-1]
          else grid[i][k] := ' ';
    end;
  end;
  delete_full_lines:=count;
end;


(* ********************************* *)
procedure copy_brick2grid;

var
  x, y : integer;

begin
  for y := 0 to brick.dxy-1 do
    for x := 0 to brick.dxy-1 do
      if brick.grid[x][y] <> ' ' then
        grid[x+brick.x][y+brick.y] := brick.grid[x][y];
end;


(* ********************************* *)
function collision : boolean;

var
  x, y : integer;

begin
  (* am wahrscheinlichsten kracht es zuerst unten (Rand/Stein im Weg)... *)
  for y := temp_brick.dxy-1 downto 0 do
    for x := 0 to temp_brick.dxy-1 do
      (* unten *)
      if temp_brick.grid[x][y] <> ' ' then
      begin
        if temp_brick.y+y+1 > GRID_DY then
        begin
          collision := true;
          exit;
        end
        (* Feld belegt *)
        else if grid[temp_brick.x+x][temp_brick.y+y] <> ' ' then
        begin
          collision := true;
          exit;
        end
        (* links *)
        else if temp_brick.x+x < 0 then
        begin
          collision := true;
          exit;
        end
        (* rechts *)
        else if temp_brick.x+1+x > GRID_DX then
        begin
          collision := true;
          exit;
        end
        (* oben *)
        else if temp_brick.y+y < 0 then
        begin
          collision := true;
          exit;
        end
        ;
      end;
  collision := false;
end;


(* ********************************* *)
procedure rotate_brick;

var
  i, j : integer;

begin
  temp_brick.x := brick.x;
  temp_brick.y := brick.y;
  temp_brick.dxy := brick.dxy;
  for i:=0 to temp_brick.dxy-1 do
    for j:=0 to temp_brick.dxy-1 do
      temp_brick.grid[i][j]:=brick.grid[temp_brick.dxy-j-1][i];
end;


(* ********************************* *)
procedure new_brick;

var
  r : integer;
  x, y : integer;

begin
  brick := next_brick;
  r := random(7);
  for y := 0 to 3 do
    for x := 0 to 3 do
    begin
      next_brick.grid[x][y] := default_bricks[r*4+y][x];
      case r of
        0 :    begin
                 next_brick.x   := 3;
                 next_brick.y   := 0;
                 next_brick.dxy := 2;
               end;
        1..5 : begin
                 next_brick.x   := 3;
                 next_brick.y   := 0;
                 next_brick.dxy := 3;
               end;
        6 :    begin
                 next_brick.x   := 3;
                 next_brick.y   := -1;
                 next_brick.dxy := 4;
               end;
      end;
    end;
end;


(* ********************************* *)
procedure init_game;

var
  x, y : integer;

begin
  (* Spielstand initialisieren *)
  with score do
  begin
    new_brick;
    new_brick;
    level     := 1;
    bricks    := 1;
    lines     := 0;
    points    := 0;
    time      := 0;
    game_over := false;
    pause     := false;
  end;
  (* Grid mit Blanks initialisieren *)
  for x := 0 to GRID_DX-1 do
    for y := 0 to GRID_DY-1 do
      grid[x][y] := ' ';
end;

(* ********************************* *)
(* ********************************* *)
(* ********************************* *)


