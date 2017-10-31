obj = testMdApi.o MdSpi.o
target = app
$(target):$(obj)
	g++ -L./ThostTraderApi testMdApi.o MdSpi.o ./ThostTraderApi/thostmduserapi.so ./ThostTraderApi/thosttraderapi.so -o app
%.o:%.cpp
	g++ -c $< -o $@
