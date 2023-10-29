% 혼-셩크 옵티컬 플로우 알고리즘 이용
opticFlow = opticalFlowHS;

count=0;

nFrames = 0;
vid = videoinput('winvideo', 1, 'MJPG_640x360');
figure;

%Load video = faster
preview(vid);
disp 'preview';

%Initialize rolling averages
xcompavg = 0;
ycompavg = 0;
xcompmax = 0;
ycompmax = 0;

%Bluetooth Initialize
%
s = serialport("COM9",57600);
send_serialbt(s,1)
%}

%Time for human ready
pause(2)

while 1
        % 이미지 프레임 획득
        frameRGB = getsnapshot(vid);
        %vidDevice();
        
        % 프레임으로부터 옵티컬 플로우 계산
        flow = estimateFlow(opticFlow, rgb2gray(frameRGB));
        
        % 연산량으로 인해 10프레임마다 화면 출력
        if rem(count, 3) == 0
            imshow(frameRGB);
            hold on
            plot(flow, 'DecimationFactor',[5,5],'ScaleFactor',275)
            %%
            %% 화면 텍스트 표시 관련 코드 내용
            xcomp = sum(flow.Vx,'all');
            ycomp = sum(flow.Vy,'all');
            
            %%Weighted Rolling Average
            xcompavg = xcomp*0.2 + xcompavg*0.8;
            ycompavg = ycomp*0.2 + ycompavg*0.8;
            
            %%Max Weighted Rolling Average w/ 5% decay and min 50 value
            xcompmax = max(abs(xcompavg), xcompmax)*0.2 + xcompmax*0.75;
            ycompmax = max(abs(ycompavg), ycompmax)*0.2 + ycompmax*0.75;
            xcompmax = max(xcompavg,50);
            ycompmax = max(ycompavg,50);
            
            if (abs(xcompavg)+abs(ycompavg)< (xcompmax+ycompmax)*0.5)
                %%fprintf('stop %.3f %.3f\n',xcompavg,ycompavg);
            else
            if (abs(xcompavg)>abs(ycompavg)*2)
                if xcompavg > xcompmax*0.8
                    fprintf('Left %.3f\n',xcompavg);
                    %
                    xcompavg = 0;
                    ycompavg = 0;
                    send_serialbt(s,5)
                    send_serialbt(s,5)
                    pause(2)
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    pause(0.5)
                    %}
                elseif xcompavg < -(xcompmax*0.8)
                    fprintf('Right %.3f\n',xcompavg);
                    %
                    xcompavg = 0;
                    ycompavg = 0;
                    send_serialbt(s,4)
                    send_serialbt(s,4)
                    send_serialbt(s,4)
                    pause(2)
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    pause(0.5)
                    %}
                else
                    %%fprintf('stop %.3f %.3f\n',xcompavg,ycompavg);
                end
            else
                if ycompavg > ycompmax*0.8
                    fprintf('Down %.3f\n',ycompavg);
                    %
                    xcompavg = 0;
                    ycompavg = 0;
                    send_serialbt(s,3)
                    send_serialbt(s,3)
                    send_serialbt(s,3)
                    pause(4)
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    pause(0.5)
                    %}
                elseif ycompavg < -(ycompmax*0.8)
                    fprintf('Up %.3f\n',ycompavg);
                    %
                    xcompavg = 0;
                    ycompavg = 0;
                    send_serialbt(s,2)
                    send_serialbt(s,2)
                    send_serialbt(s,2)
                    pause(4)
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    send_serialbt(s,1);
                    pause(0.5)
                    %}
                else
                    %%fprintf('stop %.3f %.3f\n',xcompavg,ycompavg);
                end
            end
            end
            %% 
        end
        
        %%
        %% 옵티컬 플로우 활용 제스쳐 방향 인식 코드 내용
        %%
        
        %Robot Control
        
        % 카운트 관련 변수
        count = count + 1;
        nFrames = nFrames + 1;
        
        hold off
end