#include "utility.h"
#include <algorithm>
#include <array>
#include <time.h>
#define DEBUG false
Statement StatementInfo()
{
	std::cout<<"Which statement?\n";
	std::string statement;
	std::cin>>statement;
	Statement s(statement);
	s.PrintStatementData();
	return s;
}
Statement FindStatement(std::string caseNumber)
{
	return Statement(caseNumber, true);
}
void Search(std::vector<std::string> terms, std::string statementNumber, foundList& fl)
{
	std::cout<<"SEARCH1\n";
	std::string string;
	//Find shortest word, and only hash that many characters
	int shortest = terms[0].length();
	for(auto term : terms)
	{
		//std::cout<<"B\n";
		int len = term.length();
		if(len < shortest)
		{
			shortest = len;
		}
	}
	std::cout<<"Shortest: "<<shortest<<"\n";
	
	//Open the statement file
	Statement s(statementNumber);
	std::ifstream& file = s.fs;
	if(!file.is_open()) 
	{
		return;
	}
	
	//Print some basic information about the statement
	PrintDashes();
	s.PrintStatementData();
	PrintDashes();
	
	//Setup dictionary
	Dictionary dic(shortest);
	//Setup dumpfile for each statement
	std::ofstream outfile;
	std::ofstream outfileAll;
	outfile.open(std::string("output/")+AppendZeros(s.statementNumber));
	//outfileAll.open("output/all", std::ios_base::app);
	outfileAll.open("output/all");
	
	//Add every word in the file to the dictionary
	while(!file.eof())
	{
		file>>string;
		if(file.eof()) break;
		//std::cout<<"---\n";
		//std::cout<<string<<"\n";
		dic.addWord(string);
		//std::cout<<string<<"\n";
		//std::cout<<"---\n";
	}
	//std::cout<<"Done 1?\n";
	
	//Spit out all words from this file into the dumpfile
	dic.DumpTo(outfile);
	dic.DumpTo(outfileAll);
	//std::cout<<"Done 2?\n";
	
	dic.PrintDic();
	
	//Vector of the amount of times each word pops up
	std::vector<int> sums = Sum(dic, terms);
	//std::cout<<"Done 3?\n";
	//Print how many times each word appears in this statement
	for(unsigned i = 0; i < sums.size() && i < terms.size(); ++i)
	{
		if(sums[i]==0) continue;
		std::cout<<"Statement "<<AppendZeros(s.statementNumber)<<" found the word "<<
		terms[i]<<" "<<sums[i]<<" times.\n";
		fl.AddNum(terms[i], AppendZeros(s.statementNumber), sums[i]);
	}
	//std::cout<<"Done 4?\n";
	//dic.PrintDic();
	//Done with this statement
	PrintDashes();
	outfile.close();
	outfileAll.close();
	//std::cout<<"Done 5.\n";
}
float Search(std::vector<std::string> terms, std::string statementNumber, std::vector<float> weights)
{
	std::cout<<"SEARCH2\n";
	std::string string;
	//Find shortest word, and only hash that many characters
	int shortest = terms[0].length();
	//std::cout<<"A\n";
	for(auto term : terms)
	{
		int len = term.length();
		if(len < shortest)
		{
			shortest = len;
		}
	}
	
	//Open the statement file
	Statement s(statementNumber);
	std::ifstream& file = s.fs;
	if(!file.is_open()) 
	{
		std::cout<<"Couldn't open "<<statementNumber<<"\n";
		return 0.0f;
	}
	
	//Print some basic information about the statement
	//PrintDashes();
	//s.PrintStatementData();
	//PrintDashes();
	
	//Setup dictionary
	Dictionary dic(shortest);
	
	//Add every word in the file to the dictionary
	while(!file.eof())
	{
		file>>string;
		if(file.eof()) break;
		dic.addWord(string);
	}
	
	//Print how many times each word appears in this statement
	float percentage = 0.0f;
	for(unsigned i = 0; i < terms.size(); ++i)
	{
		if(weights[i]==0||dic[terms[i]]==0) continue;
		//std::cout<<terms[i]<<" "<<weights[i]<<" "<<dic[terms[i]]<<"="<<dic[terms[i]]*weights[i]<<std::endl;
		percentage += dic[terms[i]]*weights[i];
		//std::cout<<"PERCENTAGE SO FAR: "<<percentage<<std::endl;
	}
	std::cout<<"FINAL PERCENTAGE: "<<percentage<<std::endl;
	//Done with this statement
	//PrintDashes();
	return percentage;
}
void SearchAll(std::vector<std::string> terms, foundList& fl)
{
	for(int i = 0; i < 200; ++i)
	{
		//search1
		Search(terms,std::to_string(i), fl);
	}
}

