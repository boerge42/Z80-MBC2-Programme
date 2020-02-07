(*
***************************************************
* Tetris-Clone for Turbo Pascal 3.0
* ---------------------------------
*         Uwe Berger, 2019
*
* ...Defines, Types...
*
* ---------
* Have fun!
*
***************************************************
*)

type
  score_t = record
              level     : integer;
              bricks    : integer;
              lines     : integer;
              points    : integer;
              time      : integer;
              game_over : boolean;
              pause     : boolean;
            end;

  line_t = array[0..3] of char;

  brick_t = record
              x   : integer;
              y   : integer;
              dxy : integer;
              grid: array[0..3] of line_t;
            end;

  gridline_t = array[0..19] of char;   (* keine Kostantenrechn. angebbar *)
  grid_t = array[0..9] of gridline_t;

const
  default_bricks : array[0..27] of line_t =
    (
       ('O', 'O', ' ', ' '),
       ('O', 'O', ' ', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       ('J', ' ', ' ', ' '),
       ('J', 'J', 'J', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', 'L', ' '),
       ('L', 'L', 'L', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       (' ', 'S', 'S', ' '),
       ('S', 'S', ' ', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       ('Z', 'Z', ' ', ' '),
       (' ', 'Z', 'Z', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       ('T', 'T', 'T', ' '),
       (' ', 'T', ' ', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' '),
       ('I', 'I', 'I', 'I'),
       (' ', ' ', ' ', ' '),
       (' ', ' ', ' ', ' ')
    );

  GRID_DX   = 10;
  GRID_DY   = 20;

(* ************************************** *)
(* ************************************** *)
(* ************************************** *)

