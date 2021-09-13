#include "pch.h";

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;


std::vector<string> servers;
string THook;
string Address;
string Username;
string Password;
string Database;
string Prefix;
string Name;
void connectDB() {
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, Address.c_str(), Username.c_str(), Password.c_str(), Database.c_str(), 3306, NULL, 0);
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
        string txt = "CREATE TABLE IF NOT EXISTS " + THook + " (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,server TEXT,name TEXT,message TEXT)";
        int temp_state = mysql_query(conn,txt.c_str());
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
    string txt = "SELECT * FROM "+ THook;
    int temp_state = mysql_query(conn,txt.c_str());
    if (!temp_state)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            //printf("ID: %s, Server: %s, Name: %s, Text: %s\n", row[0], row[1], row[2], row[3]);
            for (Player* pl : liteloader::getAllPlayers())
            {
                WPlayer player = WPlayer{ *pl };            
                //std::cout << row[1] << "..." << row[2] << "..." << row[3] << "\n";
                string txt(Prefix),
                    pos("{server}"),
                    des(row[1]);
                if (row[1] != Name) {
                    for (size_t index = 0; (index = txt.find(pos, index)) != string::npos;) {
                        txt.replace(index, pos.length(), des);
                    }
                    txt = txt + row[3];
                }
                else {
                    txt = row[3];
                }
                player.sendText(txt, TextType::RAW);
            }
            string text = "DELETE FROM "+THook+" WHERE id = " + std::string(row[0]);
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
        string txt = "INSERT INTO " + svname + " (server,name,message) VALUES ('"+Name+"','" + e.pl->getNameTag() + "','" + e.msg + "')";
        int state = mysql_query(conn, txt.c_str());
        if (state)
        {
            puts("Can't insert into table");
        }
    }
    return false;
}
void loadCfg() {
    std::filesystem::create_directory("plugins\\MultiServerChat");
    std::string config_file = "plugins\\MultiServerChat\\Config.json";
    std::ifstream fs;
    fs.open(config_file, std::ios::in);
    if (!fs)
    {     
        Address = "play.unikfamily.com";
        Username = "motcaiten";
        Password = "";
        Database = "unikfamily_db";
        THook = "sur_translate";
        servers = { "sur","sky","sky_vic" };
        Prefix = "[§6{server}§r] ";
        Name = "Sur";
        std::cout << "[MultiServerChat] " << config_file << " not found, creating file(default value used)\n";
        std::ofstream of(config_file);
        if (of)
        {
            std::string text =std::string("{\n  \"Address\": \"") + Address + "\",\n  \"Username\": \""+Username+ "\",\n  \"Password\": \"" + Password + "\",\n  \"Database\": \""+ Database+ "\",\n  \"THook\": \""+THook+"\",\n  \"Servers\": [\"sur\",\"sky\",\"sky_vic\"],"+"\n  \"Prefix\": \""+Prefix+"\",\n  \"Name\": \"" + Name + "\"\n}";
            of << text;
        }
        else
        {
            std::cout << "[MultiServerChat] " << "config file creation failed, please create \"config\" folder\n";
        }
    }
    else
    {
        std::string json;
        char buf[1024];
        while (fs.getline(buf, 1024))
        {
            json.append(buf);
        }
        rapidjson::Document document;
        document.Parse(json.c_str());
        Address = document["Address"].GetString();
        Username = document["Username"].GetString();
        Password = document["Password"].GetString();
        Database = document["Database"].GetString();
        Prefix = document["Prefix"].GetString();
        THook = document["THook"].GetString();
        Name = document["Name"].GetString();
        auto arraylist = document["Servers"].GetArray();
        for (rapidjson::Value::ConstValueIterator itr = arraylist.Begin(); itr != arraylist.End(); ++itr) {
            servers.push_back(itr->GetString());
        }
    }
    std::cout << "\n[MultiServerChat] Loaded!\n";
}
void multiserverchat_entry() {
    Event::addEventListener(playerChatEvent);
    loadCfg();
    connectDB();
    puts("\n                  _ _   _               _           _   \n                 | | | (_)             | |         | |  \n  _ __ ___  _   _| | |_ _ _____   _____| |__   __ _| |_ \n | '_ ` _ \| | | | | __| / __\ \ / / __| '_ \ / _` | __|\n | | | | | | |_| | | |_| \__ \\ V / (__| | | | (_| | |_ \n |_| |_| |_|\__,_|_|\__|_|___/ \_/ \___|_| |_|\__,_|\__|");
    puts("                               made by tungnguyen\n");
}



