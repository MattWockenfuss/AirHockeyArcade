#pragma once

#include <iostream>
#include <string>

#include "../../sqlite/sqlite3.h"

struct leaderboardrecord {
    struct leaderboardrecord* next;

    int id;
    std::string p1name;
    std::string p2name;
    int p1score;
    int p2score;
    int game_type;
    std::string timestamp;
};


class LeaderboardInterface {
    public:
        sqlite3* db;
        struct leaderboardrecord* head = nullptr;
        struct leaderboardrecord* tail = nullptr;
        int recordLength = -1;

        void addRecord(struct leaderboardrecord record);


        void openDB();
        void refreshRecordsList();
        void closeDB();

        void addScore(std::string p1name, std::string p2name, int p1score, int p2score, int game_type);
        void printTest();
    private:
        void executeSQL(std::string sql);
};