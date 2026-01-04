#ifndef ENTRUSTMENT_H
#define ENTRUSTMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;
typedef string client_string;

#define length 6
#define space_length 20

typedef string entrustment_string;
typedef double entrustment_double;
typedef int entrustment_int;

class entrustment
{
public:
    entrustment() = default;

    entrustment(entrustment_string name,
                entrustment_string time,
                entrustment_string location,
                entrustment_string content,
                entrustment_double profit,
                client_string boss)
        : e_name(name),
          e_time(time),
          e_location(location),
          e_content(content),
          e_profit(profit),
          boss_id(boss),
          worker_id(""),
          e_state(false),
          e_finished(false)
    {
        for (char c : time)
            if (isalnum(c))
                e_id += c;
    }

    /* ===== getter ===== */
    entrustment_string getId() const { return e_id; }
    entrustment_string getName() const { return e_name; }
    entrustment_string getTime() const { return e_time; }
    entrustment_string getLocation() const { return e_location; }
    entrustment_string getContent() const { return e_content; }
    entrustment_double getStar() const { return e_star; }
    entrustment_double getProfit() const { return e_profit; }
    entrustment_string getWorkId() const { return worker_id; }
    entrustment_string getBossId() const { return boss_id; }
    entrustment_string getComments() const { return e_comments; }

    /* ===== setter ===== */
    void setId(const entrustment_string &v) { e_id = v; }
    bool getState() const { return e_state; }
    void setState(bool v) { e_state = v; }
    void setStar(entrustment_double v) { e_star = v; }
    void setWorker(const client_string &id) { worker_id = id; }
    bool isFinished() const { return e_finished; }
    void setFinished(bool v) { e_finished = v; }

    /* ===== 序列化 ===== */
    string serialize() const
    {
        ostringstream oss;
        oss << e_id << " "
            << e_name << " "
            << e_time << " "
            << e_location << " "
            << e_content << " "
            << e_star << " "
            << e_profit << " "
            << e_state << " "
            << e_finished << " "
            << boss_id << " "
            << worker_id;
        return oss.str();
    }

    static entrustment deserialize(const string &line)
    {
        istringstream iss(line);
        entrustment e;
        iss >> e.e_id >> e.e_name >> e.e_time >> e.e_location >> e.e_content >> e.e_star >> e.e_profit >> e.e_state >> e.e_finished >> e.boss_id >> e.worker_id;
        return e;
    }

private:
    entrustment_string e_id;
    entrustment_string e_name;
    entrustment_string e_time;
    entrustment_string e_location;
    entrustment_string e_content;
    entrustment_double e_star{};
    entrustment_double e_profit{};

    entrustment_string e_comments;
    client_string boss_id;
    client_string worker_id;

    bool e_state{};
    bool e_finished{};
};

#endif
