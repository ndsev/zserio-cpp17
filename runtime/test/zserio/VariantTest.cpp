#include <algorithm>
#include <array>
#include <memory>
#include <utility>

#include "gtest/gtest.h"
#include "zserio/HashCodeUtil.h"
#include "zserio/TrackingAllocator.h"
#include "zserio/Variant.h"

// helper type for the visitor
template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace zserio
{

struct ThrowingStruct
{
    ThrowingStruct()
    {
        throw std::runtime_error("oops");
    }
};

struct ThrowingMove
{
    ThrowingMove() = default;
    ThrowingMove(ThrowingMove&&)
    {
        throw std::runtime_error("bum");
    }
};

struct ThrowingBig
{
    explicit ThrowingBig(int) :
            big()
    {
        throw std::runtime_error("dang");
    }
    std::array<std::byte, 30> big;
};

struct BigObj
{
    BigObj() :
            data()
    {}
    explicit BigObj(char obj) :
            data() // make clang-tidy happy
    {
        data[0] = obj;
    }
    char value() const
    {
        return data[0];
    }
    bool operator==(const BigObj& obj) const
    {
        return data[0] == obj.data[0];
    }
    bool operator<(const BigObj& obj) const
    {
        return data[0] < obj.data[0];
    }
    std::array<char, 30> data;
};

} // namespace zserio

namespace std
{

template <>
struct hash<zserio::BigObj>
{
    size_t operator()(const zserio::BigObj& obj) const
    {
        return static_cast<size_t>(obj.data[0]);
    }
};

} // namespace std

namespace zserio
{

template <class ALLOC>
class VariantTest : public testing::Test
{
public:
    using AllocatorType = ALLOC;

    enum class Idx1 : size_t
    {
        A,
        B,
        C,
        D,
        E
    };
    using Variant1 = BasicVariant<ALLOC, Idx1, int32_t, std::string, BigObj, BigObj>;

    template <typename INDEX, typename... T>
    using Variant = BasicVariant<ALLOC, INDEX, T...>;

