QSON_OBJS := $(patsubst %,$(BUILD_DIR)/obj/%.o,$(call RESOLVE_SOURCES,qson,.c))

$(BUILD_DIR)/libqson.so: $(QSON_OBJS)
	$(info > Linking $@)
	gcc --shared $(QSON_OBJS) -o $@
BUILD_TARGETS += $(BUILD_DIR)/libqson.so

