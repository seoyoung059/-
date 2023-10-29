%% main program for robotic behaviours

clc; clear; close all;
%
[objx, objy, oradius] = mapenv();

head = pi/2; x=200; y=200;              % starting position and heading angle 
objectNumber = 10;
diameter = 55; radius = diameter /2;    % robot diameter 
wdiameter = 5; wradius = wdiameter /2;  % wheel diameter
B = 50; % wheel base
t = 0:0.1:2*pi+0.2; % to draw robot body
dt=0.1;

Numberofloop = 500; % Numberofloop

pi_x = 0.0; pi_y = 0.0;             % path integral initialize
obstacle_avoidance=false;           % obstacle avoidance mode flag
savex=zeros(1,Numberofloop);        % to plot x
savey=zeros(1,Numberofloop);        % to plot y
savef=zeros(1,Numberofloop);        % to plot f
collide = false;                    % collide flag (if 1, the loop ends)
wL =0.0; wR = 0.0;                  % angular velocity initialize
dhead = head;           % desired head initialize
N=0;                    % loop number

for N=1:1:Numberofloop
% (x,y, head) is the current position and head angle
rx = x + radius * cos(t); ry = y + radius * sin(t); % robot drawing

%draw robot body
subplot(1,3,1); hold off;
if(obstacle_avoidance)    p1 = plot(rx, ry,'Color','r'); % red when obstacle avoiding mode
else                      p1 = plot(rx, ry,'Color','b'); % blue when random movement mode
end
set(gcf,'units','pixels','pos',[500 500 1500 500])
axis([0 1100 0 1100])
hold on;    
line([x x+radius*cos(head)], [y y+radius*sin(head)],'LineWidth',1.5) % mark head


mapenv(); % draw the environment with obstacles

% get sensor value
[IR] = IRsensor_reading (head, x, y, objx, objy, oradius, objectNumber);

% sensored obstacle position
[X,Y] = Obstacle_location(head,x,y,IR);



% IR sensor value plot
subplot(2,6,9);
bar(IR);
axis([0 9 0 1024])
hold off;


%. . .
% robot motor control part,
% map from sensor values to motor commands (L,R)
% it depends on what kind of behaviours you wish to test

% Random Movement mode
obstacle_avoidance=false;
maxL=50;
maxR=50;
wL = maxL*rand;
wR = maxR*rand;

% Obstacle Avoidance
if(max(IR(1,3:4)) > 1024/8)                     % obstacle in front angle 
    obstacle_avoidance=true;
    if(sum(IR(1,1:3))+IR(1,7)>sum(IR(1,4:6))+IR(1,8))   % change head angle to right
       wL = -maxL/2;
       wR = maxR/2;
    else                                        % change head angle to left
        wL = maxL/2;
        wR = -maxR/2;
    end
elseif(max(IR(1,2),IR(1,5)) > 1024/4)           % obstacle in side angle
    obstacle_avoidance=true;
    if(IR(1,2)>IR(1,5))                         % change head angle to right
       wL = -maxL/2;
       wR = maxR/2;
    else                                        % change head angle to left
        wL = maxL/2;
        wR = -maxR/2;
    end
end
 
%%%
VL=wL*wradius;%wheel speed
VR=wR*wradius;%wheel speed
V = (VL + VR) / 2; %robot speed
%  %fprintf('%d %f %f \n', N,VL, VR);


%% kinematic simulation - next position (x,y) and next head angle
% which uses transient motor actions [ VL and VR ]
%. . .
if(VL==VR)
x=x+VL*dt*cos(head);
y=y+VL*dt*sin(head);
else
w=(VR-VL)/B;
R=(B*(VR+VL))/((VR-VL)*2);
x=R*sin(w*dt+head)+x-R*sin(head);
y=-R*cos(w*dt+head)+y+R*cos(head);
head=mod(head+w*dt,2*pi);
end

%% F score
 F=(VL+VR)/2*(1-abs(VL-VR)/(maxL*wradius+maxR*wradius))*(1-max(IR)/1023);
 savef(1,N)=F;
 
 subplot(2,3,2);
 plot(savef(1:N));
 
%% Trajectory
 savex(1,N)=x;
 savey(1,N)=y;

 subplot(1,3,3);
 if(obstacle_avoidance)     plot(x, y,'.','Color','r');     % obstacle avoiding mode
 else                       plot(x, y,'.','Color','b');     % random movement mode
 end
 hold on;
 axis([0 1100 0 1100])

%% path integration
pi_x = pi_x + V*cos(head)*dt;
pi_y = pi_y + V*sin(head)*dt;

% check if the robot collides with any obstacle or with the wall
% this can be checked by measuring the distance between the
% robot center position and a center position of an obstacle
% (1) distance(robot center, center of an object)
% < radius of robot + radius of an object.
for obj = 1:1:objectNumber
    objdistance=sqrt((x-objx(obj))^2 + (y-objy(obj))^2);
    if (objdistance <= radius + oradius(obj))
        fprintf('collide_obj\n');
        collide = true;
    end
end
% (2) distance(robot center, nearest wall) < radius of robot
if ((x>=30)&&(x<=1070)&&(y>=30)&&(y<=1070)) 
    if (min([abs(x-30),abs(x-1070),abs(y-30),abs(y-1070)]) < radius)
        fprintf('collide_wall\n');
        collide = true;
    end
else
    fprintf('collide_map\n');
    collide = true;
end

if collide
    break;
end

subplot(2,3,2); title('F Value');
subplot(2,6,9); title('IR Value');
subplot(2,6,10); title('Sensored distance in Robot Coord');
subplot(1,3,3); title('Robot Trajectory');


pause(0.0001); % this may be unnecessary
end

% %fprintf(sum(savef));