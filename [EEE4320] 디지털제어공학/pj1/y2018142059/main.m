%% main program for robotic behaviours

%
%[objx, objy, oradius] = mapenv();

head = 0; x=200; y=200; % starting position and heading angle 
objectNumber = 10;
diameter = 55; radius = diameter /2; % robot diameter 
B = 50; % wheel base
t = 0:0.1:2*pi+0.2; % to draw robot body


while(1)
hold off;

% (x,y, head) is the current position and head angle
rx = x + r * cos(t); ry = y + r * sin(t); % robot drawing
plot(rx, ry) % draw robot body
axis([0 1100 0 1100])
hold on;
line([x x+radius*cos(head)], [y y+radius*sin(head)]) % mark head

%mapenv(); % draw the environment with obstacles


[IR] = IRsensor_reading (head, x, y, objx, objy, oradius, objectNumber);
%. . .
% robot motor control part,
% map from sensor values to motor commands (L,R)
% it depends on what kind of behaviours you wish to test
%. . .
%. . .


% kinematic simulation - next position (x,y) and next head angle
% which uses transient motor actions [ VL and VR ]
%. . .


% check if the robot collides with any obstacle or with the wall
% this can be checked by measuring the distance between the
% robot center position and a center position of an obstacle
% (1) distance(robot center, center of an object)
% < radius of robot + radius of an object.
% (2) distance(robot center, nearest wall) < radius of robot
%
%. . .


pause(0.0001); % this may be unnecessary
end