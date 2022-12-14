% задаем массивы данных:
x = [0.1; 0.2; 0.3; 0.4; 0.5; 0.6; 0.7];
y = [23; 59; 84; 96; 120; 133; 154];

% строим матрицу V – матрицу Вандермонда с вырезанным первым столбцом:
V = vander(x);
V(:,1) = [];
 
% вычисляем элементы матрицы А
A = V'*V;
% вычисляем элементы вектора b 
b = V'*y;

% находим коэффициенты для полинома
a = A\b;

xi = min(x):0.1:max(x);
y2=0;
for k=0:5
    y2 = y2 + a(5-k+1) * xi.^k;
end

% cтроим график функции
hold on;
grid on;

plot(x,y,'*');
plot(xi,y2,'r');
