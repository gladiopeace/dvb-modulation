
TARGET:= ../dvb_modulator.a

SRC_LIST:= object.cpp  file.cpp tsfile.cpp \
		channelcode.cpp framestructure.cpp modulation.cpp \
		dvb_c.cpp dvb_s.cpp

OBJ_LIST:= $(patsubst %.cpp, obj/%.o,$(SRC_LIST))

INCLUDE:=

$(TARGET): $(OBJ_LIST)
	ar -r $@ $^

$(OBJ_LIST):obj/%.o:%.cpp
	$(CC) $(FLAG)  $(INCLUDE) -c $< -o $@

test:
	@echo TARGET=$(TARGET)
	@echo SRC_LIST=$(SRC_LIST)
	@echo OBJ_LIST=$(OBJ_LIST)
	@echo INCLUDE=$(INCLUDE)
	@echo CC=$(CC)
	@echo FLAG=$(FLAG)

clean:
	-rm -f $(TARGET) $(OBJ_LIST)


CC := g++
FLAG = -g -Werror

DPF:= .dpf

####################### include head file dependence #####################

$(shell rm -f $(DPF))
$(foreach src,$(SRC_LIST),\
	$(shell	$(CC) -MM $(FLAG) $(INCLUDE) $(src) | \
	sed 's/^\(.*\)\.o/obj\/\1\.o/'>> $(DPF);\
	))
	

sinclude $(DPF)