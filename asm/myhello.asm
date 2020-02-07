          org  100h
bdos      equ  0005h
writestr  equ  0009h

start:    mvi c, writestr
          lxi d, hello$
          call bdos
          mvi c, writestr
          lxi d, crlf
          call bdos 
          ret

hello:    db 'Hallo Uwe, wie geht es dir?$'
crlf:     db 10, 10, 13, '$'
          end
