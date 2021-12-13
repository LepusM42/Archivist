#include "utility.h"
#include "dictionary.h"
#include <vector>
#include <iostream>
#include <math.h>
std::string CleanString(std::string key)
{
	if(key.empty()) return key;
	
	std::string key2;
	
	for(unsigned i=0; i<=key.length();++i)
	{
		key[i] = toupper(key[i]);
		
		if(key[i] < 'A' || key[i] > 'Z')
		{
			
		}
		else
		{
			key2 += key[i];
		}
	}
	//std::cout<<"Finidhse "<<key2<<"\n";
	return key2;
}

std::vector<int> Sum(Dictionary& dic, std::vector<std::string> vec)
{
	std::vector<int> sums;
	std::cout<<"Sum Begin.\n";
	for(std::string watch : vec)
	{
		std::cout<<watch<<"\n";
		int sum = dic[watch];
		std::cout<<sum<<"\n";
		sums.push_back(sum);
	}
	std::cout<<"Sum Done.\n";
	return sums;
}

std::string AppendZeros(std::string statement)
{
	int zLeft = 7 - statement.length();
	if(zLeft <= 0) return statement;
	std::string zeros;
	for(int i = zLeft; i > 0; --i)
	{
		zeros += "0";
	}
	return zeros + statement;
}

clock_t StartTime()
{
	return clock();
}

void PrintElapsed(clock_t initialTime)
{
	auto timeTaken = 1000*(clock()-initialTime)/(double)(CLOCKS_PER_SEC);
	if(timeTaken/1000>1)
	{
		timeTaken /= 1000;
		if(timeTaken/60>1)
		{
			timeTaken /= 60;
			if(timeTaken/60>1)
			{
				timeTaken /= 60;
				std::cout<<"Time taken: "<<timeTaken<<"h.\n";
			}
			else
			{
				std::cout<<"Time taken: "<<timeTaken<<"m.\n";
			}
		}
		else
		{
			std::cout<<"Time taken: "<<timeTaken<<"s.\n";
		}
	}
	else
	{
		std::cout<<"Time taken: "<<timeTaken<<"ms.\n";
	}
}

void PrintDashes()
{
	for(int i=0;i<80;++i) std::cout<<"-";
	std::cout<<std::endl;
}

std::vector<std::string> ReadTerms(foundList& fl)
{
	std::cout<<"Type 'quit' at any time to stop.\n";
	std::vector<std::string> sentenceVec;
	std::string inStr;
	while(inStr != "quit")
	{
		std::cin>>inStr;
		if(inStr=="quit") break;
		sentenceVec.push_back(inStr);
		fl.AddWord(inStr);
	}
	return sentenceVec;
}

bool compare(std::string s1, std::string s2)
{
	if(s1.empty() != s2.empty())
		return false;
		
	if(s1.length() != s2.length()) return false;
		
	s1 = CleanString(s1);
	s2 = CleanString(s2);
	if( s2.compare(0,s1.length(),s1)==0 ||
	s1.compare(0,s2.length(),s2)==0)
	{
		return true;
	}
	return false;
}

bool compareSentences(std::vector<std::string> s1, std::vector<std::string> s2)
{
	if(s1.size()<s2.size()) return false;
	//larger sentence
	for(unsigned i = 0; i < s1.size(); ++i)
	{
		unsigned i2 = i;
		bool good = true;
		for(unsigned j = 0; j < s2.size(); ++j)
		{
			if(compare(s1[i], s2[j])) 
			{
				++i;
				good = true;
			}
			else
			{
				good = false;
				break;
			}
		}
		if(good)
		{
			return true;
		}
		else
		{
			i = i2;
		}
	}
	return false;
}

int compareAllSentences(std::vector<std::vector<std::string>> statement,
std::vector<std::string> sentence)
{
	int timesFound = 0;
	for(auto s : statement)
	{
		if(compareSentences(s, sentence)) ++timesFound;
	}
	return timesFound;
}

float CTF(std::string term, Statement& s)
{
	//std::cout<<"Calculating CTF of '"<<term<<"'...\n";
	//Open the statement file
	std::ifstream& file = s.fs;
	if(!file.is_open()) 
	{
		return 0.0f;
	}
	
	//Setup dictionary
	Dictionary dic(term.length());
	std::string string;
	
	//Add every word in the file to the dictionary
	while(!file.eof())
	{
		file>>string;
		if(file.eof()) break;
		dic.addWord(CleanString(string));
	}
	float tf = (float)dic[term]/dic.Size();
	
	return tf;
}

