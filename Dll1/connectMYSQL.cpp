#include "pch.h";

int temp_state;
MYSQL* conn;
void connectDB() {
    printf("MySQL client version: %s\n", mysql_get_client_info());
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "127.0.0.1", "root", "", "unikfamily_db", 3306, NULL, 0);
    if (conn) {
        temp_state = mysql_query(conn, "CREATE TABLE IF NOT EXISTS skyblock (id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,server TEXT,name TEXT,message TEXT)");
        if (temp_state)
        {
            puts("Error on creating table");
        }
    }
    else { puts("Connection to database has failed!"); }



    temp_state = mysql_query(conn, "SELECT * FROM skyblock");
    if (!temp_state)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            printf("ID: %s, Server: %s, Name: %s, Text: %s\n", row[0], row[1], row[2], row[3]);

            string text = "DELETE FROM skyblock WHERE id = " + std::string(row[0]);
            std::cout << text + "\n";
            temp_state = mysql_query(conn, text.c_str());
            if (temp_state)
            {
                puts("Can't delete data in table");
            }
        }
    }
    else { std::cout << "Query failed: " << mysql_error(conn) << std::endl; }
}