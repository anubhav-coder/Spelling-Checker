# include<fstream>
# include<string>
# include<algorithm>
# include<iostream>
# include "SpellCorrector.h"

using namespace std;
bool sortBySecond(const pair<std::string,int> &left, const pair<std::string,int> &right)
{
	return left.second<right.second;
}
char filterNonAlphabetic(char &letter)
{
	if(isalpha(letter))
		return tolower(letter);
	return '-';
}
void SpellCorrector::load(const std::string &filename)
{
	ifstream file(filename.c_str(),ios_base::binary|ios_base::in);//construct object and optionally open file
	file.seekg(0,ios_base::end);//assign the pointer to read the words at the last
	int length=file.tellg();//using position of the pointer at the last getting the length of the file
	file.seekg(0,ios_base::beg);//assigning the pointer at the beginning
	string line(length,'0');//string of the name line is formed having all '0' characters and length of length...
	file.read(&line[0],length);//read the data of n characters and store it as a array of characters..
	transform(line.begin(),line.end(),line.begin(),filterNonAlphabetic);//apply on every element of the array the unary operation as given by the filterNonAlphabetic function
	string::size_type begin=0;//size_type provide string type that can hold any large string size
	string::size_type end=line.size();
	for(string::size_type i=0;;)
	{
		while(line[++i]=='-' && i<end);//find first '-'character
		if(i>=end){break;}
		begin=i;
		while(line[++i]!='-' &&i<end);//find first of not '-' character
		dictionary[line.substr(begin,i-begin)]++;//see how it happens here
	}
}
void SpellCorrector::edits(const std::string &word, Vector &result)
{
	for(string::size_type i=0;i<word.size();i++)//deleting
	{
		//pushing to result the word after deleting 1 character at position i
		result.push_back(word.substr(0,i)+word.substr(i+1));//deleting eath time ith character i.e 1 character each time
	}
	for(string::size_type i=0;i<word.size()-1;i++)//trainsposition
	{
		result.push_back(word.substr(0,i)+word[i+1]+word.substr(i+2));//again to see how it happens
	}
	for(char j='a';j<='z';j++)
	{
		for(string::size_type i=0;i<word.size();i++)//alteration
			result.push_back(word.substr(0,i)+j+word.substr(i+1));//replacing each word by another character
		for(string::size_type i=0;i<word.size()+1;i++)//insertion
			result.push_back(word.substr(0,i)+j+word.substr(i));//inserting at the position i
	}
}
void SpellCorrector::known(SpellCorrector::Vector &result, SpellCorrector::Dictionary &candidates)
{
	Dictionary::iterator end=dictionary.end();//return an iterator to the end of the unordered_map
	for(unsigned int i=0;i<result.size();i++)
	{
		Dictionary::iterator value=dictionary.find(result[i]);
		if(value!=end) 
			candidates[value->first]=value->second;
	}
}
string SpellCorrector::correct(const std::string &word)
{
	Vector result;
	Dictionary candidates;
	if(dictionary.find(word)!=dictionary.end())
		return word;
	edits(word,result);
	known(result,candidates);
	if(candidates.size()>0)
	{
		return max_element(candidates.begin(),candidates.end(),sortBySecond)->first;
	}
	for(unsigned int i=0;i<result.size();i++)
	{
		Vector subResult;
		edits(result[i],subResult);
		known(subResult,candidates);
	}
	if(candidates.size()>0)
	{
		return max_element(candidates.begin(),candidates.end(),sortBySecond)->first;
	}
	return "";
}
int main()
{
	SpellCorrector corrector;
	corrector.load("big.txt");
	string request;
	while(request!="quit")
	{
		cout<<"Type one word\n";
		cin>>request;
		string correct(corrector.correct(request));
		if(correct!="")
			cout<<"Did you mean: "<<correct<<"?\n\n\n";
		else
			cout<<"No Corrections available\n\n\n";
	}
	return 0;
}