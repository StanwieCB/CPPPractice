//programmed three times
//author:hank zhang(StanwieCB)
//12-16-2016

//----------------------------------------------------------------------//
//any question or suggestion about this program or my github is welcome
//contact:hankzhangCB@gmail.com
//----------------------------------------------------------------------//

#ifndef _PROJECT2_H
#define _PROJECT2_H
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct Word
{
	string word;
	int freq;
	Word() { freq = 1; }
	Word(string str) :word(str), freq(1) {};
	bool operator==(Word& other) { return word == other.word; }

	void toLower()
	{
		transform(word.begin(), word.end(), word.begin(), ::tolower);
	}
};

class MyCounter
{
private:
	enum class counterType { WCounter, CPPCounter };		//0: WordsCounter  1:CPPCounter	

	//all words counted
	vector<Word> words;
	ifstream inFile;
	string fileName;		//file to be read
	counterType ct;		//0: wordsCounter  1:CPPCounter	
	int resCnt = 0;			//outResult number


	//CPPcounter initializer
	void initializeCPPCounter()
	{
		ifstream keywords("keywords.txt");
		string tmpKey;
		Word tmpWord;
		while (keywords>>tmpKey)
		{
			tmpWord.word = tmpKey;
			tmpWord.freq = 0;
			words.push_back(tmpWord);
		}
	}

	//skip quote in CPP counter by replace quote with "......"
	void skipQuote(string& str)
	{
		int len = str.length();
		//replace '......' first
		int i1 = 0, i2 = 0;
		while (true)
		{
			i1 = str.find("'", i2);
			//skip \'
			while (i1 != string::npos && i1 - 1 >= 0 && str[i1 - 1] == '\\')
			{
				i1 = str.find("'", i1 + 1);
			}
			if (i1 == string::npos)
			{
				break;
			}
			i2 = str.find("'", i1 + 1);
			//skip \'
			while (i2 != string::npos && str[i2 - 1] == '\\')
			{
				i2 = str.find("'", i2 + 1);
			}
			if (i2 == string::npos)
			{
				break;
			}
			for (int i = i1; i <= i2; i++)
			{
				str[i] = '.';
			}
		}
		//replace "......" second
		i1 = 0, i2 = 0;
		while (true)
		{
			i1 = str.find("\"", i2);
			//skip \"
			while (i1 != string::npos && i1 - 1 >= 0 && str[i1 - 1] == '\\')
			{
				i1 = str.find("\"", i1 + 1);
			}
			if (i1 == string::npos)
			{
				break;
			}
			i2 = str.find("\"", i1 + 1);
			//skip \"
			while (i2 != string::npos && str[i2 - 1] == '\\')
			{
				i2 = str.find("\"", i2 + 1);
			}
			if (i2 == string::npos)
			{
				break;
			}
			for (int i = i1; i <= i2; i++)
			{
				str[i] = '.';
			}
		}
	}

	//compare by lexicographical order
	static bool cmp1(Word a, Word b)
	{
		return (a.word < b.word);
	}
	//compare by reverse-lexicographical order
	static bool cmp2(Word a, Word b)
	{
		return (a.word > b.word);
	}
	//compare by frequency high to low order
	static bool cmp3(Word a, Word b)
	{
		return (a.freq > b.freq);
	}
	//compare by frequency low to high order
	static bool cmp4(Word a, Word b)
	{
		return (a.freq < b.freq);
	}

protected:
	//function to get counter type
	void getCounterType()
	{
		string ctHelper;

		cout << "enter 0 to count words in a English passage, 1 to count keywords in a CPP file:";
		cin >> ctHelper;

		while (ctHelper[0] - '0' != 0 && ctHelper[0] - '0' != 1)
		{
			cout << "WRONG TYPE!\n";
			cout << "enter 0 to count words in a English passage, 1 to count keywords in a CPP file:";
			cin >> ctHelper;
		}

		if (ctHelper[0] - '0')
			ct = counterType::CPPCounter;
		else
			ct = counterType::WCounter;
	}

	//function to open target file
	void openFile()
	{
		cout << "Please enter the name of the file:";
		cin >> fileName;
		inFile.open(fileName);

		if (!inFile)
			cout << "input file does not exist!\n\n";
		else
			cout << "open file successful!\n\n";
	}

