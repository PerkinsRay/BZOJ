
#ifndef ASIAFUTFLOATPNLTRACKER_H
#define ASIAFUTFLOATPNLTRACKER_H

#include <bb/core/hash_map.h>
#include <bb/core/compat.h>
#include <bb/core/Log.h>
#include <bb/core/acct.h>
#include <bb/core/instrument.h>
#include <bb/core/side.h>
#include <stdint.h>
#include <bb/servers/tdcore/TdRiskControl.h>
#include <jd/tdcore/PositionOffsetFlag.h>
#include <unordered_map>

template <class PriceListener>
class AsiaFutFloatPNLTracker
{
    PriceListener m_priceListener;
    struct CacheEntry
    {
        CacheEntry() : m_cost(0), m_position(0)
        {
        }
        double m_cost;
        int m_position;
    };

    struct CacheEntrys
    {
        CacheEntrys()
        {
        }
        CacheEntry m_entry[2];
        CacheEntry &operator[](bb::side_t side)
        {
            return m_entry[reinterpret_cast<int>(side)];
        }
    };
    std::unordered_map<bb::instrument_t, CacheEntrys> InstrCache;

  public:
    void fill(const bb::instrument_t &instr, double px, int32_t sz, bb::side_t side, bool isClosingOrder)
    {
        CacheEntry &cache = InstrCache[instr][side];
        if (isClosingOrder)
        {
            if (cache.m_position == 0)
            {
                cache.m_cost = 0;
            }
            else
            {
                int newPos = cache.m_position - sz;
                if (newPos < 0)
                {
                    newPos = 0;
                }
                cache.m_cost = cache.m_cost * newPos / cache.m_position;
                cache.m_position = newPos;
            }
        }
        else
        {
            cache.m_cost += sz * px;
            cache.m_position += sz;
        }
    }
    double getFloatPNL()
    {
        double pnl = 0;
        for (auto &KeyVal : InstrCache)
        {
            double px = m_priceListener.getPrice(KeyVal.first);
            auto &cachebid = KeyVal.second[bb::side_t::BID];
            auto &cacheask = KeyVal.second[bb::side_t::ASK];
            pnl += (px * cachebid.m_position - cachebid.m_cost);
            pnl -= (px * cacheask.m_position - cacheask.m_cost);
        }
        return pnl;
    }
};

#endif