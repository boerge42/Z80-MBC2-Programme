program mandel;

{$U+}

const
  breite = 80;
  hoehe  = 23;
  tiefe  = 93;
  xa     = -2.0;
  xe     = 0.7;
  ya     = 1.2;
  ye     = -1.2;

var
  dx, dy, cx, cy : real;
  farbe, schwarz : integer;
  spalte, zeile, zaehler : integer;
  realteil, zx, zy, zxx, zyy : real;

begin
  clrscr;
  dx:=(xe-xa)/(breite+1);
  dy:=(ya-ye)/(hoehe+1);
  schwarz:=0;

  (*  *)
  cx:=xa;
  for spalte:=1 to breite do
  begin
    cx:=xa+spalte*dx;
    for zeile:=1 to hoehe do
    begin
      cy:=ya-zeile*dy;
      zx:=0; zy:=0; zaehler:=0; zxx:=0; zyy:=0;
      repeat
        zxx:=zx*zx;
        zyy:=zy*zy;
        realteil:=zxx-zyy+cx;
        zy:=2*zx*zy+cy;
        zx:=realteil;
        zaehler:=zaehler+1;
      until (sqrt(zxx+zyy)>2.0) or (zaehler=tiefe);
      gotoxy(1,24);
      write('                  ');
      gotoxy(1,24);
      write(spalte, ', ', zeile, ' --> ', zaehler);

      gotoxy(spalte, zeile);
      if zaehler >= tiefe then
      begin
        write(' ');
      end
      else
       { write(zaehler mod 10); }
       write(chr(33+zaehler));
      begin
      end;
    end;
  end;

end.
