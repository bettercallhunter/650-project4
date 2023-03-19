#include "query_funcs.h"

void sanitize(string &str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\'') {
            str.insert(i, "'");
            i++;
        }
    }
}
void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)

{
    sanitize(first_name);
    sanitize(last_name);
    work W(*C);
    const char *retrive = "SELECT * from PLAYER order by PLAYER_ID desc limit 1; ";
    result R(W.exec(retrive));

    int PLAYER_ID = atoi(R[0]["PLAYER_ID"].c_str()) + 1;

    const char *sql =
        "INSERT INTO PLAYER (PLAYER_ID,TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << PLAYER_ID << "," << team_id << "," << jersey_num << ",'" << first_name << "','" << last_name << "'," << mpg << "," << ppg << "," << rpg << "," << apg << "," << spg << "," << bpg << ");";
    W.exec(ss.str());
    W.commit();
}
void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {
    sanitize(name);
    work W(*C);
    const char *retrive = "SELECT * from TEAM order by TEAM_ID desc limit 1; ";
    result R(W.exec(retrive));

    int TEAM_ID = atoi(R[0]["TEAM_ID"].c_str()) + 1;

    const char *sql =
        "INSERT INTO TEAM (TEAM_ID,NAME,STATE_ID,COLOR_ID,WINS,LOSSES) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << TEAM_ID << ",'" << name << "'," << state_id << "," << color_id << "," << wins << "," << losses << ");";
    W.exec(ss.str());
    W.commit();
}
void add_state(connection *C, string name) {
    sanitize(name);
    work W(*C);
    const char *retrive = "SELECT * from STATE order by STATE_ID desc limit 1; ";
    result R(W.exec(retrive));

    int STATE_ID = atoi(R[0]["STATE_ID"].c_str()) + 1;

    const char *sql =
        "INSERT INTO STATE (STATE_ID,NAME) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << STATE_ID << ",'" << name << "');";
    W.exec(ss.str());
    W.commit();
}

void add_color(connection *C, string name) {
    sanitize(name);
    work W(*C);
    const char *retrive = "SELECT * from COLOR order by COLOR_ID desc limit 1; ";
    result R(W.exec(retrive));

    int COLOR_ID = atoi(R[0]["COLOR_ID"].c_str()) + 1;

    const char *sql =
        "INSERT INTO COLOR (COLOR_ID,NAME) "
        "VALUES ";
    stringstream ss(sql);
    ss << sql << "(" << COLOR_ID << ",'" << name << "');";
    W.exec(ss.str());
    W.commit();
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
void query1(connection *C,
            int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg) {
    string sql = "SELECT * FROM PLAYER ";
    stringstream ss;
    ss << sql;
    int flag = 0;
    if (use_mpg) {
        ss << " WHERE MPG BETWEEN " << min_mpg << " AND " << max_mpg;
        flag = 1;
    }
    if (use_ppg) {
        if (!flag) {
            ss << " WHERE ";
            flag = 1;
        } else {
            ss << " AND ";
        }
        ss << " PPG BETWEEN " << min_ppg << " AND " << max_ppg;
    }
    if (use_rpg) {
        if (!flag) {
            ss << " WHERE ";
            flag = 1;
        } else {
            ss << " AND ";
        }
        ss << " RPG BETWEEN " << min_rpg << " AND " << max_rpg;
    }
    if (use_apg) {
        if (!flag) {
            ss << " WHERE ";
            flag = 1;
        } else {
            ss << " AND ";
        }
        ss << " APG BETWEEN " << min_apg << " AND " << max_apg;
    }
    if (use_spg) {
        if (!flag) {
            ss << " WHERE ";
            flag = 1;
        } else {
            ss << " AND ";
        }
        ss << " APG BETWEEN " << min_spg << " AND " << max_spg;
    }
    if (use_bpg) {
        if (!flag) {
            ss << " WHERE ";
            flag = 1;
        } else {
            ss << " AND ";
        }
        ss << " BPG BETWEEN " << min_bpg << " AND " << max_bpg;
    }
    ss << "ORDER BY MPG DESC";
    ss << ";";
    std::cout << ss.str() << endl;

    work W(*C);
    result R(W.exec(ss.str()));
    std::cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
    for (auto row : R) {
        for (int i = 0; i < row.size() - 1; i++) {
            cout << row[i] << " ";
        }
        cout << row[row.size() - 1] << endl;
    }
}

void query2(connection *C, string team_color) {
    string sql("SELECT t.name FROM TEAM t JOIN COLOR c ON c.COLOR_ID = t.COLOR_ID WHERE c.NAME = ");
    stringstream ss;
    ss << sql << "'" << team_color << "'"
       << ";";
    work W(*C);
    result R(W.exec(ss.str()));
    for (auto row : R) {
        std::cout << row[0] << endl;
    }
}

void query3(connection *C, string team_name) {
    string sql("SELECT p.FIRST_NAME, p.LAST_NAME FROM PLAYER p JOIN TEAM t ON t.TEAM_ID = p.TEAM_ID WHERE t.NAME = ");
    stringstream ss;
    ss << sql << "'" << team_name << "'"
       << "ORDER BY PPG DESC;";
    work W(*C);
    result R(W.exec(ss.str()));
    for (auto row : R) {
        std::cout << row[0] << "  " << row[1] << endl;
    }
}

void query4(connection *C, string team_state, string team_color) {
    string sql("SELECT p.UNIFORM_NUM, p.FIRST_NAME, p.LAST_NAME FROM PLAYER p JOIN TEAM t ON t.TEAM_ID = p.TEAM_ID JOIN STATE s on s.STATE_ID = t.STATE_ID JOIN COLOR c on c.COLOR_ID = t.COLOR_ID ");
    stringstream ss;
    ss << sql;
    ss << "WHERE s.NAME = "
       << "'" << team_state << "'"
       << "AND c.NAME = "
       << "'" << team_color << "'" << endl;
    work W(*C);
    result R(W.exec(ss.str()));
    for (auto row : R) {
        for (int i = 0; i < row.size() - 1; i++) {
            std::cout << row[i] << " ";
        }
        std::cout << row[row.size() - 1];
        std::cout << endl;
    }
}

void query5(connection *C, int num_wins) {
    string sql("SELECT p.FIRST_NAME, p.LAST_NAME ,t.NAME,t.WINS FROM PLAYER p JOIN TEAM t ON t.TEAM_ID = p.TEAM_ID ");
    stringstream ss;
    ss << sql;
    ss << "WHERE t.WINS >" << num_wins << ";";
    work W(*C);
    result R(W.exec(ss.str()));
    for (auto row : R) {
        for (int i = 0; i < row.size() - 1; i++) {
            std::cout << row[i] << " ";
        }
        std::cout << row[row.size() - 1];
        std::cout << endl;
    }
}
