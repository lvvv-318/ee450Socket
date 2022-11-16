// using g++ -o serverEE -std=c++11 serverEE.cpp
# include <iostream>
# include <sstream>
# include <iomanip>
# include <list>
# include <set>
# include <vector>
# include <unordered_map>
# include <cstring>
# include <fstream>
# include <queue>
# include <limits>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <map>

using namespace std;

const string INPUT = "ee.txt";

int main(int argc, const char* argv[]) {

    map<string, vector<string>> ee_map;

    ifstream ins(INPUT);

    // read data from the txt file
    string line;
    while(getline(ins, line)) {
        stringstream ss(line);
        string col1, col2, col3, col4, col5;  
        getline(ss, col1, ',');
        getline(ss, col2, ',');
        getline(ss, col3, ',');
        getline(ss, col4, ',');
        getline(ss, col5);
        vector<string> values;
        values.push_back(col2);
        values.push_back(col3);
        values.push_back(col4);
        values.push_back(col5);
        ee_map.insert(make_pair(col1, values));
    }

    // for(map<string,vector<string>>::iterator itr=ee_map.begin();itr!=ee_map.end();itr++){
    //     cout<<"Course Code: "<<itr->first<< endl<<"values: "<<itr->second[0] << " , " <<
    //     itr->second[1]<<" , " <<itr->second[2]<<" , "<<itr->second[3]<<endl;
    // }
    
    return 0;

}