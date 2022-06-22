#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <sstream>

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    void upload(const char *fileName) {

        // create file
        ofstream file;
        file.open(fileName);

        // upload to file
        string line = this->read();
        while (line != "done" && line != "done\n") {
            file << line << endl;
            line = this->read();
        }
        file.close();
    }

    virtual ~DefaultIO() {};

};

class StandardIO : public DefaultIO {
public:
    StandardIO() {};

    string read() {
        string s;
        cin >> s;
        return s;
    }

    void read(float *f) {
        cin >> *f;
    }

    void write(string text) {
        cout << text;
    }

    void write(float f) {
        cout << f;
    }

    ~StandardIO() {}
};


typedef struct command_helper {
    TimeSeries *trainFile;
    TimeSeries *anomaliesFile;
    HybridAnomalyDetector *detector = new HybridAnomalyDetector();
    vector<AnomalyReport> ars;
} command_helper;


class Command {
protected:
    DefaultIO *dio;
    string description;
    command_helper *helper;
public:
    Command(DefaultIO *dio, command_helper *helper_data) : dio(dio), helper(helper_data) {}

    string getDescription() {
        return description;
    }

    virtual void execute() = 0;

    virtual ~Command() {}
};

// implement here your command classes

class UploadCommand : public Command {
public:
    UploadCommand(DefaultIO *dio, command_helper *ch) : Command(dio, ch) {
        this->description = "1.upload a time series csv file\n";
    }

    void execute() override {

        const char *trainFileName = "train.csv";
        const char *anomaliesFileName = "test.csv";
        this->dio->write("Please upload your local train CSV file.\n");
        this->dio->upload(trainFileName);
        this->dio->write("Upload complete.\n");
        this->dio->write("Please upload your local test CSV file.\n");
        this->dio->upload(anomaliesFileName);
        this->dio->write("Upload complete.\n");

        // update command_helper
        this->helper->trainFile = new TimeSeries(trainFileName);
        this->helper->anomaliesFile = new TimeSeries(anomaliesFileName);
    }

    ~UploadCommand() {}
};

class ThresholdCommand : public Command {
public:
    ThresholdCommand(DefaultIO *defaultIO, command_helper *ch) : Command(defaultIO, ch) {
        this->description = "2.algorithm settings\n";
    }

    void execute() override {

        float threshold = 2;
        this->dio->write("The current correlation threshold is ");
        this->dio->write(this->helper->detector->getThreshold());
        this->dio->write("\n");
        this->dio->write("Type a new threshold\n");
        this->dio->read(&threshold);

        while (threshold < 0 || threshold > 1) {
            this->dio->write("please choose a value between 0 and 1.\n");
            this->dio->write("Type a new threshold\n");
            this->dio->read(&threshold);
        }
        this->helper->detector->setThreshold(threshold);
    }

    ~ThresholdCommand() {}
};

class AnomalyDetectionCommand : public Command {
public:
    AnomalyDetectionCommand(DefaultIO *defaultIO, command_helper *ch) : Command(defaultIO, ch) {
        this->description = "3.detect anomalies\n";
    }

    void execute() override {
        this->helper->detector->learnNormal(*this->helper->trainFile);
        this->helper->ars = this->helper->detector->detect(*this->helper->anomaliesFile);
        this->dio->write("anomaly detection complete.\n");
    }

    ~AnomalyDetectionCommand() {}
};

class DisplayResultsCommand : public Command {
public:
    DisplayResultsCommand(DefaultIO *defaultIO, command_helper *ch) : Command(defaultIO, ch) {
        this->description = "4.display results\n";
    }

    void execute() override {
        vector<AnomalyReport> *ars_ptr = &(this->helper->ars);
        int size = ars_ptr->size();
        for (int i = 0; i < size; i++) {
            this->dio->write((*ars_ptr)[i].timeStep);
            this->dio->write("\t");
            this->dio->write((*ars_ptr)[i].description);
            this->dio->write("\n");
        }
        this->dio->write("Done.\n");
    }

    ~DisplayResultsCommand() {}
};

typedef struct range {
    int start;
    int end;
} range;

class UploadAndAnalyzeCommand : public Command {
public:
    UploadAndAnalyzeCommand(DefaultIO *defaultIO, command_helper *ch) : Command(defaultIO, ch) {
        this->description = "5.upload anomalies and analyze results\n";
    }

    /*
     * converting float to string and returns only three digits after the dot
     */
    string readyToBePrinted(float inFloat) {
        ostringstream oss;
        oss << inFloat;
        string inString(oss.str());
        string delim = ".";
        int delimIndex = inString.find(delim);
        string result = inString.substr(0, delimIndex + 4);
        // lose zeros
        string::iterator it = result.end();
        for (int i = 0; i < 3; i++) {
            if (*(it - 1) == '0')
                result.pop_back();
            else
                break;
            it = result.end();
        }
        it = result.end();
        if (*(it - 1) == '.')
            result.pop_back();
        return result;
    }

