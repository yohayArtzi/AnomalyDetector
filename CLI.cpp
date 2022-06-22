#include "CLI.h"

CLI::CLI(DefaultIO *defaultIO) : dio(defaultIO), ts(nullptr) {

    // create commands
    command_helper *ch = new command_helper;
    Command *upload = new UploadCommand(dio, ch);
    Command *threshold = new ThresholdCommand(dio, ch);
    Command *detectAnomalies = new AnomalyDetectionCommand(dio, ch);
    Command *display = new DisplayResultsCommand(dio, ch);
    Command *analyze = new UploadAndAnalyzeCommand(dio, ch);
    Command *exit = new ExitCommand(dio, ch);

    // add commands to vector
    this->cmds.push_back(upload);
    this->cmds.push_back(threshold);
    this->cmds.push_back(detectAnomalies);
    this->cmds.push_back(display);
    this->cmds.push_back(analyze);
    this->cmds.push_back(exit);
}

int readDigit(DefaultIO *defaultIO) {
    string in = defaultIO->read();
    return std::stoi(in);
}

void CLI::start() {

    int opt = 0;
    while (opt != 6) {

        // print menu
        this->dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        for (Command *cmd: this->cmds) {
            this->dio->write(cmd->getDescription());
        }

        // manage client's choices
        opt = readDigit(dio);
        cmds[opt - 1]->execute();
    }
}


CLI::~CLI() {
    for (Command *cmd: this->cmds)
        delete cmd;
}

