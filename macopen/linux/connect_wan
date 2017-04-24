uci set network.wan.proto=dhcp &&
uci commit &&
/etc/init.d/network restart &&
./macopenOnOpenwrt eth0.2 3&&
uci set network.wan.proto=pppoe &&
uci commit &&
/etc/init.d/network restart 
