#include "MdSpi.h"
#include <iostream>
#include <memory.h>
#include <time.h>


using namespace std;

#pragma warning(disable : 4996)


extern CThostFtdcMdApi* pUserApi;


extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];	
extern int iInstrumentID;

// reqNo
extern int iRequestID;

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	// user loign request
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> send user login request: " << ((iResult == 0) ? "success" : "fail") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		/// get current day
		cerr << "--->>> get current day = " << pUserApi->GetTradingDay() << endl;
		// marketData request
		SubscribeMarketData();	

	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> subscribe market data : " << ((iResult == 0) ? "success" : "fail") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << __FUNCTION__ << endl;
}
using namespace std;
#include <iostream>
#include <fstream>
#include <string>

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t= localtime(&tt);
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
		   t->tm_year + 1900,
		   t->tm_mon + 1,
		   t->tm_mday,
		   t->tm_hour,
		   t->tm_min,
		   t->tm_sec);

	//cerr << __FUNCTION__ << endl;
	cerr << "--->>> new price=" << pDepthMarketData->LastPrice << endl;
	
	string s = "D:\\" ;
	s.append(pDepthMarketData->InstrumentID);
	s.append(".csv");

	ofstream outfile;
	outfile.open(s, ios::app);
	outfile << pDepthMarketData->InstrumentID<<","<< pDepthMarketData->UpdateTime<<"."<< pDepthMarketData->UpdateMillisec<<","<< pDepthMarketData->LastPrice
		<< "," << pDepthMarketData-> Volume << "," << pDepthMarketData-> BidPrice1 << "," << pDepthMarketData-> BidVolume1 << "," << pDepthMarketData-> AskPrice1 
		<< "," << pDepthMarketData-> AskVolume1 << "," << pDepthMarketData-> OpenInterest << "," << pDepthMarketData->Turnover<< endl;

	outfile.close();

}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// if ErrorID != 0, it means recv a error message
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}