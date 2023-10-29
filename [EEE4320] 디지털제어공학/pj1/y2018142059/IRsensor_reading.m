function [D] = IRsensor_reading(head,x,y,a,b,rr,onum)

%head�� �κ��� ����
%(x,y)�� �κ��� ��ġ
%(a,b)�� ��ֹ��� ��ġ
% rr �� ��ֹ� ������
% onum �� ��ֹ� ��
% D�� ���� ��.(8���� �迭�� ouput�� ����),���� ������� �ݽð� ����
% M�� ���ڱ� ��(�׷���)


XY=zeros(2,length(x));
XY(1,:)=x;
XY(2,:)=y;
r=55/2;  % robot radius

head0 = head + (0-90)*pi/180;          % �� ������ ����...
head1 = head + (45-90)*pi/180;
head2 = head + (90-90)*pi/180;
head3 = head + (90-90)*pi/180;
head4 = head + (135-90)*pi/180;
head5 = head + (180-90)*pi/180;
head6 = head + (270-90)*pi/180;
head7 = head + (270-90)*pi/180;

position0 = head + (22-90)*pi/180;      %% �� ������ ��ġ ����....
position1 = head + (45-90)*pi/180;
position2 = head + (78-90)*pi/180;
position3 = head + (102-90)*pi/180;
position4 = head + (135-90)*pi/180;
position5 = head + (158-90)*pi/180;
position6 = head + (248-90)*pi/180;
position7 = head + (292-90)*pi/180; 

T0 = [XY(1)+r*cos(position0) XY(2)+r*sin(position0)]; % �� ������ ó�� ��ġ....
T1 = [XY(1)+r*cos(position1) XY(2)+r*sin(position1)]; %r �� ���� �ణ �����ؼ� �κ� ���� ������ ��ġ�� ��������
T2 = [XY(1)+r*cos(position2) XY(2)+r*sin(position2)]; % �������� �ǵ��� ������ ��ġ�� r���� �ణ ������
T3 = [XY(1)+r*cos(position3) XY(2)+r*sin(position3)]; % ���� ���̰� �����Ƿ� �׳� r�� �����.
T4 = [XY(1)+r*cos(position4) XY(2)+r*sin(position4)];
T5 = [XY(1)+r*cos(position5) XY(2)+r*sin(position5)];
T6 = [XY(1)+r*cos(position6) XY(2)+r*sin(position6)];
T7 = [XY(1)+r*cos(position7) XY(2)+r*sin(position7)];

%put three distance lines for a one sensor
dangle=pi/12;
D(1) = mean([Psensor(T0,head0,a,b,rr,onum),Psensor(T0,head0 - dangle,a,b,rr,onum),Psensor(T0,head0 + dangle ,a,b,rr,onum)]);  
D(2) = mean([Psensor(T1,head1,a,b,rr,onum),Psensor(T1,head1 - dangle,a,b,rr,onum),Psensor(T1,head0 + dangle ,a,b,rr,onum)]);    %Psensor �Լ��� ����� ���.
D(3) = mean([Psensor(T2,head2,a,b,rr,onum),Psensor(T2,head2 - dangle,a,b,rr,onum),Psensor(T2,head0 + dangle ,a,b,rr,onum)]);  
D(4) = mean([Psensor(T3,head3,a,b,rr,onum),Psensor(T3,head3 - dangle,a,b,rr,onum),Psensor(T3,head0 + dangle ,a,b,rr,onum)]);  
D(5) = mean([Psensor(T4,head4,a,b,rr,onum),Psensor(T4,head4 - dangle,a,b,rr,onum),Psensor(T4,head0 + dangle ,a,b,rr,onum)]);  
D(6) = mean([Psensor(T5,head5,a,b,rr,onum),Psensor(T5,head5 - dangle,a,b,rr,onum),Psensor(T5,head0 + dangle ,a,b,rr,onum)]);  
D(7) = mean([Psensor(T6,head6,a,b,rr,onum),Psensor(T6,head6 - dangle,a,b,rr,onum),Psensor(T6,head0 + dangle ,a,b,rr,onum)]);  
D(8) = mean([Psensor(T7,head7,a,b,rr,onum),Psensor(T7,head7 - dangle,a,b,rr,onum),Psensor(T7,head0 + dangle ,a,b,rr,onum)]);  

hold on;
    line([XY(1) XY(1)+r*cos(head)], [XY(2) XY(2)+r*sin(head)])
    line([T0(1) T0(1)+30*cos(head0)], [T0(2) T0(2)+30*sin(head0)])
    line([T1(1) T1(1)+30*cos(head1)], [T1(2) T1(2)+30*sin(head1)])
    line([T2(1) T2(1)+30*cos(head2)], [T2(2) T2(2)+30*sin(head2)])
    line([T3(1) T3(1)+30*cos(head3)], [T3(2) T3(2)+30*sin(head3)])
    line([T4(1) T4(1)+30*cos(head4)], [T4(2) T4(2)+30*sin(head4)])
    line([T5(1) T5(1)+30*cos(head5)], [T5(2) T5(2)+30*sin(head5)])
    line([T6(1) T6(1)+30*cos(head6)], [T6(2) T6(2)+30*sin(head6)])
    line([T7(1) T7(1)+30*cos(head7)], [T7(2) T7(2)+30*sin(head7)]) 
    
%     
%     line([T0(1) T0(1)+30*cos(head0-dangle)], [T0(2) T0(2)+30*sin(head0-dangle)])
%     line([T1(1) T1(1)+30*cos(head1-dangle)], [T1(2) T1(2)+30*sin(head1-dangle)])
%     line([T2(1) T2(1)+30*cos(head2-dangle)], [T2(2) T2(2)+30*sin(head2-dangle)])
%     line([T3(1) T3(1)+30*cos(head3-dangle)], [T3(2) T3(2)+30*sin(head3-dangle)])
%     line([T4(1) T4(1)+30*cos(head4-dangle)], [T4(2) T4(2)+30*sin(head4-dangle)])
%     line([T5(1) T5(1)+30*cos(head5-dangle)], [T5(2) T5(2)+30*sin(head5-dangle)])
%     line([T6(1) T6(1)+30*cos(head6-dangle)], [T6(2) T6(2)+30*sin(head6-dangle)])
%     line([T7(1) T7(1)+30*cos(head7-dangle)], [T7(2) T7(2)+30*sin(head7-dangle)]) 
%     
%     
%     line([T0(1) T0(1)+30*cos(head0+dangle)], [T0(2) T0(2)+30*sin(head0+dangle)])
%     line([T1(1) T1(1)+30*cos(head1+dangle)], [T1(2) T1(2)+30*sin(head1+dangle)])
%     line([T2(1) T2(1)+30*cos(head2+dangle)], [T2(2) T2(2)+30*sin(head2+dangle)])
%     line([T3(1) T3(1)+30*cos(head3+dangle)], [T3(2) T3(2)+30*sin(head3+dangle)])
%     line([T4(1) T4(1)+30*cos(head4+dangle)], [T4(2) T4(2)+30*sin(head4+dangle)])
%     line([T5(1) T5(1)+30*cos(head5+dangle)], [T5(2) T5(2)+30*sin(head5+dangle)])
%     line([T6(1) T6(1)+30*cos(head6+dangle)], [T6(2) T6(2)+30*sin(head6+dangle)])
%     line([T7(1) T7(1)+30*cos(head7+dangle)], [T7(2) T7(2)+30*sin(head7+dangle)]) 

end
