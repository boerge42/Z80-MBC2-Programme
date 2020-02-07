program fact;

const
   count = 1000;
   f  = 20;

var
   i : integer;
   bla : real;
   ts  : real;

function factorial(n : integer) : real;
begin
   if n <= 1 then
      factorial := n
   else
      factorial := n * factorial(n - 1);
end;

function read_rtc : real;
var ret : real;
begin
   port[1] := $84;
   ret := port[$0]      +
          port[0] * 60  +
          port[0] * 3600;
   read_rtc := ret;
end;

begin
   writeln(f, '!=', factorial(f):18:0);
   ts := read_rtc;
   write(count,'x ',f,'!...');
   for i := 1 to count do
      bla := factorial(f);
   ts := read_rtc - ts;
   writeln(' in ',ts:5:0, 's');
end.
