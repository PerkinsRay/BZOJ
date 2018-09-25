struct timeval_t
{
    int32_t sec;
    unsigned int usec : 20;
    unsigned int nsec : 10;
    unsigned int unused : 2;
};

enum mtype_t
{
    MSG_NONE = 0,
    MSG_ALL = 1,
    MSG_TEST = 2,                          // TestMsg
    MSG_CONNECT = 3,                       // ConnectMsg
    MSG_CLIENT_IDENTIFY = 4,               // ClientIdentifyMsg
    MSG_TIMEOUT = 5,                       // TimeoutMsg
    MSG_TEST4 = 16,                        // Test4Msg
    MSG_TEST2 = 17,                        // Test2Msg
    MSG_CLIENT_ACCEPT = 19,                // ClientAcceptMsg
    MSG_CLIENT_DISCONNECT = 49,            // ClientDisconnectMsg
    MSG_TD_ORDER = 50,                     // TdOrderMsg
    MSG_TD_CANCEL = 51,                    // TdCancelMsg
    MSG_TD_ORDER_FUTURE = 52,              // TdOrderFutureMsg
    MSG_TD_CANCEL_REJECT = 57,             // TdCancelRejectMsg
    MSG_TD_FILL = 58,                      // TdFillMsg
    MSG_TD_FILL_FUTURE = 59,               // TdFillFutureMsg
    MSG_TD_STATUS_ORDER = 182,             // TdStatusOrderMsg
    MSG_TD_STATUS_CHANGE = 183,            // TdStatusChangeMsg
    MSG_TD_ORDER_NEW = 184,                // TdOrderNewMsg
    MSG_TD_FILL_NEW = 185,                 // TdFillNewMsg
    MSG_TIMESTAMP = 226,                   // TimestampMsg
    MSG_QD_SUBSCRIBE = 272,                // QdSubscribeMsg
    MSG_QD_UNSUBSCRIBE = 273,              // QdUnsubscribeMsg
    MSG_SHFE_QD = 300,                     // ShfeQdMsg   -- the same for MSG_DCE_L1
    MSG_TD_POSITION_OFFSET = 301,          // TdPositionOffsetMsg
    MSG_TD_POSITION_OFFSET_FRAME = 302,    // TdPositionOffsetFrameMsg
    MSG_TD_POSITION_OFFSET_REQUEST = 304,  // TdPositionOffsetRequestMsg
    MSG_SHFE_DEPTH_UPDATE = 305,           // ShfeDepthUpdateMsg
    MSG_CFFEX_QD = 807,                    // CffexQdMsg
    MSG_CFFEX_INFO_ORDER = 808,            // CffexInfoOrderMsg
    MSG_CFFEX_INFO_TRADE = 809,            // CffexInfoTradeMsg
    MSG_WIND_FUTURES_MARKET_DATA = 813,    // WindFuturesMarketDataMsg
    MSG_DCE_L2_BEST_AND_DEEP = 833,        // DceL2BestAndDeepMsg
    MSG_DCE_L2_TEN_ENTRUST = 834,          // DceL2TenEntrustMsg
    MSG_DCE_L2_STATS = 835,                // DceL2StatsMsg
    MSG_DCE_L2_REALTIME_PRICE = 836,       // DceL2RealtimePriceMsg
    MSG_DCE_L2_MATCH_PRICE_QTY = 837,      // DceL2MatchPriceQtyMsg
    MSG_SHFE_DEPTH_REFRESH_COMPLETE = 892, // ShfeDepthRefreshCompleteMsg
    MSG_SHFE_DEPTH_ACTIVITY_UPDATE = 909,  // ShfeDepthActivityUpdateMsg
    MSG_SHFE_DEPTH_SNAPSHOT = 950,         //ShfeDepthSnapshotMsg

    MSG_TD_CLIENT_TRADE_INFO_REGISTER = 2014, // TdClientTradeInfoRegisterMsg
    MAX_VALID_MTYPE_LEN
};
enum mktdest_t
{
    MKT_UNKNOWN = 0,
    MKT_SHFE = 24,
    MKT_CZCE = 27,
    MKT_DCE = 28,
    MKT_CFFEX = 67,
    MKT_SHSE = 80,
    MKT_SZSE = 81,
    MKT_INE = 93,
    MKT_MAX
};

enum product_t
{
    PROD_STOCK = 0,
    PROD_FUTURE = 1,
    PROD_OPTION = 2,
    PROD_UNKNOWN = 9,
    PROD_SYM_ONLY = 15
};
enum right_t
{
    RT_PUT = 0,
    RT_CALL = 1,
    RT_UNKNOWN = 2,
    RT_NONE = 3,
};

union currency_t {
    int32_t IntVal;
    char str[4];
    constexpr currency_t(const char *s) : IntVal(s[0] + (1 << 8) * s[1] + (1 << 16) * s[2] + (1 << 24) * s[3])
    {
    }
    currency_t()
    {
        IntVal = 0xFFFFFFFF;
    }
    currency_t(int32_t val)
    {
        IntVal = val;
    }
};
static const constexpr currency_t CNY = currency_t("CNY");
struct instrument_t
{
    uint32_t sym;
    mktdest_t mkt;
    product_t prod;
    int32_t exp;   // only valid for futures and options
    float strike;  // only valid for options
    right_t right; // only valid for options
    currency_t currency;
    int32_t pad[5];
    instrument_t(uint32_t sym_, mktdest_t mkt_, product_t prod_, int32_t exp_ = 0, float strike_ = 0,right_t rt_ = RT_NONE, currency_t curr_ = CNY): sym(sym_), mkt(mkt_), prod(prod_), exp(exp_), strike(strike_), right(rt_), currency(curr_)
    {
        memset(pad, 0, sizeof pad);
    }
    instrument_t(){
        memset(this, 0, sizeof (instrument_t));
    }
};

struct source_t
{
    char type;        ///  0- 7 (SRC_ISLD, SRC_ARCA, etc.)
    uint8_t reserved; ///  7-15 (0)
    char orig;        /// 16-23 (ORIG_CARTERET, ORIG_SPIKE, etc.)
    char dest;        /// 22-31 (DEST_CARTERET, DEST_SPIKE, etc.)
};
struct message_hdr_t
{
    static const uint32_t kEndianSentinelValue = 0x01020304;

    source_t source;
    uint32_t symbol;
    mtype_t mtype;
    uint32_t unused;     // was "sig" before; seems that was always zero, this should be too
    timeval_t time_sent; // time the message was produced by a BB program (not exchange time)
    uint32_t size;       // size of data portion (not including header)
    uint32_t seq;        // for gap detection (not always used)
    uint32_t endian;     // store 0x01020304 so that we can determine endian as well as guard against null headers
    uint32_t modifier;   // bitwise combination of the MOD_* enum values (usually zero)
};
struct msg_shfe_qd : public message_hdr_t
{
    instrument_t instr;
    timeval_t exchange_time;
    int32_t update_index;
    int32_t total_volume;
    double bid;
    double ask;
    int32_t bid_size;
    int32_t ask_size;
    double last_trade_price;
    double open_price;
    double high_price;
    double low_price;
    double prev_settlement_price;
    double limit_up;
    double limit_down;
    double prev_open_interest;
    int32_t open_interest;
    char unused1[4];
    double turnover;
    char unused2[8];
    char instr_str[32];
    double prev_close_price;
    double close_price;
    double settlement_price;
    double pre_delta;
    double current_delta;
    timeval_t action_day;
    timeval_t nic_timestamp;
    timeval_t qd_time_sent;
    timeval_t arb_nic_timestamp;
};