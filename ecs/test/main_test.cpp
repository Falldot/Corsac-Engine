//
// Created by Falldot on 26.11.2021.
//

//#define CORSAC_TEST_TIME_OFF
//#define CORSAC_TEST_RESULT_OFF
#define TEST_ENABLE
#define CORSAC_DEBUG 1
#define CORSAC_EXCEPTIONS_ENABLED 1

#include "Test.h"

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}

void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}



#include "Corsac/ecs.h"
#include <iostream>
#include <conio.h>
#include "windows.h"

/*
 * Починить группы и тюплы
 * Сделать загрузку карт
 * Сделать систему отображения на карте
 * Сделать систему смены активной карты
 * Сделать возможность перехода с карты на карту, после того как ушел на право до границы
 * Проверку выхода за границу карты
 */

corsac::Component<int, int> Position;
corsac::Component<int, int> Direction;
corsac::Component<int> Speed;

corsac::Component<> W;
corsac::Component<> A;
corsac::Component<> S;
corsac::Component<> D;

corsac::Group<Position, Direction, Speed> Transform;

corsac::Group<Transform> Person;

void KeyEventSystem()
{
    while(_kbhit()) {
        char c = _getche();
        for (auto start = Person.begin(), end = Person.end(); start < end; ++start) {
            if (c == 'a') {
                A.add(*start);
            }
            if (c == 'd') {
                D.add(*start);
            }
            if (c == 'w') {
                W.add(*start);
            }
            if (c == 's') {
                S.add(*start);
            }
        }
    }
}

void WMovebleEvent()
{
    for(auto start = W.begin(), end = W.end(); start < end; ++start)
    {
        if(Direction.has(*start))
            corsac::Entity<>(*start).fit<Direction>(0, -1).remove<W>();
    }
}

void SMovebleEvent()
{
    for(auto start = S.begin(), end = S.end(); start < end; ++start)
    {
        if(Direction.has(*start))
            corsac::Entity<>(*start).fit<Direction>(0, 1).remove<S>();
    }
}

void AMovebleEvent()
{
    for(auto start = A.begin(), end = A.end(); start < end; ++start)
    {
        if(Direction.has(*start))
            corsac::Entity<>(*start).fit<Direction>(-1, 0).remove<A>();
    }
}

void DMovebleEvent()
{
    for(auto start = D.begin(), end = D.end(); start < end; ++start)
    {
        if(Direction.has(*start))
            corsac::Entity<>(*start).fit<Direction>(1, 0).remove<D>();
    }
}

void Move()
{
    for(auto start = Transform.begin(), end = Transform.end(); start < end; ++start)
    {
        corsac::Entity<Transform>(*start)
                .set<Position>(
                        Position.get<0>(*start) + (Direction.get<0>(*start) * Speed.get(*start)),
                        Position.get<1>(*start) + (Direction.get<1>(*start) * Speed.get(*start))
                )
                .fit<Direction>(0, 0);
    }
}

void Draw()
{
    Sleep(1000/60);
    //system("cls");
    std::string map[29];

    for(int i = 0; i < 29; i++){
        for(int c = 0; c < 50; c++)
        {
            if((i == 0)||(c == 0)||(i == 28)||(c == 49))
                map[i] += "#";
            else
                map[i] += " ";
        }
    }

    for(auto start = Transform.begin(), end = Transform.end(); start < end; ++start)
    {
        auto x = Position.get<0>(*start);
        auto y = Position.get<1>(*start);
        map[y][x] = 'X';
    }

    std::string output = "";

    for(auto i = 0; i < 29; i++)
    {
        output += map[i] + '\n';
    }

    std::cout << output;
}

#include "Test.h"

#include "sparse_set_test.h"

int main()
{
    auto assert = new corsac::Block("ECS");

    assert->add_block("sparse_set_test", [](corsac::Block *assert) {
        sparse_set_test(assert);
    });

    assert->start();

    corsac::Entity<Person>()
            .fit<Position>(2, 2)
            .fit<Speed>(1)
            .fit<Direction>(0, 0);

    while(true)
    {
        KeyEventSystem();
        WMovebleEvent();
        SMovebleEvent();
        DMovebleEvent();
        AMovebleEvent();
        Move();
        Draw();
    }
    return 0;
}



