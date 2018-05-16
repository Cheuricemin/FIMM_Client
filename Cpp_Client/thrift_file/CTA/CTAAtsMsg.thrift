include "AtsType.thrift"
include "AtsGeneral.thrift"

namespace csharp CTAMsg
namespace cpp CTAMsg

enum TradingType{
    Unknown,
    Manual,
    Hitter,
    Contrib,
    Spread,
    ContribHedge,
    Max
}

struct MarketMakerMsg{
	1:bool Active,
	2:i32 MMLots,
	3:double MMValo,
	4:i32 TotalMMBidQty,
	5:i32 MMBidQty,
	6:i32 MMAskQty,
	7:i32 TotalMMAskQty,
	8:double FairPrice,
	9:i32 Position,
	10:double ConvergeRate,
	11:double MovingAvgRatio,
	12:double RiskReward,
	13:double MinGain,
	14:i32 MinDepth,
	15:i32 MaxDepth,
	16:i32 MaxTickCounter,
	17:double Aggresivity,
	18:double HitterAgg,
	19:i32 EntrySize,
	20:i32 MinSizeInc,
	21:i32 MinSizeDec,
	22:i32 MaxSize,
	23:i32 MaxNbOrder,
	24:i32 Residu,
	25:double MovingAvg,
	26:double MovingVolume,
	27:double TradingImpact2Pos,
	28:double TradingImpactPosLimit,
	29:double TradingImpact
}


struct IAFutureMsg{
	1:AtsGeneral.AtsInstrumentMsg AtsInstrument,
	2:string AtsName,
	3:bool isTrdFuture,
	4:bool isFuture1,
	5:bool isFuture2,
	6:string Maturity, 
	7:MarketMakerMsg MarketMaker
}



struct AtsFutureSpreadMsg{
	1:string Ats,
	2:double FairSpread,
	3:double PosShift,
	4:double AdjustSpread,
	5:double Ratio,
	6:double MinGainHit,
	7:double MinGainCon,
	8:double MinGainConExit,
	9:i32 MinInterval,
	10:i32 HitSize,
	11:i32 ConSize,
	12:double ConMinDiff,
	13:double ConMaxDistance,
	14:bool HitStatus,
	15:bool ConStatus,
	16:bool ConBuy,
	17:bool ConSell,
	18:i32 MaxWaitInterval,
	19:i32 SpreadPos,
	20:i32 Future1Pos,
	21:i32 Future2Pos,
	22:double BidHit,
	23:double AskHit,
	24:double BidCon,
	25:double AskCon,
	26:string Fut1Code,
	27:string Fut2Code,
	28:i32 MaxNumOrders,
	29:i32 MaxPos,
	30:bool UseMarketOrder,
	31:double PosConvex,
	32:double ConvergeRate,
	33:string LastKLineTime,
	34:string KInterval,
	35:bool IgnoreUpDnLimit,
	36:double Speed,
	37:double SpeedConvergeRate,
	38:double LastSpread,
	39:double Volatility,
	40:double MaxVolRange,
	41:double RiskDegree,
	42:i32 Fut2Cancel,
	43:double FastConvergeRate
	
}

struct KlineConfigMsg
{
	1:i32 KlineNum,
	2:i32 KlineDur
}

struct TWAPDataMsg
{
	1:i32 Time,
	2:double Alpha,
	3:i32 Max_Order_Size,
	4:i32 Min_Order_Size,
	5:i32 Sleep_Time
}

struct KlineDataMsg
{
	1:string Time,
	2:double Open,
	3:double High,
	4:double Low,
	5:double Close,
	6:double TR,
	7:i32 Signal
}

struct TurtleAlgoMsg
{
	1:string LastKLineTime,
	2:string KInterval,
	3:list<KlineDataMsg> PriceDatas,
	4:KlineDataMsg CurrentPrice,
	5:i32 NumOpenKline,
	6:i32 NumCloseKline,
	7:double MA_Fast,
	8:double MA_Slow,
	9:double Lambda_Fast,
	10:double Lambda_Slow,
	11:i32 Signal,
	12:double ATR,
	13:double PosRatio,
	14:double LastUpdatePrice,
	15:i32 MaxPosLimit,
	16:bool UpdateKline,
	17:bool EnableTurtle,
	18:string Instr_Code
}

struct CTAAtsMsg{
	1:AtsGeneral.AtsMsg Ats,
	2:list<IAFutureMsg> AllFutures,
	3:bool KeepOrders,
	4:bool ForceOpen,
	5:AtsFutureSpreadMsg AtsSpread,
	6:list<string> AtsIntraDataCols,
	7:string FeedsourcesStr,
	8:string ConnectionsStr,
	9:list<AtsGeneral.UnderlyingMsg> Underlyings,
	10:bool DoFutureMarketMaker,
	11:bool MarketMakerCanReduce,
	12:TurtleAlgoMsg Turtle

}

