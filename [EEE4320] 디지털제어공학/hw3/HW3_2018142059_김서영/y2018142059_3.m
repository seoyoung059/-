%
% Robot Simulation
%

close all;

head =90*pi/180 ; x=300; y=100; % starting position and heading angle
diameter = 55; radius = diameter /2; % robot diameter
wdiameter=5.5; wradius = wdiameter /2;%wheel diameter
B = 50; % distance between two wheels
t = 0:0.1:2*pi+0.2; % to draw robot body
dt=0.1;
I=1; c=1; % wheel motor parameters
% kp=9; ki=0; kd=0;     % P parameteres
% kp=1; ki=1; kd=0;     % PI parameters
% kp=1; ki=1; kd=0.1;   % PID parameters
kp=5; ki=1; kd=0.1;   % PID parameters overdamped
% kp=1; ki=10; kd=0.1;  % PID parameters underdamped

eL=0; eL1=0; eL2=0;
eR=0; eR1=0; eR2=0;


scrsz = get(0,'ScreenSize');
figure('Position',[100 100 scrsz(3)*0.8 scrsz(4)*0.8])
time=0;
Numberofloop=1000;
wL=0;% anguler velocity of left wheel
wR=0;% anguler velocity of right wheel
tL=0;
tR=0;
w = 0;
savex=zeros(1,Numberofloop);
savey=zeros(1,Numberofloop);
saveR=zeros(1,Numberofloop);
savehead = zeros(1,Numberofloop);
savewL=zeros(1,Numberofloop);
savewR=zeros(1,Numberofloop);


for N=1:1:Numberofloop

 %Robot Drawing
 rx = x + radius * cos(t); ry = y + radius * sin(t);
 subplot(2,2,1),plot(rx, ry)% draw robot body
 axis([-600 600 -600 600])
%  title('Robot Location');
 daspect([1 1 1])
 line([x x+radius*cos(head)], [y y+radius*sin(head)])
 
 %Robot Trajectory
 subplot(2,2,2),plot(x, y,'.');
 axis([-600 600 -600 600]),hold on;
%  title('Robot Trajectory');
 daspect([1 1 1])
 
 %Robot Angular Velocity
 subplot(2,2,3),plot(time,w ,'.');
 axis([0 Numberofloop*dt -pi/2 pi/2]),hold on;
%  title('Robot Angular Velocity');
%  grid on;
%  grid minor;
 
 %Wheel Angular velocity
 subplot(2,2,4),plot(time, wL,'x'),hold on
 plot(time, wR,'o'),
%  grid on;
%  grid minor;
%  title('Wheel Angular Velocity');
%  legend('Left Wheel', 'Right Wheel');
 axis([0 Numberofloop*dt 0 15]),hold on;
 
 %%%%
 %you may determine desired angular velocity here.
 if (N==1)
 dwL = 0; dwR = 0;
 elseif (N==10)
 dwL = 5; dwR = 10;
 elseif (N==200)
 dwL = 7; dwR = 10;
 elseif (N==400)
 dwL = 10; dwR = 10;
 elseif (N==600)
 dwL = 10; dwR = 5;
 elseif (N==800)
 dwL = 8; dwR = 5;    
 end
 %%%%
 %%%%
 
 %you may determine torque here.
 %left 
 eL2=eL1;
 eL1=eL;
 eL=dwL-wL;
 %right
 eR2=eR1;
 eR1=eR;
 eR=dwR-wR;
 
 %PID
 tL=tL+kp*(eL-eL1)+ki*eL*dt+kd*(eL-2*eL1+eL2)/dt;
 tR=tR+kp*(eR-eR1)+ki*eR*dt+kd*(eR-2*eR1+eR2)/dt;
 %%%%
 %%%%
 %you may apply torque to robot system here
 wL = wL+dt/I*(-c*wL+tL);
 wR = wR+dt/I*(-c*wR+tR);
 %%%%
 VL=wL*wradius;%wheel speed
 VR=wR*wradius;%wheel speed

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

 pause(0.01); 
 savex(1,N)=x;
 savey(1,N)=y;
 savehead(1,N)=head;
 savewL(1,N)=wL;
 savewR(1,N)=wR;
 time=time+dt;
end