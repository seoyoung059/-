%% Digital Control Engineering & Robotics HW#2
% 2018142059 김서영

%% constants
L_r=55;         %[mm] robot diameter
L_b=50;         %[mm] length of wheel base
wr=1;           %[mm] radious of robot wheel

%% simulation time
total_time=100;  %[s]Total simulation time
dt = 0.1;       %[s] time step

%% motor commands
W_l = rand*10;  %left angular speed
W_r = rand*10;  %right angular speed

%% calculation
V_l = wr*W_l;       %left speed
V_r = wr*W_r;       %right speed
V = (V_l+V_r)/2;    %average speed

R = (L_b*(V_l+V_r))/(2*(V_r-V_l));  %moving circle radius
w = (V_r - V_l)/L_b;                %moving angular speed

%% robot simulation
%simulation initiate
x = rand * 10; y = rand * 10 + 50;  %initial position
head = 0;                           %initial head angle
hold off;

%for plot
trace_x = zeros(1, total_time/dt);  %x axis trace
trace_y = zeros(1, total_time/dt);  %y axis trace
aspeed_l = zeros(1, total_time/dt); %left angular speed
aspeed_r = zeros(1, total_time/dt); %right angular speed
n=0;                                %time step

%simulation for loop
for i = 0:dt:total_time             %simulation
    n = n+1;                        %nth time step
    
    %commands
    command_period = 100;                %command period
    if (mod(i,command_period*dt)==0)   %random exploration motor command
        W_l = rand*10;
        W_r = rand*10;   
    end
    
    %speed
    V_l = wr*W_l;                   %left speed
    V_r = wr*W_r;                   %right speed
    V = (V_l+V_r)/2;                %average speed
    
    %movement
    if(V_l < V_r)                   %turning left
        R = (L_b*(V_l+V_r))/(2*(V_r-V_l));
        w = (V_r - V_l)/L_b;
    
        x = x + R * sin(head+w*dt) - R * sin(head);
        y = y - R * cos(head+w*dt) + R * cos(head);
        head = head + w*dt;
        
    elseif(V_l>V_r)                 %turning right
        R = (L_b*(V_l+V_r))/(2*(V_l-V_r));
        w = (V_r - V_l)/L_b;
    
        x = x - R * sin(head+w*dt) + R * sin(head);
        y = y + R * cos(head+w*dt) - R * cos(head);
        head = head + w*dt;
        
    else                            %go straight
        x = x + cos(head)*V*dt;
        y = y + sin(head)*V*dt;
        head=head;
    end
    
    %value for plot
    trace_x(n) = x;
    trace_y(n) = y;
    aspeed_l(n) = W_l;
    aspeed_r(n) = W_r;
    
    
    %plot
    %robot drawing
    subplot(3,1,1);
    t = 0:0.1:2*pi+0.2;
    r = 55/2;                                                              % position change
    rx = x + L_r/2 * cos(t); ry = y + L_r/2 * sin(t); 
    p1 = plot(rx,ry,'DisplayName','Robot');                                % draw robot body
    hold on;
    p2 = line([x x+L_r/2*cos(head)], [y y+L_r/2*sin(head)]);               % draw robot head
    axis([-50 600 -400 400]);                                             % axis
    p3 = plot(trace_x(1:n), trace_y(1:n),'DisplayName','Trace');           % draw robot trace
    title('Trajectory of the Robot')
    legend([p1,p3]);
    hold off;
    
    %angular speed plot
    subplot(3,1,2);
    s1 = plot(aspeed_l(1:n),'DisplayName','\omega_{left}');                %left angular speed
    title('Angular Speeds of the Left Motor');
    ylabel('Speed (rad/0.1s)'); xlabel('Time (0.1s)');
    legend();
    axis([0 total_time/dt 0 10.5]);
    
    subplot(3,1,3);
    s2 = plot(aspeed_r(1:n),'DisplayName','\omega_{right}');               %right angular speed
    title('Angular Speeds of the Right Motor');
    ylabel('Speed (rad/0.1s)'); xlabel('Time (0.1s)');
    legend();
    axis([0 total_time/dt 0 10.5]);
    
    pause(0.1);    
end