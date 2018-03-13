class BigInterger {
	unsigned __int64 arr[32];
	friend ostream & operator<<(ostream &out, const BigInterger &obj);
	friend istream & operator>>(istream &out, BigInterger &obj);

public:
	BigInterger() {
		for (int i = 0; i < 32; i++)
			arr[i] = 0;
	}
	BigInterger(unsigned __int64 i) {
		for (int i = 0; i < 31; i++)
			arr[i] = 0;
		arr[31] = i;
	}
	BigInterger(string  t) {
		for (int i = 0; i < 32; i++)
			arr[i] = 0;
		if (t.compare(0, 2, "0x") == 0) {
			int L = t.length() - 2;
			if (L > 0 && t[0] == '0' && t[1] == 'x') {
				t = t.substr(2);
				int i = 31;
				while (L > 0) {
					if (L > 16)
						arr[i--] = std::stoull(t.substr(L - 16, 16), 0, 16);
					else
						arr[i--] = std::stoull(t.substr(0, L), 0, 16);
					L -= 16;

				}
			}
		}
		else {
			int L = t.length();
			
			while (L > 0) {
				
				if (L > 19) {
					*this *= 10000000000000000000;
					*this += std::stoull(t.substr(0, 19), 0, 10);
					t = t.substr(19);
				}
				else {
					for(int i=0;i<L;i++)
						*this *= 10;
					*this += std::stoull(t.substr(0, L), 0, 10);
				}
					
				L -= 19;
			}

		}
	}
	BigInterger operator+(const BigInterger& right) const {
		BigInterger Ret;
		unsigned int overflow = 0;
		for (int i = 31; i >= 0; i--) {
			Ret.arr[i] = arr[i] + right.arr[i] + overflow;
			overflow = (Ret.arr[i] < right.arr[i]) || (overflow && (Ret.arr[i] == right.arr[i]));
		}
		return Ret;
	}
	BigInterger operator-(const BigInterger& right) const {
		BigInterger Ret;
		unsigned int overflow = 0;
		for (int i = 31; i >= 0; i--) {
			unsigned int overflown = (right.arr[i] > arr[i]) || (overflow && (right.arr[i] == arr[i]));
			Ret.arr[i] = arr[i] - right.arr[i] - overflow;
			overflow = overflown;
		}
		return Ret;
	}

	BigInterger operator,(const BigInterger& s) const {
		BigInterger Ret(1);
		unsigned __int64 m = 0x8000000000000000;
		if (s > m)
			throw string("overflow!");
		while (m) {
			Ret *= Ret;
			if (m&s.arr[31])
				Ret *= *this;
			m >>= 1;

		}
		return Ret;
	}
	BigInterger& operator+=(const BigInterger& right) {
		unsigned int overflow = 0;
		for (int i = 31; i >= 0; i--) {
			arr[i] = arr[i] + right.arr[i] + overflow;
			overflow = (arr[i] < right.arr[i]) || (overflow && (arr[i] == right.arr[i]));
		}
		return *this;
	}
	BigInterger& operator-=(const BigInterger& right) {
		unsigned int overflow = 0;
		for (int i = 31; i >= 0; i--) {
			unsigned int overflown = (right.arr[i] > arr[i]) || (overflow && (right.arr[i] == arr[i]));
			arr[i] = arr[i] - right.arr[i] - overflow;
			overflow = overflown;
		}
		return *this;
	}
	bool operator>(const BigInterger& right) const {
		for (int i = 0; i < 32; i++) {
			if (arr[i] > right.arr[i])
				return true;
			if (arr[i] < right.arr[i])
				return false;
		}
		return false;
	}
	BigInterger operator<<(int right) const {
		BigInterger Ret;
		unsigned __int64 overflow = 0;
		for (int i = 31; i >= 0; i--) {
			unsigned __int64 overflown = arr[i] >> (64 - right);
			Ret.arr[i] = (arr[i] << right) | overflow;
			overflow = overflown;
		}
		return Ret;
	}
	BigInterger operator>>(int right) const {
		BigInterger Ret;
		unsigned __int64 overflow = 0;
		for (int i = 0; i < 32; i++) {
			unsigned __int64 overflown = arr[i] << (64 - right);
			Ret.arr[i] = (arr[i] >> right) | overflow;
			overflow = overflown;
		}
		return Ret;
	}
	BigInterger& operator>>=(int right) {
		unsigned __int64 overflow = 0;
		for (int i = 0; i < 32; i++) {
			unsigned __int64 overflown = arr[i] << (64 - right);
			arr[i] = (arr[i] >> right) | overflow;
			overflow = overflown;
		}
		return *this;
	}

	BigInterger& operator<<=(int right) {
		unsigned __int64 overflow = 0;
		for (int i = 31; i >= 0; i--) {
			unsigned __int64 overflown = arr[i] >> (64 - right);
			arr[i] = (arr[i] << right) | overflow;
			overflow = overflown;
		}
		return *this;
	}
	__int64 operator&(__int64 right) {
		return arr[31] & right;
	}
	BigInterger operator*(BigInterger right) const {
		BigInterger Ret;
		BigInterger tmp2 = *this;
		int Max = 2048;
		for (int i = 0; i < 32 && right.arr[i] == 0; i++) {
			Max -= 64;
		}
		for (int i = 0; i < Max; i++)
		{
			if (right & 1)
				Ret += tmp2;
			right >>= 1;
			tmp2 <<= 1;

		}
		return Ret;
	}

	bool operator==(const BigInterger& right) const {
		for (int i = 0; i< 32; i++) {
			if (arr[i] != right.arr[i])
				return false;
		}
		return true;
	}

	BigInterger operator%(BigInterger right) const {
		BigInterger Ret;
		BigInterger tmp2 = *this;
		BigInterger tmp(1);
		int Max = 0;
		for (int i = 0; !(right.arr[0] & 0x800000000000000) && (tmp2 > right); i++) {
			Max += 1;
			right <<= 1;
			tmp <<= 1;
		}
		for (int i = 0; i <= Max; i++) {
			if (!(right > tmp2)) {
				tmp2 -= right;
				Ret += tmp;
			}
			tmp >>= 1;
			right >>= 1;
		}
		return tmp2;
	}
	BigInterger& operator*=(BigInterger right) {

		BigInterger tmp2 = *this;
		*this = 0;
		int Max = 2048;
		for (int i = 0; i < 32 && right.arr[i] == 0; i++) {
			Max -= 64;
		}
		for (int i = 0; i < Max; i++)
		{
			if (right & 1)
				*this += tmp2;
			right >>= 1;
			tmp2 <<= 1;

		}
		return *this;
	}
	BigInterger operator/(BigInterger right) const {
		if (right == 0) {
			throw string("devide by 0");
		}
		BigInterger Ret;
		BigInterger tmp2 = *this;
		BigInterger tmp(1);
		
		int Max = 0;
		for (int i = 0; !(right.arr[0] & 0x800000000000000) && (tmp2 > right); i++) {
			Max += 1;
			right <<= 1;
			tmp <<= 1;
		}
		for (int i = 0; i <= Max; i++) {
			if (!(right > tmp2)) {
				tmp2 -= right;
				Ret += tmp;
			}
			tmp >>= 1;
			right >>= 1;
		}
		return Ret;
	}
	BigInterger& operator/=(BigInterger right) {
		if (right == 0) {
			throw string("devide by 0");
		}
		BigInterger Ret;
		BigInterger tmp(1);
		int Max = 0;
		for (int i = 0; !(right.arr[0] & 0x800000000000000) && (*this > right); i++) {
			Max += 1;
			right <<= 1;
			tmp <<= 1;
		}
		for (int i = 0; i <= Max; i++) {
			if (!(right > *this)) {
				*this -= right;
				Ret += tmp;
			}
			tmp >>= 1;
			right >>= 1;
		}
		*this = Ret;
		return *this;
	}


};
istream & operator>>(istream &in, BigInterger &obj) {
	string t;
	in >> t;
	if (t.compare(0, 2, "0x") == 0) {
		int L = t.length() - 2;
		if (L > 0 && t[0] == '0' && t[1] == 'x') {
			t = t.substr(2);
			int i = 31;
			while (L > 0) {
				if (L > 16)
					obj.arr[i--] = std::stoull(t.substr(L - 16, 16), 0, 16);
				else
					obj.arr[i--] = std::stoull(t.substr(0, L), 0, 16);
				L -= 16;

			}
		}
	}
	else {
		int L = t.length();

		while (L > 0) {
			obj *= 10000000000000000000;
			if (L > 19) {
				obj += std::stoull(t.substr(0, 19), 0, 10);
				t = t.substr(0, 19);
			}
			else
				obj += std::stoull(t.substr(0, L), 0, 10);
			L -= 19;
		}
	}
	
	return in;
}
ostream & operator<<(ostream &out, const BigInterger &obj)
{
	if (out.flags()&out.hex) {
		int i = 0;
		for (; i < 31 && !obj.arr[i]; i++);
		out << "0x";
		out << obj.arr[i++];
		for (; i < 32; i++) {
			
			cout.fill('0');
			cout.width(16);
			out << obj.arr[i];
		}
	}
	else {
		BigInterger tmp(obj);
		stack<unsigned long long> st;
		while (tmp > 10000000000000000000) {	
			st.push((tmp % 10000000000000000000).arr[31]);
			tmp /= 10000000000000000000;
		}
		cout<<tmp.arr[31];
		
		while (!st.empty()) {
			cout.fill('0');
			cout.width(19);
			out<<st.top();
			st.pop();

		}
	}
	

	return out;
}