    ALLOC allocator;
};

using TestTypes = testing::Types<TrackingAllocator<uint8_t>, TrackingAllocatorNonProp<uint8_t>>;
TYPED_TEST_SUITE(VariantTest, TestTypes, );

TYPED_TEST(VariantTest, emptyConstructor)
{
    typename TestFixture::Variant1 var1;
    ASSERT_TRUE(var1.index() == TestFixture::Idx1::A);

    typename TestFixture::Variant1 var2(this->allocator);
    ASSERT_TRUE(var2.index() == TestFixture::Idx1::A);

    typename TestFixture::template Variant<size_t, BigObj> var3(this->allocator);
    ASSERT_TRUE(var3.index() == 0);
}

TYPED_TEST(VariantTest, indexConstructor)
{
    typename TestFixture::Variant1 var1(in_place_index<TestFixture::Idx1::A>, 1);
    ASSERT_EQ(var1.index(), TestFixture::Idx1::A);

    typename TestFixture::Variant1 var2(in_place_index<TestFixture::Idx1::A>, this->allocator, 1);
    ASSERT_EQ(var2.index(), TestFixture::Idx1::A);

    typename TestFixture::Variant1 var3(in_place_index<TestFixture::Idx1::B>, this->allocator, "haha");
    ASSERT_EQ(var3.index(), TestFixture::Idx1::B);
}

TYPED_TEST(VariantTest, copyConstructor)
{
    typename TestFixture::Variant1 var1;
    typename TestFixture::Variant1 var2 = var1;
    ASSERT_TRUE(var1 == var2);

    typename TestFixture::Variant1 var3(this->allocator);
    typename TestFixture::Variant1 var4 = var3;
    ASSERT_TRUE(var4 == var3);
    typename TestFixture::Variant1 var5(var3, this->allocator);
    ASSERT_TRUE(var5 == var3);
}

TYPED_TEST(VariantTest, moveConstructor)
{
    {
        typename TestFixture::Variant1 var1(this->allocator);
        var1.template emplace<TestFixture::Idx1::B>("yes");
        ASSERT_TRUE(get<TestFixture::Idx1::B>(var1) == "yes");
        ASSERT_EQ(1, this->allocator.numAllocs());

        typename TestFixture::Variant1 var2 = std::move(var1);
        ASSERT_EQ(get<TestFixture::Idx1::B>(var2), "yes");
        ASSERT_EQ(1, this->allocator.numAllocs());

        // zserio extension - same allocator, the moved-out variant remains in valueless state
        ASSERT_TRUE(var1.valueless_by_exception());
        ASSERT_EQ(static_cast<typename TestFixture::Idx1>(std::variant_npos), var1.index());

        typename TestFixture::Variant1 var3(std::move(var2), this->allocator);
        ASSERT_EQ(get<TestFixture::Idx1::B>(var3), "yes");
        ASSERT_EQ(1, this->allocator.numAllocs());

        // zserio extension - same allocator, the moved-out variant remains in valueless state
        ASSERT_TRUE(var2.valueless_by_exception());
        ASSERT_EQ(static_cast<typename TestFixture::Idx1>(std::variant_npos), var2.index());
    }

    auto alloc1 = typename TestFixture::AllocatorType{};
    auto alloc2 = typename TestFixture::AllocatorType{};

    {
        typename TestFixture::Variant1 var1(alloc1);
        var1.template emplace<TestFixture::Idx1::D>(BigObj('1'));
        ASSERT_EQ(1, alloc1.numAllocs());

        // move ctor with allocator
        typename TestFixture::Variant1 var2(std::move(var1), alloc2);
        ASSERT_EQ(1, alloc2.numAllocs());
        ASSERT_EQ(get<TestFixture::Idx1::D>(var2), BigObj('1'));
    }
}

TYPED_TEST(VariantTest, moveAssignmentOperator)
{
    {
        typename TestFixture::Variant1 var1(this->allocator);
        var1.template emplace<TestFixture::Idx1::B>("yes");
        ASSERT_TRUE(get<TestFixture::Idx1::B>(var1) == "yes");
        ASSERT_EQ(1, this->allocator.numAllocs());

        typename TestFixture::Variant1 var2(this->allocator);
        var2 = std::move(var1);
        ASSERT_EQ(get<TestFixture::Idx1::B>(var2), "yes");
        ASSERT_EQ(1, this->allocator.numAllocs());

        // zserio extension - same allocator, the moved-out variant remains in valueless state
        ASSERT_TRUE(var1.valueless_by_exception());
        ASSERT_EQ(static_cast<typename TestFixture::Idx1>(std::variant_npos), var1.index());
    }

    auto alloc1 = typename TestFixture::AllocatorType{};
    auto alloc2 = typename TestFixture::AllocatorType{};

    {
        typename TestFixture::Variant1 var1(alloc1);
        var1.template emplace<TestFixture::Idx1::D>(BigObj('1'));
        ASSERT_EQ(1, alloc1.numAllocs());

        typename TestFixture::Variant1 var2(alloc2);
        var2.template emplace<TestFixture::Idx1::D>(BigObj('2'));
        ASSERT_EQ(1, alloc2.numAllocs());
        var2 = std::move(var1);

        ASSERT_EQ(TestFixture::Idx1::D, var2.index());
        ASSERT_EQ(get<TestFixture::Idx1::D>(var2), BigObj('1'));

        if constexpr (std::is_same_v<TrackingAllocator<uint8_t>, typename TestFixture::AllocatorType>)
        {
            ASSERT_EQ(1, alloc1.numAllocs());
            ASSERT_EQ(0, alloc2.numAllocs());

            // zserio extension - same allocator, the moved-out variant remains in valueless state
            ASSERT_TRUE(var1.valueless_by_exception());
            ASSERT_EQ(static_cast<typename TestFixture::Idx1>(std::variant_npos), var1.index());
        }
        else if constexpr (std::is_same_v<TrackingAllocatorNonProp<uint8_t>,
                                   typename TestFixture::AllocatorType>)
        {
            ASSERT_EQ(TestFixture::Idx1::D, var1.index());
            ASSERT_EQ(1, alloc1.numAllocs());
            ASSERT_EQ(1, alloc2.numAllocs());
        }
    }
}

TYPED_TEST(VariantTest, swap)
{
    typename TestFixture::Variant1 var1(this->allocator);
    var1.template emplace<TestFixture::Idx1::B>("yes");
    ASSERT_TRUE(get<TestFixture::Idx1::B>(var1) == "yes");

    typename TestFixture::Variant1 var2(this->allocator);
    var2.template emplace<TestFixture::Idx1::C>(BigObj(11));

    const size_t numAllocs = this->allocator.numAllocs();

    std::swap(var1, var2);
    ASSERT_EQ(get<TestFixture::Idx1::C>(var1), BigObj(11));
    ASSERT_EQ(get<TestFixture::Idx1::B>(var2), "yes");

    var1.swap(var2);
    ASSERT_EQ(get<TestFixture::Idx1::C>(var2), BigObj(11));
    ASSERT_EQ(get<TestFixture::Idx1::B>(var1), "yes");

    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);
}

