#include "LeaderboardInterface.hpp"

//forward declaration of helper functions
static std::string getText(sqlite3_stmt* stmt, int col);





void LeaderboardInterface::openDB() {
    //connects to the SQLlite database, creates if it doesnt exist
    
    int exit = 0;
    exit = sqlite3_open("leaderboard.db", &db);

    if(exit){
        std::cerr << "[LeaderboardInterface] Error Opening 'leaderboard.db'!" << std::endl;
    }else{
        std::cout << "[LeaderboardInterface] 'leaderboard.db' Opened Successfully!" << std::endl;
    }
    
}

void LeaderboardInterface::addScore(std::string p1name, std::string p2name, int p1score, int p2score, int game_type){
    std::string sql = "INSERT INTO leaderboard (p1name, p2name, p1score, p2score, game_type) VALUES ('" + p1name + "', '" + p2name + "', " + std::to_string(p1score) + ", " + std::to_string(p2score) + ", " + std::to_string(game_type) + ");";
    executeSQL(sql);
}

void LeaderboardInterface::refreshRecordsList(int columnFilter, bool isDescending){
    /*
        This function will update our linked list of records from the db.
        It is called when open the leaderboard gamestate, as the sqlite interface is always open
        but when the gamestate is opened it is recreated, thus we need to update the list.

        it takes 2 parameters as input, the columnFilter and isDescending. columnFilter is a value between 1 and 6,
        corresponding to which column of the table we want to sort by, and isDescending is the boolean, whether we are 
        ascending or descending. 
    */

    leaderboardrecord* current = head;
    while(current != nullptr){
        leaderboardrecord* next = current -> next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    recordLength = 0;

    //okay the list is completely cleared, no old memory is being leaked

    std::string columnFilterStr = "";
    std::string order = "ASC";
    
    switch (columnFilter){
        case 1: columnFilterStr = "p1name"; break;
        case 2: columnFilterStr = "p1score"; break;
        case 3: columnFilterStr = "p2score"; break;
        case 4: columnFilterStr = "p2name"; break;
        case 5: columnFilterStr = "game_type"; break;
        case 6: columnFilterStr = "timestamp"; break;
        default: columnFilterStr = "p1name"; break;
    }

    if(isDescending) {
        order = "DESC";
    }


    std::string sql = "SELECT * FROM leaderboard ORDER BY " + columnFilterStr + " " + order + ";";
    executeSQL(sql);
    
    /*
        This code runs a SQL query and processes the results one row at a time. sqlite3_prepare_v2 turns the SQL string into a statement, 
        and sqlite3_step moves through each row returned by SQLite. SQLite handles executing the query and providing the data. For each row, 
        values are read from the columns, using sqlite3_column_int for numbers and the getText helper to safely convert text into std::string. 
        A leaderboardrecord is then created from this data and added to the linked list. Finally, sqlite3_finalize cleans up the statement.
    */

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    while(sqlite3_step(stmt) == SQLITE_ROW){
        std::string p1name = getText(stmt, 1);
        std::string p2name = getText(stmt, 2);
        int p1score = sqlite3_column_int(stmt, 3);
        int p2score = sqlite3_column_int(stmt, 4);
        int game_type = sqlite3_column_int(stmt, 5);
        std::string timestamp = getText(stmt, 6);

        std::cout << p1name << " vs " << p2name << ": " << p1score << "-" << p2score << " (Game Type: " << game_type << ", Timestamp: " << timestamp << ")" << std::endl;
        leaderboardrecord record;
        record.p1name = p1name;
        record.p2name = p2name;
        record.p1score = p1score;
        record.p2score = p2score;
        record.game_type = game_type;
        record.timestamp = timestamp;

        addRecord(record);
        recordLength++;
    }

    sqlite3_finalize(stmt);
    std::cout << "[LeaderboardInterface] Records list refreshed! " << recordLength << " records loaded." << std::endl;
}


void LeaderboardInterface::printTest(){
    /*
        This is a test function to just print the table to the console
    */
   std::string sql = "CREATE TABLE IF NOT EXISTS leaderboard("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "p1name TEXT NOT NULL, "
                    "p2name TEXT NOT NULL, "
                    "p1score INTEGER NOT NULL, "
                    "p2score INTEGER NOT NULL, "
                    "game_type INTEGER NOT NULL, "
                    "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
                    ");";
    executeSQL(sql);
    //okay now we need a bunch more test data to add
    // addScore("Rexx", "Nyx", 41, 37, 2);
    // addScore("Blip", "Zyn", 22, 18, 1);
    // addScore("Vex", "Kiro", 59, 63, 0);
    // addScore("Axel", "Pyro", 77, 45, 3);
    // addScore("N3on", "Byte", 12, 28, 2);
    // addScore("Zed", "Flux", 66, 70, 1);
    // addScore("Kain", "Dray", 33, 21, 0);
    // addScore("Echo", "Rift", 48, 52, 3);
    // addScore("Nova", "Gl1t", 19, 14, 2);
    // addScore("Stryx", "Jett", 85, 79, 1);
    // addScore("Hex", "Mako", 27, 31, 0);
    // addScore("Orin", "Zyra", 54, 60, 3);
    // addScore("Puck", "Raze", 73, 68, 2);
    // addScore("Koda", "Talon", 16, 25, 1);
    // addScore("Lynx", "Bram", 90, 84, 0);
    // addScore("Faze", "Rook", 38, 44, 3);
    // addScore("Drift", "Vorn", 57, 49, 2);
    // addScore("Quin", "Zoro", 11, 9, 1);
    // addScore("Grit", "Haze", 62, 58, 0);
    // addScore("Nyte", "Skye", 29, 35, 3);


    
    refreshRecordsList(6, true);
}




void LeaderboardInterface::closeDB() {
    //closes the currently connected database
    sqlite3_close(db);
}

void LeaderboardInterface::executeSQL(std::string sql){
    /*
        This function executes the provided sql command on the currently connected database via db
    */
    int exit = 0;
    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);

    if(exit != SQLITE_OK){
        std::cerr << "[LeaderboardInterface] Error executing SQL Command '" << sql << "'" << std::endl;
    }

}




void LeaderboardInterface::addRecord(struct leaderboardrecord record){
    //Adds record in O(1)
    struct leaderboardrecord* node = new struct leaderboardrecord(record);
    node -> next = nullptr;
    
    if(head == nullptr){
        //if empty, set the head to the new record
        head = node;
        tail = node;
    }else{
        //otherwise, add it to the tail
        tail -> next = node;
        tail = node;
    }
}

static std::string getText(sqlite3_stmt* stmt, int col){
    /*
        This function is a helper function to get text from a sqlite3 statement at the specified column index.
        we use this when the gamestate first boots up to read the contents of the SQLite database so we can 
        display that to the user.  
    */

    //in the code below, text points to the first byte of the text data for our returned 'string'
    const unsigned char* text = sqlite3_column_text(stmt, col);

    //if not null continue
    if(text == nullptr) return "";
    
    //convert the byte data TYPE from unsigned, 0 -> 255, to signed, -128 -> 127, needed to make a string
    //ASCII characters are in the range of 0 -> 127, so this works fine, we just need to reinterpret the data as signed chars instead of unsigned
    const char* cstr = (const char*)text;

    //create a copy and return
    std::string result = std::string(cstr);
    return result;
}