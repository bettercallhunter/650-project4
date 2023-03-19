#include <string>
#ifndef _MAIN_
#define _MAIN_
using namespace std;
void addPlayer(int PLAYER_ID, int TEAM_ID, int UNIFORM_NUM, string FIRST_NAME, string LAST_NAME, int MPG, int PPG, int RPG, int APG, double SPG, double BPG);
void createTable();
void ReadPlayer();
void sanitize(string &str);
void ReadTeam();
void addTeam(int TEAM_ID, string NAME, int STATE_ID, int COLOR_ID, int WINS, int LOSSES);
void ReadState();
void addState(int STATE_ID, string NAME);
void ReadColor();
void addColor(int COLOR_ID, string NAME);
#endif