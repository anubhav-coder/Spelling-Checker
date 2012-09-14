#ifndef _SPELLCORRECTOR_H_
#define _SPELLCORRECTOR_H_

# include<vector>
# include<unordered_map>
//using namespace std::tr1;
class SpellCorrector
{
private:
	typedef std::vector<std::string> Vector;
	typedef std::tr1::unordered_map<std::string,int>Dictionary;
	Dictionary dictionary;
	void edits(const std::string &word,Vector &result);
	void known(Vector &result, Dictionary &candidates);
	
public:
	void load(const std::string &filename);
	std::string correct(const std::string &word);
};
#endif