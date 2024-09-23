#include <algorithm>
#include <array>
#include <utility>

#include "gtest/gtest.h"
#include "zserio/HashCodeUtil.h"
#include "zserio/Optional.h"
#include "zserio/TrackingAllocator.h"

namespace zserio
{

struct ThrowingBig
{
    std::array<char, 30> data;

    ThrowingBig() :
            data()
    {
        throw std::runtime_error("oops");
    }
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

template <class ALLOC>
class OptionalTest : public ::testing::Test
{
public:
    using alloc_type = ALLOC;

    using IntOptional = BasicOptional<ALLOC, int>;
    using ShortOptional = BasicOptional<ALLOC, int16_t>;
    using BigOptional = BasicOptional<ALLOC, BigObj>;
    using StringOptional = BasicOptional<ALLOC, std::string>;

    ALLOC allocator;
};

using TestTypes = ::testing::Types<TrackingAllocator<uint8_t>, TrackingAllocatorNonProp<uint8_t>>;
TYPED_TEST_SUITE(OptionalTest, TestTypes, );

TYPED_TEST(OptionalTest, defaultConstructor)
{
    typename TestFixture::IntOptional opt1;
    typename TestFixture::BigOptional opt2(this->allocator);
    typename TestFixture::StringOptional opt3 = {};
    typename TestFixture::IntOptional opt4 = {};
    ASSERT_TRUE(!opt1);
    ASSERT_TRUE(!opt1.has_value());
    ASSERT_TRUE(!opt2);
    ASSERT_TRUE(!opt2.has_value());
    ASSERT_EQ(opt2.get_allocator(), this->allocator);
    ASSERT_TRUE(!opt3.has_value());
    ASSERT_TRUE(!opt4.has_value());
}

TYPED_TEST(OptionalTest, valueConstructor)
{
    typename TestFixture::StringOptional optn(std::nullopt);
    ASSERT_TRUE(!optn.has_value());
    typename TestFixture::IntOptional opt1 = 12;
    ASSERT_EQ(opt1.value(), 12);
    ASSERT_EQ(std::as_const(opt1).value(), 12);
    typename TestFixture::IntOptional opt2(std::in_place, 12);
    ASSERT_EQ(opt2.value(), 12);
    typename TestFixture::StringOptional opt3("ccc");
    ASSERT_EQ(opt3.value(), "ccc");
    typename TestFixture::StringOptional opt4(opt3.value());
    ASSERT_EQ(opt4.value(), "ccc");
    typename TestFixture::StringOptional opt5(std::in_place, this->allocator, 3, 'c');
    ASSERT_EQ(opt5.value(), "ccc");
}

TYPED_TEST(OptionalTest, copyConstructor)
{
    typename TestFixture::IntOptional opt1;
    typename TestFixture::IntOptional opt2(opt1);
    ASSERT_EQ(opt1, opt2);

    typename TestFixture::BigOptional opt3(this->allocator);
    typename TestFixture::BigOptional opt4 = opt3;
    ASSERT_EQ(opt3, opt4);
    typename TestFixture::BigOptional opt5(opt4, this->allocator);
    ASSERT_EQ(opt4, opt3);

    typename TestFixture::ShortOptional opts(10);
    opt1 = typename TestFixture::IntOptional(opts);
    ASSERT_EQ(opt1, opts);
    opt1 = typename TestFixture::IntOptional(opts, this->allocator);
    ASSERT_EQ(opt1, opts);
}

TYPED_TEST(OptionalTest, copyAssignment)
{
    typename TestFixture::StringOptional opt1("hello");
    typename TestFixture::StringOptional opt2;
    opt2 = opt1;
    ASSERT_TRUE(opt2.value() == opt1.value());

    typename TestFixture::IntOptional opti;
    typename TestFixture::ShortOptional opts(10);
    opti = opts;
    ASSERT_EQ(opti, opts);
}

TYPED_TEST(OptionalTest, valueAssignment)
{
    typename TestFixture::StringOptional opt1(this->allocator);
    opt1 = "hey";
    ASSERT_TRUE(opt1.value() == std::string("hey"));
    ASSERT_TRUE(opt1.value_or("aha") == "hey");
    ASSERT_TRUE(std::as_const(opt1).value_or("aha") == "hey");
    opt1 = {};
    ASSERT_TRUE(!opt1.has_value());
    ASSERT_TRUE(!opt1);
    ASSERT_TRUE(opt1.value_or("aha") == "aha");
    ASSERT_TRUE(std::as_const(opt1).value_or("aha") == "aha");
    opt1 = std::string("mister");
    ASSERT_TRUE(opt1.has_value());
    ASSERT_TRUE(opt1);
    ASSERT_TRUE(opt1.value() == "mister");
    ASSERT_TRUE(*opt1 == "mister");
    ASSERT_TRUE(*std::as_const(opt1) == "mister");
    opt1 = std::nullopt;
    ASSERT_TRUE(!opt1.has_value());
    ASSERT_TRUE(!opt1);
    // typename TestFixture::StringOptional koko(std::move(opt1));
    // opt1.has_value();

    typename TestFixture::IntOptional opt2;
    opt2 = 11;
    ASSERT_TRUE(opt2.has_value());
    ASSERT_TRUE(opt2.value() == 11);
    ASSERT_TRUE(*opt2 == 11);
    opt2 = {};
    ASSERT_TRUE(!opt2.has_value());
    ASSERT_TRUE(!opt2);
    opt2 = {5};
    ASSERT_TRUE(opt2.has_value());
    ASSERT_TRUE(opt2);
    ASSERT_TRUE(opt2.value() == 5);
    ASSERT_TRUE(*opt2 == 5);
    opt2 = std::nullopt;
    ASSERT_TRUE(!opt2.has_value());
    ASSERT_TRUE(!opt2);
}

TYPED_TEST(OptionalTest, moveConstructor)
{
    typename TestFixture::StringOptional opt1(this->allocator);
    opt1.emplace("yes");
    ASSERT_EQ(opt1.value(), "yes");

    const size_t numAllocs = this->allocator.numAllocs();
    typename TestFixture::StringOptional opt2 = std::move(opt1);
    ASSERT_EQ(opt2.value(), "yes");
    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);

