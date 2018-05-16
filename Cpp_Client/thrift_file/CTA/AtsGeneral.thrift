include "AtsType.thrift"

namespace csharp AtsGeneral
namespace cpp AtsGeneral

struct FeedSourceMsg{
	1:string Name,
	2:AtsType.FeedSourceStatus Status,
	3:bool Activated
}

struct ConnectionMsg{
	1:string Name,
	2:AtsType.ConnectionStatus Status,
	3:bool TradingAllowed,
	4:i32 New,
	5:i32 Mod,
	6:i32 Can,
	7:i32 Ack,
	8:i32 Rej,
	9:i32 NAck,
	10:i32 Exe
	
}

struct LogMsg{
	1:string TimeStamp,
	2:string Message
}

struct OrderMsg{
	1: string Code,
	2: i32 Id,
	3: string TimeStamp,
	4: bool Active,
	5: AtsType.OrderWay Way,
	6: AtsType.OrderAction Action,
	7: i32 TradingType,
	8: i32 Quantity,
	9: double Price,
	10: i32 ExecQty,
	11: double ExecPrice,
	12: i32 BookQty,
	13: AtsType.OrderStatus Status,
	14: string Portfolio,
	15: string LastReason,
	16:string Ack_Cancel_Ratio,
	17:AtsType.OrderOpenClose OpenClose
}

struct ExecMsg{
	1:string Code,
	2:i32 Id,
	3:AtsType.OrderWay Way,
	4:i32 Quantity,
	5:double Price,
	6:string Reference,
	7:string Time,
	8:i32 TradingType,
	9:string Portfolio,
	10:string OpenClose
}

struct PositionMsg{
	1:string Instrument,
	2:string Portfolio,
	3:i32 YesterdayPosition,
	4:i32 TodayPosition,
	5:i32 TotalPosition,
	6:i32 TodayBuyPosition,
	7:double TodayBuyPrice,
	8:i32 TodaySellPosition,
	9:double TodaySellPrice,
	10:double YesterdayPrice,
	11:double YesterdayPriceLocal,
	12:double YesterdayPriceExternal,
	13:i32 YesterdayPositionLocal,
	14:i32 YesterdayPositionManual,
	15:i32 YesterdayPositionExternal,
	16:bool UseManualPosition,
	17:AtsType.YesterdayPositionType YstPositionType,
	18:AtsType.YesterdayPriceType YstPriceType,
	19:string Connection,
	20:i32 TodayPurPosition,
	21:i32 TodayRedPosition
	
}

struct FeedMsg{
	1:string Code,
	2:string FeedSourceName,
	3:double Bid,
	4:double Ask,
	5:i32 BidQuantity,
	6:i32 AskQuantity,
	7:double Last,
	8:i32 LastQuantity,
	9:double LastOrClose,
	10:double Mid,
	11:double Close,
	12:double Settlement,
	13:optional double UpperLimit,
	14:optional double LowerLimit,
	15:double Perf,
	16:i64 DailyVolume,
	17:bool isBiddAskActive,
	18:bool isSuspended,
	19:i32 MaxDepth,
	20:optional list<double> BidDepths,
	21:optional list<double> AskDepths,
	22:optional list<i32> BidQtys,
	23:optional list<i32> AskQtys,
	24:string MarketTime
}


struct AtsInstrumentMsg{
	1:string Code,
	2:PositionMsg Position,
	3:FeedMsg FeedItem,
	4:map<i32,map<i32,OrderMsg>>  OrderContainer,
	5:double ExchangeFees,
	6:double BrokerFees,
	7:double YesterdayPnlBarycenter,
	8:double TodayPnlBarycenter
	9:i32 SizeToSend,
	10:map<double,i32> MarketMakerItemBid,
	11:map<double,i32> MarketMakerItemAsk,
	12:string InstrumentClassName
}

struct AtsMsg{
	1:string Name,
	2:bool AutoStatus,
	3:AtsType.YesterdayPositionType YesterdayPositionType,
	4:double YesterdayPnlBary,
	5:double TodayPnlBary,
	6:double YesterdayPnlMid,
	7:double TodayPnlMid,
	8:double YesterdayPnlLast,
	9:double TodayPnlLast,
	10:double FeesExchange,
	11:double FeesBroker,
	12:double TotalPnl,
}

struct FeesStructMsg{
	1:string ClassName,
	2:double FeesFloatExchange,
    3:double FeesFloatBroker,
    4:double FeesFixExchange,
    5:double FeesFixBroker,
    6:double FeesSellAmount,
	7:bool NotCloseToday
}

