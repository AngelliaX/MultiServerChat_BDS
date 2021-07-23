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
#include <api/scheduler/scheduler.h>
void schTask() {
    Handler::schedule(RepeatingTask([] {
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
                    player.sendText("<"+std::string(row[1])+"> <" + std::string(row[2]) + "> "+ std::string(row[3]), TextType::RAW);
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
    }, 1)); //0.5sec
}


bool playerChatEvent(ChatEV e) {
    string txt = "INSERT INTO sky (server,name,message) VALUES ('sur','" + e.pl->getNameTag() + "','" + e.msg + "')";
    int state = mysql_query(conn, txt.c_str());
    if (state)
    {
        puts("Can't insert into table");
    }
    return true;
}

#include <api\types\helper.h>;
#include <api\types\types.h>;
#include <api\myPacket.h>
void playerJoinEvent(JoinEV e) {

    
}

#include <stl/varint.h>
bool oncmd_wow(CommandOrigin const& ori, CommandOutput& outp, string const& str, optional<string>& str2) {
    //WPlayer player =  *ori.getEntity()->getPlayerOwner();
    WPlayer player = MakeWP(ori).val();
   
    player.sendText("this is a text",TextType::CHAT);
    puts("finished sending a text");
    return true;
}

void multiserverchat_entry(){
	Event::addEventListener(playerChatEvent);
    Event::addEventListener(playerJoinEvent);
    connectDB();
    schTask();
    puts("\n                  _ _   _               _           _   \n                 | | | (_)             | |         | |  \n  _ __ ___  _   _| | |_ _ _____   _____| |__   __ _| |_ \n | '_ ` _ \| | | | | __| / __\ \ / / __| '_ \ / _` | __|\n | | | | | | |_| | | |_| \__ \\ V / (__| | | | (_| | |_ \n |_| |_| |_|\__,_|_|\__|_|___/ \_/ \___|_| |_|\__,_|\__|");                                                      
    puts("                               made by tungnguyen\n");
}



