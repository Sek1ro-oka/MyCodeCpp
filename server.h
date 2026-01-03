#include <iostream>
#include <unordered_map>
#include "client.h"
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
    client *find_client(const client_string &id)
    {
        auto it = idMap_users.find(id);
        if (it != idMap_users.end())
            return &(it->second);
        return nullptr;
    }

    entrustment *find_entrustment(const entrustment_string &id)
    {
        auto it = idMap_events.find(id);
        if (it != idMap_events.end())
            return &(it->second);
        return nullptr;
    }

    // 账户注销
    void delete_account(client_string &id)
    {
        idMap_users.erase(id);
    }

    // 发布委托
    void dispatch(const client_string &Boss_id, const entrustment_string name, const entrustment_string time, const entrustment_string location, const entrustment_string content, entrustment_double profit)
    {
        // 委托人
        entrustment event = (*find_client(Boss_id)).Creat_entrustment(name, time, location, content, profit);
        idMap_events[event.getId()] = std::move(event);
    }

    // 委托从索引中消失
    void delete_event(entrustment_string id)
    {
        idMap_events.erase(id);
    }

    // 用户接受委托
    void accept_clint(const client_string c_id, const entrustment_string e_id)
    {
        client *c = find_client(c_id);
        if (!c)
            return;

        // 信誉限制（你之前要求的）
        if (c->getCreait() < 90)
        {
            cout << "信誉分不足 90，无法承接委托\n";
            return;
        }

        c->Accept(*find_entrustment(e_id));
        delete_event(e_id); // ✅ 正确：删除委托
    }

    // 委托如期完成
    void finish_event(client_string c_id, entrustment_string e_id)
    {
        find_client(c_id)->MissonSuccess(*find_entrustment(e_id));
        delete_event(e_id);
    }

    // 委托失败
    void finish_fail_event(client_string c_id, entrustment_string e_id)
    {
        find_client(c_id)->MissonFail(*find_entrustment(e_id));
        find_client(c_id)->setCreait(max(0.0, find_client(c_id)->getCreait() - 5)); // 信誉分降低
    }

    // 通过手机号 + 密码登录
    client *login(const client_string &phone, const client_string &password)
    {
        for (auto &pair : idMap_users)
        {
            client &user = pair.second;
            if (user.getPhone() == phone && user.getPassword() == password)
            {
                return &user; // 登录成功
            }
        }
        return nullptr; // 登录失败
    }
    // 获取所有委托（只读）
    const unordered_map<entrustment_string, entrustment> &getAllEntrustments() const
    {
        return idMap_events;
    }

    // 获取所有用户（只读）
    const unordered_map<client_string, client> &getAllUsers() const
    {
        return idMap_users;
    }

private:
    unordered_map<client_string, client> idMap_users;
    unordered_map<entrustment_string, entrustment> idMap_events;
};
