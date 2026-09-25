#pragma once

#include "PoolObject.hpp"

#include <vector>
#include <memory>
#include <type_traits>


template<typename T>
class PoolObjectManager
{
    static_assert(std::is_base_of_v<PoolObject, T>, "T is not PoolObject");
public:
    virtual void Update(float delta)
    {
        for (const auto& obj : _pool) obj->Update(delta);
    }

    virtual void Draw()
    {
        for (const auto& obj : _pool) obj->Draw();
    }

    virtual void DeactivateAll()
    {
        for (const auto& obj : _pool) if (obj->IsAlive()) obj->Deactivate();
    }

    const std::vector<std::unique_ptr<T>>& GetPool() const { return _pool; }

    int GetPoolTotal() const { return (int)_pool.size(); }
    int CountAlive() const
    {
        int n = 0;
        for (const auto& obj : _pool) if (obj->IsAlive()) n++;
        return n;
    }

    virtual ~PoolObjectManager() = default;

protected:

    T* spawnInPool()
    {
        for (auto& obj : _pool)
        {
            if (!obj->IsAlive())
                return obj.get();
        }

        _pool.push_back(std::make_unique<T>());

        return _pool.back().get();
    }

    std::vector<std::unique_ptr<T>> _pool;
};