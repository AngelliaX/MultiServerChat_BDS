#include "pch.h";

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

string servers[] = { "sur","sky","sky_vic" };
void connectDB() {
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "play.unikfamily.com", "motcaiten", "", "unikfamily_db", 3306, NULL, 0);
    if (conn) {
        for (std::string svname : servers)
        {
            string txt = "CREATE TABLE IF NOT EXISTS " + svname + " (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,server TEXT,name TEXT,message TEXT)";
            int temp_state = mysql_query(conn, txt.c_str());
            if (temp_state)
            {
                puts("Error on creating table");
            }
        }
        int temp_state = mysql_query(conn, "CREATE TABLE IF NOT EXISTS sur_translate (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,server TEXT,name TEXT,message TEXT)");
        if (temp_state)
        {
            puts("Error on creating table");
        }
    }
    else { puts("Connection to database has failed!"); }
}
//#include <api/scheduler/scheduler.h>

THook(void, "?tick@Level@@UEAAXXZ", class Level* lv) {
    original(lv);//Don't remove this line
    int temp_state = mysql_query(conn, "SELECT * FROM sur_translate");
    if (!temp_state)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            //printf("ID: %s, Server: %s, Name: %s, Text: %s\n", row[0], row[1], row[2], row[3]);
            for (Player* pl : liteloader::getAllPlayers())
            {
                WPlayer player = WPlayer{ *pl };
                std::string txt = row[3];
                player.sendText(txt, TextType::RAW);
            }
            string text = "DELETE FROM sur_translate WHERE id = " + std::string(row[0]);
            temp_state = mysql_query(conn, text.c_str());
            if (temp_state)
            {
                puts("Can't delete data in table");
            }
        }
    }
    else { std::cout << "Query failed: " << mysql_error(conn) << std::endl; }
}
bool playerChatEvent(ChatEV e) {
    for (string svname : servers)
    {
        string txt = "INSERT INTO " + svname + " (server,name,message) VALUES ('sur','" + e.pl->getNameTag() + "','" + e.msg + "')";
        int state = mysql_query(conn, txt.c_str());
        if (state)
        {
            puts("Can't insert into table");
        }
    }
    return false;
}

void multiserverchat_entry() {
    Event::addEventListener(playerChatEvent);
    connectDB();
    //schTask();
    puts("\n                  _ _   _               _           _   \n                 | | | (_)             | |         | |  \n  _ __ ___  _   _| | |_ _ _____   _____| |__   __ _| |_ \n | '_ ` _ \| | | | | __| / __\ \ / / __| '_ \ / _` | __|\n | | | | | | |_| | | |_| \__ \\ V / (__| | | | (_| | |_ \n |_| |_| |_|\__,_|_|\__|_|___/ \_/ \___|_| |_|\__,_|\__|");
    puts("                               made by tungnguyen\n");
}



