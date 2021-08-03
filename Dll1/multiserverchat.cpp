#include "pch.h";

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
void connectDB() {
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "127.0.0.1", "root", "", "unikfamily_db", 3306, NULL, 0);
    if (conn) {
        int temp_state = mysql_query(conn, "CREATE TABLE IF NOT EXISTS sur (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,server TEXT,name TEXT,message TEXT)");
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
    int temp_state = mysql_query(conn, "SELECT * FROM sur");
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
            string text = "DELETE FROM sur WHERE id = " + std::string(row[0]);
            temp_state = mysql_query(conn, text.c_str());
            if (temp_state)
            {
                puts("Can't delete data in table");
            }
        }
    }
    else { std::cout << "Query failed: " << mysql_error(conn) << std::endl; }
}

//void schTask() {
//    Handler::schedule(RepeatingTask([] {
//        
//    }, 1)); //0.5sec
//}


bool playerChatEvent(ChatEV e) {
    string txt = "INSERT INTO sky (server,name,message) VALUES ('sur','" + e.pl->getNameTag() + "','" + e.msg + "')";
    int state = mysql_query(conn, txt.c_str());
    if (state)
    {
        puts("Can't insert into table");
    }
   /* for (Player* pl : liteloader::getAllPlayers())
    {
        WPlayer player = WPlayer{ *pl };
        string color1 = "\xc2\xa7"+std::string("a");
        string color2 = "\xc2\xa7" + std::string("r");
        player.sendText("<"+ color1 + std::string(e.pl->getNameTag()) + color2 +"> " + std::string(e.msg), TextType::RAW);
    }*/
    return false;
}

void multiserverchat_entry(){
	Event::addEventListener(playerChatEvent);
    connectDB();
    //schTask();
    puts("\n                  _ _   _               _           _   \n                 | | | (_)             | |         | |  \n  _ __ ___  _   _| | |_ _ _____   _____| |__   __ _| |_ \n | '_ ` _ \| | | | | __| / __\ \ / / __| '_ \ / _` | __|\n | | | | | | |_| | | |_| \__ \\ V / (__| | | | (_| | |_ \n |_| |_| |_|\__,_|_|\__|_|___/ \_/ \___|_| |_|\__,_|\__|");                                                      
    puts("                               made by tungnguyen\n");
}



