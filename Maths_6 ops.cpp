/*README:Maths_6 ops
* Legal input includes nums (0~9), decimal point (.), ops (+,-,*,/,^) and parenthesis.
* Any illegal input may lead to error,including "Divided by zero", "Incorrect use of decimal point" and "Mismatched parentheses".
* As for Division: All the denominators will be rationalized.
* As for Exponentiation and square root operations:
* (1) Input in the form of "a^b". For example, input sqrt(2) in the form of "2^(1/2)" or "2^0.5".
* (2) If a only contains one "Num" (e.g. 2 or 2^(1/2)), b has to be a rational number; If not (e.g. 2+2^(1/2) or 3^(1/2)+2^(1/2)), b has to be a integer; Otherwise it will be too complex to solve.
* (3) If a is 0, b has to be a postive number; The result is within the realm of real numbers (otherwise "Maths Error!").
* Press the Enter key to get the result after inputting the whole equation (without '=').
* Standard output form is radical's sum, and decimal form will be provided later.
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
	LL numer, denom, radic, radix; //Num=numerator/denominator*radicand^(1/radix)
};
struct Expr
{
	Num terms[110];
	int N; //The Expr(ession) will be the sum of N "Num"s
};
LL gcd(LL, LL);
Num simplify(Num); //simplification of a radical
Expr zero(); //return "zero(=0/1*1^(1/1))"
Expr one(); //return "one(=1/1*1^(1/1))"
struct Cal //A "Cal" contains one calculation with two "Expr"s and an op.
{
	Expr a, b;
	char op;
	Expr com(); //computing
};
int idx;
string s; //the whole equation
int pri[200]; //the priority of ops
Num readnum(); //change string into num
Expr func(int, Expr); //Core function
Expr Plus(Expr, Expr);
Expr Minus(Expr, Expr);
Expr Multiply(Expr, Expr);
Expr Divide(Expr, Expr);
Expr Power(Expr, Expr);
int main()
{
	pri['+'] = pri['-'] = 1;
	pri['*'] = pri['/'] = 2;
	pri['^'] = 3;
	cin >> s;

	for (int i = 0; i < s.size(); ++i)
		if ((!isdigit(s[i]) && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/' && s[i] != '^' && s[i] != '(' && s[i] != ')' && s[i] != '.'))
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

	Expr ans = func(0, zero());
	for (int i = 1; i <= ans.N; ++i)
	{
		bool neg = 0;
		if (ans.terms[i].numer > 0 && i > 1) cout << '+';
		if (ans.terms[i].numer < 0) { neg = 1; cout << '-'; ans.terms[i].numer = -ans.terms[i].numer; }
		if (ans.terms[i].numer != 1 || ans.terms[i].radic == 1 || ans.terms[i].denom != 1) cout << ans.terms[i].numer;
		if (ans.terms[i].denom != 1) cout << '/' << ans.terms[i].denom;
		if ((ans.terms[i].numer != 1 || ans.terms[i].denom != 1) && ans.terms[i].radic != 1) cout << '*';
		if (ans.terms[i].radic != 1) cout << ans.terms[i].radic << "^(1/" << ans.terms[i].radix << ')';
		if (neg == 1) ans.terms[i].numer = -ans.terms[i].numer;
	}
	double dec_ans = 0;
	for (int i = 1; i <= ans.N; ++i) dec_ans += 1.0 * ans.terms[i].numer / ans.terms[i].denom * pow(ans.terms[i].radic, 1.0 / ans.terms[i].radix);
	//	cout.precision(10);
	cout << "\nDecimal form: " << dec_ans;
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
Num simplify(Num x) //simplification of a radical
{
	if (x.radic == 0) x.numer = 0;
	if (x.numer == 0) x.radic = 1; //0=0/denom*1^(1/radix)
	for (int i = ceil(pow(x.radic, 1.0 / x.radix)); i >= 2; --i)
	{
		LL q = pow(i, x.radix);
		if (x.radic % q == 0)
		{
			x.radic /= q;
			x.numer *= i;
		}
	} //Move the possible factors from the radic to the numer, like sqrt(8)=2*sqrt(2)
	if (x.radic == 1) x.radix = 1; //n=n/1*1^(1/1)
	for (int i = x.radix; i >= 2; --i)
		if (x.radix % i == 0)
		{
			LL k = ceil(pow(x.radic, 1.0 / i));
			if (pow(k, i) == x.radic)
			{
				x.radix /= i;
				x.radic = k;
			}
		} //Simplify the radic and the radix, like 16^(1/6)=4^(1/3)
	LL g = gcd(x.numer, x.denom);
	x.numer /= g;
	x.denom /= g;
	if (x.denom < 0) { x.numer = -x.numer; x.denom = -x.denom; }
	return x;
}
Expr zero() //return "zero(=0/1*1^(1/1))"
{
	Expr c;
	c.N = 1;
	c.terms[1].numer = 0; c.terms[1].denom = 1; c.terms[1].radic = 1; c.terms[1].radix = 1;
	return c;
}
Expr one() //return "one(=1/1*1^(1/1))"
{
	Expr c;
	c.N = 1;
	c.terms[1].numer = 1; c.terms[1].denom = 1; c.terms[1].radic = 1; c.terms[1].radix = 1;
	return c;
}
Expr Cal::com() //computing
{
	switch (op)
	{
		case '+':return Plus(a, b);
		case '-':return Minus(a, b);
		case '*':return Multiply(a, b);
		case '/':return Divide(a, b);
		case '^':return Power(a, b);
		default:handle_error("Syntax Error!");
	}
}
Num readnum() //change string into num
{
	Num c;
	LL t = 0;
	for (; idx < s.size() && s[idx] >= '0' && s[idx] <= '9'; ++idx)
		t = t * 10 + s[idx] - '0';
	int p = 0;
	if (s[idx] == '.')
	{
		idx++;
		for (; idx < s.size() && s[idx] >= '0' && s[idx] <= '9'; ++idx, ++p)
			t = t * 10 + s[idx] - '0';
	}
	c.numer = t; c.denom = pow(10, p); c.radic = 1; c.radix = 1;
	if (s[idx] == '.') handle_error("Syntax Error!");
	return simplify(c);
}
Expr func(int dep, Expr b) //Core function,b is the last-dep result
{
	Cal tmp{};
	if (isdigit(s[idx])) { tmp.a.N = 1; tmp.a.terms[1] = readnum(); }
	else tmp.a = b; //The first num comes from readnum() or b.
	while (idx < s.size())
	{
		tmp.b = zero();
		if (s[idx] == ')') return tmp.a; //This dep ends when ')' appears.
		else
			if (s[idx] == '(') tmp.op = '+'; //The default op is '+'.
			else tmp.op = s[idx++];
		tmp.b.terms[1] = readnum(); //The second num comes from readnum().
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
Expr Plus(Expr a, Expr b)
{
	Expr c = a;
	for (int i = 1; i <= b.N; ++i)
	{
		bool judge = 0;
		for (int j = 1; j <= c.N; ++j)
			if (b.terms[i].radic == c.terms[j].radic && b.terms[i].radix == c.terms[j].radix)
			{
				c.terms[j].numer = c.terms[j].numer * b.terms[i].denom + b.terms[i].numer * c.terms[j].denom;
				c.terms[j].denom *= b.terms[i].denom;
				c.terms[j] = simplify(c.terms[j]);
				judge = 1;
				break;
			} //If two "Num"s have the same radical part, their fraction part can be added directly
		if (!judge) c.terms[++c.N] = simplify(b.terms[i]); //If not, the "Expr" need a new "Num"
	}
	int cnt = 0;
	for (int i = 1; i <= c.N; ++i)
		if (c.terms[i].numer != 0) c.terms[++cnt] = c.terms[i]; //remove the zero term
	c.N = cnt;
	if (c.N == 0) c = zero();
	return c;
}
Expr Minus(Expr a, Expr b)
{
	for (int i = 1; i <= b.N; ++i)
		b.terms[i].numer = -b.terms[i].numer;
	return Plus(a, b);
}
Expr Multiply(Expr a, Expr b)
{
	Expr c;
	c.N = 0;
	for (int i = 1; i <= b.N; ++i) //the distributive law of multiplication
	{
		Expr res;
		res.N = a.N;
		for (int j = 1; j <= a.N; ++j)
		{
			res.terms[j].numer = b.terms[i].numer * a.terms[j].numer;
			res.terms[j].denom = b.terms[i].denom * a.terms[j].denom;
			res.terms[j].radix = b.terms[i].radix * a.terms[j].radix / gcd(b.terms[i].radix, a.terms[j].radix);
			res.terms[j].radic = pow(b.terms[i].radic, res.terms[j].radix / b.terms[i].radix) * pow(a.terms[j].radic, res.terms[j].radix / a.terms[j].radix);
			res.terms[j] = simplify(res.terms[j]);
		}
		c = Plus(c, res);
	}
	return c;
}
Expr Divide(Expr a, Expr b)
{
	if (b.N == 1)
	{
		if (b.terms[1].numer == 0) handle_error("Maths Error!");
		swap(b.terms[1].numer, b.terms[1].denom);
		b.terms[1].denom *= b.terms[1].radic;
		b.terms[1].radic = pow(b.terms[1].radic, b.terms[1].radix - 1);
		return Multiply(a, b); //1/(a/b*(c^(1/d)))=b/(a*c)*(c^(d-1))^(1/d)
	}
	else
	{
		long long lcm = 1;
		for (int i = 1; i <= b.N; ++i)
			lcm = lcm * b.terms[i].radix / gcd(lcm, b.terms[i].radix);
		Expr b_; //a/b=(a*b_)/(b*b_), b_ is the conjugate expression to rationalize the denominator
		if (lcm % 2 == 0) //e.g. b=sqrt(2)+sqrt(3)+sqrt(5)+sqrt(7), b_=sqrt(2)+sqrt(3)-sqrt(5)-sqrt(7)
		{
			b_ = b;
			for (int i = (b.N + 1) / 2 + 1; i <= b.N; ++i)
				b_.terms[i].numer = -b_.terms[i].numer;
		}
		else //e.g. b=2^(1/3)+3^(1/3), b_=4^(1/3)-6^(1/3)+9^(1/3)
		{
			Expr b1 = b; b1.N = (b.N + 1) / 2;
			Expr b2; b2.N = b.N - b1.N;
			for (int i = 1; i <= b2.N; ++i)
			{
				b2.terms[i] = b.terms[i + b1.N];
				b2.terms[i].numer = -b2.terms[i].numer;
			}
			b_.N = 0;
			for (int i = 1; i <= lcm; ++i)
			{
				Expr res = one();
				for (int j = 1; j < i; ++j)
					res = Multiply(res, b1);
				for (int j = i; j < lcm; ++j)
					res = Multiply(res, b2);
				b_ = Plus(b_, res);
			}
		}
		return Divide(Multiply(a, b_), Multiply(b, b_));
	}
	return zero();
}
Expr Power(Expr a, Expr b)
{
	if (b.N > 1) handle_error("Too complex to solve!");
	if (b.terms[1].radic > 1 || b.terms[1].radix > 1) handle_error("Too complex to solve!");
	if (a.terms[1].numer == 0 && b.terms[1].numer <= 0) handle_error("Maths Error!");
	if (b.terms[1].numer < 0) //e.g.2^(-1/3)=(1/2)^(1/3)
	{
		b.terms[1].numer = -b.terms[1].numer;
		a = Divide(one(), a);
		return Power(a, b);
	}
	if (a.N == 1)
	{
		bool judge = 0;
		if (a.terms[1].numer < 0)
		{
			if (b.terms[1].denom % 2 == 0) handle_error("Maths Error!");
			judge = 1; a.terms[1].numer = -a.terms[1].numer;
		}
		a.terms[1].radic = pow(a.terms[1].numer, a.terms[1].radix * b.terms[1].numer) * pow(a.terms[1].radic, b.terms[1].numer) * pow(a.terms[1].denom, a.terms[1].radix * b.terms[1].numer * (a.terms[1].radix * b.terms[1].denom - 1));
		a.terms[1].denom = pow(a.terms[1].denom, a.terms[1].radix * b.terms[1].numer);
		a.terms[1].radix *= b.terms[1].denom;
		if (judge && b.terms[1].numer % 2 == 1) a.terms[1].numer = -1;
		else a.terms[1].numer = 1;
		a.terms[1] = simplify(a.terms[1]);
		return a;
	} //(a/b*c^(1/d))^(e/f)=1/(b^(d*e))*(a^(d*e)*c^e*b^(d*e*(d*f-1)))^(1/(d*f))
	else
	{
		if (b.terms[1].denom > 1) handle_error("Too complex to solve!");
		Expr res = a;
		for (int i = 1; i <= b.terms[1].numer - 1; ++i)
			res = Multiply(res, a);
		return res;
	}
	return zero();
}