TYPED_TEST(VariantTest, assignmentOperator)
{
    typename TestFixture::Variant1 var1;
    typename TestFixture::Variant1 var2;
    var2 = var1;
    ASSERT_TRUE(var1 == var2);

    typename TestFixture::Variant1 var3(this->allocator);
    typename TestFixture::Variant1 var4;
    var4 = var3;
    ASSERT_TRUE(var4 == var3);
}

TYPED_TEST(VariantTest, emplaceIndexGet)
{
    typename TestFixture::Variant1 var1;
    var1.template emplace<TestFixture::Idx1::A>(11);
    ASSERT_EQ(var1.index(), TestFixture::Idx1::A);
    ASSERT_EQ(get<TestFixture::Idx1::A>(var1), 11);

    var1.template emplace<TestFixture::Idx1::B>("omg");
    ASSERT_EQ(var1.index(), TestFixture::Idx1::B);
    ASSERT_EQ(get<TestFixture::Idx1::B>(var1), "omg");

    var1.template emplace<TestFixture::Idx1::C>(BigObj(11));
    ASSERT_EQ(var1.index(), TestFixture::Idx1::C);
    ASSERT_EQ(get<TestFixture::Idx1::C>(var1), BigObj(11));
}

TYPED_TEST(VariantTest, getIf)
{
    typename TestFixture::Variant1 var;
    const typename TestFixture::Variant1 cvar;
    var.template emplace<TestFixture::Idx1::D>(BigObj(11));
    ASSERT_EQ(*get_if<TestFixture::Idx1::D>(&var), BigObj(11));
    ASSERT_EQ(get_if<TestFixture::Idx1::A>(&var), nullptr);
    ASSERT_EQ(get_if<TestFixture::Idx1::B>(&var), nullptr);
    ASSERT_EQ(get_if<TestFixture::Idx1::B>(&cvar), nullptr);
    ASSERT_EQ(get_if<TestFixture::Idx1::C>(&var), nullptr);
    ASSERT_EQ(get_if<TestFixture::Idx1::C>(&cvar), nullptr);
}

TYPED_TEST(VariantTest, comparison)
{
    typename TestFixture::Variant1 aaa;
    typename TestFixture::Variant1 bbb;
    // different indexes
    aaa.template emplace<TestFixture::Idx1::A>(20);
    bbb.template emplace<TestFixture::Idx1::B>("I Am");
    ASSERT_LT(aaa.index(), bbb.index());
    ASSERT_FALSE(aaa == bbb);
    ASSERT_TRUE(aaa != bbb);
    ASSERT_TRUE(aaa < bbb);
    ASSERT_TRUE(bbb > aaa);
    ASSERT_TRUE(aaa <= bbb);
    ASSERT_TRUE(bbb >= aaa);
    ASSERT_FALSE(bbb < aaa);
    ASSERT_FALSE(bbb <= aaa);
    ASSERT_FALSE(aaa > bbb);
    ASSERT_FALSE(aaa >= bbb);
    // same index
    aaa.template emplace<TestFixture::Idx1::B>("Donkey");
    ASSERT_EQ(aaa.index(), bbb.index());
    ASSERT_FALSE(aaa == bbb);
    ASSERT_TRUE(aaa != bbb);
    ASSERT_TRUE(aaa < bbb);
    ASSERT_TRUE(bbb > aaa);
    ASSERT_TRUE(aaa <= bbb);
    ASSERT_TRUE(bbb >= aaa);
    ASSERT_FALSE(bbb < aaa);
    ASSERT_FALSE(bbb <= aaa);
    ASSERT_FALSE(aaa > bbb);
    ASSERT_FALSE(aaa >= bbb);
}

