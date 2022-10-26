#include <iostream>
#include <cassert>

#include "../dotl/dotl.hpp"

using std::tuple;
using std::is_same_v;

using namespace dotl;

void static_list_test()
{
    std::cout << "static_list_test -- ";

    using list = static_list<int, float, int, uint32_t>;
    static_assert(list::len == 4, "!");
    {
        using push_list = list::push<size_t>;
        static_assert(push_list::len == 5, "!");
        static_assert(is_same_v<push_list::front, int>, "!");
    }
    {
        using unshift_list = list::unshift<size_t>;
        static_assert(unshift_list::len == 5, "!");
        static_assert(is_same_v<unshift_list::front, size_t>, "!");
    }
    {
        using tuple_list = list::to_tuple;
        static_assert(is_same_v<tuple_list, tuple<int, float, int, uint32_t>>, "!");
    }
    {
        using list_0 = static_list<>;
        using list_1 = static_list<int, float>;
        using list_2 = static_list<uint32_t>;
        using list_3 = static_list<list_1, list_2>;

        using cat_0 = cat_t<list_1, list_0, list_2>;
        using cat_1 = cat_t<list_1, list_2>;
        using cat_2 = cat_t<list_1, list_2, list_3>;

        static_assert(is_same_v<cat_0, static_list<int, float, uint32_t>>, "!");
        static_assert(is_same_v<cat_1, static_list<int, float, uint32_t>>, "!");
        static_assert(is_same_v<cat_2, static_list<int, float, uint32_t, list_1, list_2>>, "!");
    }
    {
        constexpr const size_t index_float = list::find<float>;
        static_assert(index_float == 1, "!");

        constexpr const size_t index_uint32_t_32 = list::find<uint32_t>;
        static_assert(index_uint32_t_32 == 3, "!");

        constexpr const size_t index_int = list::find<int>;
        static_assert(index_int == 0, "!");
    }

    std::cout << "OK!" << std::endl;
}

void scheme_meta_test()
{
    std::cout << "scheme_meta -- ";
    {
        using scheme_test = scheme_meta<static_list<int, int>>;
        static_assert(scheme_test::is_true_scheme_v == false, "!");
    }
    {
        using scheme_test = scheme_meta<static_list<uint32_t>, static_list<int, int>>;
        static_assert(scheme_test::is_true_scheme_v == true, "!");
        static_assert(scheme_test::is_component_v == true, "!");
        static_assert(scheme_test::component_types::len == 1, "!");
        static_assert(scheme_test::simple_types::len == 2, "!");
    }
    {
        using scheme_test = scheme_meta<static_list<uint32_t, uint64_t>, static_list<int, int, uint32_t, uint64_t>>;
        static_assert(scheme_test::is_true_scheme_v == true, "!");
        static_assert(scheme_test::is_component_v == false, "!");
        static_assert(scheme_test::component_types::len == 2, "!");
        static_assert(scheme_test::simple_types::len == 4, "!");
    }

    std::cout << "OK!" << std::endl;
}

void get_component_types_test()
{
    std::cout << "get_component_types -- ";
    using A = scheme_meta<static_list<>, static_list<int, int>>;
    using B = scheme_meta<static_list<>, static_list<uint32_t>>;
    using C = scheme_meta<static_list<A, B>, static_list<int, int, uint32_t>>;

    {
        using type = get_component_types_t<int>;
        static_assert(is_same_v<type, static_list<>>, "!");
    }
    {
        using type = get_component_types_t<A>;
        static_assert(is_same_v<type, static_list<A>>, "!");
    }
    {
        using type = get_component_types_t<C>;
        static_assert(is_same_v<type, static_list<A, B>>, "!");
    }
    {
        using type = cat_t<get_component_types_t<A>, get_component_types_t<B>>;
        static_assert(is_same_v<type, static_list<A, B>>, "!");
    }

    std::cout << "OK!" << std::endl;
}

