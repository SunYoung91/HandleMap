# HandleMap
high performance map integer (unsigned int or uint64) to object for gameserver 
高性能O(1)Map 类型设计为游戏服务器 角色临时ID 管理

linux : gcc HandleMapTest.cpp -std=c++11 -lstdc++

windows : vs2017
dont use release mode to test performance. 不要在release 模式下去测试性能。因为用了assert 宏 release 会被优化掉。
