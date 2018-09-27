#include <string>
#include <sstream>
#include <vector>

using namespace std;
// wrap words by some length
std::string wrap(std::string text, size_t line_length)
{
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;
 
    if (words >> word) {
        wrapped << word;
        size_t space_left = line_length - word.length();
        while (words >> word) {
        //while (std::getline(words, word, '\n')) {
            if (space_left < word.length() + 1) {
                wrapped << '\n' << ' ' << word;
                space_left = line_length - word.length();
            } else {
                wrapped << ' ' << word;
                space_left -= word.length() + 1;
            }
        }
    }
    return wrapped.str();
}

// explode string by delimiter
std::vector<std::string> explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}


//char *convert(const std::string & s)
//{
   //char *pc = new char[s.size()+1];
   //std::strcpy(pc, s.c_str());
  //return pc; 
//}
