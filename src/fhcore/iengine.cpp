#include <cassert>
#include <chrono>
#include "logger.h"
#include "iengine.h"

using namespace std;
using namespace chrono;
using namespace board;
using namespace logger;

namespace engine
{

IEngine::~IEngine()
{
    wait();
}

void IEngine::configure(EngineCfg cfg) noexcept
{
    assert(cfg.pBoard && Color::Empty != cfg.colorAI);

    lock();
    delete pBoard;
    pBoard = cfg.pBoard->copy();
    colorAI = cfg.colorAI;
    unlock();
}

void IEngine::compute(FUNC_CB_AIMOVE cb, void *opaque) noexcept
{
    assert(cb);

    _future = async(launch::async, [this, cb, opaque] {
        lock();
        pos_t result = timer(&IEngine::calc_ai_move_sync, this);
        unlock();
        cb(result, opaque);
    });
}

void IEngine::compute_sync(position::pos_t & result) noexcept
{
    lock();
    result = timer(&IEngine::calc_ai_move_sync, this);
    unlock();
}

void IEngine::terminate() noexcept
{
    stop_calc_and_return();
}

void IEngine::wait()
{
    _future.wait();
}

void IEngine::lock()
{
    bool is_thinking = !_lock.try_lock();
    if (is_thinking)
        throw;
}

void IEngine::unlock()
{
    _lock.unlock();
}

}
