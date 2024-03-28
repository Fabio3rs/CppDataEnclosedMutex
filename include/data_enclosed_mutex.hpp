#pragma once
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>

class GuardInvertOrder {};

template <class Obj, class MtxT = std::mutex,
          class GuardT = std::scoped_lock<MtxT>>
class ObjScopeGuard {
    GuardT lck;

  public:
    Obj *const objPtr;

    Obj *operator->() { return objPtr; }
    const Obj *operator->() const { return objPtr; }

    Obj &operator*() { return *objPtr; }
    const Obj &operator*() const { return *objPtr; }

    ObjScopeGuard(MtxT &m, Obj &obj) : lck(m), objPtr(&obj) {}

    ObjScopeGuard(GuardT &&m, Obj &obj) : lck(std::move(m)), objPtr(&obj) {}

    ObjScopeGuard(std::adopt_lock_t /**/, MtxT &m, Obj &obj)
        : lck(std::adopt_lock, m), objPtr(&obj) {}

    ObjScopeGuard(const ObjScopeGuard &) = delete;

    template <class T> auto &exec(T &&f) {
        f(*this);
        return *this;
    }

    ~ObjScopeGuard() = default;
};

template <class Obj, class MtxT = std::mutex> struct Mutex {
  public:
    using ScopeT = ObjScopeGuard<Obj, MtxT>;

    auto lock() -> ScopeT { return {mtx, obj}; }

    auto try_lock() -> std::optional<ScopeT> {
        if (!mtx.try_lock()) {
            return {};
        }
        return std::optional<ScopeT>{std::in_place, std::adopt_lock, mtx, obj};
    }

    template <class Time>
    auto try_lock_for(Time waitTime) -> std::optional<ScopeT> {
        if (!mtx.try_lock_for(waitTime)) {
            return {};
        }
        return std::optional<ScopeT>{std::in_place, std::adopt_lock, mtx, obj};
    }

    template <class Time>
    auto try_lock_until(Time waitUntil) -> std::optional<ScopeT> {
        if (!mtx.try_lock_until(waitUntil)) {
            return {};
        }
        return std::optional<ScopeT>{std::in_place, std::adopt_lock, mtx, obj};
    }

    template <class... Args>
    Mutex(Args &&...args) : obj(std::forward<Args>(args)...) {}

    Mutex() : obj{} {}

  protected:
    Obj obj;
    mutable MtxT mtx;
};

template <class Obj, class Mtx = std::shared_mutex>
struct SharedMutex : protected Mutex<Obj, Mtx> {
    using parentType = Mutex<Obj, Mtx>;
    using RwGuardStd = std::unique_lock<Mtx>;
    using RwGuardT = ObjScopeGuard<Obj, Mtx, std::unique_lock<Mtx>>;

    using ReadGuardStd = std::shared_lock<Mtx>;
    using ReadGuardT = const ObjScopeGuard<const Obj, Mtx, ReadGuardStd>;

    template <class... Args>
    SharedMutex(Args &&...args) : parentType(std::forward<Args>(args)...) {}

    auto lock_for_read() const -> ReadGuardT {
        return {parentType::mtx, parentType::obj};
    }

    auto try_lock_for_read() const -> std::optional<ReadGuardT> {
        if (!parentType::mtx.try_lock_shared()) {
            return {};
        }

        ReadGuardStd guard{parentType::mtx, std::adopt_lock};

        return std::optional<ReadGuardT>{std::in_place, std::move(guard),
                                         parentType::obj};
    }

    template <class Time>
    auto try_lock_for_read_for(Time waitTime) const
        -> std::optional<ReadGuardT> {
        if (!parentType::mtx.try_lock_shared_for(waitTime)) {
            return {};
        }

        return {std::in_place, std::adopt_lock, parentType::mtx,
                parentType::obj};
    }

    template <class Time>
    auto try_lock_for_read_until(Time waitUntil) const
        -> std::optional<ReadGuardT> {
        if (!parentType::mtx.try_lock_shared_until(waitUntil)) {
            return {};
        }

        return {std::in_place, std::adopt_lock, parentType::mtx,
                parentType::obj};
    }

    auto lock_for_rw() -> RwGuardT {
        return {parentType::mtx, parentType::obj};
    }

    auto try_lock_for_rw() -> std::optional<RwGuardT> {
        if (!parentType::mtx.try_lock()) {
            return {};
        }

        RwGuardStd lck(parentType::mtx, std::adopt_lock);

        return std::optional<RwGuardT>{std::in_place, std::move(lck),
                                       parentType::obj};
    }

    template <class Time>
    auto try_lock_for_rw_for(Time waitTime) -> std::optional<RwGuardT> {
        if (!parentType::mtx.try_lock_for(waitTime)) {
            return {};
        }

        RwGuardStd lck(parentType::mtx, std::adopt_lock);

        return std::optional<RwGuardT>{std::in_place, std::move(lck),
                                       parentType::obj};
    }

    template <class Time>
    auto try_lock_for_rw_until(Time waitUntil) -> std::optional<RwGuardT> {
        if (!parentType::mtx.try_lock_until(waitUntil)) {
            return {};
        }

        RwGuardStd lck(parentType::mtx, std::adopt_lock);

        return std::optional<RwGuardT>{std::in_place, std::move(lck),
                                       parentType::obj};
    }
};
