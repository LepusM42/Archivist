#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <list>
#pragma once
template <typename keyType, typename dataType>
struct Slot
{
	Slot(keyType k, dataType d) :
		m_key{k},
		m_data{d}
	{
		
	}
	bool operator<(Slot other)
	{
		return m_key < other.m_key;
	}
	bool operator<=(Slot other)
	{
		return m_key <= other.m_key;
	}
	bool operator>(Slot other)
	{
		return m_key > other.m_key;
	}
	bool operator>=(Slot other)
	{
		return m_key >= other.m_key;
	}
	bool operator==(Slot other)
	{
		return m_key == other.m_key;
	}
	bool operator!=(Slot other)
	{
		return m_key != other.m_key;
	}
	keyType m_key;
	dataType m_data;
	bool m_occupied{false};
};
typedef Slot<std::string, int> WordCount;
typedef std::list<WordCount> WordCountList;
typedef std::array<WordCountList, 130> WCL_Array;
class Dictionary
{
	public:
		Dictionary(int charsHashed);
		int operator[](std::string key) const;
		int& operator[](std::string key);
		void addWord(std::string key);
		std::vector<Slot<std::string, int>> Sort();
		void DumpTo(std::ofstream& outfile);
		void PrintDic();
		int Size();
		~Dictionary();
	private:
		WCL_Array array;
		int zero{0}, m_charsHashed{0};
		unsigned HashFunc(std::string k) const;
		void insert(std::string key, int data);
};