void get_simple_types_test()
{
    std::cout << "get_simple_types -- ";
    using A = scheme_meta<static_list<>, static_list<int, int>>;
    using B = scheme_meta<static_list<>, static_list<uint32_t>>;
    using C = scheme_meta<static_list<A, B>, static_list<int, int, uint32_t>>;

    {
        using type = get_simple_types_t<int>;
        static_assert(is_same_v<type, static_list<int>>, "!");
    }
    {
        using type = get_simple_types_t<A>;
        static_assert(is_same_v<type, static_list<int, int>>, "!");
    }
    {
        using type = get_simple_types_t<C>;
        static_assert(is_same_v<type, static_list<int, int, uint32_t>>, "!");
    }

    std::cout << "OK!" << std::endl;
}

void scheme_constructor_test()
{
    std::cout << "scheme_constructor -- ";

    using A = scheme_constructor_t<int>;
    using B = scheme_constructor_t<float, uint32_t>;
    using C = scheme_constructor_t<A, B>;

    static_assert(is_same_v<A, scheme_meta<static_list<>, static_list<int>>>, "!");
    static_assert(is_same_v<B, scheme_meta<static_list<>, static_list<float, uint32_t>>>, "!");
    static_assert(is_same_v<C, scheme_meta<static_list<A, B>, static_list<int, float, uint32_t>>>, "!");

    std::cout << "OK!" << std::endl;
}

void scheme_store_test()
{
    std::cout << "scheme_store_test -- ";

    struct Vector2D { int x, y; };

    struct Position : public scheme<Vector2D> {};
    struct Direction : public scheme<Vector2D> {};
    struct Speed : public scheme<int> {};
    struct Hp : public scheme<int> {};
    using Moveble = scheme<Position, Direction, Speed>;
    using Player = scheme<Moveble, Hp>;

    static_assert(Player::is_component_v == false, "!");
    static_assert(Player::is_true_scheme_v == true, "!");

    using Players = store<Player>;

    Players players;

    assert(players.size() == 0);
    assert(players.capacity() == 0);
    assert(players.create_and_get_index() == 0);

    {
        players.create()
            .set<Position>(Vector2D{3, 5});

        auto vec = players.get<Position>(1);

        assert(vec.x == 3);
        assert(vec.y == 5);

        int speed = players.get<Speed>(1);

        assert(speed == 0);
    }
    {
        players.create()
            .set<Hp>(100)
            .set<Direction>(Vector2D{0, 1})
            .set<Position>(Vector2D{10, 11});

        auto [Pos, Dir, speed] = players.get<Moveble>(2);

        assert(Pos.x == 10);
        assert(Pos.y == 11);
        assert(Dir.x == 0);
        assert(Dir.y == 1);
        assert(speed == 0);

        assert(players.get<Hp>(2) == 100);
    }

    std::cout << "OK!" << std::endl;
}

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    return new uint8_t[size];
}


struct Vector2D
{
    int x, y;
};

struct Speed : public scheme<int>
{};

struct Position : public scheme<Vector2D>
{};

struct Direction : public scheme<Vector2D>
{};

struct Moveble : public scheme<Position, Direction, Speed>
{};

int main(void)
{
    static_list_test();
    scheme_meta_test();
    get_component_types_test();
    get_simple_types_test();
    scheme_store_test();
    scheme_constructor_test();


    store<Moveble> players;

    players.create()
        .set<Position>(Vector2D{3, 4})
        .set<Direction>(Vector2D{0, 0})
        .set<Speed>(10);

	// auto position = players.get<Position>(0);
    // auto direction = players.get<Direction>(0);
    // auto speed = players.get<Speed>(0);

    // std::cout << position.x << std::endl;
    // std::cout << position.y << std::endl;
    // std::cout << direction.x << std::endl;
    // std::cout << direction.y << std::endl;
    // std::cout << speed << std::endl;

    // players.each<Position, Direction>([](int index, Vector2D pos, Vector2D dir)
    // {
    //     std::cout << pos.x << std::endl;
    // });

    return 0;
}