function [X, Y] = Obstacle_location(head,x,y,IR)
%로봇 관련 좌표 계산
%head는 로봇 방향
%(x,y)는 로봇 위치
%IR은 IRsensor에서 받은 값

r = 55/2;
%% IRsensor value to distance (digital to analog)
D = zeros(1,length(IR));        % 센서와 감지 장애물 사이 거리
for i=1:1:length(IR)
    tmp = IR(i);
    if (tmp == 1023)
        D(i) = 15;
    elseif (tmp > 756)
        D(i) = 5 * (1023 - tmp) / (1023 - 756) + 20;
    elseif (tmp > 400)
        D(i) = 5 * (756 - tmp) / (756 - 400) + 25;
    elseif (tmp > 260)
        D(i) = 5 * (400 - tmp) / (400 - 260) + 30;
    elseif (tmp > 145)
        D(i) = 5 * (260 - tmp) / (260 - 145) + 35;
    elseif (tmp > 92)
        D(i) = 5 * (145 - tmp) / (145 - 92) + 40;
    elseif (tmp > 74)
        D(i) = 5 * (92 - tmp) / (92 - 74) + 45;
    elseif (tmp > 60)
        D(i) = 5 * (74 - tmp) / (74 - 60) + 50;
    else
        D(i) = 55;
    end
end

%% obstacle location in robot coordination
theta_a = [22 45 78 180-78 180-45 180-22 180+68 360-68] / 180 * pi;      % 센서 각도 알파
theta_b = [-90 -45 0 0 45 90 180 180] / 180 * pi;                               % 센서 각도 베타
S = zeros(4, length(IR));
distance = zeros(1, length(IR));
for i=1:1:length(D)
    %(D,0,0)의 각 beta를 a에 맞추고, -r만큼 이동 후 다시 pi/2-a만큼 회전 
    d = [D(i); 0; 0; 1];
    trans_r = transfer(-r,0,0);
    rot_r = rotation(pi/2-theta_a(i));
    rot_s = rotation(theta_a(i)-theta_b(i)-pi/2);
    S(:,i) = rot_r*trans_r*rot_s*d;
    distance(i) = sqrt(S(1,i)^2+S(2,i)^2);
end
    
%% obstacle location in world coordination
X = zeros(1,length(IR));        % world coord 상의 감지 물체 x좌표
Y = zeros(1,length(IR));        % world coord 상의 감지 물체 y좌표

for j=1:1:length(D)
    % robot coord에서 -head만큼 회전, (-x,-y) 이동
    trans = transfer(-x,-y,0);
    rot = rotation(-head);

    pos = trans *rot * S(:,j);
    X(j)= pos(1);
    Y(j)= pos(2);
end

%sensor plot on world coord
subplot(1,3,1);
scatter(X,Y);

%sensor plot in robot coord
subplot(2,6,10);
scatter (S(1,:),S(2,:));
hold on;
for k = 1:1:length(S(1,:))
    line([0 S(1,k)],[0 S(2,k)])
end
axis([-100 100 -100 100]);
hold off;
subplot(1,3,1);
end



function rot = rotation(theta)
    rot = [cos(theta) sin(theta) 0 0; -sin(theta) cos(theta) 0 0; 0 0 1 0; 0 0 0 1;];
end

function trans = transfer(x,y,z)
    trans = [1 0 0 -x; 0 1 0 -y; 0 0 1 -z; 0 0 0 1;];
end