    typename TestFixture::StringOptional opt3(std::move(opt2), this->allocator);
    ASSERT_EQ(opt3.value(), "yes");
    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);
}

TYPED_TEST(OptionalTest, moveAssignmentOperator)
{
    typename TestFixture::StringOptional opt1(this->allocator);
    opt1.emplace("yes");
    ASSERT_TRUE(opt1.value() == "yes");

    const size_t numAllocs = this->allocator.numAllocs();
    typename TestFixture::StringOptional opt2(this->allocator);
    opt2 = std::move(opt1);
    ASSERT_EQ(opt2.value(), "yes");
    ASSERT_EQ(this->allocator.numAllocs(), numAllocs);
}

TYPED_TEST(OptionalTest, optComparison)
{
    typename TestFixture::StringOptional nopt;
    typename TestFixture::StringOptional opt1("great");
    typename TestFixture::StringOptional opt2("joy");
    ASSERT_TRUE(opt1 == opt1);
    ASSERT_FALSE(opt1 == nopt);
    ASSERT_TRUE(opt1 != nopt);
    ASSERT_TRUE(opt1 != opt2);
    ASSERT_TRUE(nopt < opt1);
    ASSERT_TRUE(opt1 < opt2);
    ASSERT_TRUE(nopt <= opt1);
    ASSERT_TRUE(opt1 <= opt2);
    ASSERT_TRUE(opt2 > nopt);
    ASSERT_TRUE(opt2 > opt1);
    ASSERT_TRUE(opt2 >= nopt);
    ASSERT_TRUE(opt2 >= opt1);
}

TYPED_TEST(OptionalTest, valueComparison)
{
    typename TestFixture::StringOptional opt1("great");
    ASSERT_TRUE(opt1 == "great");
    ASSERT_TRUE("great" == opt1);
    ASSERT_TRUE(opt1 != "bad");
    ASSERT_TRUE("bad" != opt1);
    ASSERT_TRUE(opt1 < "greater");
    ASSERT_TRUE("greater" > opt1);
    ASSERT_TRUE(opt1 <= "greater");
    ASSERT_TRUE("greater" >= opt1);
    ASSERT_TRUE(opt1 > "fun");
    ASSERT_TRUE("fun" < opt1);
    ASSERT_TRUE(opt1 >= "fun");
    ASSERT_TRUE("fun" <= opt1);

    ASSERT_FALSE(opt1 == std::nullopt);
    ASSERT_FALSE(std::nullopt == opt1);
    ASSERT_TRUE(opt1 != std::nullopt);
    ASSERT_TRUE(std::nullopt != opt1);
    ASSERT_FALSE(opt1 < std::nullopt);
    ASSERT_TRUE(std::nullopt < opt1);
    ASSERT_FALSE(opt1 <= std::nullopt);
    ASSERT_TRUE(std::nullopt <= opt1);
    ASSERT_TRUE(opt1 > std::nullopt);
    ASSERT_FALSE(std::nullopt > opt1);
    ASSERT_TRUE(opt1 >= std::nullopt);
    ASSERT_FALSE(std::nullopt >= opt1);
}

TYPED_TEST(OptionalTest, makeOptional)
{
    auto opt1 = ::zserio::make_optional(2.5);
    ASSERT_TRUE((std::is_same_v<decltype(opt1), Optional<double>>));
    auto opt2 = ::zserio::make_optional<BigObj>('x');
    ASSERT_TRUE((std::is_same_v<decltype(opt2), Optional<BigObj>>));
}

TYPED_TEST(OptionalTest, recursiveOpt)
{
    struct Tmp
    {
        using IS_RECURSIVE = int;
        Optional<Tmp> next;
    };
    ASSERT_TRUE(detail::is_heap_allocated_v<Tmp>);
    Optional<Tmp> opt1;
    opt1 = Tmp();
    ASSERT_TRUE(opt1.has_value());
    ASSERT_TRUE(!opt1->next.has_value());
    ASSERT_TRUE(!std::as_const(opt1)->next.has_value());
    opt1->next = Tmp();
    ASSERT_TRUE(opt1->next.has_value());
    ASSERT_TRUE(!opt1->next->next.has_value());
}

TYPED_TEST(OptionalTest, errors)
{
    typename TestFixture::IntOptional opt1;
    ASSERT_THROW(opt1.value(), BadOptionalAccess);
    ASSERT_THROW(std::as_const(opt1).value(), BadOptionalAccess);
    Optional<ThrowingBig> opt2;
    ASSERT_THROW(opt2.emplace(), std::runtime_error);
}

TYPED_TEST(OptionalTest, swap)
{
    typename TestFixture::StringOptional opt1("house", this->allocator);
    typename TestFixture::StringOptional opt2(this->allocator);
    ASSERT_EQ(*opt1, "house");
    ASSERT_TRUE(!opt2);
    opt1.swap(opt2);
    ASSERT_TRUE(!opt1);
    ASSERT_EQ(*opt2, "house");
}

TYPED_TEST(OptionalTest, hash)
{
    typename TestFixture::StringOptional opt("auto");
    zserio::calcHashCode(100, opt);
    std::hash<typename TestFixture::StringOptional>()(opt);
}

} // namespace zserio