struct InstrumentClassMsg{
	1:string Name,
	2:FeesStructMsg FeesStruct,
	3:optional string MaturityTime,
	4:optional list<MaturityMsg> Maturities,
	5:i32 PointValue,
	6:string Currency
}

struct MaturityMsg
{
	1:string MaturityStr,
	2:double OpenDays,
	3:i32 OffDays,
	4:double Actu,
	5:string MaturityTime,

}


struct UnderlyingMsg{
	1:string Name,
	2:list<string> DaysOffList,
	3:double Rate,
	4:list<InstrumentClassMsg> FutureClassList,
	5:list<InstrumentClassMsg> OptionClassList,
	6:optional InstrumentClassMsg StockClass;
}

struct TradingPeriodMsg{
	1:string StartTime,
	2:string StopTime,
	3:AtsType.TradingPhase Phase,
	4:i32 AutoStopInterval
}


struct TradingPeriodManagerMsg{
	1:list<TradingPeriodMsg> TradingPeriodList,
	2:string ShiftPriceTime,
	3:string ShiftEodTime,
#	4:string ShiftPriceTimeN,
	4:string ShiftEodTimeN,
}

struct IntraDataMsg{
	1:string name,
	2:string time,
	3:map<string,double> datas
}

struct AESDataMsg{
	1:string num,
	2:string date,
	3:string time_t
}
	
service AtsOperation{
	i64 HeartBeat(),
	void SetLocalTime(1:i64 ticks),
	void CloseAll(),
#	list<FeedSourceMsg> GetAllFeedSources(),
#	list<ConnectionMsg> GetAllConnections(),
	void FeedSourceRelease(1:string feedsourcename),
	bool SetFeedActived(1:string feedsourcename, 2:bool activated),
	bool SetConnectionTradingAllowed(1:string connectionname, 2:bool allowed),
	void ConnectionConnect(1:string connectionname, 2:bool toConnect),
	bool CancelOrder(1:i32 id),
	void CancelAllOrder(),
	i32 SetYesterdayPositionLocal(1:PositionMsg position, 2:i32 yesterdayPositionLocal),
	double SetYesterdayPriceLocalLocal(1:PositionMsg position, 2:double yesterdayPriceLocal),
	i32 SetYesterdayPositionManual(1:PositionMsg position, 2:i32 yesterdayPositionManual),
	i32 SetYesterdayPositionExternal(1:PositionMsg position, 2:i32 yesterdayPositionExternal),
	bool SetUseManualPosition(1:PositionMsg position, 2:bool useManualPosition),
	AtsType.YesterdayPositionType SetYstPositionType(1:PositionMsg position, 2:AtsType.YesterdayPositionType ystPositionType),
	AtsType.YesterdayPositionType SetAtsYstPositionType(1:AtsMsg ats, 2:AtsType.YesterdayPositionType ystPositionType),
	bool CreateManualOrder(1:double price,2:AtsType.OrderWay way,3:i32 quantity,4:string atsName, 5:string atsInstrumentCode, 6:i32 tradingtype,7:AtsType.OrderRestriction orderrestriction,8:AtsType.OrderOpenClose openclose),
	FeesStructMsg GetFeesStruct(1:string className),
	FeesStructMsg UpdateFeesStruct(1:FeesStructMsg feesStruct),
	double TickUpPrice(1:AtsInstrumentMsg instrument, 2:double price, 3:i32 ticks),
	double TickDownPrice(1:AtsInstrumentMsg instrument, 2:double price, 3:i32 ticks),
	void KillAll(1:string atsName,2:i32 tradingtype),
	TradingPeriodManagerMsg GetTradingPeriodManager(1:string atsName),
	void SetTradingPeriodManager(1:string atsName, 2:TradingPeriodManagerMsg tradingperiodManager),
	void SaveReferential(1:string atsName, 2:TradingPeriodManagerMsg tradingPeriodManager, 3:UnderlyingMsg underlying, 4:InstrumentClassMsg StockClass),
#	list<LogMsg> GetLogs(),
#	list<OrderMsg> GetOrders(),
#	list<ExecMsg> GetExecs(),
	void StartPublish(),
	void ForceSubScribe(1:string atsName,2:string instrumentCode),
	void StartAutomaton(1:string atsName),
	void StopAutomaton(1:string atsNamme),
	void CaculateATS(1:string atsName),
	void Start(1:string atsName),
	void Stop(1:string atsName),
	void SaveConfig(1:string atsName),
	void SaveConfigDaily(1:string atsName),
	bool CheckAesData(1:AESDataMsg msg),
	void SendExternalPosition(1:list<string> positions),
	list<string> RequestDeals(),
}