TYPED_TEST(VariantTest, visit)
{
    typename TestFixture::Variant1 var;
    var.template emplace<TestFixture::Idx1::B>("something");

    var.visit([](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        ASSERT_TRUE((std::is_same_v<T, std::string>));
    });

    // due to coverage tests lambdas are factored out of the visit call
    // and called twice
    int ret1 = 12345;
    auto over1 = overloaded{
            [&](std::string&) {
                ret1 = 1;
            },
            [&](auto&&) {
                ret1 = 0;
            }};
    auto over2 = overloaded{
            [](std::string&) {
                return 1;
            },
            [](auto&&) {
                return 0;
            }};

    ret1 = 0;
    var.visit(over1);
    ASSERT_EQ(ret1, 1);
    int ret2 = var.visit(over2);
    ASSERT_EQ(ret2, 1);

    var.template emplace<TestFixture::Idx1::A>();
    ret1 = 1;
    visit(over1, var);
    ASSERT_EQ(ret1, 0);
    ret2 = visit(over2, std::as_const(var));
    ASSERT_EQ(ret2, 0);
}

TYPED_TEST(VariantTest, bigElement)
{
    const size_t num = this->allocator.numAllocs();
    typename TestFixture::Variant1 var(this->allocator);
    ASSERT_EQ(var.index(), TestFixture::Idx1::A);
    ASSERT_EQ(this->allocator.numAllocs(), num);
    var.template emplace<TestFixture::Idx1::C>(10);
    ASSERT_EQ(this->allocator.numAllocs(), num + 1);
    get<TestFixture::Idx1::C>(var) = BigObj(15);
    ASSERT_EQ(this->allocator.numAllocs(), num + 1);
    var.template emplace<TestFixture::Idx1::C>(11); // causes deallocation + allocation
    ASSERT_EQ(this->allocator.numAllocs(), num + 1);
}

TYPED_TEST(VariantTest, duplicateElement)
{
    typename TestFixture::Variant1 var(this->allocator);
    var.template emplace<TestFixture::Idx1::C>(10);
    ASSERT_EQ(var.index(), TestFixture::Idx1::C);
    ASSERT_EQ(get<TestFixture::Idx1::C>(var).value(), 10);
    ASSERT_EQ(get_if<TestFixture::Idx1::D>(&var), nullptr);
    var.template emplace<TestFixture::Idx1::D>(11);
    ASSERT_EQ(var.index(), TestFixture::Idx1::D);
    ASSERT_EQ(get<TestFixture::Idx1::D>(var).value(), 11);
    ASSERT_EQ(get_if<TestFixture::Idx1::C>(&var), nullptr);
}

TYPED_TEST(VariantTest, exception)
{
    typename TestFixture::template Variant<size_t, int, ThrowingStruct> var1;
    ASSERT_THROW(var1.template emplace<1>(), std::runtime_error);
    typename TestFixture::Variant1 var;
    ASSERT_THROW(get<TestFixture::Idx1::B>(var), BadVariantAccess);
    ASSERT_THROW(get<TestFixture::Idx1::B>(std::as_const(var)), BadVariantAccess);

    typename TestFixture::template Variant<size_t, ThrowingMove, double> var2;
    ASSERT_THROW(var2.template emplace<0>(ThrowingMove()), std::runtime_error);
    ASSERT_EQ(var2.valueless_by_exception(), true);
    ASSERT_THROW(var2.visit([](auto&&) {
    }),
            BadVariantAccess);
    ASSERT_THROW(std::as_const(var2).visit([](const auto&) {
    }),
            BadVariantAccess);

    typename TestFixture::template Variant<size_t, int, ThrowingBig> var3;
    ASSERT_THROW(var3.template emplace<1>(11), std::runtime_error);
}

TYPED_TEST(VariantTest, canStoreInMap)
{
    typename TestFixture::Variant1 big1(in_place_index<TestFixture::Idx1::D>, 2);
    typename TestFixture::Variant1 big2(in_place_index<TestFixture::Idx1::D>, 7);
    std::set<typename TestFixture::Variant1> vset;
    vset.insert(typename TestFixture::Variant1());
    vset.insert(big1);
    vset.insert(big2);
    ASSERT_TRUE(vset.size() == 3);
    std::unordered_map<typename TestFixture::Variant1, int> vmap;
    typename TestFixture::Variant1 var;
    vmap[typename TestFixture::Variant1()];
    vmap[big1];
    vmap[big2];
    ASSERT_TRUE(vmap.size() == 3);
}

TEST(VariantTest, compareSizeToStd)
{
    ASSERT_EQ(sizeof(Variant<size_t, int>), sizeof(std::variant<int>));
}

} // namespace zserio
