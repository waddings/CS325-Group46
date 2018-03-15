#include <iostream>
#include <fstream>
#include <string.h>
#include <string>

using namespace std;

struct city{
    int name, x, y;
};

struct city *buildCityList(char**);

int main(int argc, char* argv[]){

  //  if(argv != 2)
    //    exit(1);
    struct city *cities = buildCityList(argv);

    return 0;
}

struct city *buildCityList(char* argv[]){
    int n = 0;
    char filename[50];
    ifstream f;
    strcpy(filename, argv[1]);
  //  cout << filename << endl;
    f.open(filename);

    string buffer;
    while(getline(f, buffer))
        n++;
  //  cout << "n is: " << n << endl;
    f.clear();
    f.seekg(0);
    struct city* cityList = new struct city[n];
    int i = 0;
    while(!f.eof()){
      f >> cityList[i].name >> cityList[i].x >> cityList[i].y;
      i++;
    }

    return cityList;
}
