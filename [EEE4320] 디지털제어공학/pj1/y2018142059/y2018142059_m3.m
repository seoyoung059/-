%% robotic behaviours main program for m3

clc; clear; close all;
[objx, objy, oradius] = mapenv();

head = pi*2/3; x=200; y=200; % starting position and heading angle 
objectNumber = 10;
diameter = 55; radius = diameter /2; % robot diameter 
wdiameter = 5; wradius = wdiameter /2; % wheel diameter
B = 50; % wheel base
t = 0:0.1:2*pi+0.2; % to draw robot body
dt=0.1;

Numberofloop = 1500; %Numberofloop

pi_x = 0.0; pi_y = 0.0; % path integral initialize
mode=0;                 % control mode (0: random movement,1: obstacle avoidance, 2: path integral(go to start point))
collide = false;        % collide flag (if 1, the loop ends)
wL =0.0; wR = 0.0;      % angular velocity initialize
dhead = head;           % desired head initialize
N=0;                    % loop number

savef=zeros(1,Numberofloop);    % to save f
savex=zeros(1,Numberofloop);    % to save x
savey=zeros(1,Numberofloop);    % to save y

rmove = 5;
for N=1:1:Numberofloop
%     N=N+1;
% (x,y, head) is the current position and head angle
rx = x + radius * cos(t); ry = y + radius * sin(t); % robot drawing

subplot(1,3,1); hold off;
% draw robot body
if(mode==1)         p1 = plot(rx, ry,'Color','r'); % red when obstacle avoiding mode
elseif(mode==2)     p1 = plot(rx, ry,'Color','g'); % green when path integral (go to start point) mode
else                p1 = plot(rx, ry,'Color','b'); % blue when random movement mode
end
set(gcf,'units','pixels','pos',[500 500 1500 500])
axis([0 1100 0 1100])
hold on;
line([x x+radius*cos(head)], [y y+radius*sin(head)],'LineWidth',1.5) % mark head
if(N > Numberofloop/rmove)                              % line between start point and the robot in path integral mode
    line([x x+sqrt(pi_x^2+pi_y^2)*cos(dhead)], [y y+sqrt(pi_x^2+pi_y^2)*sin(dhead)],'LineStyle','--','Color','g','LineWidth',0.3)
end

mapenv(); % draw the environment with obstacles

if (N==Numberofloop/rmove)          % path integral mode
    fprintf('go to home\n');
end

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
%. . .
%. . .

%random movement mode
if (N<Numberofloop/rmove)
    mode=0;
    maxL=50;
    maxR=50;
    wL = maxL*rand;
    wR = maxR*rand;
    
% go to start mode
else
    distance = sqrt(pi_x^2+pi_y^2);
    % start point에 도달했을때
    if (pi_x<=0.7 && pi_y<=0.7)
        wL=0;
        wR=0;
        break;
    %현재 head와 desired head가 같을 때 (직진)
    elseif (abs(dhead - head)<0.003)
        wL = min(maxL/2,max(distance*0.9,4));
        wR = min(maxR/2,max(distance*0.9,4));
    %현재 head와 desired head가 다를 때 (회전)
    else
        if (dhead-head>pi)
            theta=dhead-head-2*pi;
        else
            theta=dhead-head;
        end
        if (theta>0)
            wL = -abs(theta)*B/wradius/2;
            wR = +abs(theta)*B/wradius/2;
        else
            wL = +abs(theta)*B/wradius/2;
            wR = -abs(theta)*B/wradius/2;
        end
    end
    if (mode==1)
        wL=maxL/2;
        wR=maxR/2;
    end
    mode=2;
end

% obstacle avoidance
if(max(IR(1,3:4)) > 1024/8)             % obstacle in front angle
    mode=1;
    if(sum(IR(1,1:3))+IR(1,7)>sum(IR(1,4:6))+IR(1,8))   % change head angle to right
       wL = -maxL/2;
       wR = maxR/2;
    else                                                % change head angle to left
        wL = maxL/2;
        wR = -maxR/2;
    end
elseif(max(IR(1,2),IR(1,5)) > 1024/3)   % obstacle in side angle
    mode=1;
    if(IR(1,2)>IR(1,5))     % change head angle to right
       wL = -maxL/2;
       wR = maxR/2;
    else                    % change head angle to left
        wL = maxL/2;
        wR = -maxR/2;
    end
% elseif( min(IR(1:6)) ~= 0 && N >= Numberofloop) % to avoid collision between obstacle avoiding mode and path integration mode
%     mode=1;
%     % go straight
%     wL = maxL*rand;
%     wR = maxR*rand;
% %     wL = maxL/2;
% %     wR = maxR/2;
end

if mode==1
    %fprintf('avoid\n');
end
 %%%%
 VL=wL*wradius;%wheel speed
 VR=wR*wradius;%wheel speed
 V = (VL + VR) / 2; %robot speed


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
 F=(VL+VR)/2*(1-abs(VL-VR)/(20*wradius+20*wradius))*(1-max(IR)/1023);
 savef(1,N)=F;
 
 subplot(2,3,2);
 plot(savef(1:N));
 
 %% trajectory
 savex(1,N)=x;
 savey(1,N)=y;
 
 % plot trajectory
 subplot(1,3,3);
 if(mode==1)        plot(x, y,'.','Color','r');     % obstacle avoiding mode
 elseif(mode==2)    plot(x, y,'.','Color','g');     % path integration mode
 else               plot(x, y,'.','Color','b');     % random movement mode
 end
 hold on;
 axis([0 1100 0 1100])
 
%% path integration
 pi_x = pi_x + V*cos(head)*dt;
 pi_y = pi_y + V*sin(head)*dt;
 
 % 현재 위치에서 시작점을 바라보는 desired head 계산
 if (pi_y == 0)
     dhead = mod(pi_x/abs(pi_x) * -1,2*pi);
 elseif (pi_x > 0)
     dhead = mod(atan(pi_y/pi_x) + pi,2*pi);
 elseif (pi_x < 0)
     dhead = mod(atan(pi_y/pi_x),2*pi);
 else
     dhead = mod(pi_y/abs(pi_y) * -0.5,2*pi);
 end
 
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
    fprintf('collide_outofmap\n');
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

% fprintf(sum(savef(1:Numberofloop/rmove));