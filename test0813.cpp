#include<iostream>

int main(void)
{
	int numb = 1;
	int& numb2 = numb;
	swep(numb2);
	std::cout << numb << std::endl;
}
//Test online change
void swep(int& n)
{
	n = 2;
}