#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool isNullable(string s ,vector<char> nullableStates)
{
    bool flag = false;
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == '|')
        {
            if(flag)
                return true;
        }
        else if(s[i] == ' ')
            continue;
        else if(find(nullableStates.begin(), nullableStates.end(), s[i]) != nullableStates.end())
        {
            flag = true;
        }
        else
            flag = false;

    }
    return flag;
}

bool isExists(string s , string sub)
{
    vector<string> tokens;
    for(int i = 0; i < s.size(); i++)
    {
        string temp = "";
        while(i < s.size() && s[i] != '|')
        {
            if(s[i] != ' ')
                temp += s[i];
            i++;
        }
        if(temp != "")
            tokens.emplace_back(temp);
        i++;
    }
    for(int j = 0; j < tokens.size(); j++)
    {
        if(tokens[j] == sub)
            return true;
    }

    return false;
}

int main()
{
    vector<pair<char, string>> CFG;
    
    //test 1
    CFG.emplace_back('S', "AB");
    CFG.emplace_back('A', "BC | a");
    CFG.emplace_back('B', "AC | b | D");
    CFG.emplace_back('C', "c | e");
                            
    
    //test 2
    /*
    CFG.emplace_back('S', "AB | BE");
    CFG.emplace_back('A', "BA | a");
    CFG.emplace_back('B', "CC | b");
    CFG.emplace_back('C', "AB | a");
    CFG.emplace_back('G', "ABC");
                                    */

    string W = "aaccc";
    //String leng
    const int n = 5;

    // print out the inputs
    cout << "original input: " << endl;
    for(auto elem : CFG)
    {
        cout << elem.first << " -> " << elem.second << endl;
    }

    cout << endl;

    /*************************************************/
    // find the nullable States that goes to null directly and put them in the vector
    vector<char> nullableStates;
    for(auto elem : CFG)
    {
        for(int i = 0; i < elem.second.size(); i++)
        {
            // if second Elements == lambda, put first symbol into nullableStates vector
            if(elem.second[i] == 'e')
            {
                nullableStates.emplace_back(elem.first);
                break;
            }
        }
    }

    for(int j = 0; j < CFG.size(); j++)
    {
        // goes through all nullable states.
        if(find(nullableStates.begin(), nullableStates.end(), CFG[j].first) != nullableStates.end())
        {
            string temp = "" , tempString = CFG[j].second;
            vector<int> nullIndex ;
            for(int i = 0; i < CFG[j].second.size(); i++)
            {
                temp = "";
                // finds symbol in left side that matches right side
                if(CFG[j].second[i] == CFG[j].first)
                {
                    int index = i - 1;
                    //ignore '|' and ''
                    while(index >= 0 && CFG[j].second[index] != '|')
                    {
                        if(CFG[j].second[index] == ' ')
                        {
                            index--;
                            continue;
                        }
                        temp = CFG[j].second[index] + temp;
                        index--;
                    }
                    index = i + 1;
                    while(index < CFG[j].second.size() && CFG[j].second[index] != '|')
                    {
                        if(CFG[j].second[index] == ' ')
                        {
                            index++;
                            continue;
                        }
                        temp += CFG[j].second[index];
                        index++;
                    }
                    if(temp != "" && temp != " " && temp != "  " && !(isExists(CFG[j].second,temp)))
                    {
                        tempString += " | " + temp;
                    }
                    CFG[j].second = tempString;
                }
                if(CFG[j].second[i] == 'e')
                {
                    CFG[j].second.erase (i - 3 , 4);
                }
            }
        }
    }

    int currentSize = 0;

    while(currentSize != nullableStates.size())
    {
        currentSize = nullableStates.size();
        for(int j = 0; j < CFG.size(); j++)
        {
            if(!(find(nullableStates.begin(), nullableStates.end(), CFG[j].first) != nullableStates.end()))
            {
                string temp = "" , tempString = CFG[j].second;
                for(auto elem : nullableStates)
                {
                    for(int i = 0; i < CFG[j].second.size(); i++)
                    {
                        temp = "";
                        if(CFG[j].second[i] == elem)
                        {
                            int index = i - 1;
                            while(index >= 0 && CFG[j].second[index] != '|')
                            {
                                if(CFG[j].second[index] == ' ')
                                {
                                    index--;
                                    continue;
                                }
                                temp = CFG[j].second[index] + temp;
                                index--;
                            }
                            index = i + 1;
                            while(index < CFG[j].second.size() && CFG[j].second[index] != '|')
                            {
                                if(CFG[j].second[index] == ' ')
                                {
                                    index++;
                                    continue;
                                }
                                temp += CFG[j].second[index];
                                index++;
                            }
                            if(temp != "" && temp != " " && temp != "  " && !(isExists(CFG[j].second,temp)))
                            {
                                tempString += " | " + temp;
                            }
                        }
                        CFG[j].second = tempString;
                    }

                }
                if(isNullable(CFG[j].second,nullableStates))
                    nullableStates.emplace_back(CFG[j].first);
            }
        }
    }
    cout << "After Lambda" << endl;
    for(auto elem : CFG)
    {
        cout << elem.first << " -> " << elem.second << endl;
    }

    cout << endl;


    
    /**************************************************/
    //random symbol for new states
    
    char state = 240;
    char delimeter = '|';
    bool found = false;
    vector<int> numDel;
    vector<pair<char, string>> newCFG;

    //Create a new CFG where if there is a string with OR, "|", make it in to two sepearte state
    for(auto elem : CFG)
    {
        for(int i = 0; i < elem.second.size(); i++)
        {
            //Store all the places with OR, "|"
            if(elem.second[i] == delimeter)
            {
                numDel.push_back(i);
                found = true;
            } 
        }
        //If there is OR, "|", store all the string seperately with different char
        if(found)
        {
            string temp = elem.second.substr(0, numDel[0] - 1);
            newCFG.emplace_back(elem.first,temp);
            for(int i = 0; i < numDel.size(); i++)
            {
                if(i != numDel.size()-1)
                {
                    string temp = elem.second.substr(numDel[i] + 2, numDel[i+1] - numDel[i]-3);
                    newCFG.emplace_back(elem.first,temp);
                }
                else
                {
                    string temp = elem.second.substr(numDel[i] + 2, elem.second.size());
                    newCFG.emplace_back(elem.first,temp);
                    numDel.clear();
                }
            }
        }    
        //If not found, just copy the elem to the new CFG
        if (!found)
        {
            newCFG.emplace_back(elem);
        }
    }

    cout << "After getting rid of ORs" << endl;
    for(auto elem : newCFG)
    {
        cout << elem.first << " -> " << elem.second << endl;
    }

    cout << endl;


    
    //If the string is greater that two, seperate it into multiple states until all strings are less than equal to two
    for(int i = 0; i < newCFG.size(); i++)
    {
        if(newCFG[i].second.size() > 2)
        {    
            //Save one character from the string and insert the reset back to the vector
            string temp = newCFG[i].second.substr(1, newCFG[i].second.size()- 1);
            string temp2 = newCFG[i].second.substr(0, 1) + state;
            newCFG.insert(newCFG.begin() + i, pair<char,string>(newCFG[i].first, temp2));
            newCFG.insert(newCFG.begin() + i + 1, pair<char,string>(state, temp));

            //Erase the original string
            newCFG.erase(newCFG.begin() + i + 2);
            state--;

            //Loop one more time
            i--;
        }
    }


    

    //Map saves all the new states created with single variable
    map<string, char> additional;

    //Find a String that has lower case a through z in a string size equal to two
    for(int i = 0; i < newCFG.size(); i++)
    {
        for(int j = 0; j < newCFG[i].second.size(); j++)
        {
            //Found where  there is a lower case a through z in a string size equal to two
            if(newCFG[i].second[j] >= 'a' && newCFG[i].second[j] <= 'z' && newCFG[i].second.size() == 2)
            {
                string temp(1,newCFG[i].second[j]);
                //Find if that character was already saved previously
                map<string, char>::iterator it = additional.find(temp);

                //If it's already saved, replace the character with already created state
                if(it != additional.end())
                {
                    char foundChar = additional.at(temp);
                    newCFG[i].second[j] = foundChar;
                }
                //If it's not saved, create new state with the character
                else
                {
                    newCFG.emplace_back(state, temp);
                    additional.insert(pair<string,char>(temp, state));
                    newCFG[i].second[j] = state;
                    state--;
                }
            }
        }
    }

    cout << "after getting rid of unit production: " << endl;
    for(auto elem : newCFG)
    {
        cout << elem.first << " -> " << elem.second << endl;
    }

    
    bool uselessF;
    map<char,int> useless;
    for(auto elem : newCFG)
    {
        if(elem.first != 'S')
            useless.insert(pair<char,int>(elem.first,0));
    }

    for(int i=0;i<newCFG.size(); i++)
    {
        for(int j=0;j<newCFG[i].second.size();j++)
        {
            map<char, int>::iterator it = useless.find(newCFG[i].second[j]);
            if(it != useless.end() || newCFG[i].second[j] == 'a' || newCFG[i].second[j] == 'b')
            {
                it->second++;
            }
            
            //never gets found
            else if(newCFG[i].second[j] <= 'a' && newCFG[i].second[j] >= 'z' && it == useless.end())
            {
                newCFG.erase(newCFG.begin()+i);
                j--;
            }
            
        }
    }

    vector<char> noNeed;
    for(map<char,int>::iterator it = useless.begin();it != useless.end(); it++)
    {
        if(it->second == 0)
            noNeed.push_back(it->first); 
    }
    for(int i=0;i<newCFG.size(); i++)
    {
        for(int j = 0; j< noNeed.size(); j++)
        {
            if(newCFG[i].first == noNeed[j])
                newCFG.erase(newCFG.begin() + i);
        }
    }

    cout << "after useless: " << endl;
    for(auto elem : newCFG)
    {
        cout << elem.first << " -> " << elem.second << endl;
    }



    map<char, int> indexMap;
    map<int, char> charMap;
    int i = 0;
    for(auto it : CFG){
        if(indexMap.insert(pair<char,int>(it.first,i)).second){
            charMap.insert(pair<int,char>(i,it.first));
            i++;
        }
        
    }




    //declaration of constants
    const int R = newCFG.size(); //size of the CFG vector
    const int r = 4; //number of non-terminal variables

    //CYK algorithm
    bool P[n][n][r] = {false}; //initialize bool array P[y, x, r] to false

    //first step: populate first row of the array
    for(int s = 0; s < n; s++){ //iterate over W
        for(int v = 0; v < R; v++){ //iterate over productions of the CFG
            if(newCFG[v].second.find(W[s]) != string::npos)//if the production contains the terminal of the string
            P[0][s][indexMap[newCFG[v].first]] = true;
        }
    }

    //second step: fill in the rest of the array
    for(int L = 1; L < n; L++){ //L for current layer

        for(int S = 0; S < n-L; S++){ //S tracks x position

            for(int p = 0; p < L; p++){ //p tracks y position

            for(int v = 0; v < R; v++){ //cartesian product of the contents of the two cells
                for(int z = 0; z < R; z++){ //v indexes cell 1, z indexes cell 2

                    if(P[p][S][indexMap[newCFG[v].first]] == true && P[L-p-1][S+p+1][indexMap[newCFG[z].first]] == true){ //if the cartesian product has a valid result
                    string str; //create a string to be searched for, for example search for "AB" in the right hand side
                    str.push_back(newCFG[v].first);
                    str.push_back(newCFG[z].first);
                    
                        for(int x = 0; x < R; x++){ //iterate through the productions of the CFG to check if that combination exists
                        if(newCFG[x].second.find(str) != string::npos){ //if e.g "AB" was found on the right hand side
                            P[L][S][indexMap[newCFG[x].first]] = true; //set the target cell to true for the left hand side
                        }
                        }
                    }
                }
                

            
            }
            }
        }
    }

    //print array with symbols
    cout << endl << "Final state of the table" << endl;

    for(int i = 0; i < n; i++){

        for(int j = 0; j < n-i; j++){ //n-i for the staggered format
            cout << "[";
            for(int k = 0; k < r; k++){
            if(P[i][j][k] == true){
                cout << charMap[k];
            }
            }
            cout << "]";
        }
        cout << endl;
    }

    cout << endl;

    //if S is in the final cell, W is in the language
    if(P[n-1][0][0] == true){
        cout << "The string:" << W << " is a member of the language\n" << endl;
    }
    else cout << "The string:" << W << " is not a member of the language" << endl;


}
