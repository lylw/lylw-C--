#ifndef GAME_SESSION_H_
#define GAME_SESSION_H_

#include <common.h>
#include <network_common.h>

class GameSession
{
public:
    GameSession(const uint64_t& session_id)
        : _sessionId(session_id)
    {
    }

    ~GameSession()
    {
    }

public:
    void set_connection_ptr(const TcpConnectionPtr& connection)
    {
        _connection = connection;
    }

    uint64_t session_id() const
    {
        return _sessionId;
    }

    void send_message(uint32_t opcode, const ::google::protobuf::Message& message)
    {
    }

public:
    void login_request_handler(const google::protobuf::Message& message)
    {
        std::cout << "enter login handler, session id = " << _sessionId << std::endl;
    }

private:
    uint64_t _sessionId;
    TcpConnectionPtr _connection;
};

#endif