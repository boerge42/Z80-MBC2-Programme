program myclock;

type
  time_t = record
             sec  : byte;
             min  : byte;
             hour : byte;
             day  : byte;
             mon  : byte;
             year : byte;
           end;

var
  key   : char;
  ts    : time_t;
  count : integer;


begin

count := 0;
clrscr;

while key <> 'q' do
begin
  if count = 0 then
  begin
    port[1] := $84;
    ts.sec  := port[0];
    ts.min  := port[0];
    ts.hour := port[0];
    ts.day  := port[0];
    ts.mon  := port[0];
    ts.year := port[0];
    gotoxy(1, 1);
    with ts do
    begin
      write(day:2, '.', mon:2, '.', year:2, '; ', hour:2, ':', min:2, ':', sec:
    end;
  end;

  if keypressed then read(kbd, key);
  count := count + 1;
  if count > 100 then count := 0;
  delay(1);
end;

end.

