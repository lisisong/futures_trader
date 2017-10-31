// testTraderApi.cpp : main
//

#include "./ThostTraderApi/ThostFtdcMdApi.h"
#include "MdSpi.h"

// UserApi Object
CThostFtdcMdApi* pUserApi;

char  FRONT_ADDR[] = "tcp://180.168.146.187:10031";		// front addr
TThostFtdcBrokerIDType	BROKER_ID = "9999";				// company code
TThostFtdcInvestorIDType INVESTOR_ID = "105391";			// your code
TThostFtdcPasswordType  PASSWORD = "123123a";			// your password

char *ppInstrumentID[] = {"m1801"};			// market code
int iInstrumentID = 1;									// subscribe number


// request number
int iRequestID = 0;

int main(void)
{
	// init UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// create UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// register event
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	pUserApi->Init();

	pUserApi->Join();
//	pUserApi->Release();
	return 1;
}