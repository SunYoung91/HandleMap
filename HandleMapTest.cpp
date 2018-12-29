// HandleMapTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <assert.h>
#include <map>
#include <unordered_map>
#include <time.h>
#include "HandleMap.h"
#include <math.h>

using namespace kadin;

typedef kadin::HandleMap<unsigned int, 20, unsigned int > UintHandleMap;

void final_test()
{
	UintHandleMap handle_map;
	auto handle = UintHandleMap::INVAILD_HANDLE;

	auto start_time = clock();
	UintHandleMap::DataType check_data;
	for (UintHandleMap::HandleType i = 0; i < handle_map.GetMaxHoldCount(); i++) {

		for (UintHandleMap::HandleType index =  0 ; index < handle_map.GetOneHandleIndexMaxUseCount(); index++) {

			if ( 0 == i  &&  0 == index)
				index ++;

			handle = handle_map.GenHandle(i);
			assert(handle != UintHandleMap::INVAILD_HANDLE);
			auto handle_index = handle_map.GetHandleIndex(handle);
			assert(handle_index == i);

			if (handle_map.GetHandleData(handle, check_data)) {
				assert(check_data == i);
			}
			else {
				assert(false);
			}

			handle_map.FreeHandle(handle);

		}
	}

	auto elasped_time = clock() - start_time;

	std::cout << "order test time : " << elasped_time<< "\n";


	std::vector<UintHandleMap::DataType> checkList;
	UintHandleMap handle_map2;
	for (UintHandleMap::HandleType i = 0; i < handle_map2.GetMaxHoldCount(); i++) {

		handle = handle_map2.GenHandle(i);
		assert(handle != UintHandleMap::INVAILD_HANDLE);
		checkList.push_back(handle);
	}

	
	for (std::size_t i = 0; i < checkList.size(); i++) {

		handle = checkList[i];
		if (handle_map2.GetHandleData(handle, check_data)) {
			if (check_data != i) {
				assert(false);
			}
		}
		else {
			assert(false);
		}
	}

	std::cout << "final test time : " << clock() - elasped_time << "\n";

}

int main()
{

	final_test();
	return 0;
	UintHandleMap handle_map;

	std::cout << "max hold count : " << handle_map.GetMaxHoldCount() << " , handle data struct size :" << handle_map.GetHandleDataStructSize() << " , one handle index can use max count : " << handle_map.GetOneHandleIndexMaxUseCount() << "\n";
	std::vector<unsigned int> hlist;

	auto time = clock();

	for (unsigned int i = 0; i < handle_map.GetMaxHoldCount(); i++) {
		auto handle = handle_map.GenHandle(i);
		assert(handle != UintHandleMap::INVAILD_HANDLE);
		hlist.push_back(handle);
	}

	auto insert_time = clock();
	std::cout << "hlist count : " << hlist.size() << "\n";

	std::cout << "handle_map insert time : " << insert_time - time << "\n";
	

	UintHandleMap::DataType data = 0;
	for (UintHandleMap::HandleType i = 0; i < handle_map.GetMaxHoldCount(); i++) {
		assert(handle_map.GetHandleData(hlist[i], data) != false);
	}

	auto passtime = clock();
	std::cout << "handle_map find elasped ms : " << passtime - insert_time << "\n";

	
	std::unordered_map<unsigned int, unsigned int> uint_map;
	for (unsigned int i = 0; i < handle_map.GetMaxHoldCount(); i++) {
		uint_map.insert(std::pair<unsigned int, unsigned int>(i,i));
	}
	
	auto uint_map_insert_time = clock();

	std::cout << "uint_map insert elasped ms : " << uint_map_insert_time - passtime << "\n";

	for (unsigned int i = 0; i < handle_map.GetMaxHoldCount(); i++) {
		data = uint_map[i];
		assert(data == i);
	}

	auto now_time = clock();

	std::cout << "uint_map find elasped ms : " << now_time - uint_map_insert_time << "\n";

#ifdef _MSC_VER
	int b;
	std::cin >> b;
#endif // _MSC_VER

}
