/*README:Physics_6 ops
* Legal input includes nums (0~9), decimal point (.), ops (+,-,*,/,^) and parenthesis.
* Any illegal input may lead to error,including "Divided by zero", "Incorrect use of decimal point" and "Mismatched parentheses".
* As for Exponentiation and square root operations:
* (1) Input in the form of "a^b". For example, input sqrt(2) in the form of "2^(1/2)" or "2^0.5".
* (2) The program isn't able to calculate the equation if a is a negative number and b is a decimal.
* Press the Enter key to get the result after inputting the whole equation (without '=').
* You can change the result's precision (<=15) in the main function.
*/
#include <iostream>
#include <cmath>
using namespace std;
void handle_error(const char*);
struct Cal //A "Cal" contains one calculation with two nums and an op.
{
	double a, b;
	char op;
	double com(); //computing
};
string s; //the whole equation
int idx;
int pri[200]; //the priority of ops
double readnum(); //change string into num
double func(int, double); //Core function
int main()
{
	pri['+'] = pri['-'] = 1;
	pri['*'] = pri['/'] = 2;
	pri['^'] = 3;
	cin >> s;

	for (int i = 0; i < s.size(); ++i)
		if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/' && s[i] != '^' && s[i] != '(' && s[i] != ')' && s[i] != '.')
			handle_error("Syntax Error!"); //check for illegal characters
	for (int i = 0; i < s.size() - 1; ++i)
		if (pri[s[i]] && !isdigit(s[i + 1]) && s[i + 1] != '(')
			handle_error("Syntax Error!");
	for (int i = 1; i < s.size(); ++i)
		if (pri[s[i]] && !isdigit(s[i - 1]) && s[i - 1] != ')' && !(s[i] == '-' && s[i - 1] == '('))
			handle_error("Syntax Error!");
	for (int i = 1; i < s.size() - 1; ++i)
		if ((s[i] == '(' || s[i] == ')') && isdigit(s[i - 1]) && isdigit(s[i + 1]))
			handle_error("Syntax Error!"); //check for misused parentheses
	long long l = 0, r = 0;
	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == '(') ++l;
		if (s[i] == ')') ++r;
	}
	if (l != r) handle_error("Syntax Error!"); //check for mismatched parentheses

	double ans = func(0, 0);
	//	cout.precision(10);
	cout << ans;
	return 0;
}
void handle_error(const char* message)
{
	cout << message;
	exit(EXIT_FAILURE);
}
double Cal::com() //computing
{
	switch (op)
	{
		case '+':return a + b;
		case '-':return a - b;
		case '*':return a * b;
		case '/':
		{
			if (b == 0) handle_error("Maths Error!");
			return a / b;
		}
		case '^':
		{
			if (a < 0 && (long long)(b) != b) handle_error("Maths Error!");
			if (a == 0 && b <= 0) handle_error("Maths Error!");
			return pow(a, b);
		}
		default:handle_error("Syntax Error!");
	}
}
double readnum() //change string into num
{
	double t = 0;
	for (; idx < s.size() && s[idx] >= '0' && s[idx] <= '9'; ++idx)
		t = t * 10 + s[idx] - '0';
	if (s[idx] == '.')
	{
		idx++;
		for (int i = 1; idx < s.size() && s[idx] >= '0' && s[idx] <= '9'; ++idx, ++i)
			t += pow(10, -i) * (s[idx] - '0');
	}
	if (s[idx] == '.') handle_error("Syntax Error!");
	return t;
}
double func(int dep, double b) //Core function,b is the last-dep result
{
	Cal tmp{};
	if (isdigit(s[idx])) tmp.a = readnum();
	else tmp.a = b; //The first num comes from readnum() or b.
	while (idx < s.size())
	{
		if (s[idx] == ')') return tmp.a; //This dep ends when ')' appears.
		else
			if (s[idx] == '(') tmp.op = '+'; //The default op is '+'.
			else tmp.op = s[idx++];
		tmp.b = readnum(); //The second num comes from readnum().
		if (s[idx] == '(')
		{
			idx++; //Jump over '('
			tmp.b = func(pri[s[idx - 1]], tmp.b); //Go to next dep when '(' appears.
			idx++; //Jump over ')'
		}
		while (pri[s[idx]] > pri[tmp.op]) tmp.b = func(pri[s[idx]], tmp.b); //Go to next dep when an op with higher priority appears.
		tmp.a = tmp.com();
		if (pri[s[idx]] < pri[tmp.op] && pri[s[idx]] <= dep) return tmp.a; //This dep ends.
	}
	return tmp.a;
}