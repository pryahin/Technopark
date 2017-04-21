/*
 Задано N точек на плоскости. Указать (N-1)-звенную 
 несамопересекающуюся незамкнутую ломаную, 
 проходящую через все эти точки.
 */

#include <iostream>
#include <functional>

struct point
{
	int x,y;
};

bool Comp(point array, point temp)
{
    if ((array.x > temp.x) || (array.x==temp.x && array.y > temp.y))
        return true;
    return false;
}

void mySort(point* array, size_t length, std::function<bool(point,point)> Compare){
	point temp;
	int lastIndex=0;

	for (size_t i=1;i<length;i++)
	{
		temp = array[i];
		lastIndex = i-1;
		while (lastIndex >= 0 && Compare(array[lastIndex],temp))
		{
			array[lastIndex+1] = array[lastIndex];
			array[lastIndex--] = temp;
		}
	}
}

int main()
{
	size_t n = 0;
	std::cin >> n;

	point* array = new point[n];

	for (size_t i=0;i<n;i++)
		std::cin >> array[i].x >> array[i].y; 

	mySort(array, n, Comp);

	for (size_t i=0;i<n;i++)
		std::cout << array[i].x << " " << array[i].y << std::endl;


	return 0;
}
