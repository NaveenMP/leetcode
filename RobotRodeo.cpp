#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <ostream>

using namespace std;
/*
*   
*  INPUT
*  Vector of strings which contains path commands (Sequences containing G, L, R commands)
*
*  OUTPUT
*  vector of strings with YES or NO for each set of input command indication whether circle/closed-loop pattern is found 
*  
*  CONSTRAINTS:
*  1 <= |commands[i]| <= 2500
*  1 <= n <= 10
*  Each command consists of G, L, and R only
*/

vector <string> doesCircleExist(vector<string> commands);
pair<int, int> operator+=(pair<int, int> &Lobj, const pair<int, int> &Robj);
pair<int, int> operator+(const pair<int, int> &obj1, const pair<int, int> &obj2);
bool operator==(const pair<int, int> &Lobj, const pair<int, int> &Robj);
ostream& operator<<(ostream &os, const vector<pair<int, int>> vecIntPair);
ostream& operator<<(ostream &os, const pair<int, int> IntPair);

int main()
{
    vector<string> commands = {"GRGL", "RG"};
    vector <string> results = doesCircleExist(commands);

    for (auto i = 0; i < results.size(); ++i)
    {
        cout << results[i] << endl;
    }

    return 0;
}

vector <string> doesCircleExist(vector<string> commands)
{
    //Map GLR commands to NEWS commands(string) and coordinates(tuple)

    vector <string> results;
    results.resize(commands.size());

    for (size_t n = 0; n < commands.size(); ++n)
    {
        string commandDirectives = commands[n];
        string commandResult = "NO";
        size_t nDirectives = commandDirectives.size();
        size_t maxDirectives = 2500;
        vector<pair<int, int>> coordinates;
        char compass('N');

        coordinates.push_back(pair<int, int>(0,0));
        for(size_t i=0; i< maxDirectives; ++i)
        {   
            if (commandDirectives[i % nDirectives] == 'G')
            {
                pair<int,int> nextCoordinate;
                switch(compass)
                {
                    case 'N':
                        nextCoordinate = coordinates[coordinates.size()-1]+pair<int, int>(0,1);
                        break;
                    case 'E':
                        nextCoordinate = coordinates[coordinates.size()-1]+pair<int, int>(1,0);
                        break;
                    case 'W':
                        nextCoordinate = coordinates[coordinates.size()-1]+pair<int, int>(-1,0);
                        break;
                    case 'S':
                        nextCoordinate = coordinates[coordinates.size()-1]+pair<int, int>(0,-1);
                        break;   
                }
                coordinates.push_back(nextCoordinate);
            }

            if (commandDirectives[i % nDirectives] == 'L')
            {
                char compassUpdate(compass); 
                switch(compass)
                {
                    case 'N':
                        compassUpdate = 'W';
                        break; 
                    case 'E':
                        compassUpdate = 'N';
                        break; 
                    case 'W':
                        compassUpdate = 'S';
                        break; 
                    case 'S':
                        compassUpdate = 'E';
                        break;                         
                }
                compass = compassUpdate;
            }
            else if (commandDirectives[i % nDirectives] == 'R')
            {
                char compassUpdate(compass); 
                switch(compass)
                {
                    case 'N':
                        compassUpdate = 'E';
                        break;  
                    case 'E':
                        compassUpdate = 'S';
                        break; 
                    case 'W':
                        compassUpdate = 'N';
                        break; 
                    case 'S':
                        compassUpdate = 'W';
                        break; 
                    
                }
                compass = compassUpdate;
            }

            if ((i%nDirectives == 0) && (i>=(nDirectives-1)) && (coordinates[coordinates.size()-1] == pair<int, int>(0,0)))
            {
                commandResult = "YES";
                break;
            }
            //cout << "i = " << i <<"; directive = " << commandDirectives[i % nDirectives] <<", "<< compass << ", " << coordinates[coordinates.size()-1] << endl;            
        }

        //cout << endl << "For command " << commandDirectives << " : " << endl << coordinates << endl;
        
        results[n] = commandResult;

    }
    return results;

}

pair<int, int> operator+=(pair<int, int> &Lobj, const pair<int, int> &Robj)
{
    Lobj.first += Robj.first;
    Lobj.second += Robj.second;
    return Lobj;
}

pair<int, int> operator+(const pair<int, int> &obj1, const pair<int, int> &obj2)
{
    pair<int, int> result;
    result.first  = obj1.first + obj2.first;
    result.second = obj1.second + obj2.second;
    return result;
}

bool operator==(const pair<int, int> &Lobj, const pair<int, int> &Robj)
{
    bool res = false;
    if ((Lobj.first == Robj.first) && (Lobj.second == Robj.second))
        res = true;
    
    return res;
}

ostream& operator<<(ostream &os, const vector<pair<int, int>> vecIntPair)
{
    for (auto i=0; i<vecIntPair.size(); ++i)
    {
        if (i != vecIntPair.size()-1)
            os << "(" << vecIntPair[i].first << "," << vecIntPair[i].second << "),";
        else
            os << "(" << vecIntPair[i].first << "," << vecIntPair[i].second << ")" << endl;
    }
    return os; 
}

ostream& operator<<(ostream &os, const pair<int, int> IntPair)
{
    os << "(" << IntPair.first << "," << IntPair.second << ")";
    return os;
}