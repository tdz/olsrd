LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := olsrd
LOCAL_SRC_FILES := \
  src/common/autobuf.c \
  src/common/avl.c \
  src/common/list.c \
  src/build_msg.c \
  src/duplicate_handler.c \
  src/duplicate_set.c \
  src/fpm.c \
  src/gateway.c \
  src/gateway_costs.c \
  src/gateway_default_handler.c \
  src/gateway_list.c \
  src/generate_msg.c \
  src/hashing.c \
  src/hna_set.c \
  src/hysteresis.c \
  src/interfaces.c \
  src/ipcalc.c \
  src/ipc_frontend.c \
  src/link_set.c \
  src/lq_mpr.c \
  src/lq_packet.c \
  src/lq_plugin.c \
  src/lq_plugin_default_ff.c \
  src/lq_plugin_default_ffeth.c \
  src/lq_plugin_default_float.c \
  src/lq_plugin_default_fpm.c \
  src/main.c \
  src/mantissa.c \
  src/mid_set.c \
  src/mpr.c \
  src/mpr_selector_set.c \
  src/neighbor_table.c \
  src/net_olsr.c \
  src/olsr.c \
  src/olsr_cookie.c \
  src/olsr_niit.c \
  src/olsr_spf.c \
  src/packet.c \
  src/parser.c \
  src/plugin_loader.c \
  src/plugin_util.c \
  src/process_package.c \
  src/process_routes.c \
  src/rebuild_packet.c \
  src/routing_table.c \
  src/scheduler.c \
  src/tc_set.c \
  src/two_hop_neighbor_table.c \
  src/unix/ifnet.c \
  src/unix/log.c \
  src/unix/misc.c \
  src/linux/apm.c  \
  src/linux/kernel_routes_nl.c \
  src/linux/kernel_tunnel.c \
  src/linux/lq_plugin_ffeth_nl80211.c \
  src/linux/net.c \
  src/linux/nl80211_link_info.c \
  src/cfgparser/olsrd_conf.c \
  src/cfgparser/cfgfile_gen.c \
  src/builddata.c \
  src/cfgparser/oparse.c \
  src/cfgparser/oscan.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/src/cfgparser $(LOCAL_PATH)/android
LOCAL_CFLAGS := -Wall -Wextra -Wold-style-definition -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes -Wmissing-declarations -Wsign-compare -Waggregate-return -Wmissing-noreturn -Wmissing-format-attribute -Wno-multichar -Wno-deprecated-declarations -Wendif-labels -Wwrite-strings -Wbad-function-cast -Wpointer-arith -Wcast-qual -Wshadow -Wformat -Wsequence-point -Wcast-align -Wformat-security -Wformat-y2k -Werror=format-security -Winit-self -Wswitch-default -Wsync-nand -Wundef -Wlogical-op -Wunused-parameter -Wnested-externs -Winline -Wdisabled-optimization -finline-functions-called-once -funit-at-a-time -fearly-inlining -finline-limit=350   -ggdb -Isrc  -DUSE_FPM -DOLSRD_GLOBAL_CONF_FILE=\"/data/misc/olsrd/olsrd.conf\" -DINET_ADDRSTRLEN=16 -D'IPTOS_PREC(tos)=((tos)&0xe0)' -D'IPTOS_TOS(tos)=((tos)&0x1e)' -I./android -march=armv5te -mtune=xscale -msoft-float -fpic -mthumb-interwork -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -fno-omit-frame-pointer -DDEBUG

LOCAL_LDFLAGS := -Wl,-export-dynamic -Wl,-rpath,$(LIBDIR) -Wl,--warn-common
LOCAL_LDFLAGS += -ldl -llog

include $(BUILD_EXECUTABLE)
