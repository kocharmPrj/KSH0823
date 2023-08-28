var=0
result=$( ps -ef | grep iot_client | grep 5000 | cut -d ' ' -f9 )
if [ -n "$result" ]; then
    var="$result"
    kill -9 "$var"
    ./iot_client_bluetooth 10.10.141.77 5000 ARD_CDS
else
    ./iot_client_bluetooth 10.10.141.77 5000 ARD_CDS
fi