std::vector<float> CTF(std::string term)
{
	std::vector<float> tfv;
	for(int i = 0; i < 200; ++i)
	{
		//Open the statement file
		Statement s(std::to_string(i));
		tfv.push_back(CTF(term, s));
		std::cout<<(float)i/2<<"% done with term "<<term<<std::endl;
	}
	return tfv;
}

float CIDF(std::string term)
{
	int corpusCount = 0;
	int subcorpusCount = 0;
	for(int i = 0; i < 200; ++i)
	{
		//Open the statement file
		Statement s(std::to_string(i));
		std::ifstream& file = s.fs;
		if(!file.is_open()) 
		{
			continue;
		}
		++corpusCount;
		//Setup dictionary
		Dictionary dic(term.length());
		std::string string;
		//Add every word in the file to the dictionary
		while(!file.eof())
		{
			file>>string;
			if(file.eof()) break;
			dic.addWord(CleanString(string));
		}
		if(dic[term] > 0)
		{
			++subcorpusCount;
		}
	}
	//std::cout<<corpusCount<<" "<<subcorpusCount<<std::endl;
	if(subcorpusCount == 0.0f) return 0.0f;
	float idf = log((float)corpusCount/((float)subcorpusCount));
	//std::cout<<"IDF: "<<idf<<std::endl;
	return idf;
}

//term, statement
float CTFIDF(std::string term, Statement& s)
{
	float tfidf = CTF(term, s) * CIDF(term);
	//std::cout<<term<<"-"<<tfidf<<std::endl;
	return tfidf;
}

//term, topic
std::vector<float> CTFIDF(std::string term, std::string topic)
{
	std::ofstream finalDumpFile;
	finalDumpFile.open("output/"+topic+"/finalDumpFile"+term);
	std::vector<float> tfidfv;
	float sum = 0.0f;
	//std::cout<<"starting term: "<<term<<std::endl;
	if(finalDumpFile.is_open())
	{
		//std::cout<<"calculating inverse document frequency...\n";
		float idf = CIDF(term);
		if(idf == 0)
		{
			//finalDumpFile.close();
			//return tfidfv;
		}
			//std::cout<<"starting...\n";
		for(unsigned i = 0; i < 200; ++i)
		{
			//std::cout<<"going...\n";
			Statement s(std::to_string(i));
			if(s.statementName.empty()) continue;
			//std::cout<<"calculating term frequency...\n";
			float tf = CTF(term, s);
			float tfidf = tf * idf;
			tfidfv.push_back(tfidf);
			if(!tfidf) continue;
			//std::cout<<tf<<" "<<idf<<std::endl;
			//std::cout<<"'"<<term<<"'-("<<i<<", "<<s.statementName<<")-"<<tfidf<<std::endl;
			//finalDumpFile<<tf<<" "<<idf<<std::endl;
			finalDumpFile<<"("<<i<<", "<<s.statementName<<")-"<<tfidf<<std::endl;
			sum += tfidf;
		}
	}
	finalDumpFile << sum << std::endl;
	tfidfv.push_back(sum);
	
	std::ofstream finalDumpFileTotal;
	finalDumpFileTotal.open("output/"+topic+"/finalDumpFileTotal", std::ios_base::app);
	finalDumpFileTotal<<term<<" "<<sum<<std::endl;
	finalDumpFileTotal.close();
	
	finalDumpFile.close();
	//std::cout<<"done...\n";
	return tfidfv;
}

//termvec, topic
std::vector<std::pair<float, std::string>> CTFIDF(std::vector<std::string> terms, std::string topic)
{
	std::vector<std::pair<float, std::string>> tfidfv;
	float i = 0.0f, j = (float)terms.size();
	for(auto term : terms)
	{
		std::pair<float, std::string> pair;
		auto tfidf = CTFIDF(term, topic);
		pair.second = term;
		pair.first = tfidf.back();
		tfidfv.push_back(pair);
		std::cout<<(++i/j)*100.0f<<"% DONE...\n";
		//clock_t remaining = (1-i/j) * (StartTime() - start);
		auto r2 = 1000*(clock())/(double)(CLOCKS_PER_SEC);
		//std::cout<<r2<<"ms elapsed...\n";
	}
	std::cout<<"TFIDF CALCULATED\n";
	return tfidfv;
}

std::vector<std::string> ReadAllFrom(std::string filename)
{
	std::vector<std::string> wordVec;
	std::ifstream file(filename);
	if(file.is_open())
	{
		while(!file.eof())
		{
			std::string string;
			file >> string;
			wordVec.push_back(string);
		}
	}
	return wordVec;
}
