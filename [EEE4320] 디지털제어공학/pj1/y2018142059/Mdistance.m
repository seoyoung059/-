function [d, i, k]= Mdistance(a,b,rr,X,Y,onum,k)
    % (a,b)�� ��ֹ��� ��ġ
    % rr�� ��ֹ��� ũ��(������)
    % (X,Y)�� ������ ��ġ
    % onum�� ��ֹ��� ����
    % d�� ��ֹ����� �Ÿ�
    % i�� ��ֹ� ��ȣ.
    % k�� �浹 �� 1 (�� �Է�?)
      
    tmp = zeros(onum);
    for i = 1:1:onum    
        tmp(i) = ((X-a(i))^2+(Y-b(i))^2)^(0.5);
        if(rr(i)>tmp(i))
            k = 1;
        end
    end
    [d, i] = min(tmp);
end