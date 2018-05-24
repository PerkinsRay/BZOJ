#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;

#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <stack>
using namespace std;
#define __int64 long long
#define ArryLen 32
#define ArryMax (ArryLen - 2)
class BigInterger
{
    unsigned __int64 arr[ArryLen];
    friend ostream &operator<<(ostream &out, const BigInterger &obj);
    friend istream &operator>>(istream &out, BigInterger &obj);

  public:
    BigInterger()
    {
        for (int i = 0; i < ArryLen; i++)
            arr[i] = 0;
    }
    BigInterger(unsigned __int64 i)
    {
        for (int i = 0; i < ArryMax; i++)
            arr[i] = 0;
        arr[ArryMax] = i;
    }
    BigInterger(string t)
    {
        for (int i = 0; i < ArryLen; i++)
            arr[i] = 0;
        if (t.compare(0, 2, "0x") == 0)
        {
            int L = t.length() - 2;
            if (L > 0 && t[0] == '0' && t[1] == 'x')
            {
                t = t.substr(2);
                int i = ArryMax;
                while (L > 0)
                {
                    if (L > 16)
                        arr[i--] = std::stoull(t.substr(L - 16, 16), 0, 16);
                    else
                        arr[i--] = std::stoull(t.substr(0, L), 0, 16);
                    L -= 16;
                }
            }
        }
        else
        {
            int L = t.length();

            while (L > 0)
            {

                if (L > 19)
                {
                    *this *= 10000000000000000000ULL;
                    *this += std::stoull(t.substr(0, 19), 0, 10);
                    t = t.substr(19);
                }
                else
                {
                    for (int i = 0; i < L; i++)
                        *this *= 10;
                    *this += std::stoull(t.substr(0, L), 0, 10);
                }

                L -= 19;
            }
        }
    }
    BigInterger operator+(const BigInterger &right) const
    {
        BigInterger Ret;
        unsigned int overflow = 0;
        for (int i = ArryMax; i >= 0; i--)
        {
            Ret.arr[i] = arr[i] + right.arr[i] + overflow;
            overflow = (Ret.arr[i] < right.arr[i]) || (overflow && (Ret.arr[i] == right.arr[i]));
        }
        return Ret;
    }
    BigInterger operator-(const BigInterger &right) const
    {
        BigInterger Ret;
        unsigned int overflow = 0;
        for (int i = ArryMax; i >= 0; i--)
        {
            unsigned int overflown = (right.arr[i] > arr[i]) || (overflow && (right.arr[i] == arr[i]));
            Ret.arr[i] = arr[i] - right.arr[i] - overflow;
            overflow = overflown;
        }
        return Ret;
    }

    BigInterger operator,(const BigInterger &s) const
    {
        BigInterger Ret(1);
        unsigned __int64 m = 0x8000000000000000;
        if (s > m)
            throw string("overflow!");
        while (m)
        {
            Ret *= Ret;
            if (m & s.arr[ArryMax])
                Ret *= *this;
            m >>= 1;
        }
        return Ret;
    }
    BigInterger &operator+=(const BigInterger &right)
    {
        unsigned int overflow = 0;
        for (int i = ArryMax; i >= 0; i--)
        {
            arr[i] = arr[i] + right.arr[i] + overflow;
            overflow = (arr[i] < right.arr[i]) || (overflow && (arr[i] == right.arr[i]));
        }
        return *this;
    }
    BigInterger &operator-=(const BigInterger &right)
    {
        unsigned int overflow = 0;
        for (int i = ArryMax; i >= 0; i--)
        {
            unsigned int overflown = (right.arr[i] > arr[i]) || (overflow && (right.arr[i] == arr[i]));
            arr[i] = arr[i] - right.arr[i] - overflow;
            overflow = overflown;
        }
        return *this;
    }
    bool operator>(const BigInterger &right) const
    {
        for (int i = 0; i < ArryLen; i++)
        {
            if (arr[i] > right.arr[i])
                return true;
            if (arr[i] < right.arr[i])
                return false;
        }
        return false;
    }
    BigInterger operator<<(int right) const
    {
        BigInterger Ret;
        unsigned __int64 overflow = 0;
        for (int i = ArryMax; i >= 0; i--)
        {
            unsigned __int64 overflown = arr[i] >> (64 - right);
            Ret.arr[i] = (arr[i] << right) | overflow;
            overflow = overflown;
        }
        return Ret;
    }
    BigInterger operator>>(int right) const
    {
        BigInterger Ret;
        unsigned __int64 overflow = 0;
        for (int i = 0; i < ArryLen; i++)
        {
            unsigned __int64 overflown = arr[i] << (64 - right);
            Ret.arr[i] = (arr[i] >> right) | overflow;
            overflow = overflown;
        }
        return Ret;
    }
    BigInterger &operator>>=(int right)
    {
        unsigned __int64 overflow = 0;
        for (int i = 0; i < ArryLen; i++)
        {
            unsigned __int64 overflown = arr[i] << (64 - right);
            arr[i] = (arr[i] >> right) | overflow;
            overflow = overflown;
        }
        return *this;
    }

