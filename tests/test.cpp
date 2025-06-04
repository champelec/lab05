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

TEST(Transaction, Make_calls_correct_methods) {
    Transaction transaction;
    transaction.set_fee(1);
    MockAccount from(0, 1000);
    MockAccount to(1, 500);
    {
        InSequence seq;
        EXPECT_CALL(from, Lock());
        EXPECT_CALL(to, Lock());
        EXPECT_CALL(to, ChangeBalance(100));
        EXPECT_CALL(from, GetBalance()).Times(testing::AnyNumber()).WillRepeatedly(Return(1100));
        EXPECT_CALL(from, ChangeBalance(-101));
        EXPECT_CALL(from, GetBalance()).Times(testing::AnyNumber()).WillRepeatedly(Return(999));
        EXPECT_CALL(to, GetBalance()).Times(testing::AnyNumber()).WillRepeatedly(Return(600));
        EXPECT_CALL(to, Unlock());
        EXPECT_CALL(from, Unlock());
    }
    EXPECT_TRUE(transaction.Make(from, to, 100));
}

TEST(Transaction, Make_fails_if_insufficient_funds) {
    Transaction transaction;
    transaction.set_fee(10);
    MockAccount from(0, 100);
    MockAccount to(1, 200);
    InSequence seq;
    EXPECT_CALL(from, Lock());
    EXPECT_CALL(to, Lock());
    EXPECT_CALL(to, ChangeBalance(100));
    EXPECT_CALL(from, GetBalance()).Times(testing::AnyNumber()).WillRepeatedly(Return(105));
    EXPECT_CALL(to, ChangeBalance(-100));
    EXPECT_CALL(from, GetBalance()).Times(testing::AnyNumber()).WillRepeatedly(Return(105));
    EXPECT_CALL(to, GetBalance()).Times(testing::AnyNumber()).WillRepeatedly(Return(200));
    EXPECT_CALL(to, Unlock());
    EXPECT_CALL(from, Unlock());
    EXPECT_FALSE(transaction.Make(from, to, 100));
}

TEST(Account, Unlock_method_covered) {
    Account acc(123, 500);
    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(50));
    EXPECT_EQ(acc.GetBalance(), 550);
    acc.Unlock();
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
