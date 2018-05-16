/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef SimpleAtsMsg_TYPES_H
#define SimpleAtsMsg_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>
#include "AtsType_types.h"
#include "AtsGeneral_types.h"


namespace SimpleMsg {

struct TradingType {
  enum type {
    Unknown = 0,
    Manual = 1,
    Hitter = 2,
    Max = 3
  };
};

extern const std::map<int, const char*> _TradingType_VALUES_TO_NAMES;

class TrdInstrumentMsg;

class SimpleAtsMsg;

typedef struct _TrdInstrumentMsg__isset {
  _TrdInstrumentMsg__isset() : AtsInstrument(false), AtsName(false) {}
  bool AtsInstrument :1;
  bool AtsName :1;
} _TrdInstrumentMsg__isset;

class TrdInstrumentMsg {
 public:

  TrdInstrumentMsg(const TrdInstrumentMsg&);
  TrdInstrumentMsg& operator=(const TrdInstrumentMsg&);
  TrdInstrumentMsg() : AtsName() {
  }

  virtual ~TrdInstrumentMsg() throw();
   ::AtsGeneral::AtsInstrumentMsg AtsInstrument;
  std::string AtsName;

  _TrdInstrumentMsg__isset __isset;

  void __set_AtsInstrument(const  ::AtsGeneral::AtsInstrumentMsg& val);

  void __set_AtsName(const std::string& val);

  bool operator == (const TrdInstrumentMsg & rhs) const
  {
    if (!(AtsInstrument == rhs.AtsInstrument))
      return false;
    if (!(AtsName == rhs.AtsName))
      return false;
    return true;
  }
  bool operator != (const TrdInstrumentMsg &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TrdInstrumentMsg & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TrdInstrumentMsg &a, TrdInstrumentMsg &b);

inline std::ostream& operator<<(std::ostream& out, const TrdInstrumentMsg& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _SimpleAtsMsg__isset {
  _SimpleAtsMsg__isset() : Ats(false), AllFutures(false), KeepOrders(false), ForceOpen(false), AtsIntraDataCols(false), FeedsourcesStr(false), ConnectionsStr(false), Underlyings(false), AllOptions(false), AllStocks(false) {}
  bool Ats :1;
  bool AllFutures :1;
  bool KeepOrders :1;
  bool ForceOpen :1;
  bool AtsIntraDataCols :1;
  bool FeedsourcesStr :1;
  bool ConnectionsStr :1;
  bool Underlyings :1;
  bool AllOptions :1;
  bool AllStocks :1;
} _SimpleAtsMsg__isset;

class SimpleAtsMsg {
 public:

  SimpleAtsMsg(const SimpleAtsMsg&);
  SimpleAtsMsg& operator=(const SimpleAtsMsg&);
  SimpleAtsMsg() : KeepOrders(0), ForceOpen(0), FeedsourcesStr(), ConnectionsStr() {
  }

  virtual ~SimpleAtsMsg() throw();
   ::AtsGeneral::AtsMsg Ats;
  std::vector<TrdInstrumentMsg>  AllFutures;
  bool KeepOrders;
  bool ForceOpen;
  std::vector<std::string>  AtsIntraDataCols;
  std::string FeedsourcesStr;
  std::string ConnectionsStr;
  std::vector< ::AtsGeneral::UnderlyingMsg>  Underlyings;
  std::vector<TrdInstrumentMsg>  AllOptions;
  std::vector<TrdInstrumentMsg>  AllStocks;

  _SimpleAtsMsg__isset __isset;

  void __set_Ats(const  ::AtsGeneral::AtsMsg& val);

  void __set_AllFutures(const std::vector<TrdInstrumentMsg> & val);

  void __set_KeepOrders(const bool val);

  void __set_ForceOpen(const bool val);

  void __set_AtsIntraDataCols(const std::vector<std::string> & val);

  void __set_FeedsourcesStr(const std::string& val);

  void __set_ConnectionsStr(const std::string& val);

  void __set_Underlyings(const std::vector< ::AtsGeneral::UnderlyingMsg> & val);

  void __set_AllOptions(const std::vector<TrdInstrumentMsg> & val);

  void __set_AllStocks(const std::vector<TrdInstrumentMsg> & val);

  bool operator == (const SimpleAtsMsg & rhs) const
  {
    if (!(Ats == rhs.Ats))
      return false;
    if (!(AllFutures == rhs.AllFutures))
      return false;
    if (!(KeepOrders == rhs.KeepOrders))
      return false;
    if (!(ForceOpen == rhs.ForceOpen))
      return false;
    if (!(AtsIntraDataCols == rhs.AtsIntraDataCols))
      return false;
    if (!(FeedsourcesStr == rhs.FeedsourcesStr))
      return false;
    if (!(ConnectionsStr == rhs.ConnectionsStr))
      return false;
    if (!(Underlyings == rhs.Underlyings))
      return false;
    if (!(AllOptions == rhs.AllOptions))
      return false;
    if (!(AllStocks == rhs.AllStocks))
      return false;
    return true;
  }
  bool operator != (const SimpleAtsMsg &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SimpleAtsMsg & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(SimpleAtsMsg &a, SimpleAtsMsg &b);

inline std::ostream& operator<<(std::ostream& out, const SimpleAtsMsg& obj)
{
  obj.printTo(out);
  return out;
}

} // namespace

#endif