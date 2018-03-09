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
	BigInterger operator+(const BigInterger& right) {
		BigInterger Ret;
		unsigned int overflow = 0;
		for (int i = 31; i >= 0; i--) {
			Ret.arr[i] = arr[i] + right.arr[i]+overflow;
			overflow = (Ret.arr[i] < right.arr[i]) || (overflow && (Ret.arr[i] == right.arr[i]));
		}
		return Ret;
	}
	BigInterger operator-(const BigInterger& right) {
		BigInterger Ret;
		unsigned int overflow = 0;
		for (int i = 31; i >= 0; i--) {
			unsigned int overflown = (right.arr[i] > arr[i]) || (overflow && (right.arr[i] == arr[i]));
			Ret.arr[i] = arr[i] - right.arr[i] - overflow;
			overflow = overflown;
		}
		return Ret;
	}

	BigInterger operator,(unsigned __int64 s) {
		BigInterger Ret(1);
		unsigned __int64 m = 0x8000000000000000;
		while (m) {
			Ret *= Ret;
			if (m&s)
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
	bool operator>(const BigInterger& right) const{
		for (int i = 0; i < 32; i++) {
			if (arr[i] > right.arr[i])
				return true;
			if (arr[i] < right.arr[i])
				return false;
		}
		return false;
	}
	BigInterger operator<<(int right) {
		BigInterger Ret;
		unsigned __int64 overflow = 0;
		for (int i = 31; i >= 0; i--) {
			unsigned __int64 overflown = arr[i] >> (64 - right);
			Ret.arr[i] = (arr[i] << right) | overflow;
			overflow = overflown;
		}
		return Ret;
	}
	BigInterger operator>>(int right) {
		BigInterger Ret;
		unsigned __int64 overflow = 0;
		for (int i = 0; i<32; i++) {
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
	BigInterger operator*( BigInterger right) {
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
	BigInterger operator/(BigInterger right) {
		BigInterger Ret;
		BigInterger tmp2 = *this;
		BigInterger tmp(1);
		int Max = 0;
		for (int i = 0; !(right.arr[0]&0x800000000000000) && (tmp2 > right); i++){
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

	
};
istream & operator>>(istream &in, BigInterger &obj) {
	string t;
	in >> t;
	int L = t.length()-2;
	if (L > 0 && t[0] == '0' && t[1] == 'x') {
		t = t.substr(2);
		int i = 31;
		while (L > 0) {
			if (L > 16)
				obj.arr[i--]= std::stoull(t.substr(L - 16, 16),0,16);
			else
				obj.arr[i--] = std::stoull(t.substr(0, L), 0, 16);
			L -= 16;

		}
	}
	return in;
}
ostream & operator<<(ostream &out, const BigInterger &obj)
{
	int i = 0;
	for (; i < 31 && !obj.arr[i]; i++);
	out << "0x";
	for (; i < 32; i++) {
		
		out << hex << obj.arr[i];
		cout.fill('0');
		cout.width(16);
	}

	return out;
}
int main() {
	__int64 input;
	cin >> input;
	vector<BigInterger> arr(input+1>3? input + 1:3);
	arr[0] = 1;
	arr[1] = 1;
	arr[2] = 3;
	for (int i = 3; i <= input; i++) {
		arr[i] = arr[i - 1] * 3 - arr[i - 2];
	}
	BigInterger Res = 0;
	for (__int64 i = 0; i <= input; i++)
		Res += (arr[input - i] * (i*i));
	cout << Res;
	return 0;
}
