/*README:Maths_6 ops_pro
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
* Compared with the last version, this version uses "vector" to save data so that it has a larger available memeory.
*/
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#define LL long long
#define Expr vector<Num> //The Expr(ession) will be the sum of N "Num"s
using namespace std;
void handle_error(const char*);
struct Num
{
	LL numer, denom, radic, radix; //Num=numerator/denominator*radicand^(1/radix)
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
	for (int i = 0; i < ans.size(); ++i)
	{
		bool neg = 0;
		if (ans[i].numer > 0 && i > 0) cout << '+';
		if (ans[i].numer < 0) { neg = 1; cout << '-'; ans[i].numer = -ans[i].numer; }
		if (ans[i].numer != 1 || ans[i].radic == 1 || ans[i].denom != 1) cout << ans[i].numer;
		if (ans[i].denom != 1) cout << '/' << ans[i].denom;
		if ((ans[i].numer != 1 || ans[i].denom != 1) && ans[i].radic != 1) cout << '*';
		if (ans[i].radic != 1) cout << ans[i].radic << "^(1/" << ans[i].radix << ')';
		if (neg == 1) ans[i].numer = -ans[i].numer;
	}
	double dec_ans = 0;
	for (int i = 0; i < ans.size(); ++i) dec_ans += 1.0 * ans[i].numer / ans[i].denom * pow(ans[i].radic, 1.0 / ans[i].radix);
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
	Num c;
	c.numer = 0; c.denom = 1; c.radic = 1; c.radix = 1;
	Expr C;
	C.push_back(c);
	return C;
}
Expr one() //return "one(=1/1*1^(1/1))"
{
	Num c;
	c.numer = 1; c.denom = 1; c.radic = 1; c.radix = 1;
	Expr C;
	C.push_back(c);
	return C;
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
	if (isdigit(s[idx])) tmp.a.push_back(readnum());
	else tmp.a = b; //The first num comes from readnum() or b.
	while (idx < s.size())
	{
		if (s[idx] == ')') return tmp.a; //This dep ends when ')' appears.
		else
			if (s[idx] == '(') tmp.op = '+'; //The default op is '+'.
			else tmp.op = s[idx++];
		tmp.b.clear();
		tmp.b.push_back(readnum()); //The second num comes from readnum().
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
	for (int i = 0; i < b.size(); ++i)
	{
		bool judge = 0;
		for (int j = 0; j < c.size(); ++j)
			if (b[i].radic == c[j].radic && b[i].radix == c[j].radix)
			{
				c[j].numer = c[j].numer * b[i].denom + b[i].numer * c[j].denom;
				c[j].denom *= b[i].denom;
				c[j] = simplify(c[j]);
				judge = 1;
				break;
			} //If two "Num"s have the same radical part, their fraction part can be added directly
		if (!judge) c.push_back(simplify(b[i])); //If not, the "Expr" need a new "Num"
	}
	Expr c_;
	for (int i = 0; i < c.size(); ++i)
		if (c[i].numer != 0) c_.push_back(c[i]); //remove the zero term
	if (c_.empty()) c_ = zero();
	return c_;
}
Expr Minus(Expr a, Expr b)
{
	for (int i = 0; i < b.size(); ++i)
		b[i].numer = -b[i].numer;
	return Plus(a, b);
}
Expr Multiply(Expr a, Expr b)
{
	Expr c;
	c.clear();
	for (int i = 0; i < b.size(); ++i) //the distributive law of multiplication
	{
		Expr res;
		for (int j = 0; j < a.size(); ++j)
		{
			Num mul; //Single multiplication result
			mul.numer = b[i].numer * a[j].numer;
			mul.denom = b[i].denom * a[j].denom;
			mul.radix = b[i].radix * a[j].radix / gcd(b[i].radix, a[j].radix);
			mul.radic = pow(b[i].radic, mul.radix / b[i].radix) * pow(a[j].radic, mul.radix / a[j].radix);
			res.push_back(simplify(mul));
		}
		c = Plus(c, res);
	}
	return c;
}
Expr Divide(Expr a, Expr b)
{
	if (b.size() == 1)
	{
		if (b[0].numer == 0) handle_error("Maths Error!");
		swap(b[0].numer, b[0].denom);
		b[0].denom *= b[0].radic;
		b[0].radic = pow(b[0].radic, b[0].radix - 1);
		return Multiply(a, b); //1/(a/b*(c^(1/d)))=b/(a*c)*(c^(d-1))^(1/d)
	}
	else
	{
		long long lcm = 1;
		for (int i = 0; i < b.size(); ++i)
			lcm = lcm * b[i].radix / gcd(lcm, b[i].radix);
		Expr b_; //a/b=(a*b_)/(b*b_), b_ is the conjugate expression to rationalize the denominator
		if (lcm % 2 == 0) //e.g. b=sqrt(2)+sqrt(3)+sqrt(5)+sqrt(7), b_=sqrt(2)+sqrt(3)-sqrt(5)-sqrt(7)
		{
			b_ = b;
			for (int i = (b.size() + 1) / 2; i < b.size(); ++i)
				b_[i].numer = -b_[i].numer;
		}
		else //e.g. b=2^(1/3)+3^(1/3), b_=4^(1/3)-6^(1/3)+9^(1/3)
		{
			Expr b1, b2;
			for (int i = 0; i < (b.size() + 1) / 2; ++i)
				b1.push_back(b[i]);
			for (int i = 0; i < b.size() - b1.size(); ++i)
			{
				b2.push_back(b[i + b1.size()]);
				b2[i].numer = -b2[i].numer;
			}
			b_.clear();
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
	if (b.size() > 1) handle_error("Too complex to solve!");
	if (b[0].radic > 1 || b[0].radix > 1) handle_error("Too complex to solve!");
	if (a[0].numer == 0 && b[0].numer <= 0) handle_error("Maths Error!");
	if (b[0].numer < 0) //e.g.2^(-1/3)=(1/2)^(1/3)
	{
		b[0].numer = -b[0].numer;
		a = Divide(one(), a);
		return Power(a, b);
	}
	if (a.size() == 1)
	{
		bool judge = 0;
		if (a[0].numer < 0)
		{
			if (b[0].denom % 2 == 0) handle_error("Maths Error!");
			judge = 1; a[0].numer = -a[0].numer;
		}
		a[0].radic = pow(a[0].numer, a[0].radix * b[0].numer) * pow(a[0].radic, b[0].numer) * pow(a[0].denom, a[0].radix * b[0].numer * (a[0].radix * b[0].denom - 1));
		a[0].denom = pow(a[0].denom, a[0].radix * b[0].numer);
		a[0].radix *= b[0].denom;
		if (judge && b[0].numer % 2 == 1) a[0].numer = -1;
		else a[0].numer = 1;
		a[0] = simplify(a[0]);
		return a;
	} //(a/b*c^(1/d))^(e/f)=1/(b^(d*e))*(a^(d*e)*c^e*b^(d*e*(d*f-1)))^(1/(d*f))
	else
	{
		if (b[0].denom > 1) handle_error("Too complex to solve!");
		Expr res = a;
		for (int i = 1; i <= b[0].numer - 1; ++i)
			res = Multiply(res, a);
		return res;
	}
	return zero();
}