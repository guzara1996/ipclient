![coder farmer](https://raw.githubusercontent.com/interfacekun/skynet/master/img/manong.jpg "0. 0")

# ipclient
桂电出校器mac开放工具，支持小米路由(OpenWrt)


## 下载
```Bash
https://github.com/interfacekun/ipclient.git
```
## 编译
```Bash
#我的小米路由mini芯片是mt7620a
#在ubuntu上配置好相应的交叉编译环境(自已百度配置)
#用mipsel-openwrt-linux-g++ 编译
cd ipclient/macopen/linux
~/OpenWrtSDK/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-g++ -o macopenOnOpenwrt getIPandMac.cpp
```
## 运行
```Bash
#把编译好的linux文件夹上传到小米路由
#运行之前先改connect_wan.sh
#脚本的第4行就是运行./macopenOnOpenwrt eth0.2 3
#第一个参数是网卡名称（我的小米路由是eth0.2）
#第二个参数是宽带运营商（1是联通，2是电信，3是移动。我的是移动）
#运行
./connect_wan.sh
```
## 项目里还有一个python版本的，自已研究使用