class Exp :public std::enable_shared_from_this<Exp> {
public:
	Exp() = default;
	virtual BigInterger GetVal() = 0;
	virtual ~Exp() = default;
};

class Num :public Exp {
	BigInterger val;
public:
	Num(const BigInterger& i) :val(i) {

	}
	~Num() = default;
	BigInterger GetVal() {
		return val;
	}
};
using funcMap = map < std::string, function<BigInterger(const BigInterger&, const BigInterger&)>>;

class NopN :public Exp {
	static funcMap opMap;
	shared_ptr<Exp> val1;
	string op;
	shared_ptr<Exp> val2;
public:
	NopN(shared_ptr<Exp> _val1, string _op, shared_ptr<Exp> _val2) :val1(_val1), op(std::move(_op)), val2(_val2) {

	}
	~NopN() = default;
	BigInterger GetVal() {
		return opMap[op](val1->GetVal(), val2->GetVal());
	}
};

string Get(const string& str, const std::vector<string>& p, int& begin, int end) {
	while (str[begin] == ' ')
		begin++;
	for (auto & i : p) {
		if (end - begin >= i.length())
			if (str.compare(begin, i.length(), i) == 0) {
				begin += i.length();
				return i;
			}
	}
	throw string("Get Not Found");
}

shared_ptr<Exp> GetExp(const string& in, int& begin, int end);