    BigInterger &operator<<=(int right)
    {
        unsigned __int64 overflow = 0;
        for (int i = ArryMax; i >= 0; i--)
        {
            unsigned __int64 overflown = arr[i] >> (64 - right);
            arr[i] = (arr[i] << right) | overflow;
            overflow = overflown;
        }
        return *this;
    }
    __int64 operator&(__int64 right)
    {
        return arr[ArryMax] & right;
    }
    BigInterger operator*(BigInterger right) const
    {
        BigInterger Ret;
        BigInterger tmp2 = *this;
        int Max = 2048;
        for (int i = 0; i < ArryLen && right.arr[i] == 0; i++)
        {
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

    bool operator==(const BigInterger &right) const
    {
        for (int i = 0; i < ArryLen; i++)
        {
            if (arr[i] != right.arr[i])
                return false;
        }
        return true;
    }

    BigInterger operator%(BigInterger right) const
    {
        BigInterger Ret;
        BigInterger tmp2 = *this;
        BigInterger tmp(1);
        int Max = 0;
        for (int i = 0; !(right.arr[0] & 0x800000000000000) && (tmp2 > right); i++)
        {
            Max += 1;
            right <<= 1;
            tmp <<= 1;
        }
        for (int i = 0; i <= Max; i++)
        {
            if (!(right > tmp2))
            {
                tmp2 -= right;
                Ret += tmp;
            }
            tmp >>= 1;
            right >>= 1;
        }
        return tmp2;
    }
    BigInterger &operator*=(BigInterger right)
    {

        BigInterger tmp2 = *this;
        *this = 0;
        int Max = 2048;
        for (int i = 0; i < ArryLen && right.arr[i] == 0; i++)
        {
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
    BigInterger operator/(BigInterger right) const
    {
        if (right == 0)
        {
            throw string("devide by 0");
        }
        BigInterger Ret;
        BigInterger tmp2 = *this;
        BigInterger tmp(1);

        int Max = 0;
        for (int i = 0; !(right.arr[0] & 0x800000000000000) && (tmp2 > right); i++)
        {
            Max += 1;
            right <<= 1;
            tmp <<= 1;
        }
        for (int i = 0; i <= Max; i++)
        {
            if (!(right > tmp2))
            {
                tmp2 -= right;
                Ret += tmp;
            }
            tmp >>= 1;
            right >>= 1;
        }
        return Ret;
    }
    BigInterger &operator/=(BigInterger right)
    {
        if (right == 0)
        {
            throw string("devide by 0");
        }
        BigInterger Ret;
        BigInterger tmp(1);
        int Max = 0;
        for (int i = 0; !(right.arr[0] & 0x800000000000000) && (*this > right); i++)
        {
            Max += 1;
            right <<= 1;
            tmp <<= 1;
        }
        for (int i = 0; i <= Max; i++)
        {
            if (!(right > *this))
            {
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
istream &operator>>(istream &in, BigInterger &obj)
{
    string t;
    in >> t;
    if (t.compare(0, 2, "0x") == 0)
    {
        int L = t.length() - 2;
        if (L > 0 && t[0] == '0' && t[1] == 'x')
        {
            t = t.substr(2);
            int i = ArryMax;
            while (L > 0)
            {
                if (L > 16)
                    obj.arr[i--] = std::stoull(t.substr(L - 16, 16), 0, 16);
                else
                    obj.arr[i--] = std::stoull(t.substr(0, L), 0, 16);
                L -= 16;
            }
        }
    }
    else
    {
        int L = t.length();

        while (L > 0)
        {
            obj *= 10000000000000000000ULL;
            if (L > 19)
            {
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
ostream &operator<<(ostream &out, const BigInterger &obj)
{
    if (out.flags() & out.hex)
    {
        int i = 0;
        for (; i < ArryMax && !obj.arr[i]; i++)
            ;
        out << "0x";
        out << obj.arr[i++];
        for (; i < ArryLen; i++)
        {

            cout.fill('0');
            cout.width(16);
            out << obj.arr[i];
        }
    }
    else
    {
        BigInterger tmp(obj);
        stack<unsigned long long> st;
        while (tmp > 10000000000000000000ULL)
        {
            st.push((tmp % 10000000000000000000ULL).arr[ArryMax]);
            tmp /= 10000000000000000000ULL;
        }
        cout << tmp.arr[ArryMax];

        while (!st.empty())
        {
            cout.fill('0');
            cout.width(19);
            out << st.top();
            st.pop();
        }
    }

    return out;
}

struct Res
{
    bool foundhead;
    bool foundtail;
    unsigned long long requst;
    unsigned long long reply;
    Res()
    {
        foundhead = false;
        foundtail = false;
        requst = 0;
        reply = 0;
    }
};
char  ui[]={'-','\\','/'};
unordered_map<unsigned int, Res> ordermap;
map<unsigned long long, pair<unsigned int,BigInterger>> timemap;
int main(int argc, char **argv)
{
    BigInterger totalTime = 0;
    unsigned int suc = 0;
    unsigned int fail = 0;
    fstream filein(argv[1], ios::in);
    string res;
    unsigned long long timestamp;
    unsigned int orderid;
    string account;
    string comment;
    int display=0;
    cout<<"*";
    while (filein)
    {
        while(display++%1000==0){
            cout<<"\b";
            cout<<ui[display%3];
            cout.flush();
        }
        int first = filein.get();
        if (first != '[')
        {
            getline(filein, res);
            continue;
        }
        getline(filein, res, ']');
        filein >> timestamp;
        filein >> orderid;
        filein >> account;
        getline(filein, comment);
        if (res == "SEND")
        {
            Res &r = ordermap[orderid];
            r.requst = timestamp;
            r.foundhead = true;
            r.foundtail=false;
        }
        else if (res == "SUCC")
        {
            Res &r = ordermap[orderid];
            r.foundtail = true;
            r.reply = timestamp;
            if (!r.foundhead)
            {
                cout << "what the fuck " <<orderid<< endl;
            }
            auto timeval = r.reply - r.requst;
            totalTime += timeval;
            auto& perMin=timemap[r.requst/60000000000];
            perMin.second+=timeval;
            perMin.first++;
            suc++;
        }
        else if (res == "FAIL")
        {
            Res &r = ordermap[orderid];
            r.foundtail = true;
            r.reply = timestamp;
            if (!r.foundhead)
            {
                cout << "what the fuck " <<orderid<< endl;
            }
            auto timeval = r.reply - r.requst;
            totalTime += timeval;
            auto& perMin=timemap[r.requst/60000000000];
            perMin.second+=timeval;
            perMin.first++;
            fail++;
        }
    }
    cout <<"\b";
    cout <<"SECC\t"<< suc << endl;
    cout <<"FAIL\t"<< fail << endl;
    cout <<"AVG RSP TIME\t"<< (totalTime/(suc+fail)) << endl;
    cout <<"TIMESTAMP\tAVERAGE RESPONSE TIME"<< endl;
    for(auto &i : timemap){
        cout<<i.first*60<<"\t"<<(i.second.second/i.second.first)<<endl;
    }

    return 0;
}