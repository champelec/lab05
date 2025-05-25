#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include "Account.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::InSequence;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int diff));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

TEST(Transaction, construnct_and_positive) {
    Transaction first;
    EXPECT_EQ(first.fee(), 1);
    Account Petya(0, 6132);
    Account Katya(1, 2133);
    first.set_fee(32);
    EXPECT_EQ(first.fee(), 32);
    EXPECT_TRUE(first.Make(Petya, Katya, 100));
    EXPECT_EQ(Katya.GetBalance(), 2233);
    EXPECT_EQ(Petya.GetBalance(), 6000);
}

TEST(Transaction, negative) {
    Transaction second;
    second.set_fee(51);
    Account Roma(0, 10);
    Account Misha(1, 1000);
    EXPECT_THROW(second.Make(Misha, Misha, 0), std::logic_error);
    EXPECT_THROW(second.Make(Misha, Roma, -100), std::invalid_argument);
    EXPECT_THROW(second.Make(Misha, Roma, 50), std::logic_error);
    EXPECT_FALSE(second.Make(Misha, Roma, 100));
    second.set_fee(10);
    EXPECT_FALSE(second.Make(Roma, Misha, 100));
}

TEST(Account, balance_positive) {
    Account acc1(0, 1000);
    EXPECT_EQ(acc1.GetBalance(), 1000);
    acc1.Lock();
    EXPECT_NO_THROW(acc1.ChangeBalance(100));
    EXPECT_EQ(acc1.GetBalance(), 1100);
}

TEST(Accout, balance_negative) {
    Account Vasya(1, 100);
    EXPECT_THROW(Vasya.ChangeBalance(100), std::runtime_error);
    Vasya.Lock();
    EXPECT_ANY_THROW(Vasya.Lock());
}

TEST(Account, Locker) {
    MockAccount acc(0, 1111);
    EXPECT_CALL(acc, Lock()).Times(2);
    EXPECT_CALL(acc, Unlock()).Times(1);
    acc.Lock();
    acc.Lock();
    EXPECT_EQ(acc.GetBalance(), 0);
    EXPECT_NO_THROW(acc.ChangeBalance(100));
    acc.Unlock();
}

