#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

// #include "createTable.h"
#include "exerciser.h"
#include "main.hpp"
using namespace std;
using namespace pqxx;
connection *C;

void createTable() {
    const char *sql =
        "DROP TABLE IF EXISTS PLAYER;"
        "DROP TABLE IF EXISTS TEAM;"
        "DROP TABLE IF EXISTS COLOR;"
        "DROP TABLE IF EXISTS STATE;"

        "CREATE TABLE STATE("
        "STATE_ID INT PRIMARY KEY     NOT NULL,"
        "NAME           CHAR(20)    NOT NULL );"

        "CREATE TABLE COLOR("
        "COLOR_ID INT PRIMARY KEY     NOT NULL,"
        "NAME           CHAR(20)    NOT NULL );"

        "CREATE TABLE TEAM("
        "TEAM_ID INT PRIMARY KEY     NOT NULL,"
        "NAME           CHAR(20)    NOT NULL,"
        "STATE_ID            INT     NOT NULL,"
        "COLOR_ID        INT NOT NULL,"
        "WINS        INT NOT NULL,"
        "LOSSES        INT NOT NULL,"
        "FOREIGN KEY (STATE_ID) REFERENCES STATE(STATE_ID), "
        "FOREIGN KEY (COLOR_ID) REFERENCES COLOR(COLOR_ID) );"

        "CREATE TABLE PLAYER("
        "PLAYER_ID INT PRIMARY KEY     NOT NULL,"
        "TEAM_ID           INT    NOT NULL,"
        "UNIFORM_NUM            INT     NOT NULL,"
        "FIRST_NAME        CHAR(20),"
        "LAST_NAME        CHAR(20),"
        "MPG        int NOT NULL,"
        "PPG        int NOT NULL,"
        "RPG        int NOT NULL,"
        "APG        int NOT NULL,"
        "SPG        float NOT NULL,"
        "BPG         float NOT NULL,"
        "FOREIGN KEY (TEAM_ID) REFERENCES TEAM(TEAM_ID) );";

    work W(*C);
    W.exec(sql);
    W.commit();
    cout << "Table created successfully" << endl;
}
void ReadPlayer() {
    string filename = "player.txt";
    ifstream file(filename, std::ifstream::in);
    string str;
    int PLAYER_ID, TEAM_ID, UNIFORM_NUM, MPG, PPG, RPG, APG;
    string FIRST_NAME, LAST_NAME;
    double SPG, BPG;
    while (getline(file, str)) {
        stringstream ss(str);
        ss >> PLAYER_ID >> TEAM_ID >> UNIFORM_NUM >> FIRST_NAME >> LAST_NAME >> MPG >> PPG >> RPG >> APG >> SPG >> BPG;
        sanitize(FIRST_NAME);
        sanitize(LAST_NAME);
        addPlayer(PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG);
    }
}
void addPlayer(int PLAYER_ID, int TEAM_ID, int UNIFORM_NUM, string FIRST_NAME, string LAST_NAME, int MPG, int PPG, int RPG, int APG, double SPG, double BPG) {
    const char *sql =
        "INSERT INTO PLAYER (PLAYER_ID,TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << PLAYER_ID << "," << TEAM_ID << "," << UNIFORM_NUM << ",'" << FIRST_NAME << "','" << LAST_NAME << "'," << MPG << "," << PPG << "," << RPG << "," << APG << "," << SPG << "," << BPG << ");";
    work W(*C);
    W.exec(ss.str());
    W.commit();
}
void ReadTeam() {
    string filename = "team.txt";
    ifstream file(filename, std::ifstream::in);
    string str;
    int TEAM_ID, STATE_ID, COLOR_ID, WINS, LOSSES;
    string NAME;
    while (getline(file, str)) {
        stringstream ss(str);
        ss >> TEAM_ID >> NAME >> STATE_ID >> COLOR_ID >> WINS >> LOSSES;
        sanitize(NAME);
        addTeam(TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES);
    }
}
void addTeam(int TEAM_ID, string NAME, int STATE_ID, int COLOR_ID, int WINS, int LOSSES) {
    const char *sql =
        "INSERT INTO TEAM (TEAM_ID,NAME,STATE_ID,COLOR_ID,WINS,LOSSES) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << TEAM_ID << ",'" << NAME << "'," << STATE_ID << "," << COLOR_ID << "," << WINS << "," << LOSSES << ");";
    work W(*C);
    W.exec(ss.str());
    W.commit();
    cout << "Insert value successfully" << endl;
}
void ReadState() {
    string filename = "state.txt";
    ifstream file(filename, std::ifstream::in);
    string str;
    int STATE_ID;
    string NAME;
    while (getline(file, str)) {
        stringstream ss(str);
        ss >> STATE_ID >> NAME;
        sanitize(NAME);
        addState(STATE_ID, NAME);
    }
}
void addState(int STATE_ID, string NAME) {
    const char *sql =
        "INSERT INTO STATE (STATE_ID,NAME) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << STATE_ID << ",'" << NAME << "');";
    work W(*C);
    W.exec(ss.str());
    W.commit();
    cout << "Insert value successfully" << endl;
}
void ReadColor() {
    string filename = "color.txt";
    ifstream file(filename, std::ifstream::in);
    string str;
    int COLOR_ID;
    string NAME;
    while (getline(file, str)) {
        stringstream ss(str);
        ss >> COLOR_ID >> NAME;
        sanitize(NAME);
        addColor(COLOR_ID, NAME);
    }
}
void addColor(int COLOR_ID, string NAME) {
    const char *sql =
        "INSERT INTO COLOR (COLOR_ID,NAME) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << COLOR_ID << ",'" << NAME << "');";
    work W(*C);
    W.exec(ss.str());
    W.commit();
}
void ReadAll() {
    ReadColor();
    ReadState();
    ReadTeam();
    ReadPlayer();
}
int main(int argc, char *argv[]) {
    // Allocate & initialize a Postgres connection object

    try {
        // Establish a connection to the database
        // Parameters: database name, user name, user password
        C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
        if (C->is_open()) {
            cout << "Opened database successfully: " << C->dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }
    createTable();
    // TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
    //       load each table with rows from the provided source txt files
    ReadAll();
    exercise(C);

    // Close database connection
    C->disconnect();

    return 0;
}
