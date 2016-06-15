/********************************************************************
	created:	2014/03/19
	created:	19:3:2014   16:58
	filename: 	F:\BaiduYunDownload\horse4\horse\memory_pool.hpp
	file path:	F:\BaiduYunDownload\horse4\horse
	file base:	memory_pool
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once

namespace horse
{
	typedef unsigned char byte;

	template <class T, bool Enabled = false, int ALLOC_BLOCK_SIZE = 50>
	class memory_pool {};

	template <class T, int ALLOC_BLOCK_SIZE>
	class memory_pool<T, true, ALLOC_BLOCK_SIZE>
	{
		static byte* free_ptr_;

	public:
		memory_pool(void){};
		~memory_pool(void){};

		/*static*/ 
		void* operator new(std::size_t allocLength)
		{
			int sizeT = sizeof(T);

			assert(sizeof(T) == allocLength);
			assert(sizeof(T) >= sizeof(byte*));

			if(!free_ptr_)
				alloc_block();

			byte* ptr = free_ptr_;

			//在free_ptr_中4Byte的地址添加到返回的内存块前面
			free_ptr_ = *reinterpret_cast<byte**>(ptr);

			return ptr;
		}

		/*static*/ 
		void operator delete(void* deletePointer)
		{
			*reinterpret_cast<byte**>(deletePointer) = free_ptr_;
			free_ptr_ = static_cast<byte*>(deletePointer);
		}

	private:
		static void alloc_block()
		{
			free_ptr_ = new byte[sizeof(T) * ALLOC_BLOCK_SIZE];

			byte** current = reinterpret_cast<byte**>(free_ptr_);

			byte* next = free_ptr_;

			for(INT i = 0 ; i < ALLOC_BLOCK_SIZE - 1; i++)
			{
				next += sizeof(T);
				*current = next;
				current = reinterpret_cast<byte**>(next);
			}
			*current = NULL;
		}
	};

	template <class T, int ALLOC_BLOCK_SIZE>
	byte* memory_pool<T, true, ALLOC_BLOCK_SIZE>::free_ptr_;
}