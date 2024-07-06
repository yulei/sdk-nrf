#
# Copyright (c) 2024 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

include_guard(GLOBAL)

if(SB_CONFIG_APP_CPUNET_RUN)

  ExternalZephyrProject_Add(
    APPLICATION coremark_${SB_CONFIG_NETCORE_REMOTE_BOARD_TARGET_CPUCLUSTER}
    SOURCE_DIR ${APP_DIR}
    BOARD ${SB_CONFIG_BOARD}/${SB_CONFIG_SOC}/${SB_CONFIG_NETCORE_REMOTE_BOARD_TARGET_CPUCLUSTER}
    BOARD_REVISION ${BOARD_REVISION}
  )

endif()

if(SB_CONFIG_APP_CPUPPR_RUN)

  ExternalZephyrProject_Add(
    APPLICATION coremark_${SB_CONFIG_PPRCORE_REMOTE_BOARD_TARGET_CPUCLUSTER}
    SOURCE_DIR ${APP_DIR}
    BOARD ${SB_CONFIG_BOARD}/${SB_CONFIG_SOC}/${SB_CONFIG_PPRCORE_REMOTE_BOARD_TARGET_CPUCLUSTER}
    BOARD_REVISION ${BOARD_REVISION}
  )

endif()
