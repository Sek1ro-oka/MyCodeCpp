#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <stack>
#include <queue>
#include <cmath>
#include <unordered_map>
#include "client.h"

using namespace std;

typedef string client_string;
typedef double client_double;
typedef int client_int;
typedef string entrustment_string;
typedef double entrustment_double;

// 服务端代码实现
class server
{
public:
    server() = default;
    // 拷贝构造
    server(const server &other) : idMap_users(other.idMap_users), idMap_events(other.idMap_events) {}
    // 移动语义快速拷贝大对象
    server(server &&other) noexcept : idMap_users(std::move(other.idMap_users)), idMap_events(std::move(other.idMap_events)) {}

    // 显示所有用户信息
    void show_all_users()
    {
        if (idMap_users.size() <= 0)
            return;
        for (auto user : idMap_users)
        {
            user.second.showInformation();
            user.second.show_dispatch_history();
            user.second.show_accpect_history();
        }
    }

    // 用户注册
    bool register_client(client_string name, client_string id, client_string gender, client_string phone, client_string password, client_string email)
    {
        if (idMap_users.find(id) != idMap_users.end())
        {
            cout << "该序列号已存在" << endl;
            return false;
        }
        client new_user(name, id, gender, phone, password, email);
        idMap_users[id] = std::move(new_user);
        return true;
    }

    // 创建管理员账户
    bool creat_administrator_account(client_string name, client_string id, client_string gender, client_string phone, client_string password, client_string email)
    {
        if (idMap_users.find(id) != idMap_users.end())
        {
            cout << "该序列号已存在" << endl;
            return false;
        }
        client administrator(name, id, gender, phone, password, email, true);
        idMap_users[id] = std::move(administrator);
        return true;
    }

    // 通过序列号来查找委托人
    client *find_client(client_string &id)
    {
        auto it = idMap_users.find(id);
        if (it != idMap_users.end())
            return &(it->second);
        return nullptr;
    }

    // 发布委托
    void dispatch(client_string &Boss_id, entrustment_string id, entrustment_string name, entrustment_string time, entrustment_string location, entrustment_string content, entrustment_double profit)
    {
        // 委托人
        entrustment event = (*find_client(Boss_id)).Creat_entrustment(id, name, time, location, content, profit);
        idMap_events[id] = std::move(event);
    }

    // 账户注销
    void delete_account(client_string &id)
    {
        idMap_users.erase(id);
    }

private:
    unordered_map<client_string, client> idMap_users;
    unordered_map<entrustment_string, entrustment> idMap_events;
};
int main()
{
    server server1;
    server1.register_client("张三", "2024", "沃尔玛购物袋", "1242685585", "123456", "125413623@qq.com");
    server1.creat_administrator_account("张羡光", "2025", "无人直升机", "1242685585", "123456", "125413623@qq.com");
    string code = "1234567";
    string num = "2025";
    server1.dispatch(num, code, "桃花源计划", "农历2006年7月15日", "大昌市某镇", "在杨间之前找到鬼画", 100);
    server1.dispatch(num, "12535", "幽灵船计划", "农历2007年3月20日", "大海市", "在队长集结之前释放厉鬼", 10000);
    server1.show_all_users();
    return 0;
}
