%
% P/PI/PID controller simulation
%
close

dt=0.1; I=1; c=1; % wheel motor parameters
% kp=9; ki=0; kd=0;   %P parameters
% kp=1; ki=1; kd=0;   %PI parameters
kp=1; ki=1; kd=0.1; % PID parameters

wr(1)=0; wr(2)=0; wl(1)=0; wl(2)=0; % initial setting for wheel speed
tr(1)=0; tr(2)=0; tl(1)=0; tl(2)=0; % initial setting for torque

r=zeros(1000,1);
r(10:600)=10;
r(600:1000)=5;

l=zeros(1000,1);
l(10:200)=5;
l(200:400)=7;
l(400:800)=10;
l(800:1000)=8;

er(1)=0; er(2)=0;
el(1)=0; el(2)=0;

for n=3:1000, % we start with n=3 (why?, because of e(n-2))
er(n) = r(n)-wr(n-1);   el(n) = l(n)-wl(n-1);
%
tr(n)=tr(n-1)+kp*(er(n)-er(n-1))+ki*er(n)*dt+kd*(er(n)-2*er(n-1)+er(n-2))/ dt;
tl(n)=tl(n-1)+kp*(el(n)-el(n-1))+ki*el(n)*dt+kd*(el(n)-2*el(n-1)+el(n-2))/ dt;
%
wr(n)=wr(n-1) + dt/I*(-c*wr(n-1) + tr(n));
wl(n)=wl(n-1) + dt/I*(-c*wl(n-1) + tl(n));
% transient angular speed
WR=wr(n); WL=wl
end;

subplot(2,1,1);
plot((1:1000)*dt - dt, l); hold on;
plot((1:1000)*dt - dt, wl);
legend();
title('Left Wheel Angular Velocity');
axis([0 100 0 12])

subplot(2,1,2);
plot((1:1000)*dt - dt, r); hold on;
plot((1:1000)*dt - dt, wr);
legend();
title('Right Wheel Angular Velocity');
axis([0 100 0 12])