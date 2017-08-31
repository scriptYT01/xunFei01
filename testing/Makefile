
#.PHONY : t01

nameS:= t01 t02
SRCs:=$(foreach aa1,$(nameS),$(firstword $(wildcard $(aa1).cpp $(aa1).c)))
DSTb:=$(foreach aa1,$(nameS),$(aa1).bin)

all: help01
help01:
	@echo ; echo $(nameS)
	@echo

m:
	vim Makefile

#t01 : t01.bin
#	./$< 
define CPPtemple
$(1) : $(1).bin
	@echo
	./$$<
	@echo
v$(1) : $(firstword $(wildcard src/$(1).cpp src/$(1).c))
	@echo
	vim $$<
	@echo
	make $(1)
endef

$(foreach bb1,$(nameS),$(eval $(call CPPtemple,$(bb1))))

%.bin : src/%.cpp
	@echo
	g++ -Wall -Werror -o $@  $<

c clean :
	@#echo SRCs : $(SRCs)
	@#echo DSTb : $(DSTb)
	rm -f $(DSTb)