service CTAOperation extends AtsGeneral.AtsOperation{
	bool SetFutureSpreadHitStatus(1:string atsName,2:bool status),
	bool SetFutureSpreadConStatus(1:string atsName,2:bool status),
	bool SetFutureSpreadConBuy(1:string atsName,2:bool status),
	bool SetFutureSpreadConSell(1:string atsName,2:bool status),
	double SetFutureSpreadFairSpread(1:string atsName,2:double fairspread),
	double SetFutureSpreadPosShift(1:string atsName,2:double posShift),
	double SetFutureSpreadMinGainCon(1:string atsName,2:double minGainCon),
	double SetFutureSpreadMinGainHit(1:string atsName,2:double minGainHit),
	i32 SetFutureSpreadMinInterval(1:string atsName,2:i32 MinInterval),
	i32 SetFutureSpreadMaxWaitInterval(1:string atsName,2:i32 MaxWaitInterval),
	i32 SetFutureSpreadHitSize(1:string atsName,2:i32 HitSize),
	i32 SetFutureSpreadConSize(1:string atsName,2:i32 ConSize),
	double SetFutureSpreadConMinDiff(1:string atsName,2:double ConMinDiff),
	double SetFutureSpreadMinGainConExit(1:string atsName,2:double MinGainConExit),
	double SetFutureSpreadConMaxDistance(1:string atsName,2:double ConMaxDistance),
	i32 SetFutureSpreadFuture1Pos(1:string atsName,2:i32 Future1Pos),
	i32 SetFutureSpreadFuture2Pos(1:string atsName,2:i32 Future2Pos),
	i32 SetFutureSpreadMaxNumOrders(1:string atsName,2:i32 MaxNumOrders),
	i32 SetFutureSpreadMaxPos(1:string atsName,2:i32 MaxPos),
	bool SetFutureSpreadUseMarketOrder(1:string atsName,2:bool useMarketORder),
	double SetFutureSpreadPosConvex(1:string atsName,2:double posConvex),
	bool SetFutureSpreadIgnoreUpDnLimit(1:string atsName,2:bool IgnoreUpDnLimit),
	double SetFutureSpreadConvergeRate(1:string atsName,2:double ConvergeRate),
	double SetFutureSpreadFastConvergeRate(1:string atsName,2:double FastConvergeRate),
	double SetFutureSpreadRatio(1:string atsName,2:double Ratio),
	string SetAtsFutureSpreadFut1(1:string atsName,2:string code),
	string SetAtsFutureSpreadFut2(1:string atsName,2:string code),
	string SetAtsFutureSpreadLastKLineTime(1:string atsName,2:string KLineTime),
	string SetAtsFutureSpreadKInterval(1:string atsName,2:string KInterval),
	CTAAtsMsg CreateAutomaton(1:string automatonName, 2:string underlyingName, 3:string subscrib_list, 4:string feedsourcesStr, 5:string connectionsStr),
	void SpreadBidHitTrade(1:string atsName),
	void SpreadAskHitTrade(1:string atsName),
	void SpreadBidConTrade(1:string atsName),
	void SpreadAskConTrade(1:string atsName),
	bool SetKeepOrders(1:string atsName, 2:bool keepOrders),
	bool SetForceOpen(1:string atsName, 2:bool forceOpen),
	
	double SetFutureSpeedConvergeRate(1:string atsName,2:double SpeedConvergeRate),
	double SetFutureSpeed(1:string atsName,2:double Speed),
	
	double SetFutureLastSpread(1:string atsName,2:double LastSpread),
	double SetVolatility(1:string atsName,2:double Vol),
	double SetMaxVolRange(1:string atsName,2:double MaxVolRange),
	
	bool SetMarketMakerActive(1:string atsName,2:string code, 3:bool active),
	double SetMarketMakerDouble(1:string atsName,2:string code,3:string param, 4:double value),
	i32 SetMarketMakerInt(1:string atsName,2:string code,3:string param, 4:i32 value),
	bool SetDoFutureMarketMaker(1:string atsName, 2:bool value),
	bool SetMarketMakerCanReduce(1:string atsName, 2:bool value),
	void ForceContribe(1:string atsName),
	void ResetMovingAvg(1:string atsName),
	
	bool SetTwapData(1:string atsName,2:TWAPDataMsg data),
	bool SetTurtleAlgoData(1:string atsName,2:TurtleAlgoMsg data),
	bool SetKlineConfig(1:string atsName,2:KlineConfigMsg data),
	string SetTurtleFuture(1:string atsName,2:string code),
	
	double SetRiskDegree(1:string atsName,2:double RiskDegree),
	void MatchOrders()
}