program my_blink;

begin
  while keypressed <> true do
  begin
    port[1] := 0;
    port[0] := 1;
    delay(200);
    port[1] := 0;
    port[0] := 0;
    delay(200);
  end;
end.
