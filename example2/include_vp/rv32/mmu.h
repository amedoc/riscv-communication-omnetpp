#pragma once

#include "../../../example_sensor_riscv/include_vp/common/mmu.h"

#include "../../../example_sensor_riscv/include_vp/rv32/iss.h"

namespace rv32 {

    typedef GenericMMU<ISS> MMU;

}  // namespace rv64
