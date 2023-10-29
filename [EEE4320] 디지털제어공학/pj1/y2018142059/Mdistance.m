function [d, i, k]= Mdistance(a,b,rr,X,Y,onum,k)
    % (a,b)는 장애물의 위치
    % rr은 장애물의 크기(반지름)
    % (X,Y)는 센서의 위치
    % onum은 장애물의 개수
    % d는 장애물과의 거리
    % i는 장애물 번호.
    % k는 충돌 시 1 (왜 입력?)
      
    tmp = zeros(onum);
    for i = 1:1:onum    
        tmp(i) = ((X-a(i))^2+(Y-b(i))^2)^(0.5);
        if(rr(i)>tmp(i))
            k = 1;
        end
    end
    [d, i] = min(tmp);
end