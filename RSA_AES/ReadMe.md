#### ReadME

1. 运行RsaInit文件下的RsaInit程序，将初始化Server和Client的Rsa公钥，写入到对应的TXT中
       ServerRsa：ServerRsa的密钥(n,e,d)，供Server解密
       ServerPk：ServerRsa的公钥(n,e)，供Client加密
       ClientRsa：ClientRsa的密钥(n,e,d)，供Client解密
       ClientPk：ClientRsa的公钥(n,e)，供Server加密

2. 通信功能要求先运行Server再运行Client，否则可能导致失败；
   Client首先进入发送模式，发送KEY、IV将预设好的AES密钥发送到服务器端，此后可以进行信息交互。
   ![image-20230103141513177](./../../../../../BaiduSyncdisk/Typro%E7%AC%94%E8%AE%B0/%E6%88%AA%E5%9B%BE/image-20230103141513177.png)
3. 通信时AES使用CBC模式，因此不为16字节倍数时使用点号（ASCII = 46）填充
4. 在发送消状态下输入"END"可以切换消息处理状态（接收/发送）
5. 在发送消状态下输入"EXIT"将退出程序
6. 主函数的代码在分别在Client、Server、RSAInit目录下，三个项目所需的资源文件在Resource中。

