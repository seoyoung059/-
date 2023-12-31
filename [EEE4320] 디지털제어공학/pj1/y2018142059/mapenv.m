function [xx, yy, rr] = mapenv()

%장애물 모양
t=0:0.1:2*pi+0.1;

% 장애물 크기와 위치
rr=[ 50  75  48  65  30  58  45  81  47  83];
xx=[ 90 130 230 780 600 890 450 570 420 960];
yy=[600 730 360 610 820 330 550 250 890 970];

% 장애물 생성
for i = 1:10
    x=rr(i)*cos(t) + xx(i); y=rr(i)*sin(t) + yy(i);
    fill(x,y,'g');
    hold on
end

%아래벽
hold on
x = [1 1 1100 1100 1];
y = [1 30 30 1 1];
fill(x, y, 'b*')
%오른쪽벽
hold on
x = [1070 1070 1100 1100 1070];
y = [30 1100 1100 30 30];
fill(x, y, 'b*')
%윗벽
hold on
x = [0 0 1070 1070 0];
y = [1070 1100 1100 1070 1070];
fill(x, y, 'b*')
hold on
%왼쪽벽
x = [0 0 30 30 0];
y = [30 1070 1070 30 30];
fill(x, y, 'b*')

axis([0 1100 0 1100])
grid on