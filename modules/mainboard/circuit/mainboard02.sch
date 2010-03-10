v 20081231 1
C 40300 40500 0 0 0 title-bordered-A3.sym
C 51700 48100 1 0 0 header10-2.sym
{
T 51700 50100 5 10 0 1 0 0 1
device=HEADER10
T 52300 50200 5 10 1 1 0 0 1
refdes=J201
T 52400 47900 5 10 1 1 0 0 1
value=ICSP
T 51700 48100 5 10 0 1 0 0 1
footprint=DIN41651_10S
}
N 53100 49500 54000 49500 4
N 54000 49500 54000 47800 4
N 53100 49100 54000 49100 4
N 53100 48700 54000 48700 4
N 53100 48300 54000 48300 4
N 53100 49900 54000 49900 4
N 54000 49900 54000 50400 4
C 53800 50400 1 0 0 vcc-1.sym
{
T 53800 50400 5 10 0 1 0 0 1
netname=Vcc
T 53800 50400 5 10 0 1 0 0 1
net=Vcc:1
}
C 53900 47500 1 0 0 gnd-1.sym
{
T 53900 47500 5 10 0 1 0 0 1
netname=GND
}
C 51200 49400 1 0 0 nc-left-1.sym
{
T 51200 49800 5 10 0 0 0 0 1
value=NoConnection
T 51200 50200 5 10 0 0 0 0 1
device=DRC_Directive
}
N 51700 49900 50700 49900 4
{
T 50700 50000 5 10 1 1 0 0 1
netname=icsp_mosi
}
N 51700 49100 50700 49100 4
N 51700 48700 50700 48700 4
{
T 50700 48800 5 10 1 1 0 0 1
netname=icsp_sck
}
N 51700 48300 50700 48300 4
{
T 50700 48400 5 10 1 1 0 0 1
netname=icsp_miso
}
C 49300 49000 1 0 0 input-2.sym
{
T 49300 49200 5 10 0 0 0 0 1
net=nRST:1
T 49900 49700 5 10 0 0 0 0 1
device=none
T 50400 49400 5 10 1 1 0 7 1
value=nRST
}
C 46100 43100 1 0 0 led-2.sym
{
T 46900 43400 5 10 1 1 0 0 1
refdes=D201
T 46200 43700 5 10 0 0 0 0 1
device=LED
T 46100 43100 5 10 0 1 0 0 1
footprint=LED3
}
C 45900 47000 1 0 0 lm7805-1.sym
{
T 47500 48300 5 10 0 0 0 0 1
device=7805
T 47300 48000 5 10 1 1 0 6 1
refdes=U201
T 45900 47000 5 10 0 1 0 0 1
footprint=TO220S
}
N 44000 47600 44100 47600 4
N 46700 45900 46700 47000 4
N 46700 45900 46700 45500 4
N 48300 47600 48300 47900 4
C 48100 47900 1 0 0 vcc-1.sym
{
T 48100 47900 5 10 0 1 0 0 1
netname=Vcc
T 48100 47900 5 10 0 0 0 0 1
net=Vcc:1
}
C 46600 45200 1 0 0 gnd-1.sym
C 44100 47400 1 0 0 diode-1.sym
{
T 44500 48000 5 10 0 0 0 0 1
device=DIODE
T 44400 47900 5 10 1 1 0 0 1
refdes=D202
T 44200 47200 5 10 1 1 0 0 1
value=1N4007
T 44100 47400 5 10 0 1 0 0 1
footprint=ALF300
}
N 45900 47600 45000 47600 4
C 45300 47200 1 270 0 capacitor-2.sym
{
T 46000 47000 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 45200 46600 5 10 1 1 90 0 1
refdes=C201
T 46200 47000 5 10 0 0 270 0 1
symversion=0.1
T 45900 46200 5 10 1 1 90 0 1
value=220uF/25V
T 45300 47200 5 10 0 1 0 0 1
footprint=RCY100P
}
N 45500 47600 45500 47200 4
N 45500 46300 45500 45900 4
T 45300 48900 9 10 1 0 0 0 1
Power Circuit
C 50700 44100 1 0 0 connector8-1.sym
{
T 50800 47300 5 10 0 0 0 0 1
device=CONNECTOR_8
T 50800 46700 5 10 1 1 0 0 1
refdes=J202
T 50700 44100 5 10 0 1 0 0 1
footprint=HEADER8_2
}
N 52400 44600 54600 44600 4
{
T 53700 44800 5 10 1 1 0 0 1
netname=out_comp1
}
N 52400 46100 53000 46100 4
N 52400 45200 54600 45200 4
{
T 53600 45400 5 10 1 1 0 0 1
netname=out_comp2
}
N 52400 45500 53000 45500 4
N 52400 45800 54600 45800 4
{
T 54100 46000 5 10 1 1 0 0 1
netname=in_icp
}
N 52400 44900 53000 44900 4
N 52400 46400 54600 46400 4
{
T 53700 46500 5 10 1 1 0 0 1
netname=out_comp3
}
N 52400 44300 53000 44300 4
N 53000 46100 53000 43700 4
N 53000 43700 54600 43700 4
C 54600 43600 1 0 0 output-2.sym
{
T 55500 43800 5 10 0 0 0 0 1
net=timer_io:1
T 54800 44300 5 10 0 0 0 0 1
device=none
T 54700 43900 5 10 1 1 0 1 1
value=timer_io
}
C 45800 43100 1 90 0 jumper-1.sym
{
T 45300 43400 5 8 0 0 90 0 1
device=JUMPER
T 45400 43500 5 10 1 1 180 0 1
refdes=J204
T 45800 43100 5 10 0 1 0 0 1
footprint=JUMPER2
}
C 48700 42900 1 0 0 gnd-1.sym
{
T 48700 42900 5 10 0 1 0 0 1
netname=GND
}
N 45800 43200 46100 43200 4
N 47000 43200 47700 43200 4
N 44800 43200 44100 43200 4
C 42700 43100 1 0 0 input-2.sym
{
T 42700 43300 5 10 0 0 0 0 1
net=PB0:1
T 43300 43800 5 10 0 0 0 0 1
device=none
T 43700 43500 5 10 1 1 0 7 1
value=PB0
}
N 41800 46300 42300 46300 4
N 41800 46100 42800 46100 4
C 47700 43100 1 0 0 resistor-1.sym
{
T 48000 43500 5 10 0 0 0 0 1
device=RESISTOR
T 47900 43400 5 10 1 1 0 0 1
refdes=R201
T 47700 43100 5 10 0 1 0 0 1
footprint=ACY400
T 47900 42900 5 10 1 1 0 0 1
value=220
}
N 48800 43200 48600 43200 4
T 50300 41600 9 18 1 0 0 0 1
Main Board - Power and Debugger
T 50100 40800 9 10 1 0 0 0 1
2
T 51700 40800 9 10 1 0 0 0 1
2
T 54600 41100 9 10 1 0 0 0 1
2.0
T 54100 40800 9 10 1 0 0 0 1
Sundarapandian A
T 50200 41100 9 10 1 0 0 0 1
mainboard02.sch
C 47800 47200 1 270 0 capacitor-2.sym
{
T 48500 47000 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 47700 46600 5 10 1 1 90 0 1
refdes=C202
T 48700 47000 5 10 0 0 270 0 1
symversion=0.1
T 48400 46200 5 10 1 1 90 0 1
value=100uF/25V
T 47800 47200 5 10 0 1 0 0 1
footprint=RCY100P
}
N 48000 45900 48000 46300 4
N 48000 47200 48000 47600 4
C 42800 45800 1 0 0 switch-spdt-2.sym
{
T 43210 46650 5 10 0 0 0 0 1
device=Dual_Two_Way_Switch
T 43010 45600 5 10 1 1 0 0 1
refdes=S201
T 43110 46100 5 10 0 0 0 0 1
footprint=pwr_button
T 43500 47500 5 10 0 0 0 0 1
symversion=1.0
}
N 44000 47600 44000 46700 4
N 44000 46700 43800 46700 4
N 41800 46500 42000 46500 4
N 42000 46500 42000 46900 4
N 42000 46900 42800 46900 4
C 41900 48800 1 180 0 tb3p200.sym
{
T 41600 48950 5 10 1 1 180 0 1
refdes=TB201
T 41700 47100 5 10 0 0 180 0 1
device=TerminalBlock
T 41700 47500 5 10 0 0 180 0 1
footprint=3terminal_screw_block
}
N 41900 48600 42700 48600 4
N 42700 48600 42700 46900 4
N 41900 48000 42300 48000 4
N 42300 48000 42300 46300 4
N 47500 47600 49400 47600 4
C 49300 47500 1 270 0 led-2.sym
{
T 49700 47200 5 10 1 1 270 0 1
refdes=D203
T 49900 47400 5 10 0 0 270 0 1
device=LED
T 49300 47500 5 10 0 0 0 0 1
footprint=LED3
}
C 49300 46000 1 180 0 resistor-1.sym
{
T 49000 45600 5 10 0 0 180 0 1
device=RESISTOR
T 49100 45700 5 10 1 1 180 0 1
refdes=R202
T 49300 46000 5 10 0 1 180 0 1
footprint=ACY400
T 49100 46200 5 10 1 1 180 0 1
value=220
}
N 43800 45900 48400 45900 4
N 49300 45900 49400 45900 4
N 49400 45900 49400 46600 4
N 49400 47600 49400 47500 4
C 40700 46000 1 0 0 pwrjack-2.sym
{
T 40700 46900 5 10 0 0 0 0 1
device=PWRJACK
T 40700 46800 5 10 1 1 0 0 1
refdes=CONN201
T 40700 47100 5 10 0 0 0 0 1
footprint=pwrjack-2.1-5.5-3pin.fp
}
N 43800 47100 43800 47600 4
N 43800 47600 42700 47600 4
N 43800 46300 43900 46300 4
N 43900 46300 43900 46500 4
N 43900 46500 42700 46500 4
N 42700 46500 42700 46100 4
