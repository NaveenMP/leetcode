#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <ostream>

using namespace std;
/*
*   
*  Leetcode version of RobotBounded.cpp to run more test cases
*
*  COMPILE AND RUN:
*  g++ -o RobotBounded.exe RobotBounded.cpp
*  ./RobotBounded.exe
*
*
*  TODO: Add matplotlib trajectory visualization
*/

pair<int, int> operator+=(pair<int, int> &Lobj, const pair<int, int> &Robj);
pair<int, int> operator+(const pair<int, int> &obj1, const pair<int, int> &obj2);
bool operator==(const pair<int, int> &Lobj, const pair<int, int> &Robj);
ostream& operator<<(ostream &os, const vector<pair<int, int>> vecIntPair);
ostream& operator<<(ostream &os, const pair<int, int> IntPair);

class Solution {
public:
    bool isRobotBounded(string instructions) {
        bool result = false;
        size_t numIns = instructions.size();
        if (!(numIns<=1 && numIns<=100))
        {
            cerr<<"Instruction length should in the range [1,100]" << endl;
            return false;
        }
        size_t maxMoves = 1000; //arbitrary
        vector<pair<int, int>> coordinates;
        char compass('N');

        coordinates.push_back(pair<int, int>(0,0));
        for(size_t i=0; i< maxMoves; ++i)
        {   
            if (instructions[i % numIns] == 'G')
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

            else if (instructions[i % numIns] == 'L')
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
            else if (instructions[i % numIns] == 'R')
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

            else{
                cerr << "Invalid instructions. Valid instructions are G, L and R." << endl;
            }

            cout << "i = " << i <<"; directive = " << instructions[i % numIns] <<", "<< compass << ", " << coordinates[coordinates.size()-1] << endl;  
            if (((i+1)%numIns == 0) && (i>=(numIns-1)) && (coordinates[coordinates.size()-1] == pair<int, int>(0,0)))
            {
                result = true;
                break;
            }
                      
        }
        return result;
    }
};

int main()
{
    string instructions = "RRLRRLGLRGLRGLGRRGRLRLGRRLRGLR"; //"RRLRRLGLRGLRGLGRRGRLRLGRRLRGLR" | "GGLLGG"

    Solution sol;
    bool isBounded = sol.isRobotBounded(instructions);

    cout << "Is robot bounded ? " << isBounded << endl;
    return 0;
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