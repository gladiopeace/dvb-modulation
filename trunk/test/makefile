
CC := g++

FLAG = -g -Werror

TARGET:= gdtd



$(TARGET):  main.o ../dvb_c.a
	$(CC) $(FLAG) -o $@ $^ 


../dvb_c.a:
	@echo
	@echo -------------------- build module $(subst _b,,$@)------------------
	@echo
	@cd ../src && make 


main.o: main.c
	$(CC) $(FLAG) -I../src -c -o $@ $<


clean:
	cd ../src && make clean
	-rm -f $(TARGET) main.o ../dvb_c.a