    range extractInts(string anomaly) {
        range result;
        string delim = ",";
        int delimIndex = anomaly.find(delim);
        result.start = stoi(anomaly.substr(0, delimIndex));
        result.end = stoi(anomaly.substr(delimIndex + 1));

        return result;
    }

    map<int, string> createReports(vector<AnomalyReport> ars) {
        map<int, string> reports;
        int i = 0, j = 0;
        string currentDes = ars[0].description;
        long report_start = ars[0].timeStep;

        for (i = 0; i < ars.size(); i++) {
            if (ars[i].description != currentDes || ars[i].timeStep > ars[i - 1].timeStep + 1) {
                string format = to_string(report_start) + "," + to_string(ars[i - 1].timeStep);
                reports[j] = format;
                j++;
                currentDes = ars[i].description;
                report_start = ars[i].timeStep;
            }
        }
        // add last report
        string format = to_string(report_start) + "," + to_string(ars[i - 1].timeStep);
        reports[j] = format;

        return reports;
    }

    void sortMap(map<int, string> *myMap) {
        map<int, string> result;

        for (int i = 0; i < myMap->size() - 1; i++) {
            for (int j = 0; j < myMap->size() - i - 1; j++) {
                range r1 = extractInts((*myMap)[j]);
                range r2 = extractInts((*myMap)[j + 1]);
                if (r1.start > r2.start) {
                    result[0] = (*myMap)[j];
                    (*myMap)[j] = (*myMap)[j + 1];
                    (*myMap)[j + 1] = result[0];
                }
            }
        }
    }

    /*
     * in this function we will distinguish between 'reports' that created by the anomaly detector
     *  and 'anomalies' that given by the client. in addition, we will use 'i' as the index of reports,
     *  and 'j' as the index of anomalies.
     *
     */
    void execute() override {
        // initializations
        float n = this->helper->anomaliesFile->length();
        float p = 0.0, fp = 0.0, tp = 0.0;
        int i = 0, j = 0, sum = 0;

        // create unified anomalies
        this->dio->write("Please upload your local anomalies file.\n");
        string anomaly = this->dio->read();
        map<int, string> anomalies;
        while (anomaly != "done" && anomaly != "done\n") {
            p++;
            anomalies[j] = anomaly;
            j++;
            anomaly = this->dio->read();
        }
        sortMap(&anomalies);

        // create unified reports
        vector<AnomalyReport> ars = this->helper->ars;
        map<int, string> reports = createReports(ars);
        sortMap(&reports);

        // find n
        for (j = 0; j < anomalies.size(); j++) {
            range r = extractInts(anomalies[j]);
            sum += r.end - r.start + 1;
        }
        n -= sum;

        // find fp and tp
        range r1 = extractInts(anomalies[0]);
        long anomaly_start = r1.start;
        long anomaly_end = r1.end;
        for (i = 0, j = 0; i < reports.size(); i++) {
            range r2 = extractInts(reports[i]);
            long report_start = r2.start;
            long report_end = r2.end;

            // case 1 - report ends before anomaly start
            if (report_end < anomaly_start) {
                fp++;
                continue;
            }

            // case 3 - report starts after anomaly ends
            if (report_start > anomaly_end) {
                if (j == anomalies.size() - 1) {
                    fp++;
                } else {
                    j++;
                    range r1 = extractInts(anomalies[j]);
                    anomaly_start = r1.start;
                    anomaly_end = r1.end;
                    i--;
                }
                continue;
            }

            // case 2 - there's an overlap between anomaly and report
            if (report_end > anomaly_start) {
                if (report_end < anomaly_end)
                    tp++;
                else {
                    tp++;
                    if (j == anomalies.size() - 1)
                        continue;
                    j++;
                    range r1 = extractInts(anomalies[j]);
                    anomaly_start = r1.start;
                    anomaly_end = r1.end;
                }

            }


        }

        // print results
        float tpRate = ((int)(1000.0*tp/p))/1000.0f;
        float fpRate = ((int)(1000.0*fp/n))/1000.0f;
        this->dio->write("Upload complete.\n");
        this->dio->write("True Positive Rate: ");
        this->dio->write(tpRate);
        this->dio->write("\n");
        this->dio->write("False Positive Rate: ");
        this->dio->write(fpRate);
        this->dio->write("\n");
    }

    ~UploadAndAnalyzeCommand() {}
};

class ExitCommand : public Command {
public:
    ExitCommand(DefaultIO *defaultIO, command_helper *ch) : Command(defaultIO, ch) {
        this->description = "6.exit\n";
    }

    void execute() override {}

    ~ExitCommand() {}


};

#endif /* COMMANDS_H_ */
