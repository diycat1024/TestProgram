#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string>::iterator itor;
        for(itor = words.begin(); itor!= words.end(); itor++) {

            itor = words.erase(itor);
        }
        return words;
    }
};