	//countFrequency
	void countFrequency(int type)
	{
		//clear up old vector 
		words.clear();
		//read from the beginning
		inFile.clear();
		inFile.seekg(ios::beg);

		if (ct == counterType::CPPCounter)
			initializeCPPCounter();

		string tmpLine;			//current line to read in
		string tmpWord;
		Word newWord;			//word to be push_back to the vertor
		int cnt;
		bool commentFlag = false;

		while (getline(inFile, tmpLine))
		{
			cnt = 0;
			cout << tmpLine << "\n";
			size_t len = tmpLine.length();

			//skip quote in CPP Counter
			if (ct == counterType::CPPCounter)
				skipQuote(tmpLine);
			
			for (size_t i = 0; i < len; i++)
			{
				if (ct == counterType::CPPCounter)
				{
					//skip commont in CPP counter
					// /*ahead, skip until find */
					if (commentFlag)
					{
						if (tmpLine.find("*/") != string::npos)
						{
							i = tmpLine.find("*/") + 2;
							commentFlag = false;
						}
						// */ not found in this line, then skip to next line
						else break;
					}
					else
					{
						//when comes to //, skip to next line
						if (tmpLine.find("//") != string::npos)
							len = tmpLine.find("//");
						//when come to /*
						else if (tmpLine.find("/*") != string::npos)
						{
							len = tmpLine.find("/*");
							commentFlag = true;
						}
					}
				}

				//if tmpLine[i] is a character, cnt++
				if (i < len - 1 && (tmpLine[i] >= 'a' && tmpLine[i] <= 'z') || (tmpLine[i] >= 'A' && tmpLine[i] <= 'Z')
					|| tmpLine[i] == '_')
				{
					cnt++;
				}

				//ensure first char in tmpLine is an English character 
				//then extract the substr from tmpLine
				else if (cnt)
				{
					//reach the end of line
					if ((tmpLine[i] >= 'a' && tmpLine[i] <= 'z') || (tmpLine[i] >= 'A' && tmpLine[i] <= 'Z')
						|| tmpLine[i] == '_')
					{
						cnt++, i++;
					}
					tmpWord = tmpLine.substr(i - cnt, cnt);
					newWord.word = tmpWord;

					//if count the frequency of words regardless of case
					if (type == 2)
						newWord.toLower();

					//search if this word exists in the vector
					bool findFlag = false;
					if (newWord.word.length())
					{
						for (auto& j : words)
						{
							//word exist
							if (newWord == j)
							{
								j.freq++;
								cnt = 0;
								findFlag = true;
								break;
							}
						}
					}
					//word not found and this is a wordCounter
					if (!findFlag && ct == counterType::WCounter)
						words.push_back(newWord);
					cnt = 0;
				}
			}
		}
	}

	//display and output
	void displayAndOutput(int type)
	{
		string outFileName = "result.txt";

		//insert out file number
		char tmp[100];
		sprintf(tmp, "%d", ++resCnt);
		outFileName.insert(6, tmp);
		ofstream outResult(outFileName);
		outResult << outFileName << "\n";
		outResult << "input file name:" << fileName << "\n";
		outResult << "sort type:";
		switch (type)
		{
		case 4://output results by lexicographical order
			outResult << "sorted by lexicographical order\n\n";
			sort(words.begin(), words.end(), cmp1);
			break;
		case 5://output results by reverse-lexicographical order
			outResult << "sorted by reverse-lexicographical order\n\n";
			sort(words.begin(), words.end(), cmp2);
			break;
		case 6://output results by frequency high to low order
			outResult << "sorted by frequency high to low order\n\n";
			sort(words.begin(), words.end(), cmp3);
			break;
		case 7://output results by frequency low to high order
			outResult << "sorted by frequency low to high order\n\n";
			sort(words.begin(), words.end(), cmp4);
			break;
		}
		//output
		for (auto i : words)
		{
			if (i.freq)
			{
				cout << i.word << ":\t" << i.freq << "\n";
				outResult << i.word << ":\t" << i.freq << "\n";
			}
		}
		cout << "\n";
		outResult.close();
	}
public:
	//default constructor is fine
	MyCounter() {};

	//destructor
	~MyCounter()
	{
		inFile.close();
	}

	//menu
	void menu()
	{
		//string type to avoid illegal input
		string choice;

		//show menu
		cout << "Welcome to StanwieCB's Words/CPP Counter!\n";
		getCounterType();
		while (true)
		{
			cout << "0:change counter type\n";
			cout << "1:enter the filename\n";
			cout << "2:count the frequency of words regardless of case\n";
			cout << "3:count the frequency of words by two cases\n";
			cout << "4:output results by lexicographical order\n";
			cout << "5:output results by reverse-lexicographical order\n";
			cout << "6:output results by frequency high to low order\n";
			cout << "7:output results by frequency low to high order\n";
			cout << "8:exist counter\n";
			cout << "\n";

			cout << "please enter your choice:(8 to quit)";
			cin >> choice;

			switch (choice[0] - '0')
			{
			case 0:
				getCounterType();
				words.clear();
				inFile.close();
				inFile.clear();
				inFile.seekg(ios::beg);
				break;
			case 1:
				words.clear();
				inFile.close();
				openFile();
				inFile.clear();
				inFile.seekg(ios::beg);
				break;
			case 2:case 3:
				countFrequency(choice[0] - '0');
				cout << "count finished!\n";
				break;
			case 4:case 5:case 6:case 7:
				displayAndOutput(choice[0] - '0');
				break;
			case 8:
				return;
			default:
				cout << "please enter a right choice\n";
				break;
			}
		}
	}

	void run() 
	{
		menu();
	}
};
#endif


