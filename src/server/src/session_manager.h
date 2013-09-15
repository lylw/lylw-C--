#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <common.h>
#include <object_pool.hpp>
#include "game_session.h"

class SessionPool
{
public:
    static SessionPool& instance()
    {
        static SessionPool sessionPool;
        return sessionPool;
    }

    ~SessionPool()
    {
    }

    GameSession* acquire(uint64_t session_id)
    {
        return _sessionPool.acquire(session_id);
    }

    void release(GameSession* session)
    {
        _sessionPool.release(session);
    }

private:
    SessionPool() {}
    ObjectPool<GameSession> _sessionPool;
};

class SessionManager
{
    typedef std::hash_map<uint64_t, GameSession*> SessionTable;

public:
    static SessionManager& instance()
    {
        static SessionManager sessionManager;
        return sessionManager;
    }

public:
    bool init()
    {
    }

    void fini()
    {
    
    }

public:
    void add_session(GameSession* session)
    {
        if (get(session->session_id()) == NULL)
        {
            _mutex.lock();
            _sessionList.insert(std::make_pair(session->session_id(), session));
            _mutex.unlock();
        }
    }

    void remove_session(uint64_t session_id)
    {
        SessionTable::const_iterator iter = _sessionList.find(session_id);
        if (iter != _sessionList.end())
        {
            _mutex.lock();
            _sessionList.erase(iter);
            _mutex.unlock();
        }
    }

    void remove_session(GameSession* session)
    {
        remove_session(session->session_id());
    }

    GameSession* get(uint64_t session_id)
    {
        SessionTable::const_iterator iter = _sessionList.find(session_id);
        if (iter != _sessionList.end())
        {
            return iter->second;
        }

        return NULL;
    }

private:
    SessionManager(){}
    SessionTable _sessionList;
    std::mutex _mutex;
};

#endif