#pragma once

#include "Book.hpp"
#include "PoolObjectManager.hpp"


class Player;


class BookManager : public PoolObjectManager<Book>
{
public:
    void SpawnBatch(int count);
    void Init(const Player* player);

private:
    const Player* _player = nullptr;
    int _textureIndex = 0;
};