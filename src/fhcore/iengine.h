#pragma once
#include <future>
#include <mutex>
#include "board.h"

namespace engine
{

typedef void *(*FUNC_CB_AIMOVE)(position::pos_t result, void *opaque);

typedef struct EngineCfg
{
    board::IBoard *pBoard;
    color::Color colorAI;
} EngineCfg;

class IEngine
{
public:
    virtual ~IEngine();
    void configure(EngineCfg cfg) noexcept;
    void compute(FUNC_CB_AIMOVE cb, void *opaque = nullptr) noexcept;
    void compute_sync(position::pos_t & result) noexcept;

protected:
    virtual position::pos_t calc_ai_move_sync() = 0;
    void wait();

private:
    void lock();
    void unlock();

protected:
    board::IBoard *pBoard { nullptr };
    color::Color colorAI { color::Color::Empty };

private:
    std::mutex _lock;
    std::future<void> _future;
};

}