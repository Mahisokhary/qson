include $(filter-out make/%,$(wildcard */build.mak))

$(BUILD_DIR)/obj/%.c.o: %.c
	$(info > Compiling $@)
	mkdir -p $(@D)
	gcc -I. -fPIC -c $< -o $@

.DEFAULT_GOAL := build
.PHONY: build
build: $(BUILD_TARGETS)

