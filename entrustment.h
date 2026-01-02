#include <iostream>

using namespace std;

typedef string client_string;
typedef double client_double;
typedef int client_int;
typedef string entrustment_string;
typedef double entrustment_double;

// 委托类
class entrustment
{
public:
    entrustment() = default;
    entrustment(entrustment_string id, entrustment_string name, entrustment_string time, entrustment_string location, entrustment_string content, entrustment_double profit) : e_id(id), e_name(name), e_time(time), e_location(location), e_content(content), e_star(0), e_profit(profit), e_state(false) { e_star = 1200; }
    entrustment_string getId() { return e_id; }
    entrustment_string getName() { return e_name; }
    entrustment_string getTime() { return e_time; }
    entrustment_string getLocation() { return e_location; }
    entrustment_string getContent() { return e_content; }
    entrustment_double getStar() { return e_star; }
    entrustment_double getProfit() { return e_profit; }
    bool getState() { return e_state; }
    void setState(bool target) { e_state = target; }

private:
    entrustment_string e_id;
    entrustment_string e_name;
    entrustment_string e_time;
    entrustment_string e_location;
    entrustment_string e_content;
    entrustment_double e_star;
    entrustment_double e_profit;
    bool e_state; // 委托接受状态
};
