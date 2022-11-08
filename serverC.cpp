// using g++ -o serverC -std=c++11 serverC.cpp
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
#include <map>

using namespace std;

const string INPUT = "cred.txt";

int main(int argc, const char* argv[]) {

    map<string, string> cred_map;

    ifstream ins(INPUT);

    // read data from the txt file
    string line;
    while(getline(ins, line)) {
        stringstream ss(line);
        string username;
        string password;
        getline(ss, username, ',');
        ss >> password;
        cred_map.insert(make_pair(username, password));
    }

    for(map<string,string>::iterator itr=cred_map.begin();itr!=cred_map.end();itr++){
        cout<<"Username: "<<itr->first<<" Password: "<<itr->second<<endl;
    }
    
    return 0;

}