shared_ptr<Exp> GetNum(const string& in, int& begin, int end) {
	string s;
	bool hex = false;
	while (in[begin] == ' ')
		begin++;
	if (in.compare(begin, 2, "0x") == 0) {
		s.append("0x");
		begin += 2;
		hex = true;
	}
	if (hex)
		for (; begin < end; begin++) {
			if ((in[begin] <= '9'&&in[begin] >= '0') || (in[begin] <= 'f'&&in[begin] >= 'a'))
				s.append(1, in[begin]);
			else
				break;
		}
	else
		for (; begin < end; begin++) {
			if ((in[begin] <= '9'&&in[begin] >= '0'))
				s.append(1, in[begin]);
			else
				break;
		}
	if (s.length() > 0)
		return shared_ptr<Exp>(new Num(BigInterger(s)));
	else
		throw string("GetNum Error");
}

shared_ptr<Exp> GetTerm(const string& in, int& begin, int end) {
	shared_ptr<Exp> Ret;
	try {
		Ret = GetNum(in, begin, end);
	}
	catch (string) {
		try
		{
			string op = Get(in, { "(" }, begin, end);
			Ret = GetExp(in, begin, end);
			op = Get(in, { ")" }, begin, end);
		}
		catch (string e)
		{
			throw string("Error");
		}
		
	}
	return Ret;
}

shared_ptr<Exp> GetPower(const string& in, int& begin, int end) {
	shared_ptr<Exp> Ret;
	Ret = GetTerm(in, begin, end);
	try
	{
		while (begin < end) {
			string op = Get(in, { "**"}, begin, end);
			auto exp2 = GetTerm(in, begin, end);
			Ret = shared_ptr<Exp>(new NopN(Ret, op, exp2));
		}
	}
	catch (string e)
	{
		if (e != "Get Not Found") {
			throw e;
		}
	}
	return Ret;
}


shared_ptr<Exp> GetFactor(const string& in, int& begin, int end) {
	shared_ptr<Exp> Ret;
	Ret = GetPower(in, begin, end);
	try
	{
		while (begin < end) {
			string op = Get(in, { "*","/","%" }, begin, end);
			auto exp2 = GetPower(in, begin, end);
			Ret = shared_ptr<Exp>(new NopN(Ret, op, exp2));
		}
	}
	catch (string e)
	{
		if (e != "Get Not Found") {
			throw e;
		}
	}
	return Ret;
}

shared_ptr<Exp> GetExp(const string& in, int& begin, int end) {
	shared_ptr<Exp> Ret;
	Ret = GetFactor(in, begin, end);
	try {
		while (begin < end) {
			string op = Get(in, { "+","-" }, begin, end);
			auto exp2 = GetFactor(in, begin, end);
			Ret = shared_ptr<Exp>(new NopN(Ret, op, exp2));
		}
	}
	catch (string e) {
		if (e != "Get Not Found") {
			throw e;
		}
	}
	


	return Ret;
}

funcMap NopN::opMap(
	{
		std::make_pair("*",[](const BigInterger& a,const BigInterger& b) {
			return a * b;
		}),
		std::make_pair("/",[](const BigInterger& a,const BigInterger& b) {
			return a / b;
		}),
		std::make_pair("+",[](const BigInterger& a,const BigInterger& b) {
			return a + b;
		}),
		std::make_pair("-",[](const BigInterger& a,const BigInterger& b) {
			return a - b;
		}),
		std::make_pair("%",[](const BigInterger& a,const BigInterger& b) {
			return a % b;
		}),
		std::make_pair("**",[](const BigInterger& a,const BigInterger& b) {
			return (a , b);
		})
	});
