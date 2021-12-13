#include "dictionary.h"
#include <vector>
#include <iostream>
#pragma once

struct foundList
{
	void AddWord(std::string s)
	{
		std::pair<std::string,std::vector<std::pair<std::string, int>>> p;
		p.first = s;
		vector.push_back(p);
	}
	
	void AddNum(std::string s, std::string n, int i)
	{
		for(auto& p : vector)
		{
			if(p.first==s)
			{
				std::pair<std::string, int> p2(n, i);
				p.second.push_back(p2);
				break;
			}
		}
	}
	
	void Print()
	{
		std::ofstream dumpFile("output/final");
		//std::cout<<"PRINTING "<<vector.size()<<std::endl;
		for(auto p : vector)
		{
			std::cout<<"---["<<p.first<<"]---"<<std::endl;
			dumpFile<<"---["<<p.first<<"]---"<<std::endl;
			for(auto p2 : p.second)
			{
				std::cout<<p2.first<<"-"<<p2.second<<", ";
				dumpFile<<p2.first<<"-"<<p2.second<<", ";
			}
			std::cout<<std::endl;
			dumpFile<<std::endl;
		}
		dumpFile.close();
	}
	
	std::vector<std::pair<std::string,std::vector<std::pair<std::string, int>>>> vector;
};

struct Statement
{
	Statement(std::string fileNumber, bool sameAs = false)
	{
		if(!opened)
		{
			if(sameAs)
			{
				statementNumber = fileNumber;
				fileNumber = "raw/"+fileNumber;
			}
			OpenFile(fileNumber);
			ReadStatementData();
			opened = fs.is_open();
		}
	}
	void OpenFile(std::string name)
	{
		filename += name;
		fs.open(filename);
		//if(fs.is_open()) 
		//std::cout<<"Opening "<<filename<<std::endl;
	}
	void ReadStatementData()
	{
		if(statementNumber.empty()) std::getline(fs, statementNumber);
		if(statementName.empty()) std::getline(fs, statementName);
		if(statementAuthor.empty()) std::getline(fs, statementAuthor);
		if(statementDate.empty()) std::getline(fs, statementDate);
	}
	void PrintStatementData()
	{
		if(opened)
		{
			std::cout<<"Statement "<<statementNumber<<": ";
			std::cout<<statementName<<std::endl;
			std::cout<<statementAuthor<<std::endl;
			std::cout<<statementDate<<std::endl;
		}
	}
	std::string statementName, statementAuthor, statementDate, statementNumber;
	//CHANGE THIS FOR EACH TOPIC
	std::string filename{"topics/buried/"};
	std::ifstream fs;
	bool opened{false};
};
std::string CleanString(std::string key);
std::vector<int> Sum(Dictionary& dic, std::vector<std::string> vec);
std::string AppendZeros(std::string);
clock_t StartTime();
void PrintElapsed(clock_t initialTime);
void PrintDashes();
std::vector<std::string> ReadTerms(foundList& fl);
bool compare(std::string key, std::string current);
bool compareSentences(std::vector<std::string> s1, std::vector<std::string> s2);
int compareAllSentences(std::vector<std::vector<std::string>> statement, std::vector<std::string> sentence);
float CTF(std::string term, Statement& s);
std::vector<float> CTF(std::string term);
float CIDF(std::string term);
float CTFIDF(std::string term, Statement& s);
std::vector<float> CTFIDF(std::string term, std::string topic);
std::vector<std::pair<float, std::string>> CTFIDF(std::vector<std::string> term, std::string topic);
std::vector<std::string> ReadAllFrom(std::string filename);
