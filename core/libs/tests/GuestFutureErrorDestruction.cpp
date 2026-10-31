#include "prx/libSceLibcInternal/include/FutureError.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <new>
#include <stdexcept>

namespace {

struct Fixture {
    LibcException::ExceptionObject base;
    std::uintptr_t tail[2] = {0x12345678, 0xabcdef};
};

void Require(bool condition) {
    if (!condition) std::abort();
}

void Check(const char* message, bool deleting) {
    Fixture local{};
    auto* object = &local;
    if (deleting) {
        void* storage = std::malloc(sizeof(Fixture));
        Require(storage != nullptr);
        object = new (storage) Fixture{};
    }
    _ZNSt11logic_errorC1EPKc_nid_postfix(&object->base, message);
    LibcException::ExceptionObject observer{};
    _ZNSt11logic_errorC1ERKS__nid_postfix(&observer, &object->base);
    Require(observer.message == object->base.message);
    if (deleting) {
        _ZNSt12future_errorD0Ev_nid_postfix(&object->base);
    } else {
        _ZNSt12future_errorD2Ev_nid_postfix(&object->base);
        Require(object->base.message == nullptr);
        Require(object->tail[0] == 0x12345678 && object->tail[1] == 0xabcdef);
    }
    if (message) Require(observer.message && std::strcmp(observer.message, message) == 0);
    else Require(observer.message == nullptr);
    _ZNSt11logic_errorD2Ev_nid_postfix(&observer);
    Require(observer.message == nullptr);
}

}

int main() {
    for (const char* message : {static_cast<const char*>(nullptr), "", "future has no shared state"})
        for (bool deleting : {false, true}) Check(message, deleting);
    for (auto function : {_ZNSt12future_errorD0Ev_nid_postfix, _ZNSt12future_errorD2Ev_nid_postfix}) {
        bool rejected = false;
        try {
            function(nullptr);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        Require(rejected);
    }
    std::puts("future_error destruction passed");
}
