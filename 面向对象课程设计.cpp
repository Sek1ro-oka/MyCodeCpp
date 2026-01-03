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
    bool register_client(client_string name, client_string gender, client_string phone, client_string password, client_string email)
    {
        client new_user(name, gender, phone, password, email);
        idMap_users[new_user.getID()] = std::move(new_user);
        return true;
    }

    // 创建管理员账户
    bool creat_administrator_account(client_string name, client_string gender, client_string phone, client_string password, client_string email)
    {
        client administrator(name, gender, phone, password, email, true);
        idMap_users[administrator.getID()] = std::move(administrator);
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
    void dispatch(client_string &Boss_id, entrustment_string name, entrustment_string time, entrustment_string location, entrustment_string content, entrustment_double profit)
    {
        // 委托人
        entrustment event = (*find_client(Boss_id)).Creat_entrustment(name, time, location, content, profit);
        idMap_events[event.getId()] = std::move(event);
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
    server1.register_client("张三", "沃尔玛购物袋", "1242685585", "123456", "125413623@qq.com");
    server1.creat_administrator_account("张羡光", "无人直升机", "1242685585", "123456", "125413623@qq.com");
    server1.show_all_users();
    return 0;
}
