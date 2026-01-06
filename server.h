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

    // 获取所有委托（只读）
    const unordered_map<entrustment_string, entrustment> &getAllEntrustments() const { return idMap_events; }

    // 获取所有用户（只读）
    const unordered_map<client_string, client> &getAllUsers() const { return idMap_users; }

    void markDirty() { dirty = true; }
    bool isDirty() const { return dirty; }
    void clearDirty() { dirty = false; }

    // 用户注册
    bool register_client(client_string name, client_string gender, client_string phone, client_string password, client_string email)
    {
        // 检查手机号是否已被注册
        if (phoneExists(phone))
            return false;

        client new_user(name, gender, phone, password, email);

        // 全局唯一 ID
        client_string id = "0";
        id += string(length - to_string(next_user_id).size(), '0');
        id += to_string(next_user_id++);

        new_user.setID(id);
        idMap_users[id] = std::move(new_user);
        return true;
    }

    // 创建管理员账户
    bool creat_administrator_account(client_string name, client_string gender, client_string phone, client_string password, client_string email)
    {
        client new_user(name, gender, phone, password, email, true);

        client_string id = "1";
        id += string(length - to_string(next_user_id).size(), '0');
        id += to_string(next_user_id++);

        new_user.setID(id);
        idMap_users[id] = std::move(new_user);
        dirty = true;
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
    void delete_account(const client_string &id)
    {
        dirty = true;
        client *c = find_client(id);
        if (!c)
            return;

        // 1. 删除用户发布的所有委托
        for (const auto &eid : c->getDispatchHistory())
            idMap_events.erase(eid);

        // 2. 重置用户承接的委托（标记为未承接）
        for (const auto &eid : c->getAcceptHistory())
        {
            entrustment *e = find_entrustment(eid);
            if (e)
            {
                e->setState(false);
                e->setWorker("");
            }
        }

        // 3. 删除用户本身
        idMap_users.erase(id);
        saveToFile(); // 立即落盘
    }

    // 生成唯一事件 ID
    string generate_event_id()
    {
        string id = "E";
        // 确保补0后总长度为6（E + 5位数字）
        int num_len = to_string(next_event_id).size();
        int pad_len = max(0, 5 - num_len); // 防止pad_len为负
        id += string(pad_len, '0');
        id += to_string(next_event_id++); // 生成后自增
        return id;
    }

    // 发布委托
    void dispatch(const client_string &Boss_id, const entrustment_string name,
                  const entrustment_string time, const entrustment_string location,
                  const entrustment_string content, entrustment_double profit)
    {
        dirty = true;
        entrustment e(name, time, location, content, profit, Boss_id);
        string eid = generate_event_id();
        e.setId(eid);
        e.setStar(calcEntrustStar(profit)); // 之前加的星级逻辑

        // 改为移动语义，避免拷贝
        idMap_events.emplace(eid, std::move(e));
        find_client(Boss_id)->accessDispatchHistory().push_back(eid);
        saveToFile();
    }

    // 委托从索引中消失
    void delete_event(entrustment_string id)
    {
        idMap_events.erase(id);
    }

    // 用户接受委托
    int accept_clint(const client_string &c_id, const entrustment_string &e_id)
    {
        entrustment *e = find_entrustment(e_id);
        if (!e)
            return 0; // 委托不存在
        if (e->getState())
            return -2; // 已被承接

        client *c = find_client(c_id);
        if (!c)
            return 0; // 用户不存在
        if (!c->canAcceptEntrustment())
            return -1; // 信誉分不足

        // 新增：星级判断（用户星级 ≥ 委托星级才能承接）
        if (c->getStar() < e->getStar())
            return -3; // 星级不足

        e->setState(true);
        e->setWorker(c_id);
        c->accessAcceptHistory().push_back(e_id);
        dirty = true;
        return 1; // 承接成功
    }

    // 委托如期完成
    void complete_event(const client_string &c_id, const entrustment_string &e_id)
    {
        dirty = true;
        client *c = find_client(c_id);
        if (!c)
            return;

        auto &ids = c->accessAcceptHistory();
        ids.erase(remove(ids.begin(), ids.end(), e_id), ids.end());
        c->updateStar(true);
        c->setCreait(min(100.0, c->getCreait() + 2));

        entrustment *e = find_entrustment(e_id);
        if (e)
        {
            e->setFinished(true);
        }
    }

    // 委托失败
    void finish_fail_event(const client_string &c_id, const entrustment_string &e_id)
    {
        dirty = true;

        client *c = find_client(c_id);
        if (!c)
            return;

        auto &ids = c->accessAcceptHistory();
        ids.erase(remove(ids.begin(), ids.end(), e_id), ids.end());
        c->updateStar(false);
        c->setCreait(max(0.0, c->getCreait() - 1));

        entrustment *e = find_entrustment(e_id);
        if (e)
        {
            e->setFinished(true);
        }
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

    // 发布者取消委托
    bool cancel_dispatch(const client_string &uid, const entrustment_string &eid)
    {
        client *c = find_client(uid);
        if (!c)
            return false;

        auto &dispatch = c->accessDispatchHistory();
        auto it = find(dispatch.begin(), dispatch.end(), eid);
        if (it == dispatch.end())
            return false;

        dispatch.erase(it);
        idMap_events.erase(eid);

        dirty = true;
        return true;
    }

    void saveToFile()
    {
        vector<pair<string, client>> users(idMap_users.begin(), idMap_users.end());
        sort(users.begin(), users.end(),
             [](auto &a, auto &b)
             { return a.first < b.first; });

        ofstream ufs("面向对象课设\\users.txt");
        for (auto &p : users)
            ufs << p.second.serialize();
        ufs.close();

        vector<pair<string, entrustment>> events(idMap_events.begin(), idMap_events.end());
        sort(events.begin(), events.end(),
             [](auto &a, auto &b)
             { return a.first < b.first; });

        ofstream efs("面向对象课设\\entrustments.txt");
        for (auto &p : events)
            efs << p.second.serialize();
        efs.close();
    }

    void loadFromFile()
    {
        idMap_users.clear();
        idMap_events.clear();

        // ===== 用户 =====
        ifstream ufs("面向对象课设\\users.txt");
        while (true)
        {
            try
            {
                client u = client::deserialize(ufs);
                idMap_users[u.getID()] = u;

                int num = stoi(u.getID().substr(1));
                next_user_id = max(next_user_id, num + 1);
            }
            catch (...)
            {
                break; // 正常 EOF
            }
        }
        ufs.close();

        // ===== 未被承接的委托 =====
        ifstream efs("面向对象课设\\entrustments.txt");
        int max_event_num = 0; // 记录已有的最大委托ID数值
        while (true)
        {
            try
            {
                entrustment e = entrustment::deserialize(efs);
                idMap_events[e.getId()] = e;

                // 解析委托ID中的数字部分（如E00003 → 3）
                string id_num_str = e.getId().substr(1); // 去掉前缀E
                // 去除前导0后转数字
                id_num_str.erase(0, id_num_str.find_first_not_of('0'));
                if (id_num_str.empty())
                    id_num_str = "0"; // 处理E00000的情况
                int id_num = stoi(id_num_str);
                max_event_num = max(max_event_num, id_num);
            }
            catch (...)
            {
                break;
            }
        }
        efs.close();
        next_event_id = max_event_num + 1;
    }

    // 检查是否存在管理员账户
    bool hasAdmin() const
    {
        for (auto p : idMap_users)
            if (p.second.isAdmin())
                return true;
        return false;
    }

    // 检查手机号是否已被注册
    bool phoneExists(const client_string &phone) const
    {
        for (auto &p : idMap_users)
            if (p.second.getPhone() == phone)
                return true;
        return false;
    }

private:
    unordered_map<client_string, client> idMap_users;
    unordered_map<entrustment_string, entrustment> idMap_events;
    client_int next_user_id = 1;
    entrustment_int next_event_id = 1;
    bool dirty = false;

    entrustment_double calcEntrustStar(entrustment_double profit)
    {
        // 报酬-星级映射规则
        if (profit <= 100)
            return 0.0; // 小白
        if (profit <= 200)
            return 1200.0; // 青铜
        if (profit <= 300)
            return 1400.0; // 铂金
        if (profit <= 500)
            return 1600.0; // 黄金
        if (profit <= 800)
            return 1800.0; // 白金
        if (profit <= 1000)
            return 2000.0; // 黑金
        if (profit <= 1500)
            return 2200.0; // 红名
        return 2400.0;     // 传奇
    }
};

#endif
