
OUTPUT=build

.phony: _debug cfg debug

all: cfg debug

_build:
	mkdir -p $(OUTPUT)

cfg: _build
	cmake -B $(OUTPUT) -H./

debug:
	cmake --build $(OUTPUT)

test:
	$(OUTPUT)/pinctrl2

ot:
	bash ./run_test.sh $(OUTPUT)

