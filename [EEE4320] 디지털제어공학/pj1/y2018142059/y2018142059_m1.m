%% main program for robotic behaviours

clc; clear; close all;
%

head = 0; x=500; y=500; % starting position and heading angle 
objectNumber = 10;
diameter = 55; radius = diameter /2; % robot diameter 
wdiameter = 5; wradius = wdiameter /2; % wheel diameter
B = 50; % wheel base
t = 0:0.1:2*pi+0.2; % to draw robot body
dt=0.1;

Numberofloop = 3000;
pi_x = 0.0; pi_y = 0.0;
dhead=head;

savex=zeros(1,Numberofloop);
savey=zeros(1,Numberofloop);


for N=1:1:Numberofloop
hold off;

% (x,y, head) is the current position and head angle
rx = x + radius * cos(t); ry = y + radius * sin(t); % robot drawing
subplot(1,2,1);
p1 = plot(rx, ry); % draw robot body
axis([0 1100 0 1100])
hold on;
line([x x+radius*cos(head)], [y y+radius*sin(head)]) % mark head
line([x x+sqrt(pi_x^2+pi_y^2)*cos(dhead)], [y y+sqrt(pi_x^2+pi_y^2)*sin(dhead)],'Color','r')
hold off;

%. . .
% robot motor control part,
% map from sensor values to motor commands (L,R)
% it depends on what kind of behaviours you wish to test

maxL=30;
maxR=30;

%torque to robot system
 if (rem(N,50)==1 && N<Numberofloop/3)
     wL = rand * maxL;
     wR = rand * maxR;
 elseif (N >= Numberofloop/3)     
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
 end
 
 %%%%
 VL=wL*wradius;%wheel speed
 VR=wR*wradius;%wheel speed
 V = (VL + VR) / 2; %robot speed

% kinematic simulation - next position (x,y) and next head angle
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

 pi_x = pi_x + V*cos(head)*dt;
 pi_y = pi_y + V*sin(head)*dt;

 %현재 위치에서 시작점을 바라보는 desired head 계산
 if (pi_y == 0)
     dhead = mod(pi_x/abs(pi_x) * -1,2*pi);
 elseif (pi_x > 0)
     dhead = mod(atan(pi_y/pi_x) + pi,2*pi);
 elseif (pi_x < 0)
     dhead = mod(atan(pi_y/pi_x),2*pi);
 else
     dhead = mod(pi_y/abs(pi_y) * -0.5,2*pi);
 end


 savex(1,N)=x;
 savey(1,N)=y;
 subplot(1,2,2);
 plot(savex(1,1:Numberofloop/3),savey(1,1:Numberofloop/3),'.'); hold on;
 plot(savex(1,Numberofloop/3:Numberofloop),savey(1,Numberofloop/3:Numberofloop),'.','Color','r');
 axis([0 1100 0 1100])

pause(0.0001); % this may be unnecessary
end