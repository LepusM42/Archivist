#include "utility.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#define DEBUG false

Dictionary::Dictionary(int charsHashed) :
m_charsHashed{charsHashed}
{
	if(DEBUG)
	std::cout<<"Dictionary constructor.\n";
}
int Dictionary::operator[](std::string key) const
{
	//std::cout<<"CONST\n";
	unsigned pos = HashFunc(key);
	for(auto entry : array[pos])
	{
		if(compare(entry.m_key, key))
		{
			//std::cout<<entry.m_key<<" VS "<<key<<"\n";
			return entry.m_data;
		}
	}
	return 0;
}
int& Dictionary::operator[](std::string key)
{
	//std::cout<<"NONCONST\n";
	unsigned pos = HashFunc(key);
	//std::cout<<"Pos: "<<pos<<"\n";
	zero = 0;
	for(auto& entry : array[pos])
	{
		if(compare(entry.m_key, key))
		{
			//std::cout<<entry.m_key<<" VS "<<key<<"\n";
			return entry.m_data;
		}
	}
	return zero;
}
void Dictionary::addWord(std::string key)
{
	key = CleanString(key);
	if(key.empty()) return;
	//std::cout<<"ADD: "<<key<<"\n";
	int data = (*this)[key];
	if(data == 0)
	{
		insert(key, 1);
	}
	else
	{
		++(*this)[key];
	}
}
std::vector<WordCount> Dictionary::Sort()
{
	std::vector<WordCount> sortVec;
	for(int i = 0; i < array.size(); ++i)
	{
		for(auto entry : array[i])
		{
			sortVec.push_back(entry);
		}
	}
	std::sort(sortVec.begin(), sortVec.end());
	return sortVec;
}
void Dictionary::DumpTo(std::ofstream& outfile)
{
	auto sortVec = Sort();
	int numWords = 0;
	for(auto term : sortVec)
	{
		std::string& key = term.m_key;
		if(key.empty()) continue;
		outfile<<key<<std::endl;
		++numWords;
	}
	if(DEBUG)
	std::cout<<numWords<<" words!\n";
}
void Dictionary::PrintDic()
{
	for(int i = 0; i < array.size(); ++i)
	{
		std::cout<<"\n---"<<(char)(i/5+'A')<<"---\n";
		for(auto entry : array[i])
		{
			std::cout<<"["<<entry.m_key<<"-"<<entry.m_data<<"]->";
		}
		std::cout<<"\n";
	}
}
int Dictionary::Size()
{
	int sum = 0;
	for(int i = 0; i < array.size(); ++i)
	{
		for(auto entry : array[i])
		{
			sum += entry.m_data;
		}
	}
	return sum;
}
Dictionary::~Dictionary()
{
	if(DEBUG) std::cout<<"Dictionary destructor.\n";
}
unsigned Dictionary::HashFunc(std::string k) const
{
	k = CleanString(k);
	//std::cout<<"Hash: "<<k<<"\n";
	unsigned hash = (k[0] - 'A') * 5;
	unsigned len = k.size();
	if(len >= 4) ++hash;
	if(len >= 6) ++hash;
	if(len >= 8) ++hash;
	if(len >= 10) ++hash;
	return hash;
}
void Dictionary::insert(std::string key, int data)
{
	unsigned pos = HashFunc(key);
	if(pos >= 130)
	{
		std::cerr<<"BAD INDEX\n";
		return;
	}
	WordCount wc(key, data);
	array[pos].push_front(wc);
}
