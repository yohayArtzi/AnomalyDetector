#include "timeseries.h"
#include "CLI.h"
//#include "SimpleAnomalyDetector.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {

    CLI cli(new StandardIO());
    cli.start();

//    SimpleAnomalyDetector* sa = new SimpleAnomalyDetector();
//    cout << sa->getThreshold() << endl;
//    HybridAnomalyDetector* ha = new HybridAnomalyDetector();
//    cout<<ha->getThreshold()<<endl;


    /*
    vector<int> v;
    v.push_back(30);
    v.push_back(20);
    v.push_back(10);

    for(int i=0; i < 3; i++)
        cout << v[i] << endl;
*/

    //StandardIO sio;
    //string hi = "hi";
    //sio.write(hi);

/*
    TimeSeries ts("testMe.txt");
    map<string, vector<float>> featuresMap = ts.getMap();
    auto it = featuresMap.begin();
    for (; it != featuresMap.end(); it++) {
        cout << "feature " << it->first << ", values: ";
        for (float f : it->second)
            cout << f << " ";
        cout << endl;
    }
    */
    return 0;
}
