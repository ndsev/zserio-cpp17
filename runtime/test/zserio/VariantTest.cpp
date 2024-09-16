#include <algorithm>
#include <array>
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
    bool operator!=(const BigObj& obj) const
    {
        return !(*this == obj);
    }
    bool operator<(const BigObj& obj) const
    {
        return data[0] < obj.data[0];
    }
    std::array<char, 30> data;
};

template <class ALLOC>
class VariantTest : public ::testing::Test
{
public:
    using alloc_type = ALLOC;

    enum class Idx1
    {
        A,
        B,
        C,
        D
    };
    using Variant1 = BasicVariant<ALLOC, Idx1, int32_t, std::string, BigObj, BigObj>;

    template <typename INDEX, typename... T>
    using Variant = BasicVariant<ALLOC, INDEX, T...>;

    ALLOC allocator;
};

using TestTypes = ::testing::Types<TrackingAllocator<uint8_t>, TrackingAllocatorNonProp<uint8_t>>;
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
    typename TestFixture::Variant1 var1(zserio::in_place_index<TestFixture::Idx1::A>, 1);
    ASSERT_EQ(var1.index(), TestFixture::Idx1::A);

    typename TestFixture::Variant1 var2(zserio::in_place_index<TestFixture::Idx1::B>, this->allocator, "haha");
    ASSERT_EQ(var2.index(), TestFixture::Idx1::B);
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
    typename TestFixture::Variant1 var1(this->allocator);
    var1.template emplace<TestFixture::Idx1::B>("yes");
    ASSERT_TRUE(zserio::get<TestFixture::Idx1::B>(var1) == "yes");

    const size_t numAllocs = this->allocator.numAllocs();
    typename TestFixture::Variant1 var2 = std::move(var1);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::B>(var2), "yes");
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::B>(&var1), nullptr);
    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);

    typename TestFixture::Variant1 var3(std::move(var2), this->allocator);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::B>(var3), "yes");
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::B>(&var2), nullptr);
    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);
}

TYPED_TEST(VariantTest, moveAssignmentOperator)
{
    typename TestFixture::Variant1 var1(this->allocator);
    var1.template emplace<TestFixture::Idx1::B>("yes");
    ASSERT_TRUE(zserio::get<TestFixture::Idx1::B>(var1) == "yes");

    const size_t numAllocs = this->allocator.numAllocs();
    typename TestFixture::Variant1 var2(this->allocator);
    var2 = std::move(var1);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::B>(var2), "yes");
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::B>(&var1), nullptr);
    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);
}

TYPED_TEST(VariantTest, swap)
{
    typename TestFixture::Variant1 var1(this->allocator);
    var1.template emplace<TestFixture::Idx1::B>("yes");
    ASSERT_TRUE(zserio::get<TestFixture::Idx1::B>(var1) == "yes");

    typename TestFixture::Variant1 var2(this->allocator);
    var2.template emplace<TestFixture::Idx1::C>(BigObj(11));

    const size_t numAllocs = this->allocator.numAllocs();

    std::swap(var1, var2);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::C>(var1), BigObj(11));
    ASSERT_EQ(zserio::get<TestFixture::Idx1::B>(var2), "yes");

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
    ASSERT_EQ(zserio::get<TestFixture::Idx1::A>(var1), 11);

    var1.template emplace<TestFixture::Idx1::B>("omg");
    ASSERT_EQ(var1.index(), TestFixture::Idx1::B);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::B>(var1), "omg");

    var1.template emplace<TestFixture::Idx1::C>(BigObj(11));
    ASSERT_EQ(var1.index(), TestFixture::Idx1::C);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::C>(var1), BigObj(11));
}

TYPED_TEST(VariantTest, getIf)
{
    typename TestFixture::Variant1 var;
    const typename TestFixture::Variant1 cvar;
    var.template emplace<TestFixture::Idx1::D>(BigObj(11));
    ASSERT_EQ(*zserio::get_if<TestFixture::Idx1::D>(&var), BigObj(11));
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::A>(&var), nullptr);
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::B>(&var), nullptr);
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::B>(&cvar), nullptr);
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::C>(&var), nullptr);
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::C>(&cvar), nullptr);
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

    var.visit(overloaded{[](std::string&) {
                             SUCCEED();
                         },
            [](auto&&) {
                FAIL();
            }});

    int ret = var.visit(overloaded{
            [](std::string&) {
                return 1;
            },
            [](auto&&) {
                return 0;
            }});
    ASSERT_EQ(ret, 1);
}

TYPED_TEST(VariantTest, bigElement)
{
    const size_t num = this->allocator.numAllocs();
    typename TestFixture::Variant1 var(this->allocator);
    ASSERT_EQ(var.index(), TestFixture::Idx1::A);
    ASSERT_EQ(this->allocator.numAllocs(), num);
    var.template emplace<TestFixture::Idx1::C>(10);
    ASSERT_EQ(this->allocator.numAllocs(), num + 1);
    zserio::get<TestFixture::Idx1::C>(var) = BigObj(15);
    ASSERT_EQ(this->allocator.numAllocs(), num + 1);
    var.template emplace<TestFixture::Idx1::C>(11); // causes deallocation + allocation
    ASSERT_EQ(this->allocator.numAllocs(), num + 1);
}

TYPED_TEST(VariantTest, duplicateElement)
{
    typename TestFixture::Variant1 var(this->allocator);
    var.template emplace<TestFixture::Idx1::C>(10);
    ASSERT_EQ(var.index(), TestFixture::Idx1::C);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::C>(var).value(), 10);
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::D>(&var), nullptr);
    var.template emplace<TestFixture::Idx1::D>(11);
    ASSERT_EQ(var.index(), TestFixture::Idx1::D);
    ASSERT_EQ(zserio::get<TestFixture::Idx1::D>(var).value(), 11);
    ASSERT_EQ(zserio::get_if<TestFixture::Idx1::C>(&var), nullptr);
}

TYPED_TEST(VariantTest, exception)
{
    typename TestFixture::template Variant<size_t, int, ThrowingStruct> var1;
    ASSERT_THROW(var1.template emplace<1>(), std::runtime_error);
    typename TestFixture::Variant1 var;
    ASSERT_THROW(zserio::get<TestFixture::Idx1::B>(var), zserio::BadVariantAccess);
    ASSERT_THROW(zserio::get<TestFixture::Idx1::B>(std::as_const(var)), zserio::BadVariantAccess);

    typename TestFixture::template Variant<size_t, ThrowingMove, double> var2;
    ASSERT_THROW(var2.template emplace<0>(ThrowingMove()), std::runtime_error);
    ASSERT_EQ(var2.valueless_by_exception(), true);
    ASSERT_THROW(var2.visit([](auto&&) {
    }),
            zserio::BadVariantAccess);
    ASSERT_THROW(std::as_const(var2).visit([](const auto&) {
    }),
            zserio::BadVariantAccess);

    typename TestFixture::template Variant<size_t, int, ThrowingBig> var3;
    ASSERT_THROW(var3.template emplace<1>(11), std::runtime_error);
}

TYPED_TEST(VariantTest, canStoreInMap)
{
    std::set<typename TestFixture::Variant1> vset;
    vset.insert({});
    std::unordered_map<typename TestFixture::Variant1, int> vmap;
    typename TestFixture::Variant1 var;
    vmap[{}];
}

} // namespace zserio

namespace std
{

template <>
struct hash<zserio::BigObj>
{
    size_t operator()(const zserio::BigObj&) const
    {
        return 0;
    }
};

} // namespace std