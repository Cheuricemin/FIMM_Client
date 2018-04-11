namespace csharp AtsType
namespace cpp AtsType

enum OrderWay{
	Undef,
    Buy,
    Sell,
    CoveredBuy,
	CoveredSell,
	Freeze,
	Unfreeze,
	PLEDGE_BOND_IN,
    PLEDGE_BOND_OUT,
    ETFPur, 
    ETFRed,
    OFPur,
    OFRed
}

enum OrderAction{
	Created,
    Modified,
    Cancelled,
    Undef
}

enum OrderOpenClose{
	Undef,
    Open,
    Close,
	CloseToday
}

enum OrderPriceMode{
    Undef,
    Limit,
    Market
}

enum OrderRestriction{
    Undef,
    None,
    ImmediateAndCancel,
    FillAndKill
}


enum OrderStatus
{
    Undef,
    WaitServer,
    WaitMarket,
    Ack,
    Nack,
    Reject,
    Cancel,
    Exec,
}

enum FeedSourceStatus{
	Up,
	Down
}

enum  ConnectionStatus
{
   Disconnected,
   WaitConnect,
   Connected
}

enum YesterdayPositionType{
    Local,
    External
}

enum YesterdayPriceType{
    Local,
    External
}

enum TradingPhase
{
	PREOPEN,
    OPEN,
    CLOSE,
    PRECLOSE,
    NO_CANCEL
}

enum InstrType
{
	Undef,
	Index,
	Stock,
	Option,
	Call,
	Put,
	Future,
	Futurespread,
	ETF,
	Repo,
	Bond,
	Fund,
	Max
}