//Make sure an input is between two numbers
int clamp(int low, int high, int num)
{
	if (num < low) return low;
	if (num > high) return high;
	return num;
}
//Check if an integer is between two numbers
bool between(int num1, int num2, int orig)
{
	if (orig >= num1 && orig <= num2)
		return 1;
	else if (orig <= num1 && orig >= num2)
		return 1;
	else return 0;
}
