#include "FtdcMduserApi.h"
#include <signal.h>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include "Msgs.h"
#include <condition_variable>
static unsigned int symmap[26][26] = {
    {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 21067 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 21002 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 21001 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 21077 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 21003 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {},
    {},
    {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 21004 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {},
    {0 /*A*/, 0 /*B*/, 34293 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 0 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {0 /*A*/, 21038 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 0 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {},
    {0 /*A*/, 21021 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 21005 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {0 /*A*/, 0 /*B*/, 61163 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 34608 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 0 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {},
    {},
    {},
    {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 0 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 21020 /*R*/, 0 /*S*/, 0 /*T*/, 0 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
    {},
    {},
    {0 /*A*/, 0 /*B*/, 0 /*C*/, 0 /*D*/, 0 /*E*/, 0 /*F*/, 0 /*G*/, 0 /*H*/, 0 /*I*/, 0 /*J*/, 0 /*K*/, 0 /*L*/, 0 /*M*/, 21006 /*N*/, 0 /*O*/, 0 /*P*/, 0 /*Q*/, 0 /*R*/, 0 /*S*/, 0 /*T*/, 0 /*U*/, 0 /*V*/, 0 /*W*/, 0 /*X*/, 0 /*Y*/, 0 /*Z*/},
};
template<product_t Prod=PROD_FUTURE>
class ParseInstr
{
  public:
    static instrument_t parse(char *strInstr, mktdest_t mkt)
    {
        instrument_t instr;
        if (strlen(strInstr) == 6)
        {
            return instrument_t( symmap[strInstr[0] - 'a'][strInstr[1] - 'a'], mkt, Prod, atoi(strInstr + 2) + 200000, 0, RT_NONE, CNY );
        }
        return instrument_t();
    }
};
class ParseTime
{
  public:
    static timeval_t parse(char *date, char *time, unsigned int milisec)
    {
        char buf[20] = {0};
        buf[8] = ' ';
        memcpy(buf, date, 8);
        memcpy(buf + 9, time, 8);
        tm tm_;
        time_t t_;
        strptime(buf, "%Y%m%d %H:%M:%S", &tm_); //将字符串转换为tm时间
        tm_.tm_isdst = -1;
        t_ = mktime(&tm_);
        return timeval_t{(int32_t)t_, milisec * 1000};
    }
    static timeval_t now()
    {
        unsigned long long count = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        timeval_t t;
        t.nsec = count % 1000;
        count /= 1000;
        t.usec = count % 1000000;
        count /= 1000000;
        t.sec = count;
        return t;
    }
};
class Multicast
{
    int fd;

  public:
    Multicast() : fd(0)
    {
    }
    bool Init(const std::string &strAddr, short port)
    {
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in addr;
        if (fd > 0)
        {
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(strAddr.c_str());
            addr.sin_port = htons(port);
            if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
            {
                return true;
            }
            else
            {
                close(fd);
                fd = 0;
            }
        }
        else
        {
            fd = 0;
        }
        return false;
    }
    int Send(const void *buf, size_t size)
    {
        return send(fd, buf, size, 0);
    }
    ~Multicast()
    {
        if (fd > 0)
        {
            close(fd);
        }
    }
};
class ShfeQD
{
    class Receiver : public CShfeFtdcMduserSpi
    {
        ShfeQD &m_pQD;
        CShfeFtdcReqUserLoginField m_reqUserLogin;
        msg_shfe_qd msg;
        Multicast mc;
        mktdest_t m_mkt;

      public:
        Receiver(ShfeQD &pQD,const std::string &oaddr, const std::string &broker_id, const std::string &username, const std::string &password, mktdest_t mkt) : m_pQD(pQD), m_mkt(mkt)
        {
            strcpy(m_reqUserLogin.ParticipantID, broker_id.c_str());
            strcpy(m_reqUserLogin.UserID, username.c_str());
            strcpy(m_reqUserLogin.Password, password.c_str());
            mc.Init(oaddr.substr(0,oaddr.find(':')), std::stoi(oaddr.substr(oaddr.find(':')+1)));
            memset(&msg, 0, sizeof(msg));
        }
        virtual void OnFrontConnected()
        {
            std::cout << m_pQD.m_addr << " - connected." << std::endl;
            strcpy(m_reqUserLogin.TradingDay, m_pQD.md_api->GetTradingDay());
            m_pQD.md_api->ReqUserLogin(&m_reqUserLogin, 0);
        }
        virtual void OnRspUserLogin(CShfeFtdcRspUserLoginField *pRspUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
        {
            if (pRspInfo->ErrorID != 0)
            {
                std::cout << m_pQD.m_addr << " - login failed." << std::endl;
            }
            else
            {
                std::cout << m_pQD.m_addr << " - login successed." << std::endl;
            }
        }
        virtual void OnRtnDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData)
        {
            msg.size = sizeof(msg_shfe_qd) - sizeof(message_hdr_t);
            msg.source.type = 101;
            msg.source.orig = msg.source.dest = 0xFF;
            msg.time_sent = ParseTime::now();
            msg.endian = message_hdr_t::kEndianSentinelValue;
            msg.mtype = MSG_SHFE_QD;
            msg.instr = ParseInstr<PROD_FUTURE>::parse(pDepthMarketData->InstrumentID, m_mkt);
            msg.symbol = msg.instr.sym;
            msg.exchange_time = ParseTime::parse(pDepthMarketData->ActionDay, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec);
            msg.total_volume = pDepthMarketData->Volume;
            msg.bid = pDepthMarketData->BidPrice1;
            msg.bid_size = pDepthMarketData->BidVolume1;
            msg.ask = pDepthMarketData->AskPrice1;
            msg.ask_size = pDepthMarketData->AskVolume1;
            msg.last_trade_price = pDepthMarketData->LastPrice;
            msg.prev_settlement_price = pDepthMarketData->PreSettlementPrice;
            msg.limit_up = pDepthMarketData->UpperLimitPrice;
            msg.limit_down = pDepthMarketData->LowerLimitPrice;
            msg.open_price = pDepthMarketData->OpenPrice;
            msg.high_price = pDepthMarketData->HighestPrice;
            msg.low_price = pDepthMarketData->LowestPrice;
            msg.prev_open_interest = pDepthMarketData->PreOpenInterest;
            msg.open_interest = pDepthMarketData->OpenInterest;
            msg.turnover = pDepthMarketData->Turnover;
            memcpy(msg.instr_str, pDepthMarketData->InstrumentID, 31);
            mc.Send(&msg, sizeof(msg));
        }
    };
    CShfeFtdcMduserApi *md_api;
    Receiver recver;
    int m_topic;
    std::string m_addr;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    friend class Receiver;

  public:
    enum ERROR_CODE
    {
        NoError,
        ErrorQDNotRunning,
        ErrorQDHasStarted,
        ErrorAPIInitFailed
    };
    ShfeQD(const std::string &iaddr,const std::string &oaddr, const std::string &broker_id, const std::string &username, const std::string &password, int topic = 1001) : m_topic(topic), m_addr(iaddr), md_api(nullptr), recver(*this,oaddr, broker_id, username, password, topic == 1001 ? MKT_SHFE : MKT_INE)
    {
    }
    ERROR_CODE run()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!md_api)
        {
            md_api = CShfeFtdcMduserApi::CreateFtdcMduserApi();
            if (md_api)
            {
                md_api->SetHeartbeatTimeout(10);
                md_api->RegisterSpi(&recver);
                md_api->SubscribeMarketDataTopic(m_topic, TERT_QUICK);
                md_api->RegisterFront((char *)m_addr.c_str());
                md_api->SetHeartbeatTimeout(10);
                md_api->Init();
                m_cv.wait(lock);
                return ERROR_CODE::NoError;
            }
            return ERROR_CODE::ErrorAPIInitFailed;
        }
        return ERROR_CODE::ErrorQDHasStarted;
    }
    ERROR_CODE stop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (md_api)
        {
            m_cv.notify_one();
            md_api->Release();
            md_api = nullptr;
            return ERROR_CODE::NoError;
        }
        return ERROR_CODE::ErrorQDNotRunning;
    }
    ~ShfeQD()
    {
        stop();
    }
};
ShfeQD *g_qd = nullptr;
void signal_handler(int sig)
{
    if (g_qd)
    {
        g_qd->stop();
    }
}
int main(int argc, char **argv)
{
    std::string input_address;
    std::string broker_id;
    std::string username;
    std::string password;
    std::string output_address;
    int topic_id;
    namespace po = boost::program_options;
    po::options_description desc(std::string("shfeqd") + std::string(" options"));
    desc.add_options()
    ("help", "produce help message")
    ("inaddr,i", po::value<std::string>(&input_address)->default_value("tcp://116.228.53.154:3202"), "input address")
    ("brokerid,b", po::value<std::string>(&broker_id)->default_value("0037"), "Broker ID")
    ("username,u", po::value<std::string>(&username)->default_value("0037c6c"), "Account username")
    ("password,p", po::value<std::string>(&password)->default_value("123456"), "Account password")
    ("topicid,t", po::value<int32_t>(&topic_id)->default_value(1001), "topic id")
    ("output,o", po::value<std::string>( &output_address )->default_value("224.0.0.99:7777"), "output mcast address");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    ShfeQD qd(input_address,output_address, broker_id, username, password, topic_id);
    g_qd = &qd;
    qd.run();
    return 0;
}