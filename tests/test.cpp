#include "data_enclosed_mutex.hpp"
#include <gtest/gtest.h>

TEST(ObjScopeGuardTest, ExecFunction) {
    int value = 0;
    std::mutex mtx;
    ObjScopeGuard<int> guard(mtx, value);

    auto incrementValue = [](ObjScopeGuard<int> &guard) { ++(*guard); };

    guard.exec(incrementValue);

    EXPECT_EQ(value, 1);
}

TEST(MutexTest, LockUnlock) {
    Mutex<int> mutex;
    auto guard = mutex.lock();
    EXPECT_EQ(*guard, 0);
    *guard = 42;
    EXPECT_EQ(*guard, 42);
}

TEST(MutexTest, TryLock) {
    Mutex<int> mutex;
    auto guard1 = mutex.try_lock();
    EXPECT_TRUE(guard1.has_value());
}

TEST(MutexTest, TryLockTwice) {
    Mutex<int> mutex;
    auto guard1 = mutex.lock();
    auto guard2 = mutex.try_lock();
    EXPECT_FALSE(guard2.has_value());
}

TEST(SharedMutexTest, LockForRead) {
    SharedMutex<int> sharedMutex;
    auto guard = sharedMutex.lock_for_read();
    EXPECT_EQ(*guard, 0);
}

TEST(SharedMutexTest, TryLockForReadWrite) {
    SharedMutex<int> sharedMutex;
    auto guard1 = sharedMutex.lock_for_rw();
    auto guard2 = sharedMutex.try_lock_for_rw();
    EXPECT_FALSE(guard2.has_value());
}

TEST(SharedMutexTest, MultipleLocksForRead) {
    SharedMutex<int> sharedMutex;
    auto guard1 = sharedMutex.lock_for_read();
    auto guard2 = sharedMutex.lock_for_read();
    EXPECT_EQ(guard1.operator->(), guard2.operator->());
}

TEST(SharedMutexTest, OneLockForWriteNoneForRead) {
    SharedMutex<int> sharedMutex;
    auto guard1 = sharedMutex.lock_for_rw();
    auto guard2 = sharedMutex.try_lock_for_read();
    EXPECT_FALSE(guard2.has_value());
}