std::vector<std::string> ParseSentence(std::ifstream& file)
{
	std::vector<std::string> sentence;
	std::string currentWord;
	while(!file.eof())
	{
		file>>currentWord;
		if(file.eof()) break;
		//std::cout<<"-"<<(int)(currentWord.back())<<"-\n";
		//std::cout<<"<"<<currentWord<<">\n";
		sentence.push_back(currentWord);
		if(currentWord.back() == '.') break;
		if(currentWord.back() == '!') break;
		if(currentWord.back() == '?') break;
		if(currentWord.back() == '\n') break;
		if(currentWord.back() == '"') break;
		if(currentWord.back() < 0) break;
	}
	return sentence;
}

std::vector<std::vector<std::string>> ParseAllSentences(std::ifstream& file)
{
	std::vector<std::vector<std::string>> sentences;
	if(file.is_open())
	{
		//std::cout<<"FILE OPEN";
		while(!file.eof())
		{
			auto s = ParseSentence(file);
			if(file.eof()) break;
			sentences.push_back(s);
		}
	}
	return sentences;
}

void PrintSentences(std::vector<std::vector<std::string>> sentences)
{
	std::cout<<sentences.size();
	for(auto s : sentences)
	{
		std::cout<<">";
		for(auto w : s)
		{
			std::cout<<w<< " ";
		}
		std::cout<<std::endl;
	}
}
//Functions called in main
void Phrase(std::string statement, foundList& fl, std::vector<std::string> sentenceVec)
{
	statement = "statements/"+statement;
	std::ifstream file(statement);
	auto allSentences = ParseAllSentences(file);
	int timesFound = compareAllSentences(allSentences, sentenceVec);
	std::cout<<"Found ";
	for(auto i : sentenceVec)
	{
		std::cout<<CleanString(i)<<" ";
	}
	std::cout<<" "<<timesFound<<" TIMES\n";
}
void DoPhrase(foundList& fl)
{
	std::cout<<"Which statement?\n";
	std::string statement;
	std::cin>>statement;
	std::cout<<"Which words?\n";
	auto sentenceVec = ReadTerms(fl);
	Phrase(statement, fl, sentenceVec);
}
void DoAllPhrase(foundList& fl)
{
	std::cout<<"Which words?\n";
	auto sentenceVec = ReadTerms(fl);
	for(int i = 0; i < 200; ++i)
	{
		Phrase(std::to_string(i), fl, sentenceVec);
	}
}
void DoAllList(foundList& fl)
{
	std::cout<<"Which words?\n";
	auto terms = ReadTerms(fl);
	for(int i = 0; i < 200; ++i)
	{
		Search(terms,std::to_string(i), fl);
	}
}
void DoOneList(foundList& fl)
{
	std::cout<<"Which statement?\n";
	std::string statementNumber;
	std::cin>>statementNumber;
	std::cout<<"Which words?\n";
	auto terms = ReadTerms(fl);
	Search(terms,statementNumber, fl);
}
void Find()
{
	std::cout<<"Which statement?\n";
	std::string statement;
	std::cin>>statement;
	Statement s = FindStatement(statement);
	s.PrintStatementData();
}
void Parse()
{
	std::cout<<"Which statement?\n";
	std::string statement;
	std::cin>>statement;
	statement = "statements/"+statement;
	std::ifstream file(statement);
	auto allSentences = ParseAllSentences(file);
	PrintSentences(allSentences);
}
//actual main
int main(int argc, char ** argv)
{
	std::cout<<"This is the archive of the Magnus Institute.\n";
	foundList fl;
	std::string inStr;
	while(inStr != "quit")
	{
		std::cout<<"What do you want to do?\n";
		std::cout<<"-all\n-find\n-parse\n-sentence\n-info\n-search\n";
		std::cout<<"Type 'quit' at any time to stop.\n";
		std::cin>>inStr;
		if(inStr=="quit") break;
		clock_t initialTime = StartTime();
		//Search all statements for keywords
		if(compare(inStr, "all"))
		{
			DoAllList(fl);
		}
		//Find a statement by case number, the print its info
		else if(compare(inStr, "find"))
		{
			Find();
		}
		//Parse and tokenize all words in a statement
		else if(compare(inStr, "parse"))
		{
			Parse();
		}
		//Search all statements for a specific sentence
		else if(compare(inStr, "allsentence"))
		{
			DoAllPhrase(fl);
		}
		//Search one statement for a specific sentence
		else if(compare(inStr, "sentence"))
		{
			DoPhrase(fl);
		}
		//Print the info of a statement by file number
		else if(compare(inStr, "info"))
		{
			StatementInfo();
		}
		//Search all statements for a specific sentence
		else if(compare(inStr, "search"))
		{
			DoOneList(fl);
		}
		//Find the term frequency of a word
		else if(compare(inStr, "tf"))
		{
			//auto tf = CTF("meat");
		}
		//Find the term inverse document frequency of a word
		else if(compare(inStr, "idf"))
		{
			//auto idf = CIDF("meat");
		}
		//Find the tfidf of a word
		else if(compare(inStr, "xxx"))
		{
			auto tfidf = CTFIDF("leitner","");
		}
		//Find the tfidf of all words
		else if(compare(inStr, "yyy"))
		{
			clock_t it2=StartTime();
			std::cout<<"Which topic do you want to look for?\n";
			std::string topic;
			std::cin>>topic;
			
			std::ofstream weightfile("topics/"+topic+"/weights");
			if(weightfile.is_open()) std::cout<<"open success.\n";
			else std::cout<<"open failure.\n";
			auto termVec = ReadAllFrom("topics/"+topic+"/AllWords");
			
			//weightfile<<"XXX\n";
			//weightfile.close();
			//continue;
			
			std::cout<<termVec.size()<<" words to sort through.\n";
			std::vector<std::pair<float, std::string>> c = CTFIDF(termVec, topic);
			//std::sort(c.begin(), c.end());
			//std::reverse(c.begin(), c.end());
			
			for(auto pair : c)
			{
				if(pair.first > 0.0f)
				{
					weightfile<<pair.second<<" "<<pair.first<<std::endl;
				}
			}
			std::cout<<"DONE!\n";
			weightfile.close();
		}
		//Categorize a statement
		else if(compare(inStr, "zzz"))
		{
			std::cout<<"Which statement?\n";
			std::string statementNumber;
			std::cin>>statementNumber;
			std::array<std::string, 14> topics{"buried","corruption","dark","desolation","end","eye","flesh","hunt","lonely","slaughter","spiral","stranger","vast","web"};
			for(auto topic : topics)
			{
				std::ifstream file("topics/"+topic+"/weights");
				if(!file.is_open()) continue;
				std::cout<<topic<<std::endl;
				std::vector<std::string> terms;
				std::vector<float> weights;
				while(!file.eof())
				{
					std::string term;
					float weight;
					file >> term >> weight;
					//std::cout<<term<<" "<<weight<<std::endl;
					if(file.eof()) break;
					terms.push_back(term);
					weights.push_back(weight);
				}
				//std::cout<<"Start Search\n";
				Search(terms,statementNumber,weights);
				//std::cout<<"End Search\n";
			}
		}
		else if(compare(inStr, "SortDumpFile"))
		{
			std::cout<<"Which topic do you want to look for?\n";
			std::string topic;
			std::cin>>topic;
			std::ifstream finalDumpFileTotal("output/"+topic+"/finalDumpFileTotal");
			std::vector<std::pair<float, std::string>> v;
			while(!finalDumpFileTotal.eof())
			{
				std::pair<float, std::string> p;
				finalDumpFileTotal >> p.second >> p.first;
				v.push_back(p);
			}
			std::sort(v.begin(), v.end());
			std::reverse(v.begin(), v.end());
			std::ofstream weightFile("topics/"+topic+"/weightFile");
			for(auto entry : v)
			{
				if(entry.first > 0)
				weightFile<<entry.second<<" "<<entry.first<<"\n";
			}
			weightFile.close();
			finalDumpFileTotal.close();
		}
		PrintElapsed(initialTime);
	}

	fl.Print();
	
	//Print elapsed time
	
	//std::vector<std::string> s1{"there","once","was","a","man","from","nantucket"};
	//std::vector<std::string> s2{"who","loved","the","buckets"};
	//std::vector<std::string> s3{"a","man","from","nantucket"};
	//bool a = compareSentences(s1,s2);
	//bool b = compareSentences(s1,s3);
	//std::cout<<(a?"A TRUE\n":"A FALSE\n");
	//std::cout<<(b?"B TRUE\n":"B FALSE\n");
	
	std::cout<<"Done.\n";
	return 0;
}
