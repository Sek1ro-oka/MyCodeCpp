#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <unordered_map>
#include <fstream>
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

        client_string user_id = new_user.getID();
        client_int user_length = (to_string(idMap_users.size() + 1)).size();
        for (int i = 0; i < length - user_length; ++i)
            user_id += '0';
        user_id += to_string(idMap_users.size() + 1);
        new_user.setID(user_id);
        idMap_users[user_id] = std::move(new_user);
        return true;
    }

    // 创建管理员账户
    bool creat_administrator_account(client_string name, client_string gender, client_string phone, client_string password, client_string email)
    {
        client administrator(name, gender, phone, password, email, true);
        client_string admin_id = administrator.getID();
        client_int admin_length = to_string(idMap_users.size() + 1).size();
        for (int i = 0; i < length - admin_length; ++i)
            admin_id += '0';
        admin_id += to_string(idMap_users.size() + 1);
        administrator.setID(admin_id);

        idMap_users[admin_id] = std::move(administrator);
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
        event.setStar(1200.0);

        entrustment_string event_id = event.getId();
        entrustment_int event_length = to_string(idMap_events.size() + 1).size();
        for (int i = 0; i < length - event_length; ++i)
            event_id += '0';
        event_id += to_string(idMap_events.size() + 1);
        event.setId(event_id);

        idMap_events[event.getId()] = std::move(event);
    }

    // 委托从索引中消失
    void delete_event(entrustment_string id)
    {
        idMap_events.erase(id);
    }

    // 用户接受委托
    void accept_clint(const client_string &c_id, const entrustment_string &e_id)
    {
        client *c = find_client(c_id);
        entrustment *e = find_entrustment(e_id);
        if (!c || !e)
            return;

        e->setState(true);
        e->setWorker(c_id);

        // 拷贝一份进入用户承接历史
        c->Accept(*e);

        // 再从公共委托列表删除
        idMap_events.erase(e_id);
    }

    // 委托如期完成
    void complete_event(const client_string &c_id, const entrustment_string &e_id)
    {
        client *c = find_client(c_id);
        if (!c)
            return;

        auto &list = c->accessAcceptHistory();
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            if (it->getId() == e_id)
            {
                it->setFinished(true);
                c->MissonSuccess(*it);
                list.erase(it); //  从承接列表删除
                break;
            }
        }
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

    void saveToFile()
    {
        ofstream ufs("面向对象课设\\users.txt");
        for (auto &p : idMap_users)
            ufs << p.second.serialize() << endl;
        ufs.close();

        ofstream efs("面向对象课设\\entrustments.txt");
        for (auto &p : idMap_events)
            efs << p.second.serialize() << endl;
        efs.close();
    }

    void loadFromFile()
    {
        idMap_users.clear();
        idMap_events.clear();

        // ===== 用户 =====
        ifstream ufs("面向对象课设\\users.txt");
        while (ufs.peek() != EOF)
        {
            try
            {
                client u = client::deserialize(ufs);
                if (!u.getID().empty())
                    idMap_users[u.getID()] = u;
            }
            catch (...)
            {
                break; // 文件读完 or 格式错误，安全退出
            }
        }
        ufs.close();

        // ===== 未被承接的委托 =====
        ifstream efs("面向对象课设\\entrustments.txt");
        string line;
        while (getline(efs, line))
        {
            if (line.empty())
                continue;
            entrustment e = entrustment::deserialize(line);
            idMap_events[e.getId()] = e;
        }
        efs.close();
    }

private:
    unordered_map<client_string, client> idMap_users;
    unordered_map<entrustment_string, entrustment> idMap_events;
};

#endif
