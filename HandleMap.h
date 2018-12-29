#pragma once
#include <vector>

namespace kadin{
	template<class T, int IndexBitCount,class D >
	class HandleMap
	{
	public:
		static const T INVAILD_HANDLE = 0;
		~HandleMap() {};
		HandleMap() {};
		typedef T HandleType;
		typedef D DataType ;
	
		T GenHandle(D data);
		bool GetHandleData(T id , D& data );
		T GetMaxHoldCount() { return kMaxHandleIndex; }; //zero is represent for invaild handle 
		T GetHandleDataStructSize() { return sizeof(HandleData); };
		T GetOneHandleIndexMaxUseCount() { return kMaxUseMask + 1; };
		T GetHandleIndex(T handle) { return handle & kMaxHandleIndex; };
		void FreeHandle(T handle);
	private:

		// |use_count | handle_index |
		const T kUseCountBitCount = sizeof(T) * 8 - IndexBitCount;
		const T kMaxHandleIndex = (1 << IndexBitCount) - 1; 
		const T kMaxUseMask  = (1 << kUseCountBitCount) - 1;

		typedef struct
		{
			T use_count;
			D data;
		} HandleData, *PHandleData;

	
		PHandleData GetHandleStructData(T handle);
		

		T GetUseCount(T handle)
		{
			return handle >> IndexBitCount;
		}

		T MakeHandle(T index, T use_count)
		{
			return (use_count << IndexBitCount) | (index & kMaxHandleIndex);
		}
		
		T _gen_handle_count = 0;  
		std::vector<HandleData> _handle_list;
		std::vector<T> _free_handle_list;
		std::vector<T> _cant_use_handle_list;

	};
};


//implementation
namespace kadin{

/*
#define GetUseCount(h) h >> IndexBitCount
#define GetIndex(h) h & kMaxHandleIndex;
#define MakeHandle(index,use_count) (use_count << IndexBitCount) | (index & kMaxHandleIndex) 
*/

template<class T, int IndexBitCount, class D>
inline T HandleMap<T, IndexBitCount, D>::GenHandle(D data)
{

	std::size_t handle_list_size = _handle_list.size();

	if (handle_list_size > kMaxHandleIndex) {
		return INVAILD_HANDLE;
	}

	_gen_handle_count++;

	std::size_t free_list_size = _free_handle_list.size();

	if (free_list_size > 0) {
		T handle = _free_handle_list[free_list_size - 1]; //get last handle , so the first free handle will be use at last;
		PHandleData p = GetHandleStructData(handle);
		assert(p);
		p->use_count++;
		p->data = data;
		_free_handle_list.pop_back();
		return MakeHandle(handle, p->use_count);
	}


	HandleData hd;
	hd.data = data;
	hd.use_count = 0;

	if (0 == handle_list_size) // the first handle use_count should be set to 1 because zero is invalid handle
		hd.use_count++;

	_handle_list.push_back(hd);

	return MakeHandle((T)handle_list_size, hd.use_count);

}

template<class T, int IndexBitCount, class D>
bool HandleMap<T, IndexBitCount, D>::GetHandleData(T id, D & data)
{
	PHandleData hd = GetHandleStructData(id);
	if (!hd)
		return false;
	
	if (hd ->use_count != GetUseCount(id)) {
		return false;
	}

	data = hd->data;
	return true;
}

template<class T, int IndexBitCount, class D>
typename
HandleMap<T, IndexBitCount, D>::PHandleData HandleMap<T, IndexBitCount, D>::GetHandleStructData(T handle)
{
	T index = GetHandleIndex(handle);

	if (index < _handle_list.size()) {
		return &_handle_list[index];
	}

	return nullptr;
}

template<class T, int IndexBitCount, class D>
void HandleMap<T, IndexBitCount, D>::FreeHandle(T handle)
{
	T index = GetHandleIndex(handle);

	if (index < _handle_list.size()) {

		HandleData &hd = _handle_list[index];

		assert(hd.use_count <= kMaxUseMask);

		if (hd.use_count == kMaxUseMask) {
			_cant_use_handle_list.push_back(handle);
		}
		else {
			_free_handle_list.push_back(handle);
		}
	}
}

}