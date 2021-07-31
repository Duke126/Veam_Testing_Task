#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <string>
#include"windows.h"

using namespace std;

string getSource(ifstream *configFile, int N);
string getDestination(ifstream *configFile, int N);
string getName(ifstream *configFile, int N);
int spaceBeforeAdress(string line);

string getSource(ifstream *configFile, int N){

    string line, addressSource;
    int i, space;
    int n = 0;
    int backslashNum = 0;
    int h=0;

    while ( getline (*configFile,line) ) {
        string::size_type index = line.find("source_path");
        if (string::npos != index){ ///Check as find() returns string::npos in case of absence source_path
            if (n == N){
                i=spaceBeforeAdress(line); /// Space before first "
                space = i;
                ///Count the number of backslashes
                for (int j=0; j<(line.size()); j++){
                        if (line[j]=='\\'){
                            backslashNum = backslashNum+1;
                        }
                }

                addressSource.resize( (line.size()-space-1+backslashNum) );
                for (i; i<(line.size()-1); i++){
                    if (line[i] != '\\') addressSource[i-space+h] = line[i];
                    else{
                        addressSource[i-space+h] = line[i]; /// Change \ from configFile to \\ for copy function
                        addressSource[i-space+h+1] = line[i];
                        h++;
                    }
                }
                n=n+1;
            }
            else n=n+1;
        }
    }

    configFile->clear();
    configFile->seekg(0, ios_base::beg);

    return addressSource;
}

string getDestination(ifstream *configFile, int N){

    string line, addressDestination;
    int i, space;
    int n = 0;
    int backslashNum = 0;
    int h=0;

    while ( getline (*configFile,line) ) {
        string::size_type index = line.find("destination_path");
        if (string::npos != index){
            if (n == N){
                i=spaceBeforeAdress(line);
                space = i;
                for (int j=0; j<(line.size()); j++){
                        if (line[j]=='\\'){
                            backslashNum = backslashNum+1;
                        }
                }

                addressDestination.resize( (line.size()-space-1+backslashNum) );
                for (i; i<(line.size()-1); i++){
                    if (line[i] != '\\') addressDestination[i-space+h] = line[i];
                    else{
                        addressDestination[i-space+h] = line[i]; /// Change \ from configFile to \\ for copy function
                        addressDestination[i-space+h+1] = line[i];
                        h++;
                    }
                }
                n=n+1;
            }
            else n=n+1;
        }
    }

    configFile->clear();
    configFile->seekg(0, ios_base::beg);

    return addressDestination;
}

string getName(ifstream *configFile, int N){

    string line, fileName;
    int i, space;
    int n = 0;

    while ( getline (*configFile,line) ) {
        string::size_type index = line.find("file_name");
        if (string::npos != index){
            if (n == N){
                i=spaceBeforeAdress(line); /// Space before first "
                space = i;
                fileName.resize( (line.size()-space-1) );
                for (i; i<(line.size()-1); i++){
                    fileName[i-space] = line[i];
                }
                n=n+1;
            }
            else n=n+1;
        }
    }
    configFile->clear();
    configFile->seekg(0, ios_base::beg);

    return fileName;
}

int spaceBeforeAdress(string line){

    int space = 0;

    string::size_type index = line.find('\"');
    if (string::npos != index){
        while (line[space] != '\"'){
            space++;
        }
    }

    return space+1;
}

int main()
{
    int N = 0; /// Number of relocatable files
    int i, space;
    int a = 0;
    string line, addressSource, addressDestination, fileName;
    ifstream configFile ("config.xml");

    if (configFile.is_open()){

        while ( getline (configFile,line) ) {
            string::size_type index = line.find("<file");
            if (string::npos != index){
                N = N + 1;
            }
        }

        configFile.clear();
        configFile.seekg(0, ios_base::beg);

        for (int j=0; j<N; j++){
            addressSource = getSource(&configFile, j);
            addressDestination = getDestination(&configFile, j);
            fileName = getName(&configFile, j);

            addressSource = addressSource + '\\' + '\\' + fileName;
            cout << addressSource.c_str() << "!" << endl;
            addressDestination = addressDestination + '\\' + '\\' + fileName;
            cout << addressDestination.c_str() << "!" << endl;

            a=CopyFile(addressSource.c_str(), addressDestination.c_str(), true);
        }

        configFile.close();
    }
    else cout << "File cann't be opened" << endl;
    return 0;
}
