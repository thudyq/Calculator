/*README:Maths_4 ops for integer
* Legal input includes nums (0~9), ops (+,-,*,/) and parenthesis.
* Any illegal input may lead to error,including "Divided by zero" and "Mismatched parentheses".
* Press the Enter key to get the result after inputting the whole equation (without '=').
* Standard output form is improper fraction, and mixed number form and decimal form will be provided later.
* You can change the decimal's precision (<=15) in the main function.
*/
#include <iostream>
#include <algorithm>
#include <cmath>
#define LL long long
using namespace std;
void handle_error(const char*);
struct Num
{
	LL numer, denom; //Num=numerator/denominator
};
LL gcd(LL, LL);
Num simplify(Num); //reduction of a fraction
Num zero(); //return "zero(=0/1)"
struct Cal //A "Cal" contains one calculation with two "Num"s and an op.
{
	Num a, b;
	char op;
	Num com(); //computing
};
int idx;
string s; //the whole equation
int pri[200]; //the priority of ops
Num readnum(); //change string into num
Num func(int, Num); //Core function
int main()
{
	pri['+'] = pri['-'] = 1;
	pri['*'] = pri['/'] = 2;
	cin >> s;

	for (int i = 0; i < s.size(); ++i)
		if ((!isdigit(s[i]) && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/' && s[i] != '(' && s[i] != ')'))
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
	LL l = 0, r = 0;
	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == '(') ++l;
		if (s[i] == ')') ++r;
	}
	if (l != r) handle_error("Syntax Error!"); //check for mismatched parentheses

	Num ans = func(0, zero());
	if (ans.denom != 1) cout << ans.numer << '/' << ans.denom;
	else cout << ans.numer;
	cout << "\nMixed number form: ";
	if (ans.denom == 1) cout << ans.numer;
	else
		if (abs(ans.numer) <= ans.denom) cout << ans.numer << '/' << ans.denom;
		else
			if (ans.numer > 0) cout << ans.numer / ans.denom << '+' << ans.numer % ans.denom << '/' << ans.denom;
			else cout << "-(" << -ans.numer / ans.denom << '+' << -ans.numer % ans.denom << '/' << ans.denom << ')';
	//	cout.precision(10);
	cout << "\nDecimal form: " << 1.0 * ans.numer / ans.denom;
	return 0;
}
void handle_error(const char* message)
{
	cout << message;
	exit(EXIT_FAILURE);
}
LL gcd(LL a, LL b)
{
	LL r;
	while (a % b)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return b;
}
Num simplify(Num x) //reduction of a fraction
{
	LL g = gcd(x.numer, x.denom);
	x.numer /= g;
	x.denom /= g;
	if (x.denom < 0) { x.numer = -x.numer; x.denom = -x.denom; }
	return x;
}
Num zero() //return "zero(=0/1)"
{
	Num c;
	c.numer = 0;
	c.denom = 1;
	return c;
}
Num Cal::com() //computing
{
	Num c;
	switch (op)
	{
		case '+':
		{
			c.numer = a.numer * b.denom + b.numer * a.denom;
			c.denom = a.denom * b.denom;
			return simplify(c);
		}
		case '-':
		{
			c.numer = a.numer * b.denom - b.numer * a.denom;
			c.denom = a.denom * b.denom;
			return simplify(c);
		}
		case '*':
		{
			c.numer = a.numer * b.numer;
			c.denom = a.denom * b.denom;
			return simplify(c);
		}
		case '/':
		{
			if (b.numer == 0) handle_error("Maths Error!");
			c.numer = a.numer * b.denom;
			c.denom = a.denom * b.numer;
			return simplify(c);
		}
		default:handle_error("Syntax Error!");
	}
}
Num readnum() //change string into num
{
	Num c;
	LL t = 0;
	for (; idx < s.size() && s[idx] >= '0' && s[idx] <= '9'; ++idx)
		t = t * 10 + s[idx] - '0';
	c.numer = t; c.denom = 1;
	return c;
}
Num func(int dep, Num b) //Core function,b is the last-dep result
{
	Cal tmp{};
	if (isdigit(s[idx])) tmp.a = readnum();
	else tmp.a = b; //The first num comes from readnum() or b.
	while (idx < s.size())
	{
		tmp.b = zero();
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