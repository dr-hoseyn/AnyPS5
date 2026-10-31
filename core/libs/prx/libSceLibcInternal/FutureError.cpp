#include "prx/libSceLibcInternal/include/FutureError.hpp"
#include <stdexcept>

extern "C" {

void APS5_VABI _ZNSt12future_errorD0Ev_nid_postfix(LibcException::ExceptionObject* self) {
    if (!self) throw std::invalid_argument("future_error deleting destructor: null object");
    _ZNSt11logic_errorD0Ev_nid_postfix(self);
}

void APS5_VABI _ZNSt12future_errorD2Ev_nid_postfix(LibcException::ExceptionObject* self) {
    if (!self) throw std::invalid_argument("future_error base destructor: null object");
    _ZNSt11logic_errorD2Ev_nid_postfix(self);
}

}
