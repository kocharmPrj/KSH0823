#CC:=arm-linux-gcc
CC:=gcc

TARGET_SRV=iot_server_2
OBJECT_SRV=$(TARGET_SRV).o 


#LDFLAGS=-D_REENTRANT -pthread -lmysqlclient
LDFLAGS=-D_REENTRANT -pthread

#all : $(TARGET_SRV)

$(TARGET_SRV):$(OBJECT_SRV)
	$(CC) -o $@ $(OBJECT_SRV) $(LDFLAGS)
%.o:%.c
	$(CC) -c -o $@ $<
clean:
	rm -f *.o $(TARGET_SRV) $(TARGET_CLN)
