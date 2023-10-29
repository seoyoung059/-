clear all;
close all;
s = serialport("COM9",57600);
pause(1)

send_serialbt(s,1) %%1 stop 2 forward 3 backward 4 right 5 left

figure;
axes;

pause(0.5)

while true
  fprintf("Command?... ");
  w = waitforbuttonpress;
  value = double(get(gcf,'CurrentCharacter'));
  if value == 28
      fprintf("Left\n");
      send_serialbt(s,5);
      send_serialbt(s,5);
      pause(0.5)
      send_serialbt(s,1);
      send_serialbt(s,1);
  elseif value == 29
      fprintf("Right\n");
      send_serialbt(s,4);
      send_serialbt(s,4);
      pause(0.5)
      send_serialbt(s,1);
      send_serialbt(s,1);
  elseif value == 30
      fprintf("Forward\n");
      send_serialbt(s,2);
      send_serialbt(s,2);
  elseif value == 31
      fprintf("Backward\n");
      send_serialbt(s,3);
      send_serialbt(s,3);
  elseif value == 115 %%s key
      fprintf("Stop\n");
      send_serialbt(s,1);
      send_serialbt(s,1);
  elseif value == 113 %%q
      fprintf("Curve Left Forward\n");
      send_serialbt(s,5);
      send_serialbt(s,5);
      pause(0.3)
      send_serialbt(s,2);
      send_serialbt(s,2);
  elseif value == 101 %%e
      fprintf("Curve Right Forward\n");
      send_serialbt(s,4);
      send_serialbt(s,4);
      pause(0.3)
      send_serialbt(s,2);
      send_serialbt(s,2);
  elseif value == 97 %%aw
      fprintf("Curve Left backward\n");
      send_serialbt(s,4);
      send_serialbt(s,4);
      pause(0.3)
      send_serialbt(s,3);
      send_serialbt(s,3);
  elseif value == 100 %%d
      fprintf("Curve Right backward\n");
      send_serialbt(s,5);
      send_serialbt(s,5);
      pause(0.3)
      send_serialbt(s,3);
      send_serialbt(s,3);
  else
      fprintf("Stop %f\n",value);